cmake_minimum_required(VERSION 4.2)
project(sim3 C)

set(CMAKE_C_STANDARD 11)
link_libraries(raylib m)

add_executable(sim3 main.c
        GUI/Grid.h
        GUI/Grid.c
        GUI/GUI.h
        GUI/Input.c
        GUI/Input.h)
