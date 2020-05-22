#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "structs.h"
#include "Pilha_alunos.h"

pilha_disciplinas aloc_disc(char *disc) {
    pilha_disciplinas nova_disciplina = (pilha_disciplinas) calloc(1, sizeof(struct _disciplinas));
    nova_disciplina->disciplina = strdup(disc);
    nova_disciplina->alunos = NULL;
    return nova_disciplina;
}

pilha_disciplinas push_disciplina(char *disc, pilha_disciplinas pilha_disc) {
    pilha_disciplinas nova_disciplina;
    pilha_disciplinas aux = pilha_disc;

    if (pilha_disc == NULL || strcasecmp(pilha_disc->disciplina, disc) >= 0) {
        if (pilha_disc != NULL && strcasecmp(pilha_disc->disciplina, disc) == 0) {
            printf("Disiciplina ja existente\n");
            return pilha_disc;
        }
        aux = aloc_disc(disc);
        aux->next = pilha_disc;
        return aux;
    } else {
        while (aux->next && strcasecmp(aux->next->disciplina, disc) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->disciplina, disc) == 0) {
            printf("Disciplina ja existente\n");
            return pilha_disc;
        } else {
            nova_disciplina = aloc_disc(disc);
            nova_disciplina->next = aux->next;
            aux->next = nova_disciplina;
        }
    }
    return pilha_disc;
}

no_aluno push_aluno_to_disc(no_aluno aluno, no_aluno pilha_alunos_disciplina){
    no_aluno aux = pilha_alunos_disciplina;
    if(pilha_alunos_disciplina == 0 || strcasecmp(pilha_alunos_disciplina->nome_aluno, aluno->nome_aluno) >= 0){
        if(pilha_alunos_disciplina != 0 && pilha_alunos_disciplina == aluno){
            printf("Aluno ja existente\n");
            return pilha_alunos_disciplina;
        }
        aux = aluno;
        aux->next = pilha_alunos_disciplina;
        return aux;
    }else{
        while(aux->next && aux->next < aluno){
            aux = aux->next;
        }if(aux->next && aux->next == aluno){
            printf("Nome ja existente\n");
            return pilha_alunos_disciplina;
        }else{
            aluno->next = aux->next;
            aux->next = aluno;
        }
    }
    return pilha_alunos_disciplina;
}

void print_pilha_disciplinas(pilha_disciplinas pilha_disc){
    if(pilha_disc == NULL){
       printf("Pilha de disciplinas vazia\n");
       return;
    }
    while(pilha_disc != NULL){
        printf("%s\n", pilha_disc->disciplina);
        pilha_disc = pilha_disc->next;
    }
}

void print_alunos_disciplina(no_aluno pilha){
    if(pilha == NULL){
        printf("Pilha Vazia\n");
        return;
    }
    while(pilha != NULL){
        printf("%s: %ld\n", pilha->nome_aluno, pilha->num_est);
        pilha = pilha->next;
    }
}

pilha_disciplinas pesquisa_disciplina(char *disiciplina, pilha_disciplinas pilha_disc){
    pilha_disciplinas aux = pilha_disc;
    if(pilha_disc == NULL){
        printf("Pilha de disciplinas vazia\n");
        return pilha_disc;
    }else{
        while(aux && strcasecmp(aux->disciplina, disiciplina) != 0){
            aux = aux->next;
        }
        if(aux) return aux;
    }
    return pilha_disc;
}

/*Destroi Pilha de disciplinas e respetiva pilha de alunos de cada disciplina*/
void destroi_pilha_disciplinas(pilha_disciplinas pilha_disc){
    pilha_disciplinas aux;
    while(pilha_disc != NULL){
        aux = pilha_disc;
        pilha_disc = pilha_disc->next;
        if(aux->alunos != NULL){
            destroi_lista_alunos(aux->alunos);
        }
        free(aux);
    }
}