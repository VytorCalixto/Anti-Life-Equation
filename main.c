#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

#define PI 3.14159265358979323846

void parseArgs(int argc,char** argv, double* hx, double* hy, int* maxIter,
    char** path);
bool isInt(char* str);
double timestamp(void);
double f(double x, double y);
double topFrontier(double x);
double bottomFrontier(double x);

int main(int argc, char** argv){
    double hx, hy;
    int maxIter;
    char** path;

    parseArgs(argc,argv,&hx,&hy,&maxIter,&path);
    double sorFactor = 2 - ((hx+hy)/2);
    double nx = round(PI/hx) + 1;
    double ny = round(PI/hy) + 1;

    return 0;
}

void parseArgs(int argc,char** argv, double* hx, double* hy, int* maxIter,
    char** path){

    int c;
    struct option longopts[] = {
       { "hx", required_argument, NULL, 'x' },
       { "hy", required_argument, NULL, 'y' },
       { 0, 0, 0, 0 }
    };
    while ((c = getopt_long(argc, argv, "x:y:i:o:", longopts, NULL)) != -1) {
        switch (c) {
            case 'x':
                *hx = atof(optarg);
                if(*hx == 0.0){
                    printf("Valor inválido para a opção -hx.\n");
                    abort();
                }
                break;
            case 'y':
                *hy = atof(optarg);
                if(*hy == 0.0){
                    printf("Valor inválido para a opção -hy.\n");
                    abort();
                }
                break;
            case 'i':
                *maxIter = atoi(optarg);
                if(!*maxIter){
                    printf("Valor inválido para a opção -i.\n");
                    abort();
                }
                break;
            case 'o':
                *path = optarg;
                break;
            default:
                printf("Opção inexistente.\n");
                abort();
        }
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
