cmake_minimum_required(VERSION 3.18.4)

# Set the project
project(Chip8Emulator)

# Find the libraries
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(Threads REQUIRED)


# Set to includes for the whole project
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(src)

# Set the compile flags
add_compile_options(-std=c++11 -Wall -Wextra -g -pthread)


# Create the code library
add_library(LIBChip8Emulator 
                            src/TimedRegisters.cpp
                            src/screen.cpp
                            src/RAM.cpp
                            src/fileLoader.cpp
                            src/CPU.cpp
                         
                            
        )

# Create the final binary
add_executable(Chip8Emulator src/main.cpp)       
target_link_libraries(Chip8Emulator PRIVATE LIBChip8Emulator Threads::Threads SDL2::SDL2main SDL2::SDL2)


# Add a custom target for demo
add_custom_target(run
    COMMAND ./${PROJECT_NAME} ../roms/trip8_demo.ch8
    DEPENDS Chip8Emulator
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)