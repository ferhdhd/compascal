#include "llvm.h"
#include <string.h>

void emiteGlobal (FILE* fp, nodoID* nodo) {
    
    while (nodo) {
    fprintf(fp, "@%s = global %s 0\n", nodo->nome, converteTipo(nodo->tipo));
    nodo = nodo->prev;
    }
}

void emiteFunc (FILE* fp, nodoID* nodo) {

    while (nodo->escopo != 0) { // precisa iterar sobre as vars locais e parametros ate chegar na def da func
        nodo = nodo->prev;
    }

    fprintf(fp, "\ndefine %s @%s(", converteTipo(nodo->tipo), nodo->nome); // escreve o nome da funcao e seu tipo
    nodo = emiteParametrosFunc(fp, nodo->prox);

}

nodoID* emiteParametrosFunc (FILE* fp, nodoID* nodo) {
    nodoID *aux;
    do {
        if (!strcmp("parametro", nodo->tipo_simbolo)) {
            fprintf(fp, "%s ", converteTipo(nodo->tipo));
            fprintf(fp, "%%");
            fprintf(fp, "%s_t", nodo->nome);
        } else if (!strcmp("parametro-ponteiro", nodo->tipo_simbolo)) {
            fprintf(fp, "ptr ");
            fprintf(fp, "%%");
            fprintf(fp, "%s_t", nodo->nome);
        }
        
        if (nodo->prox && (!strcmp("parametro", nodo->prox->tipo_simbolo) || !strcmp("parametro-ponteiro", nodo->prox->tipo_simbolo))) {
            fprintf(fp, ", "); // na primeira iteracao, nao coloca virgula
        }

        aux = nodo;
        nodo = nodo->prox;
    } while (nodo && (!strcmp("parametro", nodo->tipo_simbolo) || !strcmp("parametro-ponteiro", nodo->tipo_simbolo)));
    fprintf(fp, ") {\n"); // fim dos parametros
    // parametros escritos, agora eles precisam ser alocados como ponteiros na funcao em llvm
    // vamos voltar até o primeiro nodo de parâmetro
    printf("tipo-simb: %s\n" , aux->tipo_simbolo);
    while (aux && (!strcmp("parametro", aux->tipo_simbolo) || !strcmp("parametro-ponteiro", aux->tipo_simbolo)))
        aux = aux->prev;
    aux = aux->prox; // apontando para o primeiro parametro

    do {
        if (!strcmp("parametro", aux->tipo_simbolo)) {
            fprintf(fp, "%%%s = alloca %s\n", aux->nome, converteTipo(aux->tipo));
            fprintf(fp, "store %s %%%s_t, ptr %%%s\n", converteTipo(aux->tipo), aux->nome, aux->nome);
        } else if (!strcmp("variavel", aux->tipo_simbolo)) {
            fprintf(fp, "%%%s = alloca %s\n", aux->nome, converteTipo(aux->tipo));
        }
        aux = aux->prox;
    } while (aux);


    return nodo;
}

char *converteTipo (char* tipo) {
    if (!strcmp(tipo, "INTEIRO"))
        return "i32";
    else if (!strcmp(tipo, "REAL"))
        return "float";
    else if (!strcmp(tipo, "VOID"))
        return "void";
    
}

void emiteNumero(FILE *fp, exp_t *novo_exp, int id_atual) {
    fprintf(fp, "%%%d = add %s 0, %s\n", id_atual, converteTipo(novo_exp->tipo), novo_exp->nome);
}

void emiteVar(FILE *fp, exp_t *novo_exp, int id_atual) {
    fprintf(fp, "%%%d = load %s, ptr %s\n", id_atual, converteTipo(novo_exp->tipo), novo_exp->nome);
}

void emiteSoma (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, int id_atual) {
    if (!strcmp("REAL", exp_esq->tipo))
        fprintf(fp, "%%%d = add %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    else
        fprintf(fp, "%%%d = add %s %%%d, %%%d\n", id_atual, converteTipo(exp_dir->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
}

void emiteOpMult (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, char *op, int id_atual) {
    if (!strcmp("*", op)) {
    
    }
 }


