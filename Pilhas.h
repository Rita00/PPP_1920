#ifndef PPP_2020_PILHA_H
#define PPP_2020_PILHA_H

#include <stdio.h>
#include "structs.h"

/****** Headers da interface da pilha de alunos */
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

/****** Headers da interface da pilha de disciplinas */
pilha_disciplinas push_disciplina(char *disc);

pilha_disciplinas remove_disciplina(disciplina pop_disciplina);

pilha_alunos push_aluno_to_disc(aluno aluno);

void print_pilha_disciplinas();

void print_alunos_disciplina();

disciplina pesquisa_disciplina(char *disiciplina);

void destroi_pilha_disciplinas();

void write_output();

/****** Headers da interface da pilha de notas de cada aluno */
pilha_notas aloc_disciplina(char *prova, float nota, char *disciplina);

char preencher_notas(char *prova, float nota, pilha_notas pilha);

pilha_notas push_dis_to_aluno(char *prova, float nota, char *disciplina, pilha_notas pilha);

pilha_notas pesquisa_disciplina_aluno(char *disciplina, pilha_notas pilha);

void print_pilha_disciplinas_aluno(pilha_notas pilha);

void destroi_pilha_notas(pilha_notas pilha);

pilha_disciplinas read_provas(FILE *input_file);

pilha_disciplinas validar_fich_disc(char *linha, int num_linha);

void classificacao_final();

void print_class_final();

#endif //PPP_2020_PILHA_ALUNOS_H