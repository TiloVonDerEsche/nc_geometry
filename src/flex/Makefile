# Default to Linux GCC
CC = gcc
EXE = .bin
# Change this for different targets (e.g., make TARGET=XP)
ifeq ($(TARGET), XP)
    CC = i686-w64-mingw32-gcc
    EXE = .exe
    LDFLAGS = -static
else ifeq ($(TARGET), WIN64)
    CC = x86_64-w64-mingw32-gcc
    EXE = .exe
    LDFLAGS = -static
endif

OBJS = main.o lex.yy.o lf.yy.o label_finder.o grammar.tab.o helper.o
OUT = ./../../generate_track_list$(EXE)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUT) -lm $(LDFLAGS)

# Generic rule for all .o files to save space
%.o: %.c
	$(CC) -c $< -o $@

# Keep your Flex/Bison rules as they are (using Linux natives)
lex.yy.c: lex.l grammar.tab.h
	flex lex.l

grammar.tab.c grammar.tab.h: grammar.y
	bison -dv grammar.y

# Special path for label_finder objects
lf.yy.o: ./label_finder/lf.yy.c
	$(CC) -c $< -o $@

./label_finder/lf.yy.c: ./label_finder/lf.l
	flex -o $@ $<

label_finder.o: ./label_finder/label_finder.c
	$(CC) -c ./label_finder/label_finder.c -o label_finder.o

clean:
	rm -f *.o *.yy.c *.tab.c *.tab.h *.output ./label_finder/*.yy.c $(OUT)
