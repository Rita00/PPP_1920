#ifndef PPP_2020_PILHA_DISCIPLINAS_H
#define PPP_2020_PILHA_DISCIPLINAS_H

#include "structs.h"

no_disciplina push_disciplina(char *disc, no_disciplina pilha_disc);

no_aluno push_aluno_to_disc(no_aluno aluno, no_aluno pilha_alunos_disciplina);

void print_pilha_disciplinas(no_disciplina pilha_disc);

void print_alunos_disciplina(no_aluno pilha);

no_disciplina pesquisa_disciplina(char *disiciplina, no_disciplina pilha_disc);

void destroi_pilha_disciplinas(no_disciplina pilha_disc);

#endif //PPP_2020_PILHA_DISCIPLINAS_H
