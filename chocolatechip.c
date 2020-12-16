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
 *
 *
 * completed:
 *	 - chip8 cpu data structure
 *	 - SDL display data structure
 *	 - initialization function for CPU
 *	 - initialization function for display (SDL)
 *
 * todo:
 *	 - keyboard
 *	 - sound (just need beep)
 *	 - get/fetch/execute
 *
 */

#include "cpu.h"
#include "display.h"
#include "input.h"

int
main(int argc, char **argv)
{
	struct cpu      chip8;
	struct display  screen;

	if (argc != 2) {
		(void)fprintf(stderr,
			"One ROM file needed. Usage: chocolatechip rom.ch8\n");
		return 1;
	}

	init_cpu(&chip8, argv[1]);
	init_display(&screen);

	while (1) {
		update_timers(&chip8);
		keyboard_input(&chip8, &screen);
		decode_execute(&chip8, &screen, fetch(&chip8));
	}

	kill_display(&screen);

	return 0;
}
