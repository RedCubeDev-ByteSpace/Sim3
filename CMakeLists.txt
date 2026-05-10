cmake_minimum_required(VERSION 4.2)
project(sim3 C)

set(CMAKE_C_STANDARD 11)
link_libraries(raylib)

add_executable(sim3 main.c)
