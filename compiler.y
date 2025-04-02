%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "listaID.h"
    void yyerror(const char *s);    
    int yylex();
    extern int yylineno;

    int escopo_atual = 0;
    nodoID *ts = NULL;

%}

%define parse.error detailed

%union {
    char *str;
    char *tipo;
    nodoID *lID;
}

%token <str> ID
%token NUM OPERADOR_MULTIPLICATIVO OR MENOS MAIS OPERADOR_RELACIONAL 
%token OPERADOR_ATRIBUICAO DO WHILE ELSE THEN IF END BEGIN_TOKEN
%token DOIS_PONTOS PONTO_VIRGULA FECHA_PARENTESES ABRE_PARENTESES
%token PROCEDURE FUNCTION REAL INTEIRO VAR PONTO_FINAL PROGRAM EOL VIRGULA

%type <lID> LISTA_DE_IDENTIFICADORES 
%type <lID> FUNCTION PROCEDURE
%type <tipo> TIPO

%%

PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES {destroiLista($4);} FECHA_PARENTESES PONTO_VIRGULA
        DECLARACOES {printTs(ts);}
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

DECLARACOES_DE_SUBPROGRAMAS: DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA
                           | /* empty */
                           ;

DECLARACAO_DE_SUBPROGRAMA: CABECALHO_DE_SUBPROGRAMA {escopo_atual++;} DECLARACOES ENUNCIADO_COMPOSTO {escopo_atual--;}
                         ;

CABECALHO_DE_SUBPROGRAMA: FUNCTION ID {$1 = concatNodo(NULL, $2, "funcao", escopo_atual); ts = attTabelaSimbolos(ts, $1);} ARGUMENTOS DOIS_PONTOS TIPO {setTipo($1, $6);} PONTO_VIRGULA 
                        | PROCEDURE ID {$1 = concatNodo(NULL, $2, "funcao", escopo_atual); setTipo($1, "void"); ts = attTabelaSimbolos(ts, $1);} ARGUMENTOS PONTO_VIRGULA 
                        ;

ARGUMENTOS: ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES
          | /* empty */
          ;

LISTA_DE_PARAMETROS: LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO 
                   | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO 
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO 
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO 
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

EXPRESSAO: EXPRESSAO_SIMPLES
         | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES
         ;

EXPRESSAO_SIMPLES: TERMO
                 | SINAL TERMO  
                 | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES 
                 | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES 
                 | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES 
                 ;

TERMO: FATOR
     | TERMO OPERADOR_MULTIPLICATIVO FATOR
     ;

FATOR: ID
     | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES
     | NUM 
     | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES 
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
    printf("Digite uma expressão e pressione Enter:\n");
    yyparse();
    return 0;
}
