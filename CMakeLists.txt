cmake_minimum_required(VERSION 3.20)

project(Chip8Emulators)

set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(chip8emu)

target_include_directories(chip8emu PRIVATE src)

target_sources(chip8emu PRIVATE
    src/main.cpp
    src/chip8.h
    src/chip8.cpp
)

target_link_libraries(chip8emu PRIVATE Qt::Core Qt::Widgets)
