/*
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * cpu.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

uint8_t chip8_fontset[80] = {
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

void
init_cpu(struct cpu *chip8, char *romfile)
{
	/* Clears memory, stack, and V registers */
	(void)memset(chip8->memory, 0, 4096);
	(void)memset(chip8->stack, 0, 16);
	(void)memset(chip8->V, 0, 16);

	/* Loads fontset into memory */
	(void)memcpy(chip8->memory, chip8_fontset, 80);

	/* Opens ROM */
	if (!(chip8->rom = fopen(romfile, "rb"))) {
		(void)fprintf(stderr, "Invalid ROM filename: %s\n", romfile);
		exit(1);
	}

	printf("%s rom successfully loaded\n", romfile);

	/* Load game ROM into memory starting from 0x200 (512 in decimal) */
	(void)fread(chip8->memory + 0x200, 1, 4096 - 0x200, chip8->rom);
	(void)fclose(chip8->rom);

	/* Sets program counter to point at the first ROM instruction */
	chip8->PC = 0x200;

	/* Initializes delay and sound to zero */
	chip8->delay = 0;
	chip8->sound = 0;
}

uint16_t
fetch(struct cpu *chip8)
{
	uint16_t opcode;
	opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC+1];

	chip8->PC += 2;
	return opcode;
}

void
decode_execute(struct cpu *chip8, uint16_t opcode)
{

}

void
update_timers(struct cpu *chip8)
{
	if (chip8->delay > 0)
		chip8->delay--;
	if (chip8->sound > 0) {
		chip8->sound--;
		printf("beep\n");
	}
}
