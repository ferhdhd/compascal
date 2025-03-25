%option yylineno

%{
#include "parser.tab.h"
char err_buffer[500];
int yyerror(char *s);
%}

DIGITOS [0-9]+
NUM {DIGITOS}(\.{DIGITOS})?([Ee][+-]?{DIGITOS})?
ID [a-zA-Z][a-zA-Z1-9]*

%%

{NUM} { return NUM; }
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
{ID} { return ID; }
"program" { return PROGRAM; }

\n { return EOL; }
[ \t] {/*ignorar espacos em branco, incluindo tabs*/}
. { sprintf(err_buffer, "caracter estranho '%c'", *yytext); yyerror(err_buffer); }

%%