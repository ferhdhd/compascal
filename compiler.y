%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "listaID.h"
    #include "llvm.h"
    void yyerror(const char *s);    
    int yylex();
    extern int yylineno;

    int escopo_atual = 0;
    int id_atual = 0;
    int func_tem_retorno = 0;
    int cont_if = 0;
    int cont_while = 0;

    nodoID *retorno = NULL;
    nodoID *ts = NULL;
    nodoID *lista_exp = NULL;

    FILE *llvm_file;

%}

%define parse.error detailed

%union {
    char *str;
    char *tipo;
    nodoID *lID;
    exp_t *exp;
    int numero;
}

%token <str> NUM <str> ID <str> OPERADOR_MULTIPLICATIVO <str> MAIS <str> MENOS <str> OR
%token <lID> FUNCTION PROCEDURE
%token <str> OPERADOR_RELACIONAL  
%token <numero> IF <numero> WHILE
%token OPERADOR_ATRIBUICAO DO ELSE THEN END BEGIN_TOKEN
%token DOIS_PONTOS PONTO_VIRGULA FECHA_PARENTESES ABRE_PARENTESES
%token REAL INTEIRO VAR PONTO_FINAL PROGRAM VIRGULA

%type <lID> LISTA_DE_IDENTIFICADORES 
%type <tipo> TIPO
%type <exp> FATOR
%type <exp> TERMO
%type <exp> LISTA_DE_EXPRESSOES
%type <exp> EXPRESSAO
%type <exp> EXPRESSAO_SIMPLES
%type <str> VARIAVEL
%type <str> SINAL

%left '+' '-'
%left '*' '/'

%%

PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES {destroiLista($4);} FECHA_PARENTESES PONTO_VIRGULA
        DECLARACOES {printTs(ts); emiteGlobal(llvm_file, ts);}
        DECLARACOES_DE_SUBPROGRAMAS {printTs(ts);}
        ENUNCIADO_COMPOSTO 
        PONTO_FINAL {fprintf(llvm_file, "ret i32 0\n}");}
        ;

LISTA_DE_IDENTIFICADORES: ID {$$ = concatNodo(NULL, $1, "variavel", escopo_atual);}
                        | LISTA_DE_IDENTIFICADORES VIRGULA ID {$$ = concatNodo($1, $3, "variavel", escopo_atual);}
                        ;

DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA {setTipo($3, $5); ts = attTabelaSimbolos(ts, $3); printTs(ts);} 
           | /* empty */ 
           ;

TIPO: INTEIRO {$$ = "INTEIRO";}
    | REAL  {$$ = "REAL";}
    ;

DECLARACOES_DE_SUBPROGRAMAS: DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA {printTs(ts); ts = destroiLocais(ts); printTs(ts);} PONTO_VIRGULA
                           | /* empty */
                           ;

DECLARACAO_DE_SUBPROGRAMA: CABECALHO_DE_SUBPROGRAMA DECLARACOES {emiteFunc(llvm_file, ts);} ENUNCIADO_COMPOSTO {if (func_tem_retorno <= 0) emiteErroRetorno(ts); fprintf(llvm_file, "\n}\n"); escopo_atual--; func_tem_retorno = 0; id_atual = 0; cont_if = 0;}
                         ;

CABECALHO_DE_SUBPROGRAMA: FUNCTION ID {$1 = concatNodo(NULL, $2, "funcao", escopo_atual); ts = attTabelaSimbolos(ts, $1); escopo_atual++; retorno = concatNodo(NULL, $2, "retorno", escopo_atual); ts = attTabelaSimbolos(ts, retorno);} ARGUMENTOS DOIS_PONTOS TIPO {setTipoUm($1, $6); setTipoUm(retorno, $6);} PONTO_VIRGULA 
                        | PROCEDURE ID {$1 = concatNodo(NULL, $2, "procedure", escopo_atual); setTipo($1, "VOID"); ts = attTabelaSimbolos(ts, $1); escopo_atual++; func_tem_retorno++;} ARGUMENTOS PONTO_VIRGULA 
                        ;

ARGUMENTOS: ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES
          | /* empty */
          ;

LISTA_DE_PARAMETROS: LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($1, $3); setTipoSimb($1, "parametro"); ts = attTabelaSimbolos(ts, $1);} 
                   | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($2, $4); setTipoSimb($2, "parametro-ponteiro"); ts = attTabelaSimbolos(ts, $2);} 
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($3, $5); setTipoSimb($3, "parametro"); ts = attTabelaSimbolos(ts, $3);} 
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($4, $6); setTipo($4, "parametro-ponteiro"); ts = attTabelaSimbolos(ts, $4);} 
                   ;

