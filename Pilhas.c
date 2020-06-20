#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>
#include "Pilhas.h"

#define DIM 200
#define DEBUG 0 //For Testing Purposes Only

static pilha_alunos pilha_al = NULL;
static pilha_disciplinas pilha_disc = NULL;

/****** Interface da pilha de alunos */
//Alocação de nó de uma aluno
aluno aloc_aluno(char *nome, int num) {
    aluno novo_aluno = (aluno) calloc(1, sizeof(struct _alunos));
    novo_aluno->nome_aluno = strdup(nome);
    novo_aluno->num_est = num;
    return novo_aluno;
}

//Adicionar um nó na pilha alunos
void push_aluno(char *nome, int num) {
    pilha_alunos novo_aluno = (pilha_alunos) calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = pilha_al;

    if (aux == NULL || strcasecmp(aux->info_aluno->nome_aluno, nome) >= 0) {
        if (aux != NULL && strcasecmp(aux->info_aluno->nome_aluno, nome) == 0) {
            if (DEBUG) printf("Nome ja exixtente\n");
        }
        novo_aluno->info_aluno = aloc_aluno(nome, num);
        novo_aluno->next = aux;
        pilha_al = novo_aluno;
    } else {
        while (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, nome) == 0) {
            if (DEBUG) printf("Nome ja existente\n");
        } else {
            novo_aluno->info_aluno = aloc_aluno(nome, num);
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
}

/* Pesquisar determinado aluno por número de estudante */
aluno pesquisa_aluno(long num_aluno) {
    pilha_alunos aux = pilha_al;
    if (pilha_al == NULL) {
        if (DEBUG) printf("Lista Vazia\n");
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

//Remover um aluno da pilha de alunos
pilha_alunos remove_aluno(aluno pop_aluno) {
    pilha_alunos aux = pilha_al, aux_rem;

    if (pilha_al == NULL) {
        if (DEBUG) printf("Lista Vazia\n");
        return pilha_al;
    } else if (pilha_al->next == NULL) { //A pilha tem apenas 1 no
        aux_rem = pilha_al;
        pilha_al = pilha_al->next;
        free(aux_rem);
        return pilha_al;
    } else if (pilha_al->info_aluno == pop_aluno) { //Primeiro aluno da pilha a remover
        aux_rem = pilha_al;
        pilha_al = pilha_al->next;
        free(aux_rem);
        return pilha_al;
    } else {
        while (aux && aux->next && aux->next->info_aluno != pop_aluno) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            if (DEBUG) printf("Numero de aluno inexistente\n");
            return pilha_al;
        }
        aux_rem = aux->next->next;
        free(aux->next);
        aux->next = aux_rem;
        return pilha_al;
    }
}

//Pop de um aluno, sempre no topo da pilha
aluno pop_aluno() {
    aluno rem_aluno = pilha_al->info_aluno;
    remove_aluno(pilha_al->info_aluno);
    return rem_aluno;
}

//Imprime lista alunos de uma determinada disciplina
void print_lista_alunos() {
    pilha_alunos iterador = pilha_al;
    if (pilha_al == NULL) {
        if (DEBUG) printf("Lista Vazia\n");
        return;
    }
    while (iterador != NULL) {
        printf("%s %ld\n", iterador->info_aluno->nome_aluno, iterador->info_aluno->num_est);
        iterador = iterador->next;
    }
}

//Lê ficheiro de texto recebido como input
void read_alunos_num(FILE *input_file) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) { //Se o fgets nao ler nada devolve NULL se ler devolve a string
        validar_fich_alunos(elimina_final(linha), num_linha);
        num_linha++;
    }
}

//Verifica se a linha lida está no formato correto
void validar_fich_alunos(char *linha, int num) {
    char *nome;
    int num_est;
    regex_t linha_fich;

    regcomp(&linha_fich, "[A-Za-z ]+#[0-9]+$", REG_EXTENDED);
    if (regexec(&linha_fich, linha, 0, NULL, 0) == 0) {
        nome = trim(strtok(linha, "#"));
        num_est = atoi(strtok(NULL, "\n"));
        push_aluno(nome, num_est); //Insere aluno na pilha (pilha_al)
    } else {
        //Caso a linha não esteja no formato correto apresenta erro
        fprintf(stderr, "Erro ao ler linha %d do ficheiro de alunos\n", num);
    }
}

//Verfica se um determinado aluno está aprovado às diferentes cadeiras a que está inscrito
void verfica_aprov() {
    lista_notas aux;
    pilha_alunos iterador;
    if (pilha_al == NULL) return;
    iterador = pilha_al;
    while (iterador != NULL) {
        aux = iterador->info_aluno->notas;
        if (aux == NULL) return;
        while (aux != NULL) {
            if (aux->med_final >= 9.45) iterador->info_aluno->aproved++;
            aux = aux->next;
        }
        iterador = iterador->next;
    }
}

