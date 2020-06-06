#ifndef PPP_2020_PILHA_NOTAS_ALUNOS_H
#define PPP_2020_PILHA_NOTAS_ALUNOS_H

#include "Pilha_alunos.h"
#include "structs.h"

pilha_disciplinas_aluno aloc_disciplina(char *prova, float nota, char *disciplina);

char preencher_notas(char *prova, float nota, pilha_disciplinas_aluno pilha);

pilha_disciplinas_aluno push_dis_to_aluno(char *prova, float nota, char *disciplina, pilha_disciplinas_aluno pilha);

pilha_disciplinas_aluno pesquisa_disciplina_aluno(char *disciplina, pilha_disciplinas_aluno pilha);

void print_pilha_disciplinas_aluno();

void destroi_lista_disciplinas();

pilha_disciplinas read_provas(FILE *input_file);

pilha_disciplinas validar_fich_disc(char *linha, int num_linha);

void classificacao_final();

void print_class_final();

#endif //PPP_2020_PILHA_NOTAS_ALUNOS_H

