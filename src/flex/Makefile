default:
	clear
	flex -l lex.l
	bison -dv grammar.y
	gcc -o ./../../generate_track_list.bin main.c -lm
