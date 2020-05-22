#ifndef PPP_2020_STRUCTS_H
#define PPP_2020_STRUCTS_H

typedef struct _alunos *no_aluno;
typedef struct _pilha_alunos *pilha_alunos;
typedef struct _notas_alunos *pilha_disciplinas_aluno;
typedef struct _disciplinas *no_disciplina;
typedef struct _pilha_disciplinas *pilha_disciplinas;

//Estrutura que guarda informacao na pilha (pilha alunos) com o nome e numeros de estudantes
struct _alunos {
    char *nome_aluno;
    long num_est;
    pilha_disciplinas_aluno pilha_notas;
    no_aluno next; //todo remove
};

struct _pilha_alunos{
    no_aluno novo_aluno;
    pilha_alunos next;
};

//Estrutura que guarda informacao na pilha (pilha_disciplinas_aluno) com notas e disciplinas de cada estudante
struct _notas_alunos {
    char *disciplina;
    float nota1;
    float nota2;
    char *prova1;
    char *prova2;
    pilha_disciplinas_aluno next;
};

//Estrutura que guarda informacao dos alunos de determinada disciplina
struct _disciplinas{
    char *disciplina;
    no_aluno alunos;
    no_disciplina next; //todo remove
};

struct _pilha_disciplinas{
    no_disciplina nova_disciplina;
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
