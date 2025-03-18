cmake_minimum_required(VERSION 3.20)

project(Chip8Emulators)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(chip8emu)

target_include_directories(chip8emu PRIVATE 
    src
    ${OPENGL_INCLUDE_DIR} 
    ${GLUT_INCLUDE_DIR}
)

target_sources(chip8emu PRIVATE
    src/main.cpp
    src/chip8.h
    src/chip8.cpp
)

target_link_libraries(chip8emu PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
