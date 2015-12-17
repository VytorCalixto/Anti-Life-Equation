CFLAGS=-I/home/soft/likwid/include -llikwid -L/usr/lib -L/home/soft/likwid/lib -lm -std=c99 -Wall -DLIKWID_PERFMON -O3 -march=native -mavx -fstrict-aliasing

all: main-new.c main-old.c
	gcc -o pdeSolver main-new.c $(CFLAGS)
	gcc -o oldSolver main-old.c $(CFLAGS)

clean:
	rm -fr *.o *Solver
