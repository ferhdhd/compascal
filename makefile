all: compiler

compiler: compiler.y compiler.l listaID.c
	bison -d compiler.y
	flex compiler.l
	gcc -o compiler compiler.tab.c lex.yy.c listaID.c -lfl

clean:
	rm -f compiler compiler.tab.c compiler.tab.h lex.yy.c
