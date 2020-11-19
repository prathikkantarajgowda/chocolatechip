#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chocolatechip.h"


/*
 * todo: 
 *	 - keyboard
 *	 - graphics (SDL2?)
 */

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("One ROM file needed. Usage: chocolatechip rom.ch8\n");
		return 1;
	}

	struct cpu_t cpu;
	init_cpu(&cpu, argv[1]);
}

static void
init_cpu(struct cpu_t *cpu, char *romfile)
{
	/*
	 * todo: clear display?
	 */
	
	/* Clears memory, stack, and V registers */
	memset(cpu->memory, 0, 4096);
	memset(cpu->stack, 0, 16);
	memset(cpu->V, 0, 16);
	
	/* Loads fontset into memory */
	for (int i = 0; i < 80; i++)
		cpu->memory[i] = chip8_fontset[i];


	/* Opens ROM */
	if (!(cpu->rom = fopen(romfile, "rb"))) {
		fprintf(stderr, "Invalid ROM filename: %s", romfile);
		exit(1);
	}

	printf("%s rom successfully loaded\n", romfile);

	/* Load game ROM into memory starting from 0x200 (512 in decimal) */
	fread(cpu->memory + 0x200, 1, 4096 - 0x200, cpu->rom);
	fclose(cpu->rom);

	/* Sets program counter to point at the first ROM instruction */
	cpu->PC = 0x200;

	/* Initializes delay and sound to zero */
	cpu->delay = 0;
	cpu->sound = 0;
}

static void
update_timers(struct cpu_t *cpu)

	if (cpu->delay > 0)
		cpu->delay--;
	if (cpu->sound > 0) {
		cpu->sound--;
		printf("beep\n");
	}
}

static void
tick(struct cpu_t *cpu)
{
	update_timers(cpu);
}
