#include "listaID.h"
#include "compiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

nodoID* criaNodoVar (char *s, char *tipo_simbolo, int escopo_atual) {
    printf("Criando Nodo %s\n" ,s);
    nodoID* novoNodo = malloc(sizeof(nodoID));
    novoNodo->escopo = escopo_atual;
    novoNodo->prox = NULL;
    novoNodo->prev = NULL;
    strcpy(novoNodo->tipo, "");
    strcpy(novoNodo->tipo_simbolo, tipo_simbolo);
    strcpy(novoNodo->nome, s);

    return novoNodo;
}

nodoID* concatNodo (nodoID *head, char *s, char *tipo_simbolo, int escopo_atual) {
    printf("Concatenado Nodo %s\n" ,s);
    nodoID* novoNodo = criaNodoVar(s, tipo_simbolo, escopo_atual);

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

void setTipoSimb (nodoID* head, char* tipo_simbolo) {
    printf("Setando Tipo Símbolo: %s\n" , tipo_simbolo);
    while (head) {
        strcpy(head->tipo_simbolo, tipo_simbolo);
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
        printf("nome: %s, simbolo: %s, tipo: %s, escopo: %d \n", ts->nome, ts->tipo_simbolo, ts->tipo, ts->escopo);
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

nodoID* destroiGlobais (nodoID *head) {
    printf("Destruindo lista\n");
    
    nodoID *aux;
    if (head) {
        while (head->escopo == 1) {
            aux = head->prev;
            free(head);
            head = aux;
        }
        
        head->prox = NULL;
    }
    return head;
}


