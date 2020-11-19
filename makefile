CC=clang
CFLAGS=-Wall -Wextra -pedantic


all: chocolatechip

chocolatechip: chocolatechip.c chocolatechip.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o *~ chocolatechip
