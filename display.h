/* 
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * display.h
 *
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <SDL2/SDL.h>

#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT  32
#define SCALE          20
#define NUM_PIXELS     2048

#define WHITE          4294967295
#define BLACK          0

struct display {
	SDL_Window    *win;
	SDL_Renderer  *renderer;
	SDL_Texture   *texture;

	uint32_t pixels[NUM_PIXELS];
};

void 	init_display(struct display *);
void 	update_display(struct display *);
void 	kill_display(struct display *);

#endif
