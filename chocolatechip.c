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
load_fontset(cpu_t cpu)
{
	for (int i = 0; i < 80; i++)
		cpu->memory[i]
}
	
int
load_rom(cpu_t cpu, char *romfile)
{
	if (!(cpu->rom = fopen(romfile, "rb"))) {
		printf("Invalid ROM filename: %s", romfile);
		exit(1);
	}

	/* Load ROM into memory here */
}
