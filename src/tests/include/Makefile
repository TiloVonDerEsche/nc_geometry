includer.bin: lex.yy.c includer.c
		gcc lex.yy.c includer.c -o includer.bin
		./includer.bin

lex.yy.c: lex.l
		flex lex.l

clean:
		rm lex.yy.c *.o
