cmake_minimum_required(VERSION 3.13)
project(PPP_2020 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(PPP_2020 main.c structs.h Pilha_alunos.c Pilha_alunos.h Pilha_notas_alunos.c Pilha_notas_alunos.h structs.h Pilha_disciplinas.c Pilha_disciplinas.h)