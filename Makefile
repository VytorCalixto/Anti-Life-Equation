CFLAGS=-I/home/soft/likwid/include -llikwid -L/usr/lib -L/home/soft/likwid/lib -lm -std=c99 -Wall -DLIKWID_PERFMON -O3 -march=native -mavx -fstrict-aliasing

all: main.c
	gcc -o pdeSolver main.c $(CFLAGS)

clean:
	rm -fr *.o pdeSolver
