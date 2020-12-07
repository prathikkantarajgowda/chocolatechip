#
# chocolatechip: a Chip-8 emulator written by Prathik Gowda
#
# cpu.h
#
#

CC = clang
CFLAGS = -O2 -std=c99 -pedantic -Wall -Wextra -Wshadow
PKGCONFIGFLAGS = -D_REENTRANT -I/usr/include/SDL2
LINKSDLFLAG = -lSDL2

all: chocolatechip

chocolatechip: chocolatechip.o cpu.o display.o input.o
	$(CC) $(PKGCONFIGFLAGS) $(LINKSDLFLAG) -o $@ $^

chocolatechip.o: chocolatechip.c display.h
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $<

cpu.o: cpu.c cpu.h
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $<

display.o: display.c display.h
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $<

input.o: input.c
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f *.o *~ chocolatechip *.c# *.swp *.c.swp *.gch
