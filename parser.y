%{
    #include <stdio.h>
    #include "listaID.h"
    void yyerror(const char *s);
    int yylex();

    int escopoAtual = 0;
    listaID *head = NULL;

%}

%union {
    char *str;
    listaID *lID;
}

%token <str> ID
%token NUM OPERADOR_MULTIPLICATIVO OR MENOS MAIS OPERADOR_RELACIONAL 
%token OPERADOR_ATRIBUICAO DO WHILE ELSE THEN IF END BEGIN_TOKEN
%token DOIS_PONTOS PONTO_VIRGULA FECHA_PARENTESES ABRE_PARENTESES
%token PROCEDURE FUNCTION REAL INTEIRO VAR PONTO_FINAL PROGRAM EOL VIRGULA

%type <lID> LISTA_DE_IDENTIFICADORES

%%

PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA
        DECLARACOES 
        ;

LISTA_DE_IDENTIFICADORES: ID {$$ = concatNodo(head, $1)}
                        | LISTA_DE_IDENTIFICADORES VIRGULA ID {$$ = concatNodo(head, $3)}
                        ;

DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA 
           | /* empty */ 
           ;

TIPO: INTEIRO
    | REAL 
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main() {
    printf("Digite uma expressão e pressione Enter:\n");
    yyparse();
    return 0;
}
