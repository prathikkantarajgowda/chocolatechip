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
	/* Loads fontset into memory */
	for (int i = 0; i < 80; i++)
		cpu->memory[i];


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
	
