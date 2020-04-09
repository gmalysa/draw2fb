
HEADERS = draw2fb.h
SRC = draw2fb.c
OBJ = $(SRC:.c=.o)

all : libdraw2fb.a

%.o : %.c
	$(CC) `pkg-config --cflags freetype2` $< -c -o $@

libdraw2fb.a : $(OBJ)
	$(AR) rcs $@ $<

clean :
	rm -f $(OBJ) libdraw2fb.a

release :
	tar czf draw2fb.tar.gz $(SRC) $(HEADERS) Makefile
