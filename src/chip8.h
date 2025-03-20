#pragma once

#include <array>
#include <string>
#include <stdint.h>

class Chip8
{
public:
    static constexpr auto SCREEN_WIDTH = 64;
    static constexpr auto SCREEN_HEIGHT = 32;

    void initialize();
    void loadGame(const std::string& name);
    void emulateCycle();
    void setUpdateTimers();
    void debugRender();

public:
    // The Chip 8 has 35 opcodes which are all two bytes long.
    uint16_t opcode = 0;
    // Chip 8 has 4KB memory
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    uint8_t memory[4096];
    // 15 8-bit general purpose registers V0, V1, ..., VE and carry flag (CF)
    std::array<uint8_t, 16> V; 
    // Index register
    uint16_t I = 0;
    // Program counter (PC) can store value from 0x200 to 0xFFF
    uint16_t pc = 0x200;
    // Chip 8 has a black and white screen with 64x32 pixels
    std::array<uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> gfx;
    // Both timers are 60Hz
    uint8_t delay_timer = 0;
    // The system’s buzzer sounds whenever the sound timer reaches zero.
    uint8_t sound_timer = 0;
    std::array<uint16_t, 16> stack;
    uint16_t sp = 0;
    // Chip 8 has a HEX based keypad (0x0-0xF)
    // +-+-+-+-+
    // |1|2|3|4|
    // +-+-+-+-+
    // |Q|W|E|R|
    // +-+-+-+-+
    // |A|S|D|F|
    // +-+-+-+-+
    // |Z|X|C|V|
    // +-+-+-+-+
    std::array<uint8_t, 16> key;
    bool drawFlag = false;

    std::array<uint8_t, 80> chip8_fontset =
    { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    bool updateTimers = false;
};
