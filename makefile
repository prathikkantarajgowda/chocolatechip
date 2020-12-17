#   chocolatechip: a CHIP-8 emulator written in C99
#   Copyright (C) 2020  Prathik Gowda (gowdapra@grinnell.edu)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as
#  published by the Free Software Foundation, either version 3 of the
#  License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
