color_mapper.bin: lex.yy.c main.c parser.tab.c parser.tab.h
		gcc lex.yy.c main.c parser.tab.c -o color_mapper.bin
		./color_mapper.bin


lex.yy.c: lexer.l
		flex lexer.l

parser.tab.c parser.tab.h: parser.y
		bison -dv parser.y

clean:
		rm lex.yy.c *.o
