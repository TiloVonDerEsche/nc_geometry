SRC = ./main.c
OUT = run.bin
ROOT_DIR = .



CC = gcc
CFLAGS = -I/home/tilo/res/freeglut/include -I/home/tilo/res/glew/include
LIBS = -L/home/tilo/res/freeglut/build/lib -L/home/tilo/res/glew/lib
LDFLAGS = -lglut -lGLEW -lGL -lGLU -lm


$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS) $(LIBS)
