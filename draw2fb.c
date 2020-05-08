/**
 * @todo copyright header here
 */

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "draw2fb.h"

// These bounds can be changed pretty much whenever to add more glyphs
// But they must be <225, the maximum unsigned char (or the code below modified)
#define MIN_GLYPH 32
#define MAX_GLYPH 128

// Convert from fixed point with 1/64 pixel precision to regular pixels
#define FIX_TO_PIXELS(x) (x / 64)

/**
 * Draw a font using libfreetype and then write to fb assuming it is 1bpp
 * @todo output format should be configurable
 * @todo add support for pitch != width in the output buffer
 */
int draw2fb_string(struct draw2fb_opts_t *config, char *str, uint8_t *fb) {
	FT_Library ft;
	FT_GlyphSlot g;
	FT_Face face;
	FT_Pos maxAscent = 0, minDescent = 0;
	uint8_t curChar;
	uint8_t *fb8;
	uint8_t prevChar = '\0';
	int x, y;
	int i, k;
	int xp, yp;
	int yo;
	int lineheight;
	int retval = 0;

	// This implies 1bpp and would need to be updated
	fb8 = calloc(8, config->W * config->H / 8);

	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Unable to initialize FreeType library!\n");
		free(fb8);
		retval = 1;
		goto done;
	}

	if (FT_New_Face(ft, config->font, 0, &face)) {
		fprintf(stderr, "Unable to load font %s\n", config->font);
		retval = 1;
		goto done;
	}

	FT_Set_Pixel_Sizes(face, config->fontSize, config->fontSize);
	g = face->glyph;

	for (curChar = MIN_GLYPH; curChar < MAX_GLYPH; ++curChar) {
		FT_Pos pixelBearing;
		int descent;

		if (FT_Load_Char(face, curChar, FT_LOAD_RENDER)) {
			fprintf(stderr, "Could not load character %c\n", curChar);
			retval = 1;
			goto done;
		}

		pixelBearing = FIX_TO_PIXELS(g->metrics.horiBearingY);
		descent = pixelBearing - g->bitmap.rows;
		maxAscent = (pixelBearing > maxAscent) ? pixelBearing : maxAscent;
		minDescent = (descent < minDescent) ? descent : minDescent;
	}

	lineheight = maxAscent - minDescent;

	x = config->xoffset;
	y = config->yoffset + config->startline*lineheight;
	for ( ; *str; ++str) {
		curChar = *str;

		if (curChar == '\n') {
			y += lineheight;
			x = config->xoffset;
			prevChar = '\0';
			continue;
		}

		if (curChar < MIN_GLYPH || curChar > MAX_GLYPH) {
			fprintf(stderr, "Cannot print character %c yet! Update the source if this glyph is needed.\n", curChar);
			retval = 1;
			goto done;
		}

		FT_Load_Char(face, curChar, FT_LOAD_RENDER);

		if (curChar == ' ') {
			x += FIX_TO_PIXELS(g->advance.x);
			prevChar = curChar;
			continue;
		}

		// Adjust kerning based on previous character
		if (config->kerning && prevChar) {
			FT_Vector kern;
			FT_Get_Kerning(face, prevChar, curChar, FT_KERNING_DEFAULT, &kern);
			x += FIX_TO_PIXELS(kern.x);
		}

		// Check if this character would go off the edge
		if (x + FIX_TO_PIXELS(g->advance.x) >= config->W) {
			if (config->wrap) {
				y += lineheight;
				x = config->xoffset;
			}
			else {
				x = config->W;
				prevChar = '\0';
				continue;
			}
		}

		yo = maxAscent - g->bitmap_top;
		yp = 0;
		while ((y + yp + yo) < config->H && yp < g->bitmap.rows) {
			if ((y + yp + yo) < 0) {
				yp += 1;
				continue;
			}

			xp = 0;
			while (x + xp < config->W && xp < g->bitmap.width) {
				if (x + xp < 0) {
					xp += 1;
					continue;
				}

				fb8[x+xp + (y+yo+yp)*config->W] = g->bitmap.buffer[xp + yp*g->bitmap.pitch];
				xp += 1;
			}
			yp += 1;
		}

		x += FIX_TO_PIXELS(g->advance.x);
		prevChar = curChar;
	}

	if (!config->overdraw) {
		memset(fb, 0, config->W * config->H / 8);
	}

	// Pack the buffer we drew into single pixels
	for (i = 0; i < config->W * config->H / 8; ++i) {
		for (k = 0; k < 8; ++k) {
			uint8_t byte = fb8[i*8 + k];
			uint8_t bit = (byte & 0x80) >> 7;
			fb[i] |= (bit << k);
		}
	}

done:
	free(fb8);
	return retval;
}

