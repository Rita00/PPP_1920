#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>
#include "Pilha_alunos.h"
#include "Pilha_notas_alunos.h"
#include "structs.h"

#define DIM 200

/*Alocacao de no da pilha*/
no_aluno aloc_aluno(char *nome, int num){
    no_aluno novo_aluno = (no_aluno) calloc(1, sizeof(struct _alunos));
    novo_aluno->nome_aluno = strdup(nome);
    novo_aluno->num_est = num;
    novo_aluno->pilha_notas = NULL;
    return novo_aluno;
}

/*Inserir na lista*/
no_aluno push_aluno(char *nome, int num, no_aluno lista) {
    no_aluno novo_aluno;
    no_aluno aux = lista;

    if (!lista || strcasecmp(lista->nome_aluno, nome) >= 0) {
        if (lista && strcasecmp(lista->nome_aluno, nome) == 0) {
            printf("Nome ja exixtente\n");
            return lista;
        }
        aux = aloc_aluno(nome, num);
        aux->next = lista;
        return aux;
    } else {
        while (aux->next && strcasecmp(aux->next->nome_aluno, nome) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->nome_aluno, nome) == 0) {
            printf("Nome ja existente\n");
            return lista;
        } else {
            novo_aluno = aloc_aluno(nome, num);
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
    return lista;
}

/*Pesquisar determinado aluno por nome*/
no_aluno pesquisa_aluno(long num_aluno, no_aluno lista) {
    no_aluno aux = lista;
    if (lista == NULL) {
        printf("Lista Vazia\n");
    } else {
        while (aux && aux->num_est != num_aluno) {
            aux = aux->next;
        }
        if (aux && aux->num_est == num_aluno) {
            return aux;
        }
    }
    return NULL;
}

/*Remover um aluno da lista*/
no_aluno remove_aluno(long num_aluno, no_aluno lista) {
    no_aluno aux = lista, aux_rem;

    if (lista == NULL) {
        printf("Lista Vazia\n");
        return lista;
    } else if (lista->next == NULL) { //A lista tem apenas 1 no
        free(lista);
        return lista;
    } else {
        while (aux && aux->next && aux->num_est != num_aluno) {
            aux = aux->next;
        }
        if (aux == NULL) {
            printf("Numero de aluno inexistente\n");
            return lista;
        }

        aux->next = aux->next->next;
        free(aux);
        return lista;
    }

}

/*Imprime lista alunos*/
void print_lista_alunos(no_aluno lista) {
    if (lista == NULL) {
        printf("Lista Vazia\n");
        return;
    }
    while (lista != NULL) {
        printf("%s %ld\n", lista->nome_aluno, lista->num_est);
        lista = lista->next;
    }
}

/*Destroi lista*/
void destroi_lista_alunos(no_aluno lista) {
    no_aluno aux;
    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        if (aux->pilha_notas != NULL)
            destroi_lista_disciplinas(aux->pilha_notas);
        free(aux);
    }
}

/*Le ficheiro de texto recebido como input e insere alunos na lista*/
no_aluno read_alunos_num(FILE *input_file, no_aluno lista) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) { /*Se o fgets nao ler nada devolve NULL se ler devolve a string*/
        lista = validar_fich_alunos(elimina_final(linha), num_linha, lista);
        num_linha++;
    }
    return lista;
}

/*Verifica se o ficheiro esta no formato correto*/
no_aluno validar_fich_alunos(char *linha, int num, no_aluno lista) {
    char *nome;
    int num_est;
    regex_t linha_fich;

    regcomp(&linha_fich, "[A-Za-z ]+#[0-9]+$", REG_EXTENDED);
    if (regexec(&linha_fich, linha, 0, NULL, 0) == 0) {
        nome = trim(strtok(linha, "#"));
        num_est = atoi(strtok(NULL, "\n"));
        lista = push_aluno(nome, num_est, lista);
    } else {
        fprintf(stderr, "Erro ao ler linha %d do ficheiro\n", num);
    }
    return lista;
}
