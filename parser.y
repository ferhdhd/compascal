%{
    #include <stdio.h>
    #include "listaID.h"
    void yyerror(const char *s);    
    int yylex();
    extern int yylineno;

    int escopoAtual = 0;
    nodoID *ts = NULL;

%}

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
%type <tipo> TIPO

%%

PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES {destroiLista($4);} FECHA_PARENTESES PONTO_VIRGULA
        DECLARACOES {printTs(ts);}
        ;

LISTA_DE_IDENTIFICADORES: ID {$$ = concatNodo(NULL, $1);}
                        | LISTA_DE_IDENTIFICADORES VIRGULA ID {$$ = concatNodo($1, $3);}
                        ;

DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA {setTipo($3, $5); attTabelaSimbolos(ts, $3);} 
           | /* empty */ 
           ;

TIPO: INTEIRO {$$ = "INTEIRO";}
    | REAL  {$$ = "REAL";}
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s na linha: %d\n", s, yylineno);
}

int main() {
    printf("Digite uma expressão e pressione Enter:\n");
    yyparse();
    return 0;
}
