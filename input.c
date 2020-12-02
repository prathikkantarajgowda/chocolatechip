/*
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * input.c
 *
 */

#include <SDL2/SDL.h>

void
keyboard_input()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_1:
				printf("1 pressed\n");
				break;
			case SDLK_2:
				printf("2 pressed\n");
				break;
			case SDLK_3:
				printf("3 pressed\n");
				break;
			case SDLK_4:
				printf("4 pressed\n");
				break;
			case SDLK_q:
				printf("Q pressed\n");
				break;
			case SDLK_w:
				printf("W pressed\n");
				break;
			case SDLK_e:
				printf("E pressed\n");
				break;
			case SDLK_r:
				printf("R pressed\n");
				break;
			default:
				break;
			}
			break;
		}

	}
}

