#ifndef PPP_2020_PILHA_DISCIPLINAS_H
#define PPP_2020_PILHA_DISCIPLINAS_H

#include "structs.h"

pilha_disciplinas push_disciplina(char *disc);

pilha_disciplinas remove_disciplina(disciplina pop_disciplina);

pilha_alunos push_aluno_to_disc(aluno aluno);

void print_pilha_disciplinas();

void print_alunos_disciplina();

disciplina pesquisa_disciplina(char *disiciplina);

void destroi_pilha_disciplinas();

void write_output();

#endif //PPP_2020_PILHA_DISCIPLINAS_H
