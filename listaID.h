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


nodoID* criaNodoVar (char *s, char *tipo_simbolo, int escopo_atual);
nodoID* concatNodo (nodoID *head, char *s, char *tipo_simbolo, int escopo_atual);
void setTipo (nodoID* head, char* tipo);
nodoID* attTabelaSimbolos (nodoID* head, nodoID* aux);
void printTs (nodoID* ts);
nodoID* simboloExisteTabela (nodoID* ts,nodoID *novaLista);
int destroiLista (nodoID *head);

#endif
