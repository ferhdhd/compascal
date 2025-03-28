#include "listaID.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

nodoID* criaNodo (char *s) {
    printf("Criando Nodo %s\n" ,s);
    nodoID* novoNodo = malloc(sizeof(nodoID));

    novoNodo->ehGlobal = 99;
    novoNodo->prox = NULL;
    strcpy(novoNodo->tipo, "");
    novoNodo->ehFunc = 0;
    strcpy(novoNodo->nome, s);

    return novoNodo;
}

nodoID* concatNodo (nodoID *head, char *s) {
    printf("Concatenado Nodo %s\n" ,s);
    nodoID* novoNodo = criaNodo(s);

    if (head == NULL) {
        head = novoNodo;
    } else {
        while (head->prox != NULL)
            head = head->prox;
        head->prox = novoNodo;
    }

    return head;
}

void setTipo (nodoID* head, char* tipo) {
    printf("Setando Tipo: %s\n" , tipo);
    while (head) {
        strcpy(head->tipo, tipo);
        head = head->prox;
    }
}

void attTabelaSimbolos (nodoID* head, nodoID* aux) {
    if (!head) {
        head = aux;
    } else {
        while (head->prox)
            head = head->prox;
        head->prox = aux;
    }

    return;
}

void printTs (nodoID* ts) {
    while (ts) {
        printf("----- TABELA DE SÍMBOLOS -----\n");
        printf("nome: %s, op: %d, tipo: %s \n", ts->nome, ts->ehFunc, ts->tipo);
        printf("------------------------------\n");
        ts = ts->prox;
    }
}

int destroiLista (nodoID *head) {
    printf("Destruindo lista\n");
    
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

