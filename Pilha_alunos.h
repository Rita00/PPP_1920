#ifndef PPP_2020_PILHA_H
#define PPP_2020_PILHA_H

#include <stdio.h>
#include "Pilha_notas_alunos.h"
#include "structs.h"

/*Interface para a pilha que guarda a informacao do ficheiro de entrada "Lista_alunos_num_est.txt"*/
aluno aloc_aluno(char *nome, int num);

void push_aluno(char *nome, int num);

void read_alunos_num(FILE *input_file);

void print_lista_alunos();

void destroi_pilha_alunos(char destroi_info);

aluno pesquisa_aluno(long num_aluno);

pilha_alunos remove_aluno(aluno pop_aluno);

void validar_fich_alunos(char *linha, int num);

void verfica_aprov();

void alunos_aprov_file();

#endif //PPP_2020_PILHA_ALUNOS_H