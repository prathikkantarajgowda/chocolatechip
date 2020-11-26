CC = clang
CFLAGS = -ansi -Wall -Wextra -pedantic
PKGCONFIGFLAGS = -D_REENTRANT -I/usr/include/SDL2 
LINKSDLFLAG = -lSDL2

all: chocolatechip

chocolatechip: chocolatechip.o cpu.o display.o
	$(CC) $(PKGCONFIGFLAGS) $(LINKSDLFLAG) -o $@ $^ 

chocolatechip.o: chocolatechip.c display.h
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $<

cpu.o: cpu.c cpu.h
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $<

display.o: display.c display.h
	$(CC) $(PKGCONFIGFLAGS) -c $(CFLAGS) $< 

.PHONY: clean

clean:
	rm -f *.o *~ chocolatechip *.c# 