//Escreve para um ficheiro os alunos que tiveram sucesso escolar
void alunos_aprov_file(char *alunos_aprov) {
    aluno rem;
    FILE *fp = fopen(alunos_aprov, "w");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s\n", alunos_aprov);
        exit(-1);
    }
    if (pilha_al == NULL || fp == NULL) return;
    while (pilha_al != NULL) {
        rem = pop_aluno();
        if (rem->aproved >= 8)
            fprintf(fp, "%s\t%ld\n", rem->nome_aluno, rem->num_est);
        destroi_lista_notas(rem->notas);
        free(rem);
    }
    fclose(fp);
}

/****** Interface da pilha de disciplinas */
//Alocação de nó de uma disciplina
disciplina aloc_disciplina(char *disc) {
    disciplina nova_disciplina = (disciplina) calloc(1, sizeof(struct _disciplinas));
    nova_disciplina->nome_disc = strdup(disc);
    return nova_disciplina;
}

//Adicionar um nó na pilha disciplinas
pilha_disciplinas push_disciplina(char *disc) {
    pilha_disciplinas nova_disciplina = (pilha_disciplinas) calloc(1, sizeof(struct _pilha_disciplinas));
    pilha_disciplinas aux = pilha_disc;

    if (pilha_disc == NULL || strcasecmp(pilha_disc->info_disciplina->nome_disc, disc) >= 0) {
        if (pilha_disc != NULL && strcasecmp(pilha_disc->info_disciplina->nome_disc, disc) == 0) {
            if (DEBUG) printf("Disiciplina ja existente\n");
            return pilha_disc;
        }
        nova_disciplina->info_disciplina = aloc_disciplina(disc);
        nova_disciplina->next = pilha_disc;
        return nova_disciplina;
    } else {
        while (aux->next && strcasecmp(aux->next->info_disciplina->nome_disc, disc) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->info_disciplina->nome_disc, disc) == 0) {
            if (DEBUG) printf("Disciplina ja existente\n");
            return pilha_disc;
        } else {
            nova_disciplina->info_disciplina = aloc_disciplina(disc);
            nova_disciplina->next = aux->next;
            aux->next = nova_disciplina;
        }
    }
    return pilha_disc;
}

//Remover uma disciplina da pilha de disciplinas
pilha_disciplinas remove_disciplina(disciplina pop_disciplina) {
    pilha_disciplinas aux = pilha_disc, aux_rem;

    if (pilha_disc == NULL) {
        if (DEBUG) printf("Lista Vazia\n");
        return pilha_disc;
    } else if (pilha_disc->next == NULL) { //A lista tem apenas 1 no
        aux_rem = pilha_disc;
        pilha_disc = pilha_disc->next;
        free(aux_rem);
        return pilha_disc;
    } else if (pilha_disc->info_disciplina == pop_disciplina) { //Primeiro aluno da pilha a remover
        aux_rem = pilha_disc;
        pilha_disc = pilha_disc->next;
        free(aux_rem);
        return pilha_disc;
    } else {
        while (aux && aux->next && aux->next->info_disciplina != pop_disciplina) {
            aux = aux->next;
        }
        if (aux->next == NULL) {
            if (DEBUG) printf("Numero de aluno inexistente\n");
            return pilha_disc;
        }
        aux_rem = aux->next->next;
        free(aux->next);
        aux->next = aux_rem;
        return pilha_disc;
    }
}

//Pop de uma disciplina, sempre no topo da pilha
disciplina pop_disciplina() {
    disciplina rem_disc = pilha_disc->info_disciplina;
    remove_disciplina(pilha_disc->info_disciplina);
    return rem_disc;
}

//Adiciona um aluno à lista de alunos de uma determinada disciplina
pilha_alunos push_aluno_to_disc(aluno aluno, pilha_alunos lista_alunos) {
    pilha_alunos novo_aluno = (pilha_alunos) calloc(1, sizeof(struct _pilha_alunos));
    pilha_alunos aux = lista_alunos;
    if (lista_alunos == NULL || strcasecmp(lista_alunos->info_aluno->nome_aluno, aluno->nome_aluno) >= 0) {
        if (lista_alunos != NULL && lista_alunos->info_aluno->num_est == aluno->num_est) {
            if (DEBUG) printf("Aluno ja existente\n");
            return lista_alunos;
        }
        novo_aluno->info_aluno = aluno;
        novo_aluno->next = lista_alunos;
        return novo_aluno;
    } else {
        while (aux->next && strcasecmp(aux->next->info_aluno->nome_aluno, aluno->nome_aluno) < 0) {
            aux = aux->next;
        }
        if (aux->next && aux->next->info_aluno->num_est == aluno->num_est) {
            if (DEBUG) printf("Aluno ja existente\n");
            return lista_alunos;
        } else {
            novo_aluno->info_aluno = aluno;
            novo_aluno->next = aux->next;
            aux->next = novo_aluno;
        }
    }
    return lista_alunos;
}

