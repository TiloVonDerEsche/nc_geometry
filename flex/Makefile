default:
	clear
	flex -l lex.l
	bison -dv grammar.y
	gcc -o ./../mpf_to_track_list main.c
