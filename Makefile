CFLAGS = -lm -std=c99

all: main.c
	gcc -o pdeSolver main.c $(CFLAGS)

clean:
	rm -fr *.o pdeSolver
