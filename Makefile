CFLAGS = -lm -std=c99 -Wall -O3

all: main.c
	gcc -o pdeSolver main.c $(CFLAGS)

clean:
	rm -fr *.o pdeSolver
