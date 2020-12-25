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
 *  input.c
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

/* gets user input and updates keypad */
void
keyboard_input(struct cpu *chip8, struct display *screen)
{
	uint8_t i;
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			kill_display(screen);
			exit(0);
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				kill_display(screen);
				exit(0);
			}

			for (i = 0; i < NUM_KEYS; i++)
				if (event.key.keysym.sym == KEY_CODES[i])
					chip8->keypad[i] = 1;
		}

		if (event.type == SDL_KEYUP)
			for (i = 0; i < NUM_KEYS; i++)
				if (event.key.keysym.sym == KEY_CODES[i])
					chip8->keypad[i] = 0;
	}
}
