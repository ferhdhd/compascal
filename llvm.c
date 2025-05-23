#include "llvm.h"
#include "compiler.h"
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
        
        if (strcmp("retorno", nodo->tipo_simbolo) && nodo->prox && (!strcmp("parametro", nodo->prox->tipo_simbolo) || !strcmp("parametro-ponteiro", nodo->prox->tipo_simbolo))) {
            fprintf(fp, ", "); // na primeira iteracao, nao coloca virgula
        }

        aux = nodo;
        nodo = nodo->prox;
    } while (nodo && (!strcmp("parametro", nodo->tipo_simbolo) || !strcmp("parametro-ponteiro", nodo->tipo_simbolo)));
    fprintf(fp, ") {\n"); // fim dos parametros

    fprintf(fp, "entry:\n\n"); // define o label do começo da func

    // parametros escritos, agora eles precisam ser alocados como ponteiros na funcao em llvm
    // vamos voltar até o primeiro nodo de parâmetro
    printf("tipo-simb: %s\n" , aux->tipo_simbolo);
    // eh preciso colocar o caso de ser variavel, pois se a funcao nao tiver parametros, a primeira variavel nunca seria escrita no llvm
    while (aux && (!strcmp("parametro", aux->tipo_simbolo) || !strcmp("parametro-ponteiro", aux->tipo_simbolo)) || !strcmp("variavel", aux->tipo_simbolo) || !strcmp("retorno", aux->tipo_simbolo))
        aux = aux->prev;
    aux = aux->prox; // apontando para o primeiro parametro

    do {
        if (!strcmp("parametro", aux->tipo_simbolo)) {
            fprintf(fp, "%%%s = alloca %s\n", aux->nome, converteTipo(aux->tipo));
            fprintf(fp, "store %s %%%s_t, ptr %%%s\n", converteTipo(aux->tipo), aux->nome, aux->nome);
        } else if (!strcmp("variavel", aux->tipo_simbolo) || !strcmp("retorno", aux->tipo_simbolo)) {
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

void emiteSubtracao (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, int id_atual) {
    if (!strcmp("REAL", exp_esq->tipo))
        fprintf(fp, "%%%d = sub %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    else
        fprintf(fp, "%%%d = sub %s %%%d, %%%d\n", id_atual, converteTipo(exp_dir->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
}

void emiteOr (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, int id_atual) {
    if (!strcmp("REAL", exp_esq->tipo))
        fprintf(fp, "%%%d = or %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    else
        fprintf(fp, "%%%d = or %s %%%d, %%%d\n", id_atual, converteTipo(exp_dir->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
}


void emiteOpMult (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, char *op, int id_atual) {
    if (!strcmp("*", op)) {
        if (!strcmp("REAL", exp_esq->tipo))
            fprintf(fp, "%%%d = fmul %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
        else
            fprintf(fp, "%%%d = mul %s %%%d, %%%d\n", id_atual, converteTipo(exp_dir->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    } else if (!strcmp("div", op)) {
        fprintf(fp, "%%%d = sdiv %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    } else if (!strcmp("mod", op)) {
        fprintf(fp, "%%%d = srem %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    } else if (!strcmp("and", op)) {
        fprintf(fp, "%%%d = and %s %%%d, %%%d\n", id_atual, converteTipo(exp_esq->tipo), exp_esq->id_temporario, exp_dir->id_temporario);
    }
 }

void emiteProcSemPar (FILE *fp, char* proc, nodoID* ts) {
    nodoID *ts_proc = procuraTabelaSimbolos(ts, proc);

    if (!ts_proc) {
        char erro[1000];
        sprintf(erro, "A função não foi declarada anteriormente!\n");
        yyerror(erro);    
    }
    
    fprintf(fp, "call %s @%s()\n" , converteTipo(ts_proc->tipo), proc);
}

void emiteProcComPar (FILE *fp, char *proc, exp_t *parametros, nodoID *ts) {
    nodoID *ts_proc = procuraTabelaSimbolos(ts, proc);

    if (!ts_proc) {
        char erro[1000];
        sprintf(erro, "A função não foi declarada anteriormente!\n");
        yyerror(erro);    
    }

    nodoID *par_proc = ts_proc->prox; // primeiro parametro do proc na lista de simbolos
    
    if (((strcmp("parametro", par_proc->tipo_simbolo)) && strcmp("parametro-ponteiro", par_proc->tipo_simbolo)) && parametros) {
        char erro[1000];
        sprintf(erro, "Uma função que não tem parâmetros em sua declaração foi chamada com parâmetros!\n");
        yyerror(erro);       
    }
    
    fprintf(fp, "call %s @%s(" , converteTipo(ts_proc->tipo), proc);

    while (!strcmp("parametro-ponteiro", par_proc->tipo_simbolo) || !strcmp("parametro", par_proc->tipo_simbolo)) {
        printf("par_proc_tipo: %s\n" , par_proc->tipo_simbolo);
        printf("parametro_tipo: %s\n" , par_proc->tipo_simbolo);
        
        if (parametros == NULL) {
            char erro[1000];
            sprintf(erro, "A função espera mais parâmetros, além dos que foram delcarados!\n");
            yyerror(erro);       
        }
        
        if (!strcmp(par_proc->tipo, parametros->tipo) != 1) {
            char erro[1200];
            sprintf(erro, "o tipo do parâmetro passado %s é diferente do parâmetro %s declarado!\n", par_proc->nome, parametros->nome);
            yyerror(erro);       
        }

        fprintf(fp, "%%%d", parametros->id_temporario);
        
        if (parametros->prox) // se tiver mais parametros, printa a virgula
            fprintf(fp, ",");
        
        par_proc = par_proc->prox;
        parametros = parametros->prox;
    }
    
    fprintf(fp, ")\n");
    
    return;
}

void emiteErroRetorno(nodoID *ts) {
    while (strcmp("funcao", ts->tipo_simbolo))
        ts = ts->prev;
    
    char erro[1000];
    sprintf(erro, "Função %s não tem retorno!\n" , ts->nome);
    yyerror(erro);   
}

int armazenaVar (FILE *fp, char *var, exp_t *exp, nodoID *ts) {
    int eh_retorno = 0;
    nodoID *aux = procuraTabelaSimbolos(ts, var);
    if (aux == NULL) {
        char erro[1000];
        sprintf(erro, "a variavel %s nao foi declarada anteriormente!\n", var);
        yyerror(erro);     
    } else if (!strcmp(aux->tipo, "INTEIRO") && !strcmp(exp->tipo, "REAL")) {
        char erro[1000];
        sprintf(erro, "a variavel %s eh do tipo inteiro, enquanto a expressao eh real!\n", var);
        yyerror(erro);  
    }

    if (!strcmp("retorno", aux->tipo_simbolo))
        eh_retorno = 1;

    if (aux->escopo == 0)
        fprintf(fp, "store %s %%%d, ptr @%s\n", converteTipo(aux->tipo), exp->id_temporario, aux->nome);
    else
        fprintf(fp, "store %s %%%d, ptr %%%s\n", converteTipo(aux->tipo), exp->id_temporario, aux->nome);

    return eh_retorno;
 }
