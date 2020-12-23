/*   chocolatechip: a CHIP-8 emulator written in C99
 *   Copyright (C) 2020  Prathik Gowda (gowdapra@grinnell.edu)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * chocolatechip.c
 *
 * todo:
 *	 - sound (just need beep)
 *	 - timing
 *	 - man page?
 *
 */

#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "cpu.h"
#include "display.h"
#include "input.h"

static void usage(void);

int
main(int argc, char **argv)
{
	struct cpu      chip8;
	struct display  screen;

	srand(time(NULL));

	if (argc != 2) {
		usage();
	}

	init_cpu(&chip8, argv[1]);
	init_display(&screen);

	while (1) {
		keyboard_input(&chip8, &screen);
		cycle(&chip8, &screen);
	}

	exit(EXIT_SUCCESS);
}

static void
usage(void)
{
	(void)fprintf(stderr, "usage: chocolatechip filename\n");
	exit(EXIT_FAILURE);
}
