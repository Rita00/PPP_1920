#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>
#include "Pilhas.h"

#define DIM 200

static pilha_alunos pilha_al;
static pilha_disciplinas pilha_disc;

/****** Interface da pilha de alunos */
/*Alocacao de no da pilha*/
aluno aloc_aluno(char *nome, int num) {
    aluno novo_aluno = (aluno) calloc(1, sizeof(struct _alunos));
    novo_aluno->nome_aluno = strdup(nome);
    novo_aluno->num_est = num;
    return novo_aluno;
}

/*Inserir na lista*/
void push_aluno(char *nome, int num) {
    pilha_alunos novo_aluno = (pilha_alunos) calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha_al;

    if (!aux || strcasecmp(aux->info_aluno->nome_aluno, nome) >= 0) {
        if (aux && strcasecmp(aux->info_aluno->nome_aluno, nome) == 0) {
            //printf("Nome ja exixtente\n");
        }
        novo_aluno->info_aluno = aloc_aluno(nome, num);
        novo_aluno->next = aux;
        pilha_al = novo_aluno;
    } else {
        while (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) == 0) {
            //printf("Nome ja existente\n");
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
        //printf("Lista Vazia\n");
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
        //printf("Lista Vazia\n");
        return pilha_al;
    } else if (pilha_al->next == NULL) { //A lista tem apenas 1 no
        free(pilha_al);
        return pilha_al;
    } else if (pilha_al->info_aluno == pop_aluno) { //Primeiro aluno da pilha a remover
        aux_rem = pilha_al->next;
        free(pilha_al);
        return aux_rem;
    } else {
        while (aux && aux->next && aux->next->info_aluno != pop_aluno) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            //printf("Numero de aluno inexistente\n");
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
    pilha_alunos iterador = pilha_al;
    if (pilha_al == NULL) {
        //printf("Lista Vazia\n");
        return;
    }
    while (iterador != NULL) {
        printf("%s %ld\n", iterador->info_aluno->nome_aluno, iterador->info_aluno->num_est);
        iterador = iterador->next;
    }
}

/*Destroi lista*/
void destroi_pilha_alunos(char destroi_info) {
    pilha_alunos aux;
    while (pilha_al != NULL) {
        aux = pilha_al;
        pilha_al = pilha_al->next;
        if (destroi_info && aux->info_aluno != NULL) {
            if (aux->info_aluno->pilha_notas != NULL)
                destroi_pilha_notas(aux->info_aluno->pilha_notas);
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
        fprintf(stderr, "Erro ao ler linha %d do ficheiro de alunos\n", num);
    }
}

void verfica_aprov() {
    pilha_notas aux;
    pilha_alunos iterador;
    if (pilha_al == NULL) return;
    iterador = pilha_al;
    while (iterador != NULL) {
        aux = iterador->info_aluno->pilha_notas;
        if (aux == NULL) return;
        while (aux != NULL) {
            if (aux->med_final >= 9.45) iterador->info_aluno->aproved++;
            aux = aux->next;
        }
        iterador = iterador->next;
    }
}

void alunos_aprov_file() {
    pilha_alunos iterador = pilha_al;
    FILE *fp = fopen("Alunos_Aprovados.txt", "a");
    if (pilha_al == NULL || fp == NULL) return;
    while (iterador != NULL) {
        if (iterador->info_aluno->aproved >= 8)
            fprintf(fp, "%s\t%ld\n", iterador->info_aluno->nome_aluno, iterador->info_aluno->num_est);
        iterador = iterador->next;
    }
    fclose(fp);
}

/****** Interface da pilha de disciplinas */
disciplina aloc_disc(char *disc) {
    disciplina nova_disciplina = (disciplina) calloc(1, sizeof(struct _disciplinas));
    nova_disciplina->disc = strdup(disc);
    return nova_disciplina;
}

pilha_disciplinas push_disciplina(char *disc) {
    pilha_disciplinas nova_disciplina = (pilha_disciplinas) calloc(1, sizeof(struct _pilha_disciplinas));
    pilha_disciplinas aux = pilha_disc;

    if (pilha_disc == NULL || strcasecmp(pilha_disc->disciplina->disc, disc) >= 0) {
        if (pilha_disc != NULL && strcasecmp(pilha_disc->disciplina->disc, disc) == 0) {
            //printf("Disiciplina ja existente\n");
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
            //printf("Disciplina ja existente\n");
            return pilha_disc;
        } else {
            nova_disciplina->disciplina = aloc_disc(disc);
            nova_disciplina->next = aux->next;
            aux->next = nova_disciplina;
        }
    }
    return pilha_disc;
}

pilha_disciplinas remove_disciplina(disciplina pop_disciplina) {
    pilha_disciplinas aux = pilha_disc, aux_rem;

    if (pilha_disc == NULL) {
        //printf("Lista Vazia\n");
        return pilha_disc;
    } else if (pilha_disc->next == NULL) { //A lista tem apenas 1 no
        free(pilha_disc);
        return pilha_disc;
    } else if (pilha_disc->disciplina == pop_disciplina) { //Primeiro aluno da pilha a remover
        aux_rem = pilha_disc->next;
        free(pilha_disc);
        return aux_rem;
    } else {
        while (aux && aux->next && aux->next->disciplina != pop_disciplina) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            //printf("Numero de aluno inexistente\n");
            return pilha_disc;
        }
        aux_rem = aux->next->next;
        free(aux->next);
        aux->next = aux_rem;
        return pilha_disc;
    }
}

pilha_disciplinas pop_disciplina(char *pop_dis) {
    return remove_disciplina(pesquisa_disciplina(pop_dis));
}

pilha_alunos push_aluno_to_disc(aluno aluno) {
    pilha_alunos novo_aluno = (pilha_alunos) calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha_al;
    if (pilha_al == NULL || strcasecmp(pilha_al->info_aluno->nome_aluno, aluno->nome_aluno) >= 0) {
        if (pilha_al != NULL && pilha_al->info_aluno->num_est == aluno->num_est) {
            //printf("Aluno ja existente\n");
            return pilha_al;
        }
        novo_aluno->info_aluno = aluno;
        novo_aluno->next = pilha_al;
        return novo_aluno;
    } else {
        while (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, aluno->nome_aluno) < 0) {
            aux = aux->next;
        }
        if (aux->next && aux->next->info_aluno->num_est == aluno->num_est) {
            //printf("Aluno ja existente\n");
            return pilha_al;
        } else {
            novo_aluno->info_aluno = aluno;
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
    return pilha_al;
}

void print_pilha_disciplinas() {
    pilha_disciplinas iterador = pilha_disc;
    if (pilha_disc == NULL) {
        printf("Pilha de disciplinas vazia\n");
        return;
    }
    while (iterador != NULL) {
        printf("%s\n", iterador->disciplina->disc);
        iterador = iterador->next;
    }
}

void print_alunos_disciplina() {
    pilha_alunos iterador = pilha_al;
    if (pilha_al == NULL) {
        printf("Pilha Vazia\n");
        return;
    }
    while (iterador != NULL) {
        printf("%s: %ld\n", iterador->info_aluno->nome_aluno, iterador->info_aluno->num_est);
        iterador = iterador->next;
    }
}

disciplina pesquisa_disciplina(char *disiciplina) {
    pilha_disciplinas aux = pilha_disc;
    if (pilha_disc == NULL) {
        //printf("Pilha de disciplinas vazia\n");
        return NULL; //todo verificar o null
    } else {
        while (aux && strcasecmp(aux->disciplina->disc, disiciplina) != 0) {
            aux = aux->next;
        }
        if (aux) return aux->disciplina;
    }
    return NULL;
}

/*Destroi Pilha de disciplinas e respetiva pilha de alunos de cada disciplina*/
void destroi_pilha_disciplinas() {
    pilha_disciplinas aux;
    while (pilha_disc != NULL) {
        aux = pilha_disc->next;
        free(pilha_disc);
        pilha_disc = aux;
    }
}

void write_output() {
    pilha_disciplinas iterador = pilha_disc;
    FILE *fp;
    pilha_alunos aux;
    char nome_ficheiro[DIM];
    if (pilha_disc == NULL) return;
    while (iterador != NULL) {
        sprintf(nome_ficheiro, "Pauta_%s.txt", iterador->disciplina->disc);
        fp = fopen(nome_ficheiro, "a");
        aux = iterador->disciplina->alunos;
        while (aux != NULL) {
            fprintf(fp, "%s\t%.2f\n", aux->info_aluno->nome_aluno,
                    pesquisa_disciplina_aluno(iterador->disciplina->disc, aux->info_aluno->pilha_notas)->med_final);
            aux = aux->next;
        }
        iterador = iterador->next;
        fclose(fp);
    }
}

/****** Interface da pilha de disciplinas */
pilha_notas aloc_disciplina(char *prova, float nota, char *disciplina) {
    pilha_notas nova_disciplina = (pilha_notas) calloc(1, sizeof(struct _notas_alunos));
    nova_disciplina->disciplina = strdup(disciplina);
    if (preencher_notas(prova, nota, nova_disciplina) != 0) return NULL;
    return nova_disciplina;
}

char preencher_notas(char *prova, float nota, pilha_notas disciplina) {
    if (strcasecmp(prova, "prova1") == 0) {
        disciplina->nota1 = nota;
        disciplina->prova1 = strdup(prova);
        return 0;
    } else if (strcasecmp(prova, "prova2") == 0) {
        disciplina->nota2 = nota;
        disciplina->prova2 = prova;
        return 0;
    }
    //printf("Disciplina com notas completas\n");
    return 1;
}

pilha_notas push_dis_to_aluno(char *prova, float nota, char *disciplina, pilha_notas pilha) {
    pilha_notas nova_nota;
    pilha_notas aux = pilha;

    if (pilha == 0 || strcasecmp(pilha->disciplina, disciplina) >= 0) {
        if (pilha != 0 && strcasecmp(pilha->disciplina, disciplina) == 0) {
            preencher_notas(prova, nota, pilha);
            return pilha;
        }
        aux = aloc_disciplina(prova, nota, disciplina);
        aux->next = pilha;
        return aux;
    } else {
        while (aux->next && strcasecmp(aux->next->disciplina, disciplina) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->disciplina, disciplina) == 0) {
            preencher_notas(prova, nota, aux->next);
            return pilha;
        } else {
            nova_nota = aloc_disciplina(prova, nota, disciplina);
            nova_nota->next = aux->next;
            aux->next = nova_nota;
        }
    }
    return pilha;
}

/*Pesquisa uma determinada disciplina de um aluno*/
pilha_notas pesquisa_disciplina_aluno(char *disciplina, pilha_notas pilha) {
    pilha_notas aux = pilha;
    if (pilha == NULL) {
        //printf("Pilha Vazia. Aluno nao inscrito em disciplinas\n");
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
void print_pilha_disciplinas_aluno(pilha_notas pilha) {
    if (pilha == NULL) {
        //printf("Pilha Vazia\n");
        return;
    }
    while (pilha != NULL) {
        printf("%s:\n%s: %f\n%s: %f\n", pilha->disciplina, pilha->prova1, pilha->nota1, pilha->prova2, pilha->nota2);
        pilha = pilha->next;
    }
}

/*Destroi lista disciplinas*/
void destroi_pilha_notas(pilha_notas pilha) {
    pilha_notas aux;
    while (pilha != NULL) {
        aux = pilha;
        pilha = pilha->next;
        free(aux);
    }
}

pilha_disciplinas read_provas(FILE *input_file) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) {
        pilha_disc = validar_fich_disc(elimina_final(linha), num_linha);
        num_linha++;
    }
    return pilha_disc;
}

pilha_disciplinas validar_fich_disc(char *linha, int num_linha) {
    int num_est;
    char *discp;
    char *prova;
    float nota;
    aluno aluno;
    disciplina disc;
    regex_t linha_fich_0_19, linha_fich_20;

    regcomp(&linha_fich_0_19, "[0-9]+#[A-Za-z]+#[Pp][Rr][Oo][Vv][Aa][12]#[01]?[0-9].[0-9][0-9]*$", REG_EXTENDED);
    regcomp(&linha_fich_20, "[0-9]+#[A-Za-z]+#[Pp][Rr][Oo][Vv][Aa][12]#[2][0].[0][0]*$", REG_EXTENDED);

    if (regexec(&linha_fich_0_19, linha, 0, NULL, 0) == 0 || regexec(&linha_fich_20, linha, 0, NULL, 0) == 0) {
        num_est = atoi(strtok(linha, "#"));
        aluno = pesquisa_aluno(num_est);
        if (aluno == NULL) {
            //printf("Numero invalido\n");
            return pilha_disc;
        }
        discp = strdup(strtok(NULL, "#"));
        prova = strdup(strtok(NULL, "#"));
        nota = atof(strtok(NULL, "\n")); //todo mudar atoi e atof

        aluno->pilha_notas = push_dis_to_aluno(prova, nota, discp, aluno->pilha_notas);
        pilha_disc = push_disciplina(discp);
        disc = pesquisa_disciplina(discp);
        if (disc == NULL) {
            //printf("Disciplina Inexistente\n");
            return pilha_disc;
        }
        disc->alunos = push_aluno_to_disc(aluno);
        return pilha_disc;

    } else {
        fprintf(stderr, "Erro ao ler linha %d do ficheiro de provas\n", num_linha);
    }
    return pilha_disc;
}

void classificacao_final() {
    pilha_notas aux;
    pilha_alunos iterador;
    if (pilha_al == NULL) return; //todo print pilha vazia??
    iterador = pilha_al;
    while (iterador != NULL) {
        aux = iterador->info_aluno->pilha_notas;
        while (aux != NULL) {
            aux->med_final = (aux->nota1 + aux->nota2) / 2;
            aux = aux->next;
        }
        iterador = iterador->next;
    }
}

void print_class_final() {
    pilha_alunos iterador = pilha_al;
    pilha_notas aux;
    if (pilha_al == NULL) return;
    while (iterador != NULL) {
        printf("%s:\n", iterador->info_aluno->nome_aluno);
        aux = iterador->info_aluno->pilha_notas;
        while (aux != NULL) {
            printf("\t%s: %f\n", aux->disciplina, aux->med_final);
            aux = aux->next;
        }
        iterador = iterador->next;
    }
}