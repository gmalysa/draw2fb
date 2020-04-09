#ifndef _DRAW2FB_H_
#define _DRAW2FB_H_
/**
 * @todo copyright header
 */

struct draw2fb_opts_t {
	const char *font;		//!< Name of font to use
	int fontSize;			//!< Font size in px
	int kerning;			//!< 1 to enable kerning, 0 otherwise
	int startline;			//!< Line to start on
	int wrap;				//!< 1 to enable auto wrapping on characters that go off screen
	int xoffset;			//!< x pixel offset for the string, can be negative
	int yoffset;			//!< y pixel offset for the string, can be negative
	int overdraw;			//!< If 1, do not clear first
	int W;					//!< Width in pixels of given fb
	int H;					//!< Height in pixels of given fb
	int pitch;				//!< Pitch in bytes between lines in the framebuffer
	int bpp;				//!< Bits per pixel of the framebuffer
};

int draw2fb_string(struct draw2fb_opts_t *config, char *str, uint8_t *fb);

#endif
