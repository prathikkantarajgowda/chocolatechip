/* 
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * chocolatechip.c 
 *
 *
 *
 * completed:
 *	 - chip8 cpu data structure
 *	 - SDL display data structure
 *	 - initialization function for CPU
 *	 - initialization function for display (SDL)
 *
 * todo: 
 *	 - keyboard
 *	 - sound (just need beep)
 *	 - get/fetch/execute
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT  32
#define SCALE          30
#define NUM_PIXELS     2048

#define WHITE          4294967295
#define BLACK          0

static uint8_t chip8_fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
        0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
        0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};


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
};

struct display {
	SDL_Window    *win;
	SDL_Renderer  *renderer;
	SDL_Texture   *texture;

	uint32_t pixels[NUM_PIXELS];
};

static void init_cpu(struct cpu *, char *);
static void init_display(struct display *);
static void kill_display(struct display *);
static void update_display(struct display *);
static void update_timers(struct cpu *);
static void cycle(struct cpu *);

int
main(int argc, char **argv)
{
	struct cpu      chip8;
	struct display  screen;

	if (argc != 2) {
		fprintf(stderr,
		    "One ROM file needed. Usage: chocolatechip rom.ch8\n");
		return 1;
	}

	init_cpu(&chip8, argv[1]);
	init_display(&screen);

	kill_display(&screen);

	return 0;
}

static void
init_cpu(struct cpu *chip8, char *romfile)
{
	/* Clears memory, stack, and V registers */
	memset(chip8->memory, 0, 4096);
	memset(chip8->stack, 0, 16);
	memset(chip8->V, 0, 16);
	
	/* Loads fontset into memory */
	memcpy(chip8->memory, chip8_fontset, 80);


	/* Opens ROM */
	if (!(chip8->rom = fopen(romfile, "rb"))) {
		fprintf(stderr, "Invalid ROM filename: %s\n", romfile);
		exit(1);
	}

	printf("%s rom successfully loaded\n", romfile);

	/* Load game ROM into memory starting from 0x200 (512 in decimal) */
	fread(chip8->memory + 0x200, 1, 4096 - 0x200, chip8->rom);
	fclose(chip8->rom);

	/* Sets program counter to point at the first ROM instruction */
	chip8->PC = 0x200;

	/* Initializes delay and sound to zero */
	chip8->delay = 0;
	chip8->sound = 0;
}


static void
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
static void
update_display(struct display *screen)
{
	SDL_UpdateTexture(screen->texture, NULL, screen->pixels,
	    SCREEN_WIDTH * 4);
	SDL_RenderClear(screen->renderer);
	SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
	SDL_RenderPresent(screen->renderer);
}

static void
kill_display(struct display *screen)
{
	SDL_DestroyWindow(screen->win);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyTexture(screen->texture);
	SDL_Quit();
}

static void
update_timers(struct cpu *chip8)
{
	if (chip8->delay > 0)
		chip8->delay--;
	if (chip8->sound > 0) {
		chip8->sound--;
		printf("beep\n");
	}
}

static void
cycle(struct cpu *chip8)
{
	update_timers(chip8);
}
