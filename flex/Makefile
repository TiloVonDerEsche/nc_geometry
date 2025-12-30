default:
	clear
	flex -l lex.l
	bison -dv grammar.y
	gcc -o ./../mpf_to_hmhis_json main.c
