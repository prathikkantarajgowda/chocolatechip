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
 *
 */

#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "cpu.h"
#include "display.h"
#include "input.h"

#define DELAY_TIME 16.666f	/* delay time to produce 60fps (1000/16.666 = 60) */

const char *progname;
const char *romfile;

static void args(int, char **, struct cpu *);
static void usage(void);

int
main(int argc, char **argv)
{
	struct cpu      chip8;
	struct display  screen;

	srand(time(NULL));

	args(argc, argv, &chip8);
	init_cpu(&chip8, romfile);
	init_display(&screen);

	while (1) {
		uint64_t start = SDL_GetPerformanceCounter();
		keyboard_input(&chip8, &screen);
		cycle(&chip8, &screen);

		uint64_t end = SDL_GetPerformanceCounter();
		double time_elapsed = ((end - start) * 1000) / SDL_GetPerformanceFrequency();
		SDL_Delay((uint32_t) DELAY_TIME - time_elapsed);
	}

	exit(EXIT_SUCCESS);
}

static void
args(int argc, char **argv, struct cpu *chip8)
{
	char ch;

	chip8->debug = 0;

	if (argc < 2)
		usage();

	progname = argv[0];
	romfile  = argv[argc - 1];

	while ((ch = getopt(argc, argv, "d")) != -1) {
		switch (ch) {
		case 'd':
			chip8->debug = 1;
			break;
		default:
			usage();
		}
	}
}

static void
usage(void)
{
	(void)fprintf(stderr, "usage: chocolatechip [-d] romfile\n");
	exit(EXIT_FAILURE);
}
