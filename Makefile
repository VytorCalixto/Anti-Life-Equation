CFLAGS = -lm -std=c99 -Wall -O3 -mavx -march=native -llikwid

all: main.c
	gcc -o pdeSolver main.c $(CFLAGS)

clean:
	rm -fr *.o pdeSolver