//Lista todas as disciplinas da pilha de disciplinas
void print_pilha_disciplinas() {
    pilha_disciplinas iterador = pilha_disc;
    if (pilha_disc == NULL) {
        printf("Pilha de disciplinas vazia\n");
        return;
    }
    while (iterador != NULL) {
        printf("%s\n", iterador->info_disciplina->nome_disc);
        iterador = iterador->next;
    }
}

//Lista todos os alunos de uma determinada disciplina
void print_alunos_disciplina(pilha_alunos pilha) {
    pilha_alunos iterador = pilha;
    if (pilha == NULL) {
        printf("Pilha Vazia\n");
        return;
    }
    while (iterador != NULL) {
        printf("%s: %ld\n", iterador->info_aluno->nome_aluno, iterador->info_aluno->num_est);
        iterador = iterador->next;
    }
}

//Pesquisa uma determinada disciplina na pilha de disciplinas
disciplina pesquisa_disciplina(char *disiciplina) {
    pilha_disciplinas aux = pilha_disc;
    if (pilha_disc == NULL) {
        if (DEBUG) printf("Pilha de disciplinas vazia\n");
        return NULL;
    } else {
        while (aux && strcasecmp(aux->info_disciplina->nome_disc, disiciplina) != 0) {
            aux = aux->next;
        }
        if (aux) return aux->info_disciplina;
    }
    return NULL;
}

//Escreve em 10 ficheiros (1 para cada disciplina) a lista de alunos inscritos e respetivos resultados nas provas
void write_output(char *file_prefixe) {
    FILE *fp;
    pilha_alunos aux, aux_rem;
    disciplina rem;
    char nome_ficheiro[DIM];
    if (pilha_disc == NULL) return;
    while (pilha_disc != NULL) {
        rem = pop_disciplina();
        sprintf(nome_ficheiro, "%s%s.txt", file_prefixe, rem->nome_disc);
        fp = fopen(nome_ficheiro, "w");
        if (fp == NULL) {
            fprintf(stderr, "Erro ao abrir o ficheiro %s\n", nome_ficheiro);
            exit(-1);
        }
        aux = rem->alunos;
        while (aux != NULL) {
            lista_notas notas = pesquisa_disciplina_aluno(rem->nome_disc, aux->info_aluno->notas);
            fprintf(fp, "%s\t%.2f\n", aux->info_aluno->nome_aluno, notas->med_final);
            aux_rem = aux;
            aux = aux->next;
            free(aux_rem);
        }
        free(rem);
        fclose(fp);
    }
}

/****** Interface da pilha de disciplinas */
//Alocação de nó de uma nota
lista_notas aloc_nota(char *prova, float nota, char *disciplina) {
    lista_notas nova_disciplina = (lista_notas) calloc(1, sizeof(struct _notas_alunos));
    nova_disciplina->disc = strdup(disciplina);
    if (preencher_notas(prova, nota, nova_disciplina) != 0) return NULL;
    return nova_disciplina;
}

//Preenche no campo certo a respetiva nota e prova recebida
char preencher_notas(char *prova, float nota, lista_notas disciplina) {
    //Verifica que nota deve preencher (nota1 ou nota2)
    if (strcasecmp(prova, "prova1") == 0) {
        disciplina->nota1 = nota;
        disciplina->prova1 = strdup(prova);
        return 0;
    } else if (strcasecmp(prova, "prova2") == 0) {
        disciplina->nota2 = nota;
        disciplina->prova2 = prova;
        return 0;
    }
    if (DEBUG) printf("Disciplina com notas completas\n");
    return 1;
}

//Insere uma nota na lista de notas de um determinado aluno
lista_notas insere_dis_to_aluno(char *prova, float nota, char *disciplina, lista_notas pilha) {
    lista_notas nova_nota;
    lista_notas aux = pilha;

    if (pilha == NULL || strcasecmp(pilha->disc, disciplina) >= 0) {
        if (pilha != NULL && strcasecmp(pilha->disc, disciplina) == 0) { //Se for o primeiro nó, preencher notas
            preencher_notas(prova, nota, pilha);
            return pilha;
        }
        aux = aloc_nota(prova, nota, disciplina); //Se a lista tiver vazia
        aux->next = pilha;
        return aux;
    } else {
        while (aux->next && strcasecmp(aux->next->disc, disciplina) < 0) {
            aux = aux->next;
        }
        if (aux->next && strcasecmp(aux->next->disc, disciplina) == 0) {
            preencher_notas(prova, nota, aux->next);
            return pilha; //Se existe já a disciplina, acabar de preencher notas
        } else {
            //Se a disciplina não existe na pilha, inserir
            nova_nota = aloc_nota(prova, nota, disciplina);
            nova_nota->next = aux->next;
            aux->next = nova_nota;
        }
    }
    return pilha;
}

