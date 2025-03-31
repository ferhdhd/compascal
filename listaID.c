#include "listaID.h"
#include "compiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

nodoID* criaNodo (char *s) {
    printf("Criando Nodo %s\n" ,s);
    nodoID* novoNodo = malloc(sizeof(nodoID));

    novoNodo->ehGlobal = 99;
    novoNodo->prox = NULL;
    novoNodo->prev = NULL;
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
        novoNodo->prev = head;
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

nodoID* simboloExisteTabela (nodoID* ts, nodoID *novaLista) { 
    while (novaLista) {
        nodoID *inicioLista = ts;
        while (inicioLista) {
            if (!strcmp(inicioLista->nome, novaLista->nome) && inicioLista->escopo == novaLista->escopo)
                return novaLista;
            printf("primeiro simb: %s segundo simb: %s\n" , inicioLista->nome, novaLista->nome);
            inicioLista = inicioLista->prev;
        }
        novaLista = novaLista->prox;
    }

    return NULL;
}


nodoID* attTabelaSimbolos (nodoID* head, nodoID* aux) {
    if (head) {
        // Verificação para saber se o símbolo já existe na TS
        printf("vai pra funcao\n");
        nodoID *verificacao = simboloExisteTabela(head, aux);
        if (verificacao != NULL) {
            char erro[1000];
            sprintf(erro, "simbolo '%s' ja declarado antes", verificacao->nome);
            yyerror(erro);
        }

        while (head->prox) // Atinge o final da lista de símbolos
            head = head->prox;
        head->prox = aux; // Concatena o final da lista de símbolos com a nova lista a ser adicionada
        aux->prev = head; // Aqui tbm
    }
    while (aux->prox) // Vai até o ultimo item da TS e retorna ele
        aux = aux->prox;

    return aux;
}

void printTs (nodoID* ts) {
    printf("----- TABELA DE SÍMBOLOS -----\n");
    while (ts) {
        printf("nome: %s, op: %d, tipo: %s \n", ts->nome, ts->ehFunc, ts->tipo);
        ts = ts->prev;
    }
    printf("------------------------------\n");
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

