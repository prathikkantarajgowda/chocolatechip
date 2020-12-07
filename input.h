/*
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * input.h
 *
 */

#ifndef _input_h_
#define _input_h_

#define	NUM_KEYS	16

static const uint8_t KEY_CODES[NUM_KEYS];

void	keyboard_input(struct cpu *chip8);

#endif
