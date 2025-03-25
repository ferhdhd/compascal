%{
    #include <stdio.h>
    void yyerror(const char *s);
    int yylex();
%}

%token NUM ID OPERADOR_MULTIPLICATIVO OR MENOS MAIS OPERADOR_RELACIONAL 
%token OPERADOR_ATRIBUICAO DO WHILE ELSE THEN IF END BEGIN_TOKEN
%token DOIS_PONTOS PONTO_VIRGULA FECHA_PARENTESES ABRE_PARENTESES
%token PROCEDURE FUNCTION REAL INTEIRO VAR PONTO_FINAL PROGRAM EOL

%%

input: 
    /* Regra vazia para permitir múltiplas entradas */
    | input line
    ;

line:
    token_list EOL { printf("Linha processada!\n"); }
    ;

token_list:
    token
    | token_list token
    ;

token:
    NUM { printf("Token: NUM\n"); }
    | ID { printf("Token: ID\n"); }
    | OPERADOR_MULTIPLICATIVO { printf("Token: OPERADOR_MULTIPLICATIVO\n"); }
    | OR { printf("Token: OR\n"); }
    | MENOS { printf("Token: MENOS\n"); }
    | MAIS { printf("Token: MAIS\n"); }
    | OPERADOR_RELACIONAL { printf("Token: OPERADOR_RELACIONAL\n"); }
    | OPERADOR_ATRIBUICAO { printf("Token: OPERADOR_ATRIBUICAO\n"); }
    | DO { printf("Token: DO\n"); }
    | WHILE { printf("Token: WHILE\n"); }
    | ELSE { printf("Token: ELSE\n"); }
    | THEN { printf("Token: THEN\n"); }
    | IF { printf("Token: IF\n"); }
    | END { printf("Token: END\n"); }
    | BEGIN_TOKEN { printf("Token: BEGIN\n"); }
    | DOIS_PONTOS { printf("Token: DOIS_PONTOS\n"); }
    | PONTO_VIRGULA { printf("Token: PONTO_VIRGULA\n"); }
    | FECHA_PARENTESES { printf("Token: FECHA_PARENTESES\n"); }
    | ABRE_PARENTESES { printf("Token: ABRE_PARENTESES\n"); }
    | PROCEDURE { printf("Token: PROCEDURE\n"); }
    | FUNCTION { printf("Token: FUNCTION\n"); }
    | REAL { printf("Token: REAL\n"); }
    | INTEIRO { printf("Token: INTEIRO\n"); }
    | VAR { printf("Token: VAR\n"); }
    | PONTO_FINAL { printf("Token: PONTO_FINAL\n"); }
    | PROGRAM { printf("Token: PROGRAM\n"); }
    | error { printf("Erro léxico!\n"); yyerrok; }
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
