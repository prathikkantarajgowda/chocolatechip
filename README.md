# chocolatechip

A CHIP-8 Emulator written in C99 by Prathik Gowda (IN-PROGRESS)

## Overview
- The opcodes are handled by a simple switch case statement in the cpu.c file.
- Graphics and input are both handled by SDL2.
	- I use an integer array of size (64 x 32) to represent the display.
		- This is drawn onto an SDL window using a Texture and Renderer.
- The emulator is not finished yet - however the most difficult parts are
  finished, including the DXYN opcode. Currently will run the IBM ROM perfectly

## Resources Used
I used and recommend all three of these resources as guides/references for the
CHIP-8 system
- Guide to making a CHIP-8 emulator by Tobias V. Langhoff:
	- Link: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
	- Provides a general overview, does not provide any code. Good place to
	  start.
- Wikipedia Page on CHIP-8
	- Link: https://en.wikipedia.org/wiki/CHIP-8
	- Provides more history, but also explains specifications and opcodes. Includes minimal C "pseudocode", but also does not provide implementations
- Cowgod's CHIP-8 Technical Reference
	- Link: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
	- I didn't use this resource too much: for the most part Tobias' guide was sufficient. However, this reference is often recommended and may be more	     comprehensive than the previous sources.
