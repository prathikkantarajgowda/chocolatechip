/*
 * chocolatechip: a Chip-8 emulator written in C99 by Prathik Gowda
 *
 * cpu.c
 *
 * completed opcodes:
 * 	- 0x0NNN
 * 	- 0x00E0
 *	- 0x1NNN
 *	- 0x6XNN
 *	- 0x7XNN
 *	- 0xANNN
 *
 * todo:
 * 	- 0xDXYN
 * 	- 0x00EE
 * 	- 0x2NNN
 * 	- 0x3XNN
 * 	- 0x4XNN
 * 	- 0x5XY0
 * 	- 0x9XY0
 * 	- 0x8XY0
 * 	- 0x8XY1
 * 	- 0x8XY2
 * 	- 0x8XY3
 * 	- 0x8XY4
 * 	- 0x8XY5
 * 	- 0x8XY6
 * 	- 0x8XY7
 * 	- 0x8XYE
 * 	- 0xBNNN
 * 	- 0xCXNN
 * 	- 0xEX9E
 * 	- 0xEXA1
 * 	- 0xFX07
 * 	- 0xFX15
 * 	- 0xFX18
 * 	- 0xFX1E
 * 	- 0xFX0A
 * 	- 0xFX29
 * 	- 0xFX33
 * 	- 0xFX55
 * 	- 0xFX65
 */

#include <string.h>

#include "cpu.h"

static const uint8_t chip8_fontset[80] = {
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
	/* Clears memory, stack, V registers, keypad, and quit_flag */
	(void)memset(chip8->memory, 0, 4096);
	(void)memset(chip8->stack, 0, 16*2);
	(void)memset(chip8->V, 0, 16);
	(void)memset(chip8->keypad, 0, 16);
	chip8->quit_flag = 0;

	/* Loads fontset into memory */
	(void)memcpy(chip8->memory, chip8_fontset, 80);

	/* Opens ROM */
	if (!(chip8->rom = fopen(romfile, "rb"))) {
		(void)fprintf(stderr, "Invalid ROM filename: %s\n", romfile);
		exit(1);
	}

	(void)printf("%s rom successfully loaded\n", romfile);

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
	uint16_t opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC+1];
	chip8->PC += 2;

	return opcode;
}

void
decode_execute(struct cpu *chip8, struct display *screen, uint16_t opcode)
{
	/*
	 * x: the second nibble. A 4-bit value, the lower 4 bits of the
	 * high bit instruction
	 *
	 * y: the third nibble. A 4-bit value, the upper 4 bits of the low byte
	 * of the instruction
	 *
	 */

	uint8_t		x = opcode & 0x0F00 >> 8;
	uint8_t 	y = opcode & 0x00F0 >> 4;

	switch (opcode & 0xF000) { /* first nibble */
	case 0x0000:
		switch (opcode & 0x000F) {
		case 0x00E0:
			clear_display(screen);
			update_display(screen);
			break;
		}
		break;
	case 0x1000:
		chip8->PC = opcode & 0x0FFF;
		break;
	case 0x2000:
		break;
	case 0x3000:
		break;
	case 0x4000:
		break;
	case 0x5000:
		break;
	case 0x6000:
		chip8->V[x] = opcode & 0x00FF;
		break;
	case 0x7000:
		chip8->V[y] += opcode & 0x00FF;
		break;
	case 0x8000:
		break;
	case 0x9000:
		break;
	case 0xA000:
		chip8->I = opcode & 0x0FFF;
		break;
	case 0xB000:
		break;
	case 0xC000:
		break;
	case 0xD000:
		break;
	case 0xE000:
		break;
	case 0xF000:
		break;
	}
}

void
update_timers(struct cpu *chip8)
{
	if (chip8->delay > 0)
		chip8->delay--;
	if (chip8->sound > 0) {
		chip8->sound--;
		(void)printf("beep\n");
	}
}
