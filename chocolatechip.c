#include <stdlib.h>
#include <time.h>

#include "chip8.h"

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("One ROM file needed. Usage: chocolatechip rom.ch8\n");
		return 1;
	}

}

void
init_cpu(cpu_t cpu, char *romfile)
{
	/* Clears memory */
	for (int i = 0; i < 4096; i++)
		memory[i] = 0;

	/* Clears stack and V registers */
	for (int i = 0; i < 16; i++) {
		stack[i] = 0;
		V[i] = 0;
	}
	
	/* Loads fontset into memory */
	for (int i = 0; i < 80; i++)
		cpu->memory[i];


	/* Sets program counter to point at the first ROM instruction */
	PC = 0x200;

	/* Opens ROM */
	if (!(cpu->rom = fopen(romfile, "rb"))) {
		printf("Invalid ROM filename: %s", romfile);
		exit(1);
	}

	/* Load game ROM into memory starting from 0x200 (512 in decimal) */
	fread(cpu->memory + 0x200, 1, 4096 - 0x200, cpu->rom);

	/* Initializes delay and sound to zero */
	cpu->delay = 0;
	cpu->sound = 0;
}
	
