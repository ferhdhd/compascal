#include "listaID.h"
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
    nodo = emiteParametrosFunc(fp, nodo);

    fprintf(fp, "}\n"); // fim da funcao

}

nodoID* emiteParametrosFunc (FILE* fp, nodoID* nodo) {
    do {
        
        
        if (!strcmp("parametro", nodo->tipo_simbolo)) {
            fprintf(fp, "%s ", converteTipo(nodo->tipo));
            fprintf(fp, "%", nodo->nome);
            fprintf(fp, "%s_t ", nodo->nome);
        } else if (!strcmp("parametro-ponteiro", nodo->tipo_simbolo)) {
            fprintf(fp, "ptr ", nodo->nome);
            fprintf(fp, "\%", nodo->nome);
            fprintf(fp, "%s_t", nodo->nome);
        }
        
        if ((!strcmp("parametro", nodo->tipo_simbolo) || !strcmp("parametro-ponteiro", nodo->tipo_simbolo)) && nodo->prox != NULL) {
            fprintf(fp, ", "); // na primeira iteracao, nao coloca virgula
        }

        nodo = nodo->prox;
    } while ((!strcmp("parametro", nodo->tipo_simbolo) || !strcmp("parametro-ponteiro", nodo->tipo_simbolo)));

    fprintf(fp, ") {\n"); // fim dos parametros

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

