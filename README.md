# chocolatechip

A CHIP-8 Emulator written in C99 by Prathik Gowda

![screenshot](/screenshot.png)

## Overview
- The opcodes are handled by a simple switch case in the cpu.c file.
- Display and input are both handled by SDL2 graphics library
	- I use an integer array of size (64 x 32) to represent the display.
		- This is drawn onto an SDL window using a Texture and Renderer.
- Style loosely follows OpenBSD KNF
- Compilation, installation, and uninstallation handled by makefile

## Bugs/Issues
- Timing is off depending on the ROM. For example, Pong runs at a normal speed,
  but Space Invaders runs extremely fast.
- Audio is currently unimplemented (it "beeps" to stdout instead)

## Installation

- Prerequisites: git, make, SDL2, and either clang or gcc
- OS: Program is intended for UNIX/UNIX-likes. Tested on Debian 10. Makefile
should be BSD compatible (tested with FreeBSD's bmake on Linux)

1. Clone repository
```
$ git clone https://gitlab.com/prathikgowda/chocolatechip.git
```
2. Move into the project folder
```
cd chocolatechip
```
3.  Edit makefile "CC" macro match your favorite C compiler
```
vim makefile
```
4. Install (makefile installs to /usr/local/bin, requiring root privilages)
```
# make clean install
```

## Uninstallation
```
# make uninstall
```

## Usage
Run chocolatechip with ROM file as argument:
```
$ chocolatechip SpaceInvaders.ch8

```
To remove object files and executeables from the repository directory:
```
$ make clean
```

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
