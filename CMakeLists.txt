cmake_minimum_required(VERSION 3.13)
project(PPP_2020 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(PPP_2020 main.c structs.h Pilhas.c Pilhas.h structs.h)