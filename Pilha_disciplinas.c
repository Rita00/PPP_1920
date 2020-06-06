#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "structs.h"
#include "Pilha_alunos.h"

#define DIM 100

disciplina aloc_disc(char *disc) {
    disciplina nova_disciplina = (disciplina) calloc(1, sizeof(struct _disciplinas));
    nova_disciplina->disc = strdup(disc);
    return nova_disciplina;
}

pilha_disciplinas push_disciplina(char *disc, pilha_disciplinas pilha) {
    pilha_disciplinas nova_disciplina = (pilha_disciplinas)calloc(1, sizeof(struct _pilha_disciplinas));
    pilha_disciplinas aux = pilha;

    if (pilha == NULL || strcasecmp(pilha->disciplina->disc, disc) >= 0) {
        if (pilha != NULL && strcasecmp(pilha->disciplina->disc, disc) == 0) {
            printf("Disiciplina ja existente\n");
            return pilha;
        }
        nova_disciplina->disciplina = aloc_disc(disc);
        nova_disciplina->next = pilha;
        return nova_disciplina;
    } else {
        while (aux->next && strcasecmp(aux->next->disciplina->disc, disc) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->disciplina->disc, disc) == 0) {
            printf("Disciplina ja existente\n");
            return pilha;
        } else {
            nova_disciplina->disciplina = aloc_disc(disc);
            nova_disciplina->next = aux->next;
            aux->next = nova_disciplina;
        }
    }
    return pilha;
}

pilha_disciplinas pop_disciplina(disciplina pop_disciplina, pilha_disciplinas pilha){
    pilha_disciplinas aux = pilha, aux_rem;

    if (pilha == NULL) {
        printf("Lista Vazia\n");
        return pilha;
    } else if (pilha->next == NULL) { //A lista tem apenas 1 no
        free(pilha);
        return pilha;
    }else if(pilha->disciplina == pop_disciplina){ //Primeiro aluno da pilha a remover
        aux_rem = pilha->next;
        free(pilha);
        return aux_rem;
    } else {
        while (aux && aux->next && aux->next->disciplina != pop_disciplina) {
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

pilha_alunos push_aluno_to_disc(aluno aluno, pilha_alunos pilha){
    pilha_alunos novo_aluno = (pilha_alunos)calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha;
    if(pilha == NULL || strcasecmp(pilha->info_aluno->nome_aluno, aluno->nome_aluno) >= 0){
        if(pilha != NULL && pilha->info_aluno->num_est == aluno->num_est){
            printf("Aluno ja existente\n");
            return pilha;
        }
        novo_aluno->info_aluno = aluno;
        novo_aluno->next = pilha;
        return novo_aluno;
    }else{
        while(aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, aluno->nome_aluno) < 0){
            aux = aux->next;
        }if(aux->next && aux->next->info_aluno->num_est == aluno->num_est){
            printf("Aluno ja existente\n");
            return pilha;
        }else{
            novo_aluno->info_aluno = aluno;
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
    return pilha;
}

void print_pilha_disciplinas(pilha_disciplinas pilha){
    if(pilha == NULL){
       printf("Pilha de disciplinas vazia\n");
       return;
    }
    while(pilha != NULL){
        printf("%s\n", pilha->disciplina->disc);
        pilha = pilha->next;
    }
}

void print_alunos_disciplina(pilha_alunos pilha){
    if(pilha == NULL){
        printf("Pilha Vazia\n");
        return;
    }
    while(pilha != NULL){
        printf("%s: %ld\n", pilha->info_aluno->nome_aluno, pilha->info_aluno->num_est);
        pilha = pilha->next;
    }
}

disciplina pesquisa_disciplina(char *disiciplina, pilha_disciplinas pilha){
    pilha_disciplinas aux = pilha;
    if(pilha == NULL){
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
void destroi_pilha_disciplinas(pilha_disciplinas pilha){
    pilha_disciplinas aux = pilha;
    while(pilha != NULL){
        aux = pilha->next;
        free(pilha);
        pilha = aux;
    }
}

void write_output(pilha_disciplinas pilha){
    FILE *fp;
    pilha_alunos aux;
    char nome_ficheiro[DIM];
    if(pilha == NULL) return;
    while(pilha != NULL){
        sprintf(nome_ficheiro, "Pauta_%s.txt",pilha->disciplina->disc);
        fp = fopen(nome_ficheiro, "a");
        aux = pilha->disciplina->alunos;
        while(aux != NULL) {
            fprintf(fp, "%s\t%.2f\n", aux->info_aluno->nome_aluno, pesquisa_disciplina_aluno(pilha->disciplina->disc, aux->info_aluno->pilha_notas)->med_final);
            aux = aux->next;
        }
        pilha = pilha->next;
        fclose(fp);
    }
}