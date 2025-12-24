default:
	clear
	flex -l lex.l
	bison -dv grammar.y
	gcc -o nc_to_json main.c
