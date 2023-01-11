cmake_minimum_required(VERSION 3.23)
project(winter_play C)

set(CMAKE_C_STANDARD 11)

add_executable(winter_play main.c)
target_link_libraries(winter_play SDL2)
target_link_libraries(winter_play SDL2_image)
target_link_libraries(winter_play SDL2_ttf)
target_link_libraries(winter_play SDL2_mixer)