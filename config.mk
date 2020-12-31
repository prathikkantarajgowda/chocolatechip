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
#  config.mk
#

CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Os
# CFLAGS = -O2 -std=c99 -pedantic -fsanitize=undefined -Wall -Wcast-align
# CFLAGS += -Wcast-qual -Wextra -Wfloat-equal -Wshadow -Wsign-conversion
# CFLAGS += -Wstrict-overflow=5 -Wstrict-prototypes -Wswitch-default -Wundef

PKGCONFIGFLAGS = -D_REENTRANT -I/usr/include/SDL2
PKGCONFIGFLAGS = -lSDL2

# uncomment following 2 lines on OpenBSD
# PKGCONFIGFLAGS = -I/usr/local/include/SDL2 -D_REENTRANT
# LDFLAGS = -L/usr/local/lib -L/usr/X11R6/lib -lSDL2

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

SRC = chocolatechip.c cpu.c display.c input.c
OBJ = $(SRC:.c=.o)
