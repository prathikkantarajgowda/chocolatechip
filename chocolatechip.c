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

#include "cpu.h"
#include "display.h"
#include "input.h"

int
main(int argc, char **argv)
{
	uint16_t	op;
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
		op = fetch(&chip8);
		decode_execute(&chip8, &screen, op);

		if (chip8.draw_flag) {
			update_display(&screen);
			chip8.draw_flag = 0;
		}
	}

	kill_display(&screen);

	return 0;
}
