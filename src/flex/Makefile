default:
	clear
	flex -l lex.l
	bison -dv grammar.y
	gcc -o ./../../generate_track_list main.c
