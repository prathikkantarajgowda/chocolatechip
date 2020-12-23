/*   chocolatechip: a CHIP-8 emulator written in C99
 *   Copyright (C) 2020  Prathik Gowda (gowdapra@grinnell.edu)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * cpu.c
 *
 */

#include <string.h>

#include "cpu.h"

static const uint8_t fontset[80] = {
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

static uint16_t fetch(struct cpu *);
static void 	decode_execute(struct cpu *, struct display *, uint16_t);
static void 	op_DXYN(struct cpu *, struct display *, uint8_t, uint8_t, uint8_t);
static void 	op_FX0A(struct cpu *, uint8_t);
static void 	op_FX33(struct cpu *, uint8_t);
static void 	op_FX55(struct cpu *, uint8_t);
static void 	op_FX65(struct cpu *, uint8_t);
static void 	op_error(struct display *, uint16_t);
static void	update_timers(struct cpu *);

void
init_cpu(struct cpu *chip8, char *romfile)
{
	/* Clears memory, stack, V, keypad */
	(void)memset(chip8->memory, 0, 4096);
	(void)memset(chip8->stack, 0, 16 * 2);
	(void)memset(chip8->V, 0, 16);
	(void)memset(chip8->keypad, 0, 16);

	/* Loads fontset */
	(void)memcpy(chip8->memory, fontset, 80);

	if (!(chip8->rom = fopen(romfile, "rb"))) {
		(void)fprintf(stderr, "invalid ROM filename: %s\n", romfile);
		exit(EXIT_FAILURE);
	}

	(void)printf("%s rom successfully loaded\n", romfile);

	/* Load game ROM into memory starting from 0x200 */
	(void)fread(chip8->memory + 0x200, 1, 4096 - 0x200, chip8->rom);
	(void)fclose(chip8->rom);

	chip8->PC = 0x200;

	chip8->delay = 0;
	chip8->sound = 0;
}

void
cycle(struct cpu *chip8, struct display *screen)
{
	uint8_t i;

	for (i = 0; i <= 9; i++)
		decode_execute(chip8, screen, fetch(chip8));

	update_timers(chip8);
}

static uint16_t
fetch(struct cpu *chip8)
{
	uint16_t opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC+1];
	chip8->PC += 2;

	return opcode;
}

