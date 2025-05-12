cmake_minimum_required(VERSION 3.31)
project(PixelGameJam C)

set(CMAKE_C_STANDARD 23)

# Include FetchContent module
include(FetchContent)

# Declare Raylib as a dependency
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0  # You can specify the version you want
)

# Make Raylib available
FetchContent_MakeAvailable(raylib)

# Add your executable
add_executable(PixelGameJam main.c Game/Game.c
        Player/Player.c)

# Link Raylib to your executable
target_link_libraries(PixelGameJam PRIVATE raylib)