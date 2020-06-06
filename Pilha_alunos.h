#ifndef PPP_2020_PILHA_H
#define PPP_2020_PILHA_H

#include <stdio.h>
#include "Pilha_notas_alunos.h"
#include "structs.h"

/*Interface para a pilha que guarda a informacao do ficheiro de entrada "Lista_alunos_num_est.txt"*/
aluno aloc_aluno(char *nome, int num);

pilha_alunos push_aluno(char *nome, int num, pilha_alunos pilha);

pilha_alunos read_alunos_num(FILE *input_file, pilha_alunos pilha);

void print_lista_alunos(pilha_alunos pilha);

void destroi_pilha_alunos(pilha_alunos pilha, char destroi_info);

aluno pesquisa_aluno(long num_aluno, pilha_alunos pilha);

pilha_alunos remove_aluno(aluno pop_aluno, pilha_alunos pilha);

pilha_alunos validar_fich_alunos(char *linha, int num, pilha_alunos pilha);

void verfica_aprov(pilha_alunos pilha);

void alunos_aprov_file(pilha_alunos pilha);

#endif //PPP_2020_PILHA_ALUNOS_H