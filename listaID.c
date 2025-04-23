#include "listaID.h"
#include "compiler.h"
#include "llvm.h"
#include <stdlib.h>
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

nodoID* procuraTabelaSimbolos (nodoID* ts, char *nome) {
    while (ts) {
        if (!strcmp(ts->nome, nome))
            return ts;
        ts = ts->prev;
    }

    return NULL;
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

nodoID* destroiLocais (nodoID *head) {
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

int ehFloat (char* num) {
    for (int i = 0; i < strlen(num); i++) {
        if (num[i] == '.')
            return 1;
    }

    return 0;
}

exp_t* cria_exp(nodoID *ts, FILE *fp, char *tipo_simbolo, char *nome, int id_atual) {
    exp_t *novo_exp = malloc(sizeof(exp_t));

    strcpy(novo_exp->tipo_simbolo, tipo_simbolo); //primeiramente sao definidos vazios se ja existem em outro momento
    strcpy(novo_exp->nome, nome);                 // como variaveis ja declaradas

    if (!strcmp(novo_exp->tipo_simbolo, "numero")) {
        novo_exp->nodo_tabela = NULL;
        
        if (!strcmp("", nome) != 1) {
            if (ehFloat(nome))
                strcpy(novo_exp->tipo, "REAL");
            else
                strcpy(novo_exp->tipo, "INTEIRO");
        }
        
        emiteNumero(fp, novo_exp, id_atual);
        novo_exp->id_temporario = id_atual;
        printf("EXP: %s, id_temp: %d\n", novo_exp->nome, novo_exp->id_temporario);
    } else {
        novo_exp->nodo_tabela = procuraTabelaSimbolos(ts, nome);

        if (!novo_exp->nodo_tabela) {
                char erro[1000];
                sprintf(erro, "simbolo '%s' nao declarado anteriormente", nome);
                yyerror(erro);
        }
        strcpy(novo_exp->tipo, novo_exp->nodo_tabela->tipo);
        strcpy(novo_exp->tipo_simbolo, tipo_simbolo); // outros tipos sao definidos aqui

        emiteVar(fp, novo_exp, id_atual);
        
        novo_exp->id_temporario = id_atual;

    }

    return novo_exp;
}

exp_t* cria_exp_de_exp(nodoID *ts, FILE *fp, char *tipo_simbolo, exp_t *esq, char *op, exp_t *dir, int id_atual) {
    
    // verificacao de erros para operadores multiplicativos
    if (!strcmp(op, "and") || !strcmp(op, "mod") || !strcmp(op, "div")) {
        if (!(!strcmp(esq->tipo, "INTEIRO") && !strcmp(dir->tipo, "INTEIRO"))) {
            char erro[1000];
            sprintf(erro, "a operação %s precisa de duas expressões de tipo INTEIRO para funcionar!\n", op);
            yyerror(erro);            
        }
    }
    
    exp_t *novo_exp = malloc(sizeof(exp_t));

    strcpy(novo_exp->tipo_simbolo, tipo_simbolo);
    novo_exp->nodo_tabela = NULL;
    strcpy(novo_exp->nome, "exp");
    
    if (!strcmp(esq->tipo, "REAL") || !strcmp(dir->tipo, "REAL")) {
        strcpy(novo_exp->tipo, "REAL");
    } else {
        strcpy(novo_exp->tipo, "INTEIRO");
    }

    novo_exp->id_temporario = id_atual;

    return novo_exp;
}


