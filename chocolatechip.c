/* 
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
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

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "display.h"

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
	update_timers(&chip8);

	kill_display(&screen);

	return 0;
}
