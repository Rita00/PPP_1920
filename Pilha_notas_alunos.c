#include <stdlib.h>
#include <strings.h>
#include <memory.h>
#include <regex.h>
#include "structs.h"
#include "Pilha_notas_alunos.h"
#include "Pilha_disciplinas.h"

#define DIM 200

pilha_disciplinas_aluno aloc_disciplina(char *prova, float nota, char *disciplina) {
    pilha_disciplinas_aluno nova_disciplina = (pilha_disciplinas_aluno) calloc(1, sizeof(struct _notas_alunos));
    nova_disciplina->disciplina = strdup(disciplina);
    if (preencher_notas(prova, nota, nova_disciplina) != 0) return NULL;
    return nova_disciplina;
}

char preencher_notas(char *prova, float nota, pilha_disciplinas_aluno disciplina) {
    if (strcasecmp(prova, "prova1") == 0) {
        disciplina->nota1 = nota;
        disciplina->prova1 = strdup(prova);
        return 0;
    } else if (strcasecmp(prova, "prova2") == 0) {
        disciplina->nota2 = nota;
        disciplina->prova2 = prova;
        return 0;
    }
    printf("Disciplina com notas completas\n");
    return 1;
}

pilha_disciplinas_aluno push_dis_to_aluno(char *prova, float nota, char *disciplina, pilha_disciplinas_aluno lista) {
    pilha_disciplinas_aluno nova_nota;
    pilha_disciplinas_aluno aux = lista;

    if (lista == 0 || strcasecmp(lista->disciplina, disciplina) >= 0) {
        if (lista != 0 && strcasecmp(lista->disciplina, disciplina) == 0) {
            preencher_notas(prova, nota, lista);
            return lista;
        }
        aux = aloc_disciplina(prova, nota, disciplina);
        aux->next = lista;
        return aux;
    } else {
        while (aux->next && strcasecmp(aux->next->disciplina, disciplina) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->disciplina, disciplina) == 0) {
            preencher_notas(prova, nota, aux->next);
            return lista;
        } else {
            nova_nota = aloc_disciplina(prova, nota, disciplina);
            nova_nota->next = aux->next;
            aux->next = nova_nota;
        }
    }
    return lista;
}

/*Pesquisa uma determinada disciplina de um aluno*/
pilha_disciplinas_aluno pesquisa_disciplina_aluno(char *disciplina, pilha_disciplinas_aluno pilha) {
    pilha_disciplinas_aluno aux = pilha;
    if (pilha == NULL) {
        printf("Pilha Vazia. Aluno nao inscrito em disciplinas\n");
    } else {
        while (aux != NULL && strcasecmp(aux->disciplina, disciplina) != 0) {
            aux = aux->next;
        }
        if (aux && strcasecmp(aux->disciplina, disciplina) == 0) {
            return aux;
        }
    }
    return NULL;
}

/*Imprime lista de disciplinas com notas de um determinado aluno*/
void print_pilha_disciplinas_aluno(pilha_disciplinas_aluno pilha) {
    if (pilha == NULL) {
        printf("Pilha Vazia\n");
        return;
    }
    while (pilha != NULL) {
        printf("%s:\n%s: %f\n%s: %f\n", pilha->disciplina, pilha->prova1, pilha->nota1, pilha->prova2, pilha->nota2);
        pilha = pilha->next;
    }
}

/*Destroi lista disciplinas*/
void destroi_lista_disciplinas(pilha_disciplinas_aluno pilha) {
    pilha_disciplinas_aluno aux;
    while (pilha != NULL) {
        aux = pilha;
        pilha = pilha->next;
        free(aux);
    }
}

pilha_disciplinas read_provas(FILE *input_file, no_aluno pilha, pilha_disciplinas pilha_disc) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) {
        pilha_disc = validar_fich_disc(elimina_final(linha), num_linha, pilha, pilha_disc);
        num_linha++;
    }
    return pilha_disc;
}

pilha_disciplinas validar_fich_disc(char *linha, int num_linha, no_aluno lista, pilha_disciplinas pilha_disc) {
    int num_est;
    char *disciplina;
    char *prova;
    float nota;
    no_aluno aluno;
    regex_t linha_fich_0_19, linha_fich_20;

    regcomp(&linha_fich_0_19, "[0-9]+#[A-Za-z]+#[Pp][Rr][Oo][Vv][Aa][12]#[01]*[0-9].[0-9][0-9]*$", REG_EXTENDED);
    regcomp(&linha_fich_20, "[0-9]+#[A-Za-z]+#[Pp][Rr][Oo][Vv][Aa][12]#[2][0].[0][0]*$", REG_EXTENDED);

    if (regexec(&linha_fich_0_19, linha, 0, NULL, 0) == 0 || regexec(&linha_fich_20, linha, 0, NULL, 0) == 0) {
        num_est = atoi(strtok(linha, "#"));
        aluno = pesquisa_aluno(num_est, lista);
        if (aluno == NULL) {
            printf("Numero invalido\n");
            return pilha_disc;
        }
        disciplina = strdup(strtok(NULL, "#"));
        prova = strdup(strtok(NULL, "#"));
        nota = atof(strtok(NULL, "\n")); //todo mudar atoi e atof

        aluno->pilha_notas = push_dis_to_aluno(prova, nota, disciplina, aluno->pilha_notas);
        pilha_disc = push_disciplina(disciplina, pilha_disc);
        pilha_disc->alunos = push_aluno_to_disc(aluno, pilha_disc->alunos);
        return pilha_disc;

    } else {
        fprintf(stderr, "Erro ao ler linha %d do ficheiro\n", num_linha);
    }
    return pilha_disc;
}