ENUNCIADO_COMPOSTO: BEGIN_TOKEN {emiteMain(llvm_file);} ENUNCIADOS_OPCIONAIS END
                  ;

ENUNCIADOS_OPCIONAIS: LISTA_DE_ENUNCIADOS
                    | /* empty */
                    ;

LISTA_DE_ENUNCIADOS: ENUNCIADO
                   | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO
                   ;

ENUNCIADO: VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO {func_tem_retorno += armazenaVar(llvm_file, $1, $3, ts);}
         | CHAMADA_DE_PROCEDIMENTO
         | ENUNCIADO_COMPOSTO
         | IF EXPRESSAO {emiteComecoIf(llvm_file, $2, ++cont_if); $1 = cont_if;} THEN ENUNCIADO {emiteFimThen(llvm_file, $1);} ELSE ENUNCIADO {emiteFimElse(llvm_file, $1);}
         | WHILE EXPRESSAO DO {emiteComecoWhile(llvm_file, $2, ++cont_while; $1 = cont_while);} ENUNCIADO 
         |
         ;

VARIAVEL: ID 
        ;

CHAMADA_DE_PROCEDIMENTO: ID {emiteProcSemPar(llvm_file, $1, ts);}
                    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES {emiteProcComPar(llvm_file, $1, $3, ts);}
                    ;

LISTA_DE_EXPRESSOES: EXPRESSAO {$$ = cria_exp_lista_parametros(NULL, $1);}
                   | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO {$$ = cria_exp_lista_parametros($1, $3);}
                   ;

EXPRESSAO: EXPRESSAO_SIMPLES {$$ = $1;}
         | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES {$$ = cria_exp_comparacao($1, $2, $3, id_atual); emiteComparacao(llvm_file, $1, $2, $3, id_atual); id_atual++;}
         ;

EXPRESSAO_SIMPLES: TERMO {$$ = $1;}
                 | SINAL TERMO {
                    if (!strcmp("-", $1)) {
                        emiteMudancaSinal(llvm_file, $2, id_atual++);
                        $2->id_temporario = id_atual-1;
                    }
                    $$ = $2;
                 }
                 | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES 
                 {$$ = cria_exp_de_exp(ts, llvm_file, "exp", $1, $2, $3, id_atual);
                 emiteSoma(llvm_file, $1, $3, id_atual); 
                 id_atual++;}
                 | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES 
                 {$$ = cria_exp_de_exp(ts, llvm_file, "exp", $1, $2, $3, id_atual);
                 emiteSubtracao(llvm_file, $1, $3, id_atual); 
                 id_atual++;}
                 | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES 
                 {$$ = cria_exp_de_exp(ts, llvm_file, "exp", $1, $2, $3, id_atual);
                 emiteOr(llvm_file, $1, $3, id_atual); 
                 id_atual++;}
                 ;

TERMO: FATOR {$$ = $1;}
     | TERMO OPERADOR_MULTIPLICATIVO FATOR {$$ = cria_exp_de_exp(ts, llvm_file, "exp", $1, $2, $3, id_atual); emiteOpMult(llvm_file, $1, $3, $2, id_atual); id_atual++;}
     ;

FATOR: ID 
    {
        if (var_func_proc(ts, $1) == 0) {
            printf("%s: eh var\n" , $1);
            $$ = cria_exp(ts, llvm_file, "", $1, id_atual); 
            id_atual++;
        } else if (var_func_proc(ts, $1) == 1) {
            printf("eh proc\n");
            emiteProcSemPar(llvm_file, $1, ts);
        }
    }
     | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES {
        exp_t* aux = cria_exp(ts, llvm_file, "funcao", $1, id_atual); 
        emiteRetornoFuncao(llvm_file, $3, aux, ts, id_atual); 
        id_atual++; 
        $$ = aux;}
     | NUM {$$ = cria_exp(ts, llvm_file, "numero", $1, id_atual); id_atual++;}
     | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES {$$ = $2;}
     ;

SINAL: MAIS
     | MENOS 
     ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s na linha: %d\n", s, yylineno);
    exit(1);
}

int main() {
    
    llvm_file = fopen("arquivo.llvm", "w");

    yyparse();

    fclose(llvm_file);
    return 0;
}
