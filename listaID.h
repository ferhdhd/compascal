#ifndef LISTA_ID_H
#define LISTA_ID_H

typedef struct nodoID {
    char nome[500];
    char tipo[500];
    int ehFunc;
    int ehGlobal;
    int escopo;
    struct nodoID *prox;
    struct nodoID *prev;
} nodoID;


nodoID* criaNodo (char *s);
nodoID* concatNodo (nodoID *head, char *s);
void setTipo (nodoID* head, char* tipo);
nodoID* attTabelaSimbolos (nodoID* head, nodoID* aux);
void printTs (nodoID* ts);
nodoID* simboloExisteTabela (nodoID* ts,nodoID *novaLista);
int destroiLista (nodoID *head);

#endif
