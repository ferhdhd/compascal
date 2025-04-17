%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "listaID.h"
    #include "llvm.h"
    void yyerror(const char *s);    
    int yylex();
    extern int yylineno;

    int escopo_atual = 0;
    int id_atual = 0;

    nodoID *ts = NULL;

    FILE *llvm_file;

%}

%define parse.error detailed

%union {
    char *str;
    char *tipo;
    nodoID *lID;
    exp_t *exp;
}

%token <str> NUM <str> ID <str> OPERADOR_MULTIPLICATIVO
%token OR MENOS MAIS OPERADOR_RELACIONAL 
%token OPERADOR_ATRIBUICAO DO WHILE ELSE THEN IF END BEGIN_TOKEN
%token DOIS_PONTOS PONTO_VIRGULA FECHA_PARENTESES ABRE_PARENTESES
%token PROCEDURE FUNCTION REAL INTEIRO VAR PONTO_FINAL PROGRAM VIRGULA

%type <lID> LISTA_DE_IDENTIFICADORES 
%type <lID> FUNCTION PROCEDURE
%type <tipo> TIPO
%type <exp> FATOR
%type <exp> TERMO
// %type <exp> LISTA_DE_EXPRESSOES
%type <exp> EXPRESSAO
%type <exp> EXPRESSAO_SIMPLES
%type <exp> VARIAVEL
%type <str> SINAL

%left '+' '-'
%left '*' '/'

%%

PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES {destroiLista($4);} FECHA_PARENTESES PONTO_VIRGULA
        DECLARACOES {printTs(ts); emiteGlobal(llvm_file, ts);}
        DECLARACOES_DE_SUBPROGRAMAS {printTs(ts);}
        ;

LISTA_DE_IDENTIFICADORES: ID {$$ = concatNodo(NULL, $1, "variavel", escopo_atual);}
                        | LISTA_DE_IDENTIFICADORES VIRGULA ID {$$ = concatNodo($1, $3, "variavel", escopo_atual);}
                        ;

DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA {setTipo($3, $5); ts = attTabelaSimbolos(ts, $3);} 
           | /* empty */ 
           ;

TIPO: INTEIRO {$$ = "INTEIRO";}
    | REAL  {$$ = "REAL";}
    ;

DECLARACOES_DE_SUBPROGRAMAS: DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA {printTs(ts); ts = destroiLocais(ts); printTs(ts);} PONTO_VIRGULA
                           | /* empty */
                           ;

DECLARACAO_DE_SUBPROGRAMA: CABECALHO_DE_SUBPROGRAMA DECLARACOES {emiteFunc(llvm_file, ts);} ENUNCIADO_COMPOSTO {fprintf(llvm_file, "\n}\n"); escopo_atual--; id_atual = 0;}
                         ;

CABECALHO_DE_SUBPROGRAMA: FUNCTION ID {$1 = concatNodo(NULL, $2, "funcao", escopo_atual); ts = attTabelaSimbolos(ts, $1); escopo_atual++;} ARGUMENTOS DOIS_PONTOS TIPO {setTipo($1, $6);} PONTO_VIRGULA 
                        | PROCEDURE ID {$1 = concatNodo(NULL, $2, "funcao", escopo_atual); setTipo($1, "VOID"); ts = attTabelaSimbolos(ts, $1);} ARGUMENTOS PONTO_VIRGULA 
                        ;

ARGUMENTOS: ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES
          | /* empty */
          ;

LISTA_DE_PARAMETROS: LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($1, $3); setTipoSimb($1, "parametro"); ts = attTabelaSimbolos(ts, $1);} 
                   | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($2, $4); setTipoSimb($2, "parametro-ponteiro"); ts = attTabelaSimbolos(ts, $2);} 
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($3, $5); setTipoSimb($3, "parametro"); ts = attTabelaSimbolos(ts, $3);} 
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {setTipo($4, $6); setTipo($4, "parametro-ponteiro"); ts = attTabelaSimbolos(ts, $4);} 
                   ;

ENUNCIADO_COMPOSTO: BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END
                  ;

ENUNCIADOS_OPCIONAIS: LISTA_DE_ENUNCIADOS
                    | /* empty */
                    ;

LISTA_DE_ENUNCIADOS: ENUNCIADO
                   | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO
                   ;

ENUNCIADO: VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO
         | CHAMADA_DE_PROCEDIMENTO
         | ENUNCIADO_COMPOSTO
         | IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO
         | WHILE EXPRESSAO DO ENUNCIADO 
         ;

VARIAVEL: ID 
        ;

CHAMADA_DE_PROCEDIMENTO: ID
                    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES
                    ;

LISTA_DE_EXPRESSOES: EXPRESSAO
                   | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO
                   ;

EXPRESSAO: EXPRESSAO_SIMPLES {$$ = $1;}
         | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES
         ;

EXPRESSAO_SIMPLES: TERMO
                 | SINAL TERMO  
                 | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES 
                 {$$ = cria_exp_de_exp(ts, llvm_file, "exp", $1, $3, id_atual);
                 emiteSoma(llvm_file, $1, $3, id_atual); 
                 id_atual++;}
                 | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES 
                 | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES 
                 ;

TERMO: FATOR
     | TERMO OPERADOR_MULTIPLICATIVO FATOR {emiteOpMult(llvm_file, $1, $3, $2, id_atual);}
     ;

FATOR: ID {$$ = cria_exp(ts, llvm_file, "", $1, id_atual); id_atual++;}
     | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES
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
