#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>
#include "Pilha_alunos.h"
#include "Pilha_notas_alunos.h"
#include "structs.h"

#define DIM 200

/*Alocacao de no da pilha*/
aluno aloc_aluno(char *nome, int num){
    aluno novo_aluno = (aluno) calloc(1, sizeof(struct _alunos));
    novo_aluno->nome_aluno = strdup(nome);
    novo_aluno->num_est = num;
    return novo_aluno;
}

/*Inserir na lista*/
void push_aluno(char *nome, int num) {
    pilha_alunos novo_aluno = (pilha_alunos)calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha_al;

    if (!aux || strcasecmp(aux->info_aluno->nome_aluno, nome) >= 0) {
        if (aux && strcasecmp(aux->info_aluno->nome_aluno, nome) == 0) {
            printf("Nome ja exixtente\n");
        }
        novo_aluno->info_aluno = aloc_aluno(nome, num);
        novo_aluno->next = aux;
        pilha_al = novo_aluno;
    } else {
        while (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) == 0) {
            printf("Nome ja existente\n");
        } else {
            novo_aluno->info_aluno = aloc_aluno(nome, num);
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
}

/*Pesquisar determinado aluno por nome*/
aluno pesquisa_aluno(long num_aluno) { //todo change return ???
    pilha_alunos aux = pilha_al;
    if (pilha_al == NULL) {
        printf("Lista Vazia\n");
    } else {
        while (aux && aux->info_aluno->num_est != num_aluno) {
            aux = aux->next;
        }
        if (aux && aux->info_aluno->num_est == num_aluno) {
            return aux->info_aluno;
        }
    }
    return NULL;
}

/*Remover um aluno da lista*/
pilha_alunos remove_aluno(aluno pop_aluno) {
    pilha_alunos aux = pilha_al, aux_rem;

    if (pilha_al == NULL) {
        printf("Lista Vazia\n");
        return pilha_al;
    } else if (pilha_al->next == NULL) { //A lista tem apenas 1 no
        free(pilha_al);
        return pilha_al;
    }else if(pilha_al->info_aluno == pop_aluno){ //Primeiro aluno da pilha a remover
        aux_rem = pilha_al->next;
        free(pilha_al);
        return aux_rem;
    } else {
        while (aux && aux->next && aux->next->info_aluno != pop_aluno) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            printf("Numero de aluno inexistente\n");
            return pilha_al;
        }
        aux_rem = aux->next->next;
        free(aux->next);
        aux->next = aux_rem;
        return pilha_al;
    }
}

/*Imprime lista alunos*/
void print_lista_alunos() {
    if (pilha_al == NULL) {
        printf("Lista Vazia\n");
        return;
    }
    while (pilha_al != NULL) {
        printf("%s %ld\n", pilha_al->info_aluno->nome_aluno, pilha_al->info_aluno->num_est);
        pilha_al = pilha_al->next;
    }
}

/*Destroi lista*/
void destroi_pilha_alunos(char destroi_info) {
    pilha_alunos aux;
    while (pilha_al != NULL) {
        aux = pilha_al;
        pilha_al = pilha_al->next;
        if (destroi_info && aux->info_aluno != NULL){
            if(aux->info_aluno->pilha_notas != NULL)
                destroi_lista_disciplinas(aux->info_aluno->pilha_notas);
            free(aux->info_aluno);
        }
        free(aux);
    }
}

/*Le ficheiro de texto recebido como input e insere alunos na lista*/
void read_alunos_num(FILE *input_file) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) { /*Se o fgets nao ler nada devolve NULL se ler devolve a string*/
        validar_fich_alunos(elimina_final(linha), num_linha);
        num_linha++;
    }
}

/*Verifica se o ficheiro esta no formato correto*/
void validar_fich_alunos(char *linha, int num) {
    char *nome;
    int num_est;
    regex_t linha_fich;

    regcomp(&linha_fich, "[A-Za-z ]+#[0-9]+$", REG_EXTENDED);
    if (regexec(&linha_fich, linha, 0, NULL, 0) == 0) {
        nome = trim(strtok(linha, "#"));
        num_est = atoi(strtok(NULL, "\n")); //todo remove atoi
        push_aluno(nome, num_est);
    } else {
        fprintf(stderr, "Erro ao ler linha %d do ficheiro\n", num);
    }
}

void verfica_aprov(){
    pilha_disciplinas_aluno aux;
    if(pilha_al == NULL) return;
    while(pilha_al != NULL){
        aux = pilha_al->info_aluno->pilha_notas;
        if(aux == NULL) return;
        while(aux != NULL){
            if(aux->med_final >= 9.45) pilha_al->info_aluno->aproved++;
            aux = aux->next;
        }
        pilha_al = pilha_al->next;
    }
}

void alunos_aprov_file(){
    FILE *fp = fopen("Alunos_Aprovados.txt", "a");
    if(pilha_al == NULL || fp == NULL) return;
    while(pilha_al != NULL){
        if(pilha_al->info_aluno->aproved >= 8) fprintf(fp, "%s\t%ld\n", pilha_al->info_aluno->nome_aluno, pilha_al->info_aluno->num_est);
        pilha_al = pilha_al->next;
    }
    fclose(fp);
}