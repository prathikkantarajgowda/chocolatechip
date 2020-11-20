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
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
        0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
        0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

struct cpu {
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

static void init_cpu(struct cpu *, char *);
static void update_timers(struct cpu *);
static void tick(struct cpu *);

int
main(int argc, char **argv)
{
	struct cpu chip8;

	if (argc != 2) {
		printf("One ROM file needed. Usage: chocolatechip rom.ch8\n");
		return 1;
	}

	init_cpu(&chip8, argv[1]);

	return 0;
}

static void
init_cpu(struct cpu *chip8, char *romfile)
{
	/*
	 * todo: clear display?
	 */
	int i;
	
	/* Clears memory, stack, and V registers */
	memset(chip8->memory, 0, 4096);
	memset(chip8->stack, 0, 16);
	memset(chip8->V, 0, 16);
	
	/* Loads fontset into memory */
	for (i = 0; i < 80; i++)
		chip8->memory[i] = chip8_fontset[i];


	/* Opens ROM */
	if (!(chip8->rom = fopen(romfile, "rb"))) {
		fprintf(stderr, "Invalid ROM filename: %s", romfile);
		exit(1);
	}

	printf("%s rom successfully loaded\n", romfile);

	/* Load game ROM into memory starting from 0x200 (512 in decimal) */
	fread(chip8->memory + 0x200, 1, 4096 - 0x200, chip8->rom);
	fclose(chip8->rom);

	/* Sets program counter to point at the first ROM instruction */
	chip8->PC = 0x200;

	/* Initializes delay and sound to zero */
	chip8->delay = 0;
	chip8->sound = 0;
}

static void
update_timers(struct cpu *chip8)
{
	if (chip8->delay > 0)
		chip8->delay--;
	if (chip8->sound > 0) {
		chip8->sound--;
		printf("beep\n");
	}
}

static void
tick(struct cpu *chip8)
{
	update_timers(chip8);
}
