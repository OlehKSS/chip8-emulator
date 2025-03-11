#pragma once

#include <string>
#include <stdint.h>

class Chip8
{
public:
    void initialize();
    void loadGame(const std::string& name);
    void emulateCycle();
    void setKyes();

private:
    // The Chip 8 has 35 opcodes which are all two bytes long.
    uint16_t opcode = 0;
    // Chip 8 has 4KB memory
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    uint8_t memory[4096];
    // 15 8-bit general purpose registers V0, V1, ..., VE and carry flag (CF)
    uint8_t V[16]; 
    // Index register
    uint16_t I = 0;
    // Program counter (PC) can store value from 0x200 to 0xFFF
    uint16_t pc = 0x200;
    // Chip 8 has a black and white screen with 64x32 pixels
    uint8_t gfx[64 * 32];
    // Both timers are 60Hz
    uint8_t delay_timer = 0;
    // The system’s buzzer sounds whenever the sound timer reaches zero.
    uint8_t sound_timer = 0;
    uint16_t stack[16];
    uint16_t sp = 0;
    // Chip 8 has a HEX based keypad (0x0-0xF)
    uint8_t key[16];
};
