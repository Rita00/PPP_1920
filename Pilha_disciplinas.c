#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "structs.h"
#include "Pilha_alunos.h"
#include "Pilha_disciplinas.h"

#define DIM 100

disciplina aloc_disc(char *disc) {
    disciplina nova_disciplina = (disciplina) calloc(1, sizeof(struct _disciplinas));
    nova_disciplina->disc = strdup(disc);
    return nova_disciplina;
}

pilha_disciplinas push_disciplina(char *disc) {
    pilha_disciplinas nova_disciplina = (pilha_disciplinas)calloc(1, sizeof(struct _pilha_disciplinas));
    pilha_disciplinas aux = pilha_disc;

    if (pilha_disc == NULL || strcasecmp(pilha_disc->disciplina->disc, disc) >= 0) {
        if (pilha_disc != NULL && strcasecmp(pilha_disc->disciplina->disc, disc) == 0) {
            printf("Disiciplina ja existente\n");
            return pilha_disc;
        }
        nova_disciplina->disciplina = aloc_disc(disc);
        nova_disciplina->next = pilha_disc;
        return nova_disciplina;
    } else {
        while (aux->next && strcasecmp(aux->next->disciplina->disc, disc) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->disciplina->disc, disc) == 0) {
            printf("Disciplina ja existente\n");
            return pilha_disc;
        } else {
            nova_disciplina->disciplina = aloc_disc(disc);
            nova_disciplina->next = aux->next;
            aux->next = nova_disciplina;
        }
    }
    return pilha_disc;
}

pilha_disciplinas remove_disciplina(disciplina pop_disciplina){
    pilha_disciplinas aux = pilha_disc, aux_rem;

    if (pilha_disc == NULL) {
        printf("Lista Vazia\n");
        return pilha_disc;
    } else if (pilha_disc->next == NULL) { //A lista tem apenas 1 no
        free(pilha_disc);
        return pilha_disc;
    }else if(pilha_disc->disciplina == pop_disciplina){ //Primeiro aluno da pilha a remover
        aux_rem = pilha_disc->next;
        free(pilha_disc);
        return aux_rem;
    } else {
        while (aux && aux->next && aux->next->disciplina != pop_disciplina) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            printf("Numero de aluno inexistente\n");
            return pilha_disc;
        }
        aux_rem = aux->next->next;
        free(aux->next);
        aux->next = aux_rem;
        return pilha_disc;
    }
}

pilha_disciplinas pop_disciplina(char *pop_dis){
    return remove_disciplina(pesquisa_disciplina(pop_dis));
}

pilha_alunos push_aluno_to_disc(aluno aluno){
    pilha_alunos novo_aluno = (pilha_alunos)calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha_al;
    if(pilha_al == NULL || strcasecmp(pilha_al->info_aluno->nome_aluno, aluno->nome_aluno) >= 0){
        if(pilha_al != NULL && pilha_al->info_aluno->num_est == aluno->num_est){
            printf("Aluno ja existente\n");
            return pilha_al;
        }
        novo_aluno->info_aluno = aluno;
        novo_aluno->next = pilha_al;
        return novo_aluno;
    }else{
        while(aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, aluno->nome_aluno) < 0){
            aux = aux->next;
        }if(aux->next && aux->next->info_aluno->num_est == aluno->num_est){
            printf("Aluno ja existente\n");
            return pilha_al;
        }else{
            novo_aluno->info_aluno = aluno;
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
    return pilha_al;
}

void print_pilha_disciplinas(){
    if(pilha_disc == NULL){
       printf("Pilha de disciplinas vazia\n");
       return;
    }
    while(pilha_disc != NULL){
        printf("%s\n", pilha_disc->disciplina->disc);
        pilha_disc = pilha_disc->next;
    }
}

void print_alunos_disciplina(){
    if(pilha_al == NULL){
        printf("Pilha Vazia\n");
        return;
    }
    while(pilha_al != NULL){
        printf("%s: %ld\n", pilha_al->info_aluno->nome_aluno, pilha_al->info_aluno->num_est);
        pilha_al = pilha_al->next;
    }
}

disciplina pesquisa_disciplina(char *disiciplina){
    pilha_disciplinas aux = pilha_disc;
    if(pilha_disc == NULL){
        printf("Pilha de disciplinas vazia\n");
        return NULL; //todo verificar o null
    }else{
        while(aux && strcasecmp(aux->disciplina->disc, disiciplina) != 0){
            aux = aux->next;
        }
        if(aux) return aux->disciplina;
    }
    return NULL;
}

/*Destroi Pilha de disciplinas e respetiva pilha de alunos de cada disciplina*/
void destroi_pilha_disciplinas(){
    pilha_disciplinas aux = pilha_disc;
    while(pilha_disc != NULL){
        aux = pilha_disc->next;
        free(pilha_disc);
        pilha_disc = aux;
    }
}

void write_output(){
    FILE *fp;
    pilha_alunos aux;
    char nome_ficheiro[DIM];
    if(pilha_disc == NULL) return;
    while(pilha_disc != NULL){
        sprintf(nome_ficheiro, "Pauta_%s.txt",pilha_disc->disciplina->disc);
        fp = fopen(nome_ficheiro, "a");
        aux = pilha_disc->disciplina->alunos;
        while(aux != NULL) {
            fprintf(fp, "%s\t%.2f\n", aux->info_aluno->nome_aluno, pesquisa_disciplina_aluno(pilha_disc->disciplina->disc, aux->info_aluno->pilha_notas)->med_final);
            aux = aux->next;
        }
        pilha_disc = pilha_disc->next;
        fclose(fp);
    }
}