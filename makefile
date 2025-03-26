all:
	bison -d parser.y
	flex compiler.l
	gcc -c parser.tab.c
	gcc -c lex.yy.c
	gcc -c listaID.c
	gcc parser.tab.o lex.yy.o listaID.o -o parser -lfl

clean:
	rm -f parser parser.tab.c parser.tab.h lex.yy.c *.o