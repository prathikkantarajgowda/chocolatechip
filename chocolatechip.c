/* 
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * chocolatechip.c 
 *
 * todo: 
 *	 - keyboard
 *	 - graphics (SDL2?)
 */

#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

static unsigned char chip8_fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct cpu_t {
	FILE *rom;

	uint8_t delay;
	uint8_t sound;
	uint8_t memory[4096]; 
	uint8_t SP;
	uint8_t V[16];

	uint16_t I;
	uint16_t opcode;     
	uint16_t PC;
	uint16_t stack[16];
};

static void init_cpu(struct cpu_t *, char *);
static void update_timers(struct cpu_t *);
static void tick(struct cpu_t *);

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
{
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
