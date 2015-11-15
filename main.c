#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define PI_SQUARE 9.86960440108935861883

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
    parseArgs(argc,&argv,&hx,&hy,&maxIter,&path);
    printf("hx %f hy %f i %d path %s \n", hx, hy, maxIter, path);
    double sorFactor = 2 - ((hx+hy)/2);
    int nx = round(PI/hx) + 1;
    int ny = round(PI/hy) + 1;
    // int points = (nx+2)*(ny+2);
    int points = nx*ny;
    printf("INFO: Number of points(NX x NY): %d %d\n", nx, ny);
    printf("INFO: Overrelaxation factor: %f\n", sorFactor);
    double *a, *b, *x, *u;
    // allocating (nx+2) * (ny+2) to include the borders
    // access a[i][j] by using a[i+j*(nx+2)]
    u = malloc(points*sizeof(double));
    memset(u, 0, points*sizeof(double));
    // a = malloc(points*points*sizeof(double));
    // memset(a, 0, points*points*sizeof(double));
    // b = malloc(points*sizeof(double));
    // x = malloc(points*sizeof(double));
    // memset(x, 0, points*sizeof(double));

    double hxx = hx*hx;
    double hyy = hy*hy;

    // // Set initial value to B
    // for(int j=0; j < nx+2; ++j) {
    //     for(int k=0; k < ny+2; ++k) {
    //         b[j+k*(nx+2)] = 2*f(j*hx, k*hy)*hxx*hyy;
    //     }
    // }

    // Set top and bottom borders
    // for(int i=0; i <points; ++i) {
    //     a[0 + i*(points)] = bottomFrontier(i*hx);
    //     a[points-1 + i*(points)] = topFrontier(i*hx);
    // }

    /* 
    * i,j = x,y
    * i = x = line
    * j = y = column
    */
    for(int i=0; i <nx; ++i) {
        u[i*ny] = bottomFrontier(i*hx);
        double lol = topFrontier(i*hx);
        printf("lol %f \n", lol);
        u[(i*ny)+(ny-1)] = lol;
    }

    double up, left, right, down, fconst;
    up = (2*hxx)-(hxx*hy);
    down = (hxx*hy)+(2*hxx);
    right = (2*hyy)-(hx*hyy);
    left = (hx*hyy)+(2*hyy);
    fconst = 2*hxx*hyy;
    double denominator = 1/(4*(2*PI_SQUARE*hxx*hyy+hxx+hyy));

    for (int k = 0; k < maxIter; ++k){
        for (int i = 1; i < nx-1; ++i){
            for (int j = 1; j < ny-1; ++j){
                u[(i*ny)+j] = (u[((i+1)*ny)+j]*right + u[((i-1)*ny)+j]*left 
                            + u[(i*ny)+j+1]*up + u[(i*ny)+j-1]*down
                            + f(i*hx,j*hy)*fconst)*denominator;
            }
        }
        //TODO: calculate errors
    }

    for (int j = ny-1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            printf("%f ", u[(i*ny)+j]);
        }
        printf("\n");
    }

    // double denominator = (4*(2*PI_SQUARE*hxx*hyy+hxx+hyy));

    // double up, left, right, down;
    // up = hx*hyy-2*hyy;
    // down = -(hx*hyy+2*hyy);
    // right = hxx*hy-2*hxx;
    // left = -(hxx*hy+2*hxx);

    // for(int j=1; j < points-1; ++j) {
    //     a[j+j*points] = denominator;
    //     for(int k=1; k < points-1; ++k) {
    //         if(j > 1 ) {
    //             a[(j-1) + k*points] = down;
    //         } else {
    //             b[k] -= down*a[(j-1) + k*points];
    //         }

    //         if(j < (points-1)) {
    //             a[(j+1) + k*points] = up;
    //         } else {
    //             b[k] -= up*a[(j+1) + k*points];
    //         }

    //         if(k > 1) {
    //             a[j + (k-1)*points] = left;
    //         } else {
    //             b[k] = left*a[j + (k-1)*points];
    //         }

    //         if(k < (points-1)) {
    //             a[j + (k+1)*points] = right;
    //         } else {
    //             b[k] = right*a[j + (k+1)*points];
    //         }

    //     }
    // }

    free(u);
    // free(a);
    // free(b);
    // free(x);

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
    // f(x, y) = 4pi²[sin(2pix)sinh(piy)+sin(2pi(pi-x))sinh(pi(pi-y))]
    return 4*PI_SQUARE*(sin(2*PI*x)*sinh(PI*y)+sin(2*PI_SQUARE-2*PI*x)*sinh(PI_SQUARE*PI-y));
}

double topFrontier(double x) {
    return sin(2*PI*x)*sinh(PI_SQUARE);
}

double bottomFrontier(double x) {
    return sin(2*PI*PI-2*PI*x)*sinh(PI*PI);
}
