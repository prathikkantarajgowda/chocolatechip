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
 *  cpu.h
 */

#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>
#include <stdio.h>

#include "display.h"

struct cpu {
	FILE     *rom;

	uint8_t  delay;
	uint8_t  sound;
	uint8_t  memory[4096];
	uint8_t  SP;
	uint8_t  V[16];

	uint16_t I;
	uint16_t opcode;
	uint16_t PC;
	uint16_t stack[16];

	uint8_t keypad[16];
};

void init_cpu(struct cpu *, char *);
void cycle(struct cpu *, struct display *);

#endif
