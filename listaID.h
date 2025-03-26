#ifndef LISTA_ID_H
#define LISTA_ID_H

typedef struct nodoID {
    char *var;
    char *tipo;
    int ehGlobal;
    struct nodoID *prox;
} nodoID;


nodoID* criaNodo (char *s);
int concatNodo(nodoID *head, char *s);
int destroiLista(nodoID *head);

#endif
