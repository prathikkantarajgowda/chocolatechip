CC=gcc
CFLAGS=-Wall -Wextra -pedantic
PKGCONFIGFLAGS=-D_REENTRANT -I/usr/include/SDL2 -lSDL2


all: chocolatechip

chocolatechip: chocolatechip.c chocolatechip.h
	$(CC) $(CFLAGS) $(PKGCONFIGFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o *~ chocolatechip *.c#
