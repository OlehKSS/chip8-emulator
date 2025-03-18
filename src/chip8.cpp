#include "chip8.h"

#include <assert.h>
#include <format>
#include <fstream>
#include <print>
#include <random>

// Function to generate a random byte
uint8_t get_rand() {
    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister random number engine
    std::uniform_int_distribution<uint8_t> dis(0, 255); // Distribution range [0, 255]

    return dis(gen);
}

void Chip8::initialize() 
{
//   pc     = 0x200;  // Program counter starts at 0x200
//   opcode = 0;      // Reset current opcode	
//   I      = 0;      // Reset index register
//   sp     = 0;      // Reset stack pointer
 
  // Clear display	
  // Clear stack
  // Clear registers V0-VF
  // Clear memory

  // Load fontset
  // In the article its 0 to 80
  for (int i = 0x050; i < 0x0A0; ++i)
  {
    // memory[i] = chip8_fontset[i - 0x050];
  }

  // Reset timers
}

void Chip8::emulateCycle()
{
    // Fetch 2 bytes constituting an opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
            // Clear the sceen
            for (auto& pixel : gfx)
            {
                pixel = 0;
            }
            pc += 2;
            break;
        case 0x00EE:
            // Returns from a subroutine.
            // Anything else needed?
            sp--;
            pc = stack[sp];
            break;
        default:
            std::println("Unknown opcode: {:X}", opcode);
            break;
        }
        break;
    case 0x1000:
        // goto
        pc = opcode & 0x0FFF;
        assert(pc >= 0x200);
        break;
    case 0x2000:
        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000:
    {
        auto vx = V[(opcode & 0x0F00) >> 8];
        auto value = opcode & 0x0FF;
        pc += (vx == value) ? 4 : 2;
        break;
    }
    case 0x4000:
    {
        auto vx = V[(opcode & 0x0F00) >> 8];
        auto value = opcode & 0x0FF;
        pc += (vx != value) ? 4 : 2;
        break;
    }
    case 0x5000:
    {
        auto vx = V[(opcode & 0x0F00) >> 8];
        auto vy = V[(opcode & 0x00F0) >> 4];
        pc += (vx == vy) ? 4 : 2;
        break;
    }
    case 0x6000:
        V[(opcode & 0x0F00) >> 8] = opcode & 0x0FF;
        pc += 2;
        break;
    case 0x7000:
        V[(opcode & 0x0F00) >> 8] += opcode & 0x0FF;
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0001:
            V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0002:
            V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0003:
            V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0004:
        {
            uint16_t sum = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
            V[0xF] = (sum & 0x0F00) >> 8; // carry
            V[(opcode & 0x0F00) >> 8] = static_cast<uint8_t>(sum);
            pc += 2;
            break;
        }
        case 0x0005:
        {
            uint16_t diff = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
            V[0xF] = (diff >= 0) ? 1 : 0; // carry, set VF = 1 if VX >= VY else 0 
            V[(opcode & 0x0F00) >> 8] = static_cast<uint8_t>(diff);
            pc += 2;
            break;
        }
        case 0x0006:
        {
            auto vx = V[(opcode & 0x0F00) >> 8];
            V[(opcode & 0x0F00) >> 8] = vx >> 1;
            // Check endianess
            V[0xF] = vx & 1;
            pc += 2;
            break;
        }
        case 0x0007:
        {
            auto vx = V[(opcode & 0x0F00) >> 8];
            auto vy = V[(opcode & 0x00F0) >> 4];
            uint16_t diff = vy - vx;
            V[0xF] = (diff >= 0) ? 1 : 0; // carry, set VF = 1 if VY >= VX else 0 
            V[(opcode & 0x0F00) >> 8] = static_cast<uint8_t>(diff);
            pc += 2;
            break;
        }
        case 0x000E:
        {
            auto vx = V[(opcode & 0x0F00) >> 8];
            V[(opcode & 0x0F00) >> 8] = vx << 1;
            // Check endianess
            V[0xF] = (vx & 0x80) >> 7; // Get the most significant bit
            pc += 2;
            break;
        }
        default:
            std::println("Unknown opcode: {:X}", opcode);
            break;
        }
        break;
    case 0x9000:
    {
        auto vx = V[(opcode & 0x0F00) >> 8];
        auto vy = V[(opcode & 0x00F0) >> 4];
        pc += (vx != vy) ? 4 : 2;
        break;
    }
    case 0xA000: // ANNN: Sets I to the address NNN
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    case 0xB000:
        // goto
        pc = V[0] + (opcode & 0x0FFF);
        assert(pc >= 0x200);
        assert(pc <= 0xFFF);
        break;
    case 0xC000:
        V[(opcode & 0x0F00) >> 8] = get_rand() & (opcode & 0x00FF);
        pc += 2;
        break;
    case 0xD000:
    {
        uint16_t x = V[(opcode & 0x0F00) >> 8];
        uint16_t y = V[(opcode & 0x00F0) >> 4];
        uint16_t height = opcode & 0x000F;
        uint16_t pixel;

        V[0xF] = 0;

        for (int yline = 0; yline < height; ++yline)
        {
            pixel = memory[I + yline];

            for (int xline = 0; xline < 8; ++xline)
            {
                // Why does this work? shouldn't be 0xF
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if (gfx[(x + xline + ((y + yline) * 64))] == 1)
                    {
                        V[0xF] = 1;
                    }
                    gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }

        drawFlag = true;
        pc += 2;

        break;
    }
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            pc += (key[V[(opcode & 0x0F00) >> 8]] != 0) ? 4 : 2;
            break;
        case 0x00A1:
            pc += (key[V[(opcode & 0x0F00) >> 8]] == 0) ? 4 : 2;
            break;
        default:
            std::println("Unknown opcode: {:X}", opcode);
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007:
            V[(opcode & 0x0F00) >> 8] = delay_timer;
            pc += 2;
            break;
        case 0x000A:
        {
            // A key press is awaited, and then stored in VX 
            // (blocking operation, all instruction halted until next key event,
            // delay and sound timers should continue processing)
            // How to simulate a key press?
            // V[(opcode & 0x0F00) >> 8] = get_key();
            std::println("Opcode not implemented: {:X}", opcode);
            break;
        }
        case 0x0015:
            delay_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x0018:
            sound_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x001E:
            I += V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x0029:
            // Non implemented
            // Sets I to the location of the sprite for the character in VX(only consider the lowest nibble).
            // Characters 0-F (in hexadecimal) are represented by a 4x5 font.[24]
            // I = sprite_addr[Vx]
            std::println("Opcode not implemented: {:X}", opcode);
            break;
        case 0x0033:
        {
            const auto reg_value = V[(opcode & 0x0F00) >> 8]; 
            memory[I] = (reg_value / 100) % 10;
            memory[I + 1] = (reg_value / 10) % 10;
            memory[I + 2] = reg_value % 10;
            break;
        }
        case 0x0055:
        {
            for (int i = 0; i < V.size(); ++i)
            {
                memory[I + i] = V[i];
            }
            break;
        }
        case 0x0065:
        {
            for (int i = 0; i < V.size(); ++i)
            {
                V[i] = memory[I + i];
            }
            break;
        }     
        default:
            std::println("Unknown opcode: {:X}", opcode);
            break;
        }
        break;    
    default:
        std::println("Unknown opcode: {:X}", opcode);
        break;
    }

    if (delay_timer > 0)
    {
        delay_timer--;
    }

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
        {
            std::println("BEEP!");
        }

        sound_timer--;
    }
}

void Chip8::loadGame(const std::string& name)
{
    constexpr auto maxBufferSize = 0xFFF - 0x200;
    std::fstream fs(name, std::ios::binary | std::ios::in | std::ios::ate);

    if (!fs.is_open())
    {
        throw std::runtime_error("Failed to open game " + name);
    }

    auto size = fs.tellg();

    if (size > maxBufferSize)
    {
        throw std::runtime_error(std::format("Buffer size is too large {}", std::to_string(size)));
    }

    fs.seekg(0, std::ios::beg);

    std::string buffer(size, '\0');

    fs.read(&buffer[0], size);

    for (int i = 0; i < size; ++i)
    {
        memory[i + 0x200] = buffer[i];
    }
}