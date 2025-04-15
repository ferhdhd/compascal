%option yylineno

%{
#include "listaID.h"
#include "compiler.tab.h"
#include "compiler.h"
char err_buffer[500];
%}

DIGITOS [0-9]+
NUM {DIGITOS}(\.{DIGITOS})?([Ee][+-]?{DIGITOS})?
ID [a-zA-Z][a-zA-Z1-9]*

%%

{NUM} { yylval.str = strdup(yytext);
        return NUM; }
"*"|"/"|"div"|"mod"|"and" { return OPERADOR_MULTIPLICATIVO; }
"or" { return OR; }
"-" { return MENOS; }
"+" { return MAIS; }
"="|"<>"|">"|">="|"<"|"<=" { return OPERADOR_RELACIONAL; }
":=" { return OPERADOR_ATRIBUICAO; }
"do" { return DO; }
"while" { return WHILE; }
"else" { return ELSE; }
"then" { return THEN; }
"if" { return IF; }
"end" { return END; }
"begin" { return BEGIN_TOKEN; }
":" { return DOIS_PONTOS; }
";" { return PONTO_VIRGULA; }
")" { return FECHA_PARENTESES; }
"(" { return ABRE_PARENTESES; }
"procedure" { return PROCEDURE; }
"function" { return FUNCTION; }
"real" { return REAL; }
"integer" { return INTEIRO; }
"var" { return VAR; }
"." { return PONTO_FINAL; }
"program" { return PROGRAM; }
"," { return VIRGULA; }
{ID} { 
    yylval.str = strdup(yytext);
    return ID; }

\n {  }
[ \t] {/*ignorar espacos em branco, incluindo tabs*/}

%%