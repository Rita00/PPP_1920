#ifndef PPP_2020_PILHA_H
#define PPP_2020_PILHA_H

#include <stdio.h>
#include "Pilha_notas_alunos.h"
#include "structs.h"

/*Interface para a pilha que guarda a informacao do ficheiro de entrada "Lista_alunos_num_est.txt"*/
no_aluno aloc_aluno(char *nome, int num);

no_aluno push_aluno(char *nome, int num, no_aluno lista);

no_aluno read_alunos_num(FILE *input_file, no_aluno lista);

void print_lista_alunos(no_aluno lista);

void destroi_lista_alunos(no_aluno lista);

no_aluno pesquisa_aluno(long num_aluno, no_aluno lista);

no_aluno remove_aluno(long num_aluno, no_aluno lista);

no_aluno validar_fich_alunos(char *linha, int num, no_aluno lista);

#endif //PPP_2020_PILHA_ALUNOS_H