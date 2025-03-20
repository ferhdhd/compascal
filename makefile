all:
	bison -d parser.y
	flex compiler.l
	gcc parser.tab.c lex.yy.c -o parser -lfl

clean:
	rm -f parser parser.tab.c parser.tab.h lex.yy.c
