# chocolatechip

A Chip-8 Emulator by Prathik 

## Specifications

- Memory: 4K of RAM
    - Declaration: uint8_t memory[4096]; 
- Display: 64 x 32 pixels
    - Unsure of implementation here. Options include ncurses, Raylib, and SDL2
- Program Counter: Pointer to the current instruction in memory
    - Declaration: uint16_t PC;
- 16-Bit Index Register: Points towards locations in memory
    - Declaration: uint16_t I;
- Stack: Stack for 16-bit addresses. Used to call methods and return from them
    - Declaration: uint16_t stack[16];
- Delay Timer: 8-Bit timer which decrements 60 times per second until it reaches zero
    - Declaration: uint8_t delay;
- Sound Timer: Same as delay timer, but also beeps while not zero
    - Declaration: uint8_t sound;
- Variable Registers: 16 8-Bit variable registers numbered 0 through F hexadecimal, called V0 to VF