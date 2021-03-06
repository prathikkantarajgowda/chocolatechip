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
#
#
# Makefile
# 

include config.mk

.POSIX:
.SUFFIXES:

all: chocolatechip

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f chocolatechip $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/chocolatechip
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f chocolatechip.1 $(DESTDIR)$(MANPREFIX)/man1/chocolatechip.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/chocolatechip.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/chocolatechip\
		$(DESTDIR)$(MANPREFIX)/man1/chocolatechip.1

clean:
	rm -f *.o *~ chocolatechip *.c# *.swp *.c.swp *.gch a.out

chocolatechip: $(OBJ)
	$(CC) $(PKGCONFIGFLAGS) -o $@ $(OBJ) $(LINKFLAGS)

chocolatechip.o: chocolatechip.c cpu.h display.h input.h

cpu.o: cpu.c cpu.h

display.o: display.c display.h

input.o: input.c input.h

.PHONY: clean install uninstall

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) $(PKGCONFIGFLAGS) -c $<
