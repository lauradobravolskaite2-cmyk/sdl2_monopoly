cmake_minimum_required(VERSION 3.20)
project(sdl3_app C)

set(CMAKE_C_STANDARD 17)

# Point CMake to your SDL3 build/install
set(SDL3_DIR "$ENV{HOME}/SDL3/build")

find_package(SDL3 REQUIRED)

add_executable(sdl3_app src/main.c)
target_link_libraries(sdl3_app PRIVATE SDL3::SDL3)
