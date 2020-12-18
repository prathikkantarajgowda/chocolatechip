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
 *  display.c
 */

#include "display.h"

static void init_sdl(void);
static void create_window(struct display *);
static void create_renderer(struct display *);
static void create_texturer(struct display *);

void
init_display(struct display *screen)
{
	clear_display(screen);
	init_sdl();
	create_window(screen);
	create_renderer(screen);
	create_texturer(screen);
	SDL_RenderClear(screen->renderer);

	(void)printf("SDL, window, renderer, and texture successfully initialized\n");
	update_display(screen);
}

void
clear_display(struct display *screen)
{
	(void)memset(screen->pixels, BLACK, NUM_PIXELS * 4);
}

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

static void
init_sdl(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		(void)fprintf(stderr, "Could not initialize SDL: %s.\n",
		    SDL_GetError());
		exit(1);
	}

}

static void
create_window(struct display *screen)
{
	screen->win = SDL_CreateWindow("chocolatechip", SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH*SCALE, SCREEN_HEIGHT*SCALE, 0);

	if (!screen->win) {
		(void)fprintf(stderr, "Could not create window: %s.\n",
		    SDL_GetError());

		SDL_Quit();
		exit(1);
	}
}

static void
create_renderer(struct display *screen)
{
	screen->renderer = SDL_CreateRenderer(screen->win, -1,
	    SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	SDL_RenderSetLogicalSize(screen->renderer, SCREEN_WIDTH*SCALE,
	    SCREEN_HEIGHT*SCALE);
	if (!screen->renderer) {
		SDL_DestroyWindow(screen->win);

		(void)fprintf(stderr, "Could not create renderer: %s.\n",
		    SDL_GetError());
		exit(1);
	}

}

static void
create_texturer(struct display *screen)
{
	screen->texture = SDL_CreateTexture(screen->renderer,
	    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
	    SCREEN_WIDTH, SCREEN_HEIGHT);

	if (!screen->texture) {
		SDL_DestroyWindow(screen->win);
		SDL_DestroyRenderer(screen->renderer);

		(void)fprintf(stderr, "Could not create texture: %s.\n",
		    SDL_GetError());
		exit(1);
	}
}
