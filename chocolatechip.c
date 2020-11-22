/* 
 * chocolatechip: a Chip-8 emulator written by Prathik Gowda
 *
 * chocolatechip.c 
 *
 * todo: 
 *	 - keyboard
 *	 - graphics (SDL2?)
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  320

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
	FILE *rom;

	SDL_Surface *display;
	
	uint8_t delay;
	uint8_t sound;
	uint8_t memory[4096]; 
	uint8_t SP;
	uint8_t V[16];

	uint16_t I;
	uint16_t opcode;     
	uint16_t PC;
	uint16_t stack[16];
};

static void init_cpu(struct cpu *, char *);
static void init_display(struct cpu *chip8);
static void update_timers(struct cpu *);
static void cycle(struct cpu *);

int
main(int argc, char **argv)
{
	struct cpu chip8;

	if (argc != 2) {
		printf("One ROM file needed. Usage: chocolatechip rom.ch8\n");
		return 1;
	}

	init_cpu(&chip8, argv[1]);

	init_display(&chip8);

	return 0;
}

static void
init_cpu(struct cpu *chip8, char *romfile)
{
	/*
	 * todo: clear display?
	 */

	chip8->display = NULL;
	
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
init_display(struct cpu *chip8)
{
	SDL_Window *win;
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		fprintf(stderr, "Could not initialize SDL: %s.\n",
		    SDL_GetError());
		exit(1);
	}

	win = SDL_CreateWindow("chocolatechip", SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED, 640, 320, 0);

	if (!win) {
		fprintf(stderr, "Could not create window: %s.\n",
		    SDL_GetError());
		SDL_Quit();
		exit(1);
	}
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
