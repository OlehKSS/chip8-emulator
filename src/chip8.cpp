#include "chip8.h"

#include <format>
#include <fstream>

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
    // We stopped at Cycle emulation
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