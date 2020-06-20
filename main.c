/* Na linha de comandos deve-se passar os argumentos "Lista_alunos_num_est.txt", "Lista_Provas.txt",
 * "Pauta_" e "Alunos_Aprovados.txt", respetivamente
 * O primeiro ficheiro contém o nome e número de estudante de cada aluno
 * O segundo ficheiro as respetivas disciplinas e notas
 * O terceiro argumento vai gerar 10 ficheiros com as respetivas pautas a cada disciplina
 * O quarto ficheiro gera uma lista com os alunos que tiveram sucesso escolar */

/* Na main é apresentado um menu que serviu para testar código exclusivamente.
 * Caso o utilizador prima a tecla 0 (que sai do menu) o programa executa o seu principal objetivo que é gerar pautas
 * (uma para cada disciplina e uma final de quem passou de ano). */

/* Mesmo no menu cujo objetivo foi testar código optei por usar programação defensiva
 * sendo possível apenas a introdução de números por parte do utilizador
 * com tamanho máximo 999.999.999.999.999.999 para aceitar os números de estudante semelhantes aos da UC */

/* Para ler os ficheiros usei regex´s que validam ou invalidam determinada linha lida.
 * No ficheiro de nomes dos alunos ("Lista_alunos_num_est.txt"), para evitar que, por algum motivo, o nome contenha
 * no início ou no fim espaços a mais (" ") e isso levasse a posteriores erros no código usei três funções retiradas do
 * seguinte site: https://stackoverflow.com/questions/656542/trim-a-string-in-c */

/* O programa é composto por uma pilha de alunos
 * cada elemento (aluno) dessa pilha contém uma lista de notas */

/*Paralelamente será construida uma pilha de disciplinas
 * cada elemento (disciplina) contém uma lista (ordenada lexicograficamente) de alunos que a frequentam */

// Caso o aluno só tenha realizado uma das duas provas de uma disciplina a média final considera apenas essa nota


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Pilhas.h"

#define INT_DIM 19
#define DIM 200

int main(int argc, char *argv[]) {
    // Verifica se existem todos os ficheiros necessários à execução do programa
    if (argc < 5) {
        fprintf(stderr, "Sem argumentos suficientes\n");
        return -1;
    }

    FILE *file_alunos = fopen(argv[1], "r");
    if (file_alunos == NULL){
        fprintf(stderr, "Erro ao abrir o ficheiro %s", argv[1]);
        return -1;
    }
    read_alunos_num(file_alunos);
    fclose(file_alunos);

    FILE *file_notas = fopen(argv[2], "r");
    if(file_notas == NULL){
        fprintf(stderr, "Erro ao abrir o ficheiro %s", argv[2]);
        return -1;
    }
    read_provas(file_notas);
    fclose(file_notas);

    classificacao_final();

    disciplina pesq_disciplina;
    aluno pesq_aluno;

    verfica_aprov();

    long opcao = 1;
    long num_aluno;
    char disciplina[DIM];
    while (opcao != 0) {
        printf("1 - Listagem de alunos - For Testing Purposes Only\n");
        printf("2 - Listagem de disciplinas - For Testing Purposes Only\n");
        printf("3 - Listagem disciplinas de uma aluno - For Testing Purposes Only\n");
        printf("4 - Listagem alunos de uma disciplina - For Testing Purposes Only\n");
        printf("5 - Listagem Classificaçoes finais - For Testing Purposes Only\n");
        printf("6 - Remover aluno - For Testing Purposes Only\n");
        printf("7 - Remover disciplina - For Testing Purposes Only\n");
        printf("0 - Programa e Sair\n");
        get_number(&opcao);
        switch (opcao) {
            case 1:
                print_lista_alunos();
                break;
            case 2:
                print_pilha_disciplinas();
                break;
            case 3:
                printf("Numero de aluno:\n");
                get_number(&num_aluno);
                pesq_aluno = pesquisa_aluno(num_aluno);
                if (pesq_aluno == NULL) {
                    printf("Aluno Inexistente\n");
                    break;
                }
                print_lista_disciplinas_aluno(pesq_aluno->notas);
                break;
            case 4:
                printf("Nome da disciplina\n");
                fgets(disciplina, DIM, stdin);
                pesq_disciplina = pesquisa_disciplina(elimina_final(disciplina));
                if (pesq_disciplina == NULL) {
                    printf("Disciplina Inexistente\n");
                    break;
                }
                print_alunos_disciplina(pesq_disciplina->alunos);
                break;
            case 5:
                print_class_final();
                break;
            case 6:
                printf("Numero de aluno\n");
                get_number(&num_aluno);
                pesq_aluno = pesquisa_aluno(num_aluno);
                if (pesq_aluno == NULL) {
                    printf("Aluno Inexistente\n");
                    break;
                }
                remove_aluno(pesq_aluno);
                break;
            case 7: //
                printf("Nome da disciplina\n");
                fgets(disciplina, DIM, stdin);
                pesq_disciplina = pesquisa_disciplina(elimina_final(disciplina));
                if (pesq_disciplina == NULL) {
                    printf("Disciplina Inexistente\n");
                    break;
                }
                remove_disciplina(pesq_disciplina);
                break;
            default:
                break;
        }
    }
    write_output(argv[3]);
    alunos_aprov_file(argv[4]);
}

//Protecao de dados
void clear_input() {
    char aux;
    do {
        aux = (char) getchar();
    } while (aux != '\n');
}

char input_number(long *number) {
    char aux[INT_DIM];
    char *safe;
    fgets(aux, INT_DIM, stdin);
    if (aux[strlen(aux) - 1] != '\n') clear_input(); /*if last character was not '\n' must clear input*/
    *number = (int) strtol(aux, &safe, 10);
    if (strlen(aux) <= 1 || !strcmp(safe, aux)) return 0; /*returns 0 if input incorrect*/
    if (!strcmp(safe, "\n")) return 1; /*returns 1 if input was just a number*/
    if (!strcmp(safe, "")) return 2; /*returns 2 if input was too long*/
    return -1; /*returns -1 if input started by a number but had more than it*/
}

void input_errors(char erro) {
    switch (erro) {
        case -1:
            printf("Caracteres invalidos no final do input\n");
            break;
        case 0:
            printf("O input nao e um numero\n");
            break;
        case 1:
            /*printf("Imput correto\n");*/
            break;
        case 2:
            printf("Input demasiado longo. O maior numero possivel e 999.999.999.999.999.999\n");
            break;
        default:
            break;
    }
}

void get_number(long *num) {
    char erro;
    do {
        erro = input_number(num);
        input_errors(erro);
    } while (erro != 1);
}

char *elimina_final(char *linha) {
    if (linha[strlen(linha) - 1] == '\n' && linha[strlen(linha) - 2] == '\r') {
        linha[strlen(linha) - 2] = '\0';
    }
    if (linha[strlen(linha) - 1] == '\n' || linha[strlen(linha) - 1] == '\r') {
        linha[strlen(linha) - 1] = '\0';
    }
    return linha;
}


char *ltrim(char *s) {
    while (isspace((unsigned)*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char *back = s + strlen(s);
    while (isspace((unsigned)*--back));
    *(back + 1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s));
}

