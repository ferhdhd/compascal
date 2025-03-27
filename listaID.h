#ifndef LISTA_ID_H
#define LISTA_ID_H

typedef struct nodoID {
    char *var;
    char *tipo;
    int ehFunc;
    int ehGlobal;
    int escopo;
    struct nodoID *prox;
} nodoID;


nodoID* criaNodo (char *s);
int concatNodo(nodoID *head, char *s);
int destroiLista(nodoID *head);

#endif
