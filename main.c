#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>

void parseArgs(int argc,char** argv, double* hx, double* hy, int* maxIter,
    char** path);
bool isInt(char* str);
double timestamp(void);

int main(int argc, char** argv){
    double hx, hy;
    int maxIter;
    char** path;

    parseArgs(argc,argv,&hx,&hy,&maxIter,&path);
    double sorFactor = 2 - ((hx+hy)/2);

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
