all: parser

parser: parser.y compiler.l listaID.c
	bison -d parser.y
	flex compiler.l
	gcc -o parser parser.tab.c lex.yy.c listaID.c -lfl

clean:
	rm -f parser parser.tab.c parser.tab.h lex.yy.c
