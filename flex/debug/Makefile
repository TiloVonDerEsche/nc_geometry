default:
	clear
	flex -l test.l
	bison -dv grammar.y
	gcc -o test grammar.tab.c lex.yy.c
