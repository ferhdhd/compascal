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
} exp_t;

nodoID* concatNodo (nodoID *head, char *s, char *tipo_simbolo, int escopo_atual);
void setTipo (nodoID* head, char* tipo);
void setTipoSimb (nodoID* head, char* t_simb);
nodoID* attTabelaSimbolos (nodoID* head, nodoID* aux);
void printTs (nodoID* ts);
nodoID* simboloExisteTabela (nodoID* ts,nodoID *novaLista);
nodoID* procuraTabelaSimbolos (nodoID* ts, char *nome);
int destroiLista (nodoID *head);
nodoID* destroiLocais (nodoID *head);
int ehFloat (char* num);

exp_t* cria_exp(nodoID *ts, FILE *fp, char *tipo_simbolo, char *nome, int id_atual);

#endif