//Pesquisa uma determinada disciplina de um aluno
lista_notas pesquisa_disciplina_aluno(char *disciplina, lista_notas pilha) {
    lista_notas aux = pilha;
    if (pilha == NULL) {
        if (DEBUG) printf("Pilha Vazia. Aluno nao inscrito em disciplinas\n");
    } else {
        while (aux != NULL && strcasecmp(aux->disc, disciplina) != 0) {
            aux = aux->next;
        }
        if (aux && strcasecmp(aux->disc, disciplina) == 0) {
            return aux;
        }
    }
    return NULL;
}

//Imprime lista de notas de um determinado aluno
void print_lista_disciplinas_aluno(lista_notas pilha) {
    if (pilha == NULL) {
        if (DEBUG) printf("Pilha Vazia\n");
        return;
    }
    while (pilha != NULL) {
        if (pilha->prova1 == NULL) {
            printf("%s:\n%s: %.2f\n", pilha->disc, pilha->prova2, pilha->nota2);
        } else if (pilha->prova2 == NULL) {
            printf("%s:\n%s: %.2f\n", pilha->disc, pilha->prova1, pilha->nota1);
        } else
            printf("%s:\n%s: %.2f\n%s: %.2f\n", pilha->disc, pilha->prova1, pilha->nota1, pilha->prova2, pilha->nota2);
        pilha = pilha->next;
    }
}

//Destroi lista disciplinas
void destroi_lista_notas(lista_notas pilha) {
    lista_notas aux;
    while (pilha != NULL) {
        aux = pilha;
        pilha = pilha->next;
        free(aux);
    }
}

//Lê ficheiro "Lista_Provas.txt"
pilha_disciplinas read_provas(FILE *input_file) {
    char linha[DIM];
    int num_linha = 1;
    while (fgets(linha, DIM, input_file)) {
        pilha_disc = validar_fich_disc(elimina_final(linha), num_linha);
        num_linha++;
    }
    return pilha_disc;
}

//Valida linhas de ficheiro "Lista_Provas.txt" e insere na lista de provas de um determinado aluno
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
            if (DEBUG) printf("Numero invalido\n");
            return pilha_disc;
        }
        discp = strdup(strtok(NULL, "#"));
        prova = strdup(strtok(NULL, "#"));
        nota = (float) atof(strtok(NULL, "\n"));

        aluno->notas = insere_dis_to_aluno(prova, nota, discp, aluno->notas);
        pilha_disc = push_disciplina(discp);
        disc = pesquisa_disciplina(discp);
        if (disc == NULL) {
            if (DEBUG) printf("Disciplina Inexistente\n");
            return pilha_disc;
        }
        disc->alunos = push_aluno_to_disc(aluno, disc->alunos);
        return pilha_disc;

    } else {
        fprintf(stderr, "Erro ao ler linha %d do ficheiro de provas\n", num_linha);
    }
    return pilha_disc;
}

//Calcula a classificação final de todos os alunos a todas as disciplinas
void classificacao_final() {
    lista_notas aux;
    pilha_alunos iterador;
    if (pilha_al == NULL) return;
    iterador = pilha_al;
    while (iterador != NULL) {
        aux = iterador->info_aluno->notas;
        while (aux != NULL) {
            if (aux->prova1 == NULL) {
                aux->med_final = aux->nota2;
            } else if (aux->prova2 == NULL) {
                aux->med_final = aux->nota1;
            } else
                aux->med_final = (aux->nota1 + aux->nota2) / 2;
            aux = aux->next;
        }
        iterador = iterador->next;
    }
}

//Lista as classificações finais de todos os alunos a todas as disciplinas
void print_class_final() {
    pilha_alunos iterador = pilha_al;
    lista_notas aux;
    if (pilha_al == NULL) return;
    while (iterador != NULL) {
        printf("%s:\n", iterador->info_aluno->nome_aluno);
        aux = iterador->info_aluno->notas;
        while (aux != NULL) {
            printf("\t%s: %.2f\n", aux->disc, aux->med_final);
            aux = aux->next;
        }
        iterador = iterador->next;
    }
}