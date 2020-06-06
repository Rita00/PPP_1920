#ifndef PPP_2020_PILHA_DISCIPLINAS_H
#define PPP_2020_PILHA_DISCIPLINAS_H

#include "structs.h"

pilha_disciplinas push_disciplina(char *disc, pilha_disciplinas pilha);

pilha_disciplinas pop_disciplina(disciplina pop_disciplina, pilha_disciplinas pilha);

pilha_alunos push_aluno_to_disc(aluno aluno, pilha_alunos pilha_alunos_disciplina);

void print_pilha_disciplinas(pilha_disciplinas pilha);

void print_alunos_disciplina(pilha_alunos pilha);

disciplina pesquisa_disciplina(char *disiciplina, pilha_disciplinas pilha);

void destroi_pilha_disciplinas(pilha_disciplinas pilha);

void write_output(pilha_disciplinas pilha);

#endif //PPP_2020_PILHA_DISCIPLINAS_H
