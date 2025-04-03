#ifndef LISTA_ID_H
#define LISTA_ID_H

//typedef enum TipoSimbolo_e {VARIAVEL, FUNCAO} TipoSimbolo;

typedef struct nodoID {
    char nome[500];
    char tipo[500];
    char tipo_simbolo[100];
    int escopo;
    struct nodoID *prox;
    struct nodoID *prev;
} nodoID;

nodoID* concatNodo (nodoID *head, char *s, char *tipo_simbolo, int escopo_atual);
void setTipo (nodoID* head, char* tipo);
void setTipoSimb (nodoID* head, char* t_simb);
nodoID* attTabelaSimbolos (nodoID* head, nodoID* aux);
void printTs (nodoID* ts);
nodoID* simboloExisteTabela (nodoID* ts,nodoID *novaLista);
int destroiLista (nodoID *head);
nodoID* destroiGlobais (nodoID *head);

#endif
