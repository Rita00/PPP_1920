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
pilha_alunos push_aluno(char *nome, int num, pilha_alunos pilha) {
    pilha_alunos novo_aluno = (pilha_alunos)calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha;

    if (!pilha || strcasecmp(pilha->info_aluno->nome_aluno, nome) >= 0) {
        if (pilha && strcasecmp(pilha->info_aluno->nome_aluno, nome) == 0) {
            printf("Nome ja exixtente\n");
            return pilha;
        }
        novo_aluno->info_aluno = aloc_aluno(nome, num);
        novo_aluno->next = pilha;
        return novo_aluno;
    } else {
        while (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) == 0) {
            printf("Nome ja existente\n");
            return pilha;
        } else {
            novo_aluno->info_aluno = aloc_aluno(nome, num);
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
    return pilha;
}

/*Pesquisar determinado aluno por nome*/
aluno pesquisa_aluno(long num_aluno, pilha_alunos pilha) { //todo change return ???
    pilha_alunos aux = pilha;
    if (pilha == NULL) {
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
pilha_alunos remove_aluno(aluno pop_aluno, pilha_alunos pilha) {
    pilha_alunos aux = pilha, aux_rem;

    if (pilha == NULL) {
        printf("Lista Vazia\n");
        return pilha;
    } else if (pilha->next == NULL) { //A lista tem apenas 1 no
        free(pilha);
        return pilha;
    }else if(pilha->info_aluno == pop_aluno){ //Primeiro aluno da pilha a remover
        aux_rem = pilha->next;
        free(pilha);
        return aux_rem;
    } else {
        while (aux && aux->next && aux->next->info_aluno != pop_aluno) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            printf("Numero de aluno inexistente\n");
            return pilha;
        }
        aux_rem = aux->next->next;
        free(aux->next);
        aux->next = aux_rem;
        return pilha;
    }
}



/*Imprime lista alunos*/
void print_lista_alunos(pilha_alunos pilha) {
    if (pilha == NULL) {
        printf("Lista Vazia\n");
        return;
    }
    while (pilha != NULL) {
        printf("%s %ld\n", pilha->info_aluno->nome_aluno, pilha->info_aluno->num_est);
        pilha = pilha->next;
    }
}

/*Destroi lista*/
void destroi_pilha_alunos(pilha_alunos pilha, char destroi_info) {
    pilha_alunos aux;
    while (pilha != NULL) {
        aux = pilha;
        pilha = pilha->next;
        if (destroi_info && aux->info_aluno != NULL){
            if(aux->info_aluno->pilha_notas != NULL)
                destroi_lista_disciplinas(aux->info_aluno->pilha_notas);
            free(aux->info_aluno);
        }
        free(aux);
    }
}

/*Le ficheiro de texto recebido como input e insere alunos na lista*/
pilha_alunos read_alunos_num(FILE *input_file, pilha_alunos pilha) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) { /*Se o fgets nao ler nada devolve NULL se ler devolve a string*/
        pilha = validar_fich_alunos(elimina_final(linha), num_linha, pilha);
        num_linha++;
    }
    return pilha;
}

/*Verifica se o ficheiro esta no formato correto*/
pilha_alunos validar_fich_alunos(char *linha, int num, pilha_alunos pilha) {
    char *nome;
    int num_est;
    regex_t linha_fich;

    regcomp(&linha_fich, "[A-Za-z ]+#[0-9]+$", REG_EXTENDED);
    if (regexec(&linha_fich, linha, 0, NULL, 0) == 0) {
        nome = trim(strtok(linha, "#"));
        num_est = atoi(strtok(NULL, "\n")); //todo remove atoi
        pilha = push_aluno(nome, num_est, pilha);
    } else {
        fprintf(stderr, "Erro ao ler linha %d do ficheiro\n", num);
    }
    return pilha;
}

void verfica_aprov(pilha_alunos pilha){
    pilha_disciplinas_aluno aux;
    if(pilha == NULL) return;
    while(pilha != NULL){
        aux = pilha->info_aluno->pilha_notas;
        if(aux == NULL) return;
        while(aux != NULL){
            if(aux->med_final >= 9.45) pilha->info_aluno->aproved++;
            aux = aux->next;
        }
        pilha = pilha->next;
    }
}

void alunos_aprov_file(pilha_alunos pilha){
    FILE *fp = fopen("Alunos_Aprovados.txt", "a");
    if(pilha == NULL || fp == NULL) return;
    while(pilha != NULL){
        if(pilha->info_aluno->aproved >= 8) fprintf(fp, "%s\t%ld\n", pilha->info_aluno->nome_aluno, pilha->info_aluno->num_est);
        pilha = pilha->next;
    }
    fclose(fp);
}