static void
decode_execute(struct cpu *chip8, struct display *screen, uint16_t opcode)
{
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;
	uint8_t nn = opcode & 0x00FF;
	uint16_t nnn = opcode & 0x0FFF;

	switch (opcode & 0xF000) {
	case 0x0000:
		switch (opcode & 0x00FF) {
		case 0x00E0:
			clear_display(screen);
			update_display(screen);
			break;
		case 0x00EE:
			chip8->PC = chip8->stack[--chip8->SP];
			break;
		default:
			op_error(screen, opcode);
		}
		break;
	case 0x1000:
		chip8->PC = nnn;
		break;
	case 0x2000:
		chip8->stack[(chip8->SP++) & 0xF] = chip8->PC;
		chip8->PC = nnn;
		break;
	case 0x3000:
		if (chip8->V[x] == nn)
			chip8->PC += 2;
		break;
	case 0x4000:
		if (chip8->V[x] != nn)
			chip8->PC += 2;
		break;
	case 0x5000:
		if (chip8->V[x] == chip8->V[y])
			chip8->PC += 2;
		break;
	case 0x6000:
		chip8->V[x] = nn;
		break;
	case 0x7000:
		chip8->V[x] += nn;
		break;
	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0000:
			chip8->V[x] = chip8->V[y];
			break;
		case 0x0001:
			chip8->V[x] |= chip8->V[y];
			break;
		case 0x0002:
			chip8->V[x] &= chip8->V[y];
			break;
		case 0x0003:
			chip8->V[x] ^= chip8->V[y];
			break;
		case 0x0004:
			if ((chip8->V[x] + chip8->V[y]) > 255)
				chip8->V[0xF] = 1;
			else
				chip8->V[0xF] = 0;

			chip8->V[x] += chip8->V[y];
			break;
		case 0x0005:
			if (chip8->V[x] > chip8->V[y])
				chip8->V[0xF] = 1;
			if (chip8->V[x] < chip8->V[y])
				chip8->V[0xF] = 0;

			chip8->V[x] -= chip8->V[y];
			break;
		case 0x0006:
			if ((chip8->V[x] = chip8->V[y]) & 0x1)
				chip8->V[0xF] = 1;
			else
				chip8->V[0xF] = 0;

			chip8->V[x] >>= 1;
			break;
		case 0x0007:
			if (chip8->V[y] > chip8->V[x])
				chip8->V[0xF] = 1;
			if (chip8->V[y] < chip8->V[x])
				chip8->V[0xF] = 0;

			chip8->V[y] -= chip8->V[x];
			break;
		case 0x000E:
			if ((chip8->V[x] = chip8->V[y]) & 0x80)
				chip8->V[0xF] = 1;
			else
				chip8->V[0xF] = 0;

			chip8->V[x] <<= 1;
			break;
		default:
			op_error(screen, opcode);
		}
		break;
	case 0x9000:
		if (chip8->V[x] != chip8->V[y])
			chip8->PC += 2;
		break;
	case 0xA000:
		chip8->I = nnn;
		break;
	case 0xB000:
		chip8->PC = chip8->V[0x0] + nnn;
		break;
	case 0xC000:
		chip8->V[x] = (rand() % 256) & 0x00FF;
		break;
	case 0xD000:
		op_DXYN(chip8, screen, chip8->V[x] % SCREEN_WIDTH, chip8->V[y] % SCREEN_HEIGHT, opcode & 0x000F);
		break;
	case 0xE000:
		switch (opcode & 0x00FF) {
		case 0x009E:
			if (chip8->keypad[chip8->V[x]])
				chip8->PC += 2;
			break;
		case 0x00A1:
			if (!chip8->keypad[chip8->V[x]])
				chip8->PC += 2;
			break;
		default:
			op_error(screen, opcode);
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0007:
			chip8->V[x] = chip8->delay;
			break;
		case 0x000A:
			op_FX0A(chip8, x);
			break;
		case 0x0015:
			chip8->delay = chip8->V[x];
			break;
		case 0x0018:
			chip8->sound = chip8->V[x];
			break;
		case 0x001E:
			if ((chip8->I += chip8->V[x]) > 1000)
				chip8->V[0xF] = 1;
			break;
		case 0x0029:
			chip8->I = chip8->V[x] * 5;
			break;
		case 0x0033:
			op_FX33(chip8, chip8->V[x]);
			break;
		case 0x0055:
			op_FX55(chip8, x);
			break;
		case 0x0065:
			op_FX65(chip8, x);
			break;
		default:
			op_error(screen, opcode);
		}
		break;
	default:
		op_error(screen, opcode);
	}
}

static void
op_DXYN(struct cpu *chip8, struct display *screen, uint8_t x, uint8_t y, uint8_t height)
{
	uint8_t i, j, pixel;

	chip8->V[0xF] = 0;

	for (i = 0; i < height; i++) {
		pixel = chip8->memory[i + chip8->I];

		for (j = 0; j < 8; j++) {
			if (pixel & (0x80 >> j)) {
				if (screen->pixels[(x+j) + ((y+i) * SCREEN_WIDTH)])
					chip8->V[0xF] = 1;

				screen->pixels[(x+j) + ((y+i) * SCREEN_WIDTH)] ^= WHITE;
			}
		}
	}
	update_display(screen);
}

static void
op_FX0A(struct cpu *chip8, uint8_t x)
{
	uint8_t i, key_pressed;
	key_pressed = 0;

	for (i = 0; i < 16; i++) {
		if (chip8->keypad[i]) {
			chip8->V[x] = i;
			key_pressed = 1;
			break;
		}
	}

	if (!key_pressed)
		chip8->PC -= 2;
}

static void
op_FX33(struct cpu *chip8, uint8_t vx)
{
	int8_t i;

	for (i = 2; i >= 0; i--) {
		chip8->memory[chip8->I+i] = vx % 10;
		vx /= 10;
	}
}

static void
op_FX55(struct cpu *chip8, uint8_t x)
{
	uint8_t i;

	for (i = 0; i <= x; i++)
		chip8->memory[chip8->I+i] = chip8->V[i];
}

static void
op_FX65(struct cpu *chip8, uint8_t x)
{
	uint8_t i;

	for (i = 0; i <= x; i++)
		chip8->V[i] = chip8->memory[chip8->I + i];
}

static void
op_error(struct display *screen, uint16_t opcode)
{
	(void)fprintf(stderr, "unsupported opcode: %x\n", opcode);
	kill_display(screen);
	exit(EXIT_FAILURE);
}

static void
update_timers(struct cpu *chip8)
{
	if (chip8->delay > 0)
		chip8->delay--;

	if (chip8->sound > 0) {
		chip8->sound--;
		(void)printf("beep\n");
	}
}
