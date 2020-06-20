#ifndef PPP_2020_PILHA_H
#define PPP_2020_PILHA_H

#include <stdio.h>
#include "structs.h"

/****** Headers da interface da pilha de alunos */
aluno aloc_aluno(char *nome, int num);

void push_aluno(char *nome, int num);

aluno pesquisa_aluno(long num_aluno);

pilha_alunos remove_aluno(aluno pop_aluno);

aluno pop_aluno();

void print_lista_alunos();

void read_alunos_num(FILE *input_file);

void validar_fich_alunos(char *linha, int num);

void verfica_aprov();

void alunos_aprov_file(char *alunos_aprov);

/****** Headers da interface da pilha de disciplinas */
disciplina aloc_disciplina(char *disc);

pilha_disciplinas push_disciplina(char *disc);

pilha_disciplinas remove_disciplina(disciplina pop_disciplina);

disciplina pop_disciplina();

pilha_alunos push_aluno_to_disc(aluno aluno, pilha_alunos lista_alunos);

void print_pilha_disciplinas();

void print_alunos_disciplina(pilha_alunos pilha);

disciplina pesquisa_disciplina(char *disiciplina);

void write_output(char *file_prefixe);

/****** Headers da interface da pilha de notas de cada aluno */
lista_notas aloc_nota(char *prova, float nota, char *disciplina);

char preencher_notas(char *prova, float nota, lista_notas pilha);

lista_notas insere_dis_to_aluno(char *prova, float nota, char *disciplina, lista_notas pilha);

lista_notas pesquisa_disciplina_aluno(char *disciplina, lista_notas pilha);

void print_lista_disciplinas_aluno(lista_notas pilha);

void destroi_lista_notas(lista_notas pilha);

pilha_disciplinas read_provas(FILE *input_file);

pilha_disciplinas validar_fich_disc(char *linha, int num_linha);

void classificacao_final();

void print_class_final();

#endif //PPP_2020_PILHA_ALUNOS_H