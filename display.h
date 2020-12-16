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
 *  display.h
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <SDL2/SDL.h>

#define NUM_PIXELS	2048
#define	SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32
#define SCALE	        20
#define NUM_PIXELS	2048

#define WHITE	        4294967295
#define BLACK	        0

struct display {
	SDL_Window	*win;
	SDL_Renderer	*renderer;
	SDL_Texture	*texture;

	uint32_t	pixels[NUM_PIXELS];
};

void	init_display(struct display *);
void	clear_display(struct display *);
void	update_display(struct display *);
void	kill_display(struct display *);

#endif
