cmake_minimum_required(VERSION 3.20)

project(Chip8Emulators)

set(CMAKE_CXX_STANDARD 23)

add_executable(chip8emu)

target_include_directories(chip8emu PRIVATE src)
target_sources(chip8emu PRIVATE
    src/main.cpp
    src/chip8.h
    src/chip8.cpp
)
