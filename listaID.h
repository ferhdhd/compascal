#ifndef LISTA_ID_H
#define LISTA_ID_H

#include <stdio.h>


typedef struct nodoID {
    char nome[500];
    char tipo[500];
    char tipo_simbolo[100];
    int escopo;
    struct nodoID *prox;
    struct nodoID *prev;
} nodoID;

typedef struct exp {
    nodoID *nodo_tabela;
    char nome[500];
    char tipo_simbolo[500];
    char tipo[500];
    int id_temporario;
    struct exp *prox;
} exp_t;

nodoID* concatNodo (nodoID *head, char *s, char *tipo_simbolo, int escopo_atual);
void setTipo (nodoID* head, char* tipo);
void setTipoUm (nodoID* head, char* tipo);
void setTipoSimb (nodoID* head, char* t_simb);
nodoID* attTabelaSimbolos (nodoID* head, nodoID* aux);
void printTs (nodoID* ts);
nodoID* simboloExisteTabela (nodoID* ts,nodoID *novaLista);
nodoID* procuraTabelaSimbolos (nodoID* ts, char *nome);
nodoID* procuraTabelaSimbolosFunc (nodoID* ts, char *nome, char *tipo_simbolo);
int destroiLista (nodoID *head);
nodoID* destroiLocais (nodoID *head);
int ehFloat (char* num);

exp_t* cria_exp(nodoID *ts, FILE *fp, char *tipo_simbolo, char *nome, int id_atual);
exp_t* cria_exp_de_exp(nodoID *ts, FILE *fp, char *tipo_simbolo, exp_t *esq, char* op, exp_t *dir, int id_atual);
exp_t* cria_exp_lista_parametros(exp_t* raiz, exp_t* nova);
exp_t* cria_exp_comparacao(exp_t *esq, char *op, exp_t *dir, int id_atual);

int var_func_proc (nodoID* ts, char *nome);

#endif
