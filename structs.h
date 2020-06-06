#ifndef PPP_2020_STRUCTS_H
#define PPP_2020_STRUCTS_H

typedef struct _alunos *aluno;
typedef struct _pilha_alunos *pilha_alunos;
typedef struct _notas_alunos *pilha_notas;
typedef struct _disciplinas *disciplina;
typedef struct _pilha_disciplinas *pilha_disciplinas;

//Estrutura que guarda informacao na pilha (pilha alunos) com o nome e numeros de estudantes
struct _alunos {
    char *nome_aluno;
    long num_est;
    pilha_notas pilha_notas;
    int aproved;
};

struct _pilha_alunos {
    aluno info_aluno;
    pilha_alunos next;
};

//Estrutura que guarda informacao na pilha (pilha_notas) com notas e disciplinas de cada estudante
struct _notas_alunos {
    char *disciplina;
    float nota1;
    float nota2;
    char *prova1;
    char *prova2;
    float med_final;
    pilha_notas next;
};

//Estrutura que guarda informacao dos alunos de determinada disciplina
struct _disciplinas {
    char *disc;
    pilha_alunos alunos;
};

struct _pilha_disciplinas {
    disciplina disciplina;
    pilha_disciplinas next;
};

//Fucoes para protecao de dados
void clear_input();

char input_number(long *number);

void input_errors(char erro);

void get_number(long *num);

char *elimina_final(char *linha);

char *ltrim(char *s);

char *rtrim(char *s);

char *trim(char *s);

#endif //PPP_2020_STRUCTS_H
