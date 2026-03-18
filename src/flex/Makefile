OBJS = main.o lex.yy.o lf.yy.o label_finder.o grammar.tab.o helper.o

generate_track_list.bin: $(OBJS)
	gcc $(OBJS) -o ./../../generate_track_list.bin -lm

helper.o: helper.c helper.h
	gcc -c helper.c -o helper.o

main.o: main.c
	gcc -c main.c -o main.o

./label_finder/lf.yy.c: ./label_finder/lf.l
	flex -o ./label_finder/lf.yy.c ./label_finder/lf.l

lf.yy.o: ./label_finder/lf.yy.c
	gcc -c ./label_finder/lf.yy.c -o lf.yy.o

label_finder.o: ./label_finder/label_finder.c
	gcc -c ./label_finder/label_finder.c -o label_finder.o

lex.yy.c: lex.l grammar.tab.h
	flex lex.l

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c -o lex.yy.o

grammar.tab.c grammar.tab.h: grammar.y
	bison -dv grammar.y

grammar.tab.o: grammar.tab.c
	gcc -c grammar.tab.c -o grammar.tab.o

clean:
	rm -f *.o *.yy.c *.tab.c *.tab.h *.output ./label_finder/*.yy.c
