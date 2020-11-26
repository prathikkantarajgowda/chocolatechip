/* 
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * display.c
 *
 */

#include "display.h"

void
init_display(struct display *screen)
{
	int i, j;
	i = j = 0;

	/* Initializes pixels to black */
	memset(screen->pixels, BLACK, NUM_PIXELS * 4);
	
	/* Initializes SDL */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		fprintf(stderr, "Could not initialize SDL: %s.\n",
		    SDL_GetError());
		exit(1);
	}

	/* Creates window */
	screen->win = SDL_CreateWindow("chocolatechip", SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH*SCALE, SCREEN_HEIGHT*SCALE, 0);
	if (!screen->win) {
		fprintf(stderr, "Could not create window: %s.\n",
		    SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	/* Creates renderer */
	screen->renderer = SDL_CreateRenderer(screen->win, -1,
	    SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(screen->renderer, SCREEN_WIDTH*SCALE,
	    SCREEN_HEIGHT*SCALE);
	if (!screen->renderer) {
		SDL_DestroyWindow(screen->win);
		fprintf(stderr, "Could not create renderer: %s.\n",
		    SDL_GetError());
		exit(1);
	}

	/* Creates texture */
	screen->texture = SDL_CreateTexture(screen->renderer,
	    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
	    SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!screen->texture) {
		SDL_DestroyWindow(screen->win);
		SDL_DestroyRenderer(screen->renderer);
		fprintf(stderr, "Could not create texture: %s.\n",
		    SDL_GetError());
		exit(1);
	}

	SDL_RenderClear(screen->renderer);

	printf("SDL, window, renderer, and texture successfully initialized\n");

	for (i = 0; i < 2048; i++)
		if (i % 2 == 0)
			screen->pixels[i] = WHITE;

	update_display(screen);

	SDL_Delay(5000);
}

/*
 * Updates display after there has been a chance to the pixels array
 */
void
update_display(struct display *screen)
{
	SDL_UpdateTexture(screen->texture, NULL, screen->pixels,
	    SCREEN_WIDTH * 4);
	SDL_RenderClear(screen->renderer);
	SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
	SDL_RenderPresent(screen->renderer);
}

void
kill_display(struct display *screen)
{
	SDL_DestroyWindow(screen->win);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyTexture(screen->texture);
	SDL_Quit();
}

