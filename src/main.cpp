#include <iostream>

#include "chip8.h"

int main(int argc, char** argv)
{
    Chip8 myChip8;

    // setupGraphics();
    // setupInput();

    myChip8.initialize();
    myChip8.loadGame("pong");

    std::cout << "Created an emulator.\n";

    // Emulation loop
    // for (;;)
    // {
    //     myChip8.emulateCycle();

    //     // if (myChip8.drawFlag)
    //     // {
    //     //     drawGraphics();
    //     // }

    //     // Store key press state (Press and Release)
    //     myChip8.setKyes();
    // }

    return 0;
}
