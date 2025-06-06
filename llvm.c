#include "llvm.h"
#include "compiler.h"
#include <string.h>

void emiteReadWrite(FILE *fp) {
    fprintf(fp, "declare i32 @printf(ptr noundef, ...)\n");
    fprintf(fp, "declare i32 @__isoc99_scanf(ptr noundef, ...)\n");
    fprintf(fp, "@read_int = private unnamed_addr constant [3 x i8] c""%%d\\00"", align 1\n");
    fprintf(fp, "@write_int = private unnamed_addr constant [4 x i8] c""%%d\\0A\\00"", align 1\n");
    fprintf(fp, "@read_float = private unnamed_addr constant [3 x i8] c""%%f\\00"", align 1\n");
    fprintf(fp, "@write_float = private unnamed_addr constant [4 x i8] c""%%f\\0A\\00"", align 1\n\n");
}

void emiteGlobal (FILE* fp, nodoID* nodo) {
    
    while (nodo) {
    fprintf(fp, "@%s = global %s 0\n", nodo->nome, converteTipo(nodo->tipo));
    nodo = nodo->prev;
    }
}

void emiteFunc (FILE* fp, nodoID* nodo) {

    while (strcmp("retorno", nodo->tipo_simbolo)) { // precisa iterar sobre as vars locais e parametros ate chegar na def da func
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
    else if (!strcmp(tipo, "1-BIT"))
        return "i1";
}

void emiteNumero(FILE *fp, exp_t *novo_exp, int id_atual) {
    fprintf(fp, "%%%d = add %s 0, %s\n", id_atual, converteTipo(novo_exp->tipo), novo_exp->nome);
}

void emiteVar(FILE *fp, exp_t *novo_exp, int id_atual) {
    if (novo_exp->nodo_tabela->escopo)
        fprintf(fp, "%%%d = load %s, ptr %%%s\n", id_atual, converteTipo(novo_exp->tipo), novo_exp->nome);
    else
        fprintf(fp, "%%%d = load %s, ptr @%s\n", id_atual, converteTipo(novo_exp->tipo), novo_exp->nome);
    
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

void emiteMudancaSinal (FILE *fp, exp_t *exp, int id_atual) {
    if (!strcmp("REAL", exp->tipo))
        fprintf(fp, "%%%d = fmul %s %%%d, -1\n", id_atual, converteTipo(exp->tipo), exp->id_temporario);
    else
        fprintf(fp, "%%%d = mul %s %%%d, -1\n", id_atual, converteTipo(exp->tipo), exp->id_temporario);
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

void emiteComparacao(FILE *fp, exp_t *esq, char *comparacao, exp_t *dir, int id_atual) {
    fprintf(fp, "%%%d = icmp ", id_atual);

    if (!strcmp(">", comparacao))
        fprintf(fp, "sgt");
    else if (!strcmp(">=", comparacao))
        fprintf(fp, "sge");
    else if (!strcmp("<", comparacao))
        fprintf(fp, "slt");
    else if (!strcmp("<=", comparacao))
        fprintf(fp, "sle");
    else if (!strcmp("=", comparacao))
        fprintf(fp, "eq");
    else if (!strcmp("<>", comparacao))
        fprintf(fp, "ne");
    
    fprintf(fp, " %s %%%d, %%%d\n", converteTipo(esq->tipo), esq->id_temporario, dir->id_temporario);
}

void emiteComecoIf(FILE *fp, exp_t *exp, int cont_if) {
    if (strcmp(exp->tipo, "1-BIT")) {
        char erro[1000];
        sprintf(erro, "Expressão incompatível com a declaração do IF!\n");
        yyerror(erro); 
    }

    fprintf(fp, "br i1 %%%d, label %%then_%d, label %%else_%d\n", exp->id_temporario, cont_if, cont_if);
    fprintf(fp, "then_%d:\n", cont_if);
}

void emiteFimThen(FILE *fp, int cont_if) {
    fprintf(fp, "br label %%end_if_%d\n", cont_if);
    fprintf(fp, "else_%d:\n", cont_if);
}

void emiteFimElse(FILE *fp, int cont_if) {
    fprintf(fp, "br label %%end_if_%d\n", cont_if);
    fprintf(fp, "end_if_%d:\n", cont_if);
}

void emiteComecoWhile(FILE *fp, int cont_while) {
    fprintf(fp, "br label %%while_%d\n", cont_while);
    fprintf(fp, "while_%d:\n", cont_while);
}

void emiteDoWhile(FILE *fp, exp_t *exp, int cont_while) {
    if (strcmp(exp->tipo, "1-BIT")) {
        char erro[1000];
        sprintf(erro, "Expressão incompatível com a declaração do WHILE!\n");
        yyerror(erro); 
    }

    fprintf(fp, "br i1 %%%d, label %%do_%d, label %%end_while_%d\n", exp->id_temporario, cont_while, cont_while);
    fprintf(fp, "do_%d:\n", cont_while);
}

void emiteFimWhile(FILE *fp, int cont_while) {
    fprintf(fp, "br label %%while_%d\n" , cont_while);
    fprintf(fp, "%%end_while_%d:\n" , cont_while);
}

void emiteMain (FILE *fp) {
    fprintf(fp, "\ndefine i32 @main() {\n");
    fprintf(fp, "entry:\n");
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

int emiteWrite(FILE *fp, exp_t *parametros, nodoID *ts, int id_atual) {
    while (parametros) {
        if (!strcmp("REAL", parametros->tipo)) {
            fprintf(fp, "%%%d = fpext float %%%d to double\n", id_atual++, parametros->id_temporario);
            fprintf(fp, "%%%d = call i32 (ptr, ...) @printf(ptr @write_float, double %%%d)\n", id_atual++, id_atual-1);
        } else {
            fprintf(fp, "%%%d = call i32 (ptr, ...) @printf(ptr @write_int, i32 %%%d)\n", id_atual++, parametros->id_temporario);
        }
        parametros = parametros->prox;
    }
    return id_atual;
}

int emiteRead(FILE *fp, exp_t *parametros, nodoID *ts, int id_atual) {
    while (parametros) {
        if (parametros->nodo_tabela && strcmp("variavel", parametros->nodo_tabela->tipo_simbolo)) {
            printf("tipo simb: %s\n" , parametros->tipo_simbolo);
            char erro[1000];
            sprintf(erro, "O parâmetro da função READ não é uma variável!\n");
            yyerror(erro);
        }
        
        if (!strcmp("REAL", parametros->tipo)) {
            if (parametros->nodo_tabela->escopo == 1)
                fprintf(fp, "%%%d = call i32 (ptr, ...) @scanf(ptr @read_float, ptr %%%s)\n", id_atual++, parametros->nodo_tabela->nome);
            else
                fprintf(fp, "%%%d = call i32 (ptr, ...) @scanf(ptr @read_float, ptr @%s)\n", id_atual++, parametros->nodo_tabela->nome);
        } else {
            if (parametros->nodo_tabela->escopo == 1)
                fprintf(fp, "%%%d = call i32 (ptr, ...) @scanf(ptr @read_int, ptr %%%s)\n", id_atual++, parametros->nodo_tabela->nome);
            else
                fprintf(fp, "%%%d = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @%s)\n", id_atual++, parametros->nodo_tabela->nome);
        }
        parametros = parametros->prox;
    }
    return id_atual;
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
            sprintf(erro, "A função espera mais parâmetros, além dos que foram declarados!\n");
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

void emiteRetornoFuncao(FILE *fp, exp_t *parametros, exp_t *funcao, nodoID *ts, int id_atual) {
    nodoID *ts_func = procuraTabelaSimbolosFunc(ts, funcao->nome, "funcao");

    if (!ts_func) {
        char erro[1000];
        sprintf(erro, "A função não foi declarada anteriormente!\n");
        yyerror(erro);    
    }

    printTs(ts);
    nodoID *par_func = ts_func->prox->prox; // primeiro parametro do proc na lista de simbolos
    
    if (((strcmp("parametro", par_func->tipo_simbolo)) && strcmp("parametro-ponteiro", par_func->tipo_simbolo)) && parametros) {
        char erro[1000];
        sprintf(erro, "Uma função que não tem parâmetros em sua declaração foi chamada com parâmetros!\n");
        yyerror(erro);       
    }
    
    fprintf(fp, "%%%d := call %s @%s(" , id_atual, converteTipo(ts_func->tipo), funcao->nome);

    while (par_func && (!strcmp("parametro-ponteiro", par_func->tipo_simbolo) || !strcmp("parametro", par_func->tipo_simbolo))) {
        printf("par_func_tipo: %s\n" , par_func->tipo_simbolo);
        printf("parametro_tipo: %s\n" , par_func->tipo_simbolo);
        
        if (parametros == NULL) {
            char erro[1000];
            sprintf(erro, "A função espera mais parâmetros, além dos que foram declarados!\n");
            yyerror(erro);       
        }
        
        if (!strcmp(par_func->tipo, parametros->tipo) != 1) {
            char erro[1200];
            sprintf(erro, "o tipo do parâmetro passado %s é diferente do parâmetro %s declarado!\n", par_func->nome, parametros->nome);
            yyerror(erro);       
        }

        fprintf(fp, "%%%d", parametros->id_temporario);
        
        if (parametros->prox) // se tiver mais parametros, printa a virgula
            fprintf(fp, ",");
        
        par_func = par_func->prox;
        parametros = parametros->prox;
    }

    if (!par_func && parametros) {
        char erro[1000];
        sprintf(erro, "Foram passados mais parâmetros em relação aos que a função espera!\n");
        yyerror(erro);    
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
    
    printf("tipo simobolo armazena var: %s\n", aux->tipo_simbolo);

    if (aux->escopo == 0)
        fprintf(fp, "store %s %%%d, ptr @%s\n", converteTipo(aux->tipo), exp->id_temporario, aux->nome);
    else
        fprintf(fp, "store %s %%%d, ptr %%%s\n", converteTipo(aux->tipo), exp->id_temporario, aux->nome);

    return eh_retorno;
 }
