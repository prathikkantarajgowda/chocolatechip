/*
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * input.c
 *
 */

#include <stdint.h>

#include <SDL2/SDL.h>

#include "input.h"

#define	NUM_KEYS	16

static const uint8_t KEY_CODES[NUM_KEYS] = {
	SDLK_1, SDLK_2, SDLK_3, SDLK_4,
	SDLK_q, SDLK_w, SDLK_e, SDLK_r,
	SDLK_a, SDLK_s, SDLK_d, SDLK_f,
	SDLK_z, SDLK_x, SDLK_c, SDLK_v,
};

void
keyboard_input(struct cpu *chip8)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			chip8->quit_flag = 1;
			return;
		}
		if (event.type == SDL_KEYDOWN) {
			printf("key pressed");
			for (int i = 0; i < NUM_KEYS; i++)
				if (event.key.keysym.sym == KEY_CODES[i])
					chip8->keypad[i] = 1;
		}
		if (event.type == SDL_KEYUP)
			for (int i = 0; i < NUM_KEYS; i++)
				if (event.key.keysym.sym == KEY_CODES[i])
					chip8->keypad[i] = 1;
	}
}
