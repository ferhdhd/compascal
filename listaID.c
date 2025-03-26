#include "listaID.h"
#include <stdlib.h>
#include <string.h>

nodoID* criaNodo (char *s) {
    nodoID* novoNodo = malloc(sizeof(nodoID));

    novoNodo->ehGlobal = 99;
    novoNodo->prox = NULL;
    novoNodo->tipo = NULL;
    strcpy(novoNodo->var, s);

    return novoNodo;
}

int concatNodo (nodoID *head, char *s) {
    nodoID* novoNodo = criaNodo(s);

    if (head == NULL) {
        head = novoNodo;
    } else {
        while (head->prox != NULL)
            head = head->prox;
        head->prox = novoNodo;
    }

    return 1;
}

int destroiLista(nodoID *head) {
    nodoID *aux;
    if (head) {
        while (head != NULL) {
            aux = head->prox;
            free(head);
            head = aux;
        }
        return 1;
    }
    return 0;
}

