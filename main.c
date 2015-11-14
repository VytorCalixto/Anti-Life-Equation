#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

void parseArgs(int argc, char*** argv, double* hx, double* hy, int* maxIter,
    char** path);
double timestamp(void);
double f(double x, double y);
double topFrontier(double x);
double bottomFrontier(double x);

int main(int argc, char** argv){
    double hx, hy;
    int maxIter;
    char* path;
    int i=0;
    parseArgs(argc,&argv,&hx,&hy,&maxIter,&path);
    printf("hx %f hy %f i %d path %s \n", hx, hy, maxIter, path);
    double sorFactor = 2 - ((hx+hy)/2);
    int nx = round(PI/hx) + 1;
    int ny = round(PI/hy) + 1;
    printf("INFO: Number of points(NX x NY): %d %d\n", nx, ny);
    printf("INFO: Overrelaxation factor: %f\n", sorFactor);
    double *a, *b, *x;
    // allocating (nx+2) * (ny+2) to include the borders
    a = malloc((nx+2)*(ny+2)*sizeof(double));
    memset(a, 0, (nx+2)*(ny+2)*sizeof(double));
    b = malloc((nx+2)*sizeof(double));
    x = malloc((nx+2)*sizeof(double));
    memset(x, 0, (nx+2)*sizeof(double));
    // Set top and bottom borders
    for(int j=0; j <ny+2; ++j) {
        a[0 + j*(nx+2)] = topFrontier(j*hx);
        a[nx+1 + j*(nx+2)] = bottomFrontier(j*hx);
    }

    return 0;
}

void parseArgs(int argc, char*** argv, double* hx, double* hy, int* maxIter,
    char** path){
    int i;
    int argSet[4] = {0,0,0,0};
    for(i=1; i < argc; i+=2){
        if(!strcmp((*argv)[i],"-hx")){
            *hx = atof((*argv)[i+1]);
            if(*hx == 0.0){
                fputs("Valor inválido para a opção -hx.\n", stderr);
                exit(-1);
            }
            argSet[0] = 1;
        }else if(!strcmp((*argv)[i],"-hy")){
            *hy = atof((*argv)[i+1]);
            if(*hy == 0.0){
                fputs("Valor inválido para a opção -hy.\n", stderr);
                exit(-1);
            }
            argSet[1] = 1;
        }else if(!strcmp((*argv)[i],"-i")){
            *maxIter = atoi((*argv)[i+1]);
            if(*maxIter == 0){
                fputs("Valor inválido para a opção -i.\n", stderr);
                exit(-1);
            }
            argSet[2] = 1;
        }else if(!strcmp((*argv)[i],"-o")){
            *path = (*argv)[i+1];
            argSet[3] = 1;
        }else{
            fprintf(stderr, "Opção %s inexistente.\n", (*argv)[i]);
            exit(-1);
        }
    }
    if(!(argSet[0] && argSet[1] && argSet[2] && argSet[3])){
        fputs("Argumento faltando.\n", stderr);
        exit(-1);
    }
}

double timestamp(void) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec + tp.tv_usec/1000000.0));
}

double f(double x, double y) {
    double squared_pi = PI*PI;
    // f(x, y) = 4pi²[sin(2pix)sinh(piy)+sin(2pi(pi-x))sinh(pi(pi-y))]
    return 4*squared_pi*(sin(2*PI*x)*sinh(PI*y)+sin(2*squared_pi-2*x)*sinh(squared_pi*PI-y));
}

double topFrontier(double x) {
    return sin(2*PI*x)*sinh(PI*PI);
}

double bottomFrontier(double x) {
    return sin(2*PI*PI-2*PI*x)*sinh(PI*PI);
}
