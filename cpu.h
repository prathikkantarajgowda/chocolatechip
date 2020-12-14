/*
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * cpu.h
 *
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
	uint8_t draw_flag;
};

void	        init_cpu(struct cpu *, char *);
void	        update_timers(struct cpu *);
uint16_t	fetch(struct cpu *);
void	        decode_execute(struct cpu *, struct display *, uint16_t);
void		draw(uint8_t, uint8_t, uint8_t, struct cpu *, struct display *);

#endif
