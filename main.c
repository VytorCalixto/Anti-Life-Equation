#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <likwid.h>

#define PI 3.14159265358979323846
#define PI_SQUARE 9.86960440108935861883

void parseArgs(int argc, char*** argv, double* hx, double* hy, int* maxIter,
    char** path);
void writeData(char* path, double sorTime, double resTime, int maxIter,
    double** resNorms, int nx, int ny, double hx, double hy, double** x);
double timestamp(void);
double f(double x, double y);
double topFrontier(double x);
double bottomFrontier(double x);

typedef struct{
    double dg; //diagonal, Ui,j
    double up; //up, Ui+1,j
    double dw; //down, Ui-1,j
    double rt; //right, Ui,j+1
    double lt; //left, Ui,j-1
} Point;

double calculateResidual(Point** a, double** b, double** x, int nx, int ny);

int main(int argc, char** argv){
    double hx, hy;
    int maxIter;
    char* path;
    parseArgs(argc,&argv,&hx,&hy,&maxIter,&path);
    // printf("#hx %f hy %f i %d path %s \n", hx, hy, maxIter, path);
    double omega = 2 - ((hx+hy)/2);
    int nx = round(PI/hx) + 1;
    int ny = round(PI/hy) + 1;
    int points = (nx)*(ny);
    // printf("#INFO: Number of points(NX x NY): %d x %d\n", nx, ny);
    // printf("#INFO: Overrelaxation factor: %f\n", omega);
    Point *a;
    double *b, *x, *resNorms;

    a = malloc(points*sizeof(Point));
    if(a == NULL) {
        fprintf(stderr, "Erro ao alocar vetor A\n");
        exit(1);
    }
    memset(a, 0, points*sizeof(Point));
    b = malloc(points*sizeof(double));
    if(b == NULL) {
        fprintf(stderr, "Erro ao alocar vetor B\n");
        exit(1);
    }
    x = malloc(points*sizeof(double));
    if(x == NULL) {
        fprintf(stderr, "Erro ao alocar vetor X\n");
        exit(1);
    }
    memset(x, 0, points*sizeof(double));
    resNorms = malloc(maxIter*sizeof(double));
    if(resNorms == NULL) {
        fprintf(stderr, "Erro ao alocar vetor da norma dos resíduos\n");
        exit(1);
    }
    memset(resNorms, 0, maxIter*sizeof(double));

    double hxx = hx*hx;
    double hyy = hy*hy;

    // Set initial value to B
    for(int i=0; i < ny; ++i) {
        for(int j=0; j < nx; ++j) {
            b[i*nx+j] = 2*f(j*hx, i*hy)*hxx*hyy;
        }
    }

    double denominator = 4*(2*PI_SQUARE*hxx*hyy+hxx+hyy);
    double up, left, right, down;
    up = hx*hyy-2*hyy;
    down = -(hx*hyy+2*hyy);
    right = hxx*hy-2*hxx;
    left = -(hxx*hy+2*hxx);

    for(int i=0; i < points; ++i) {
        int mod = i % (nx);
        a[i].dg = denominator;
        if(mod > 0) {
            a[i].lt = left;
        }

        if(mod < (nx-1)) {
            a[i].rt = right;
        }

        if(i >= nx) {
            a[i].dw = down;
        } else {
            b[i] -= down*bottomFrontier(i*hx);
        }

        if(i < (points - nx)) {
            a[i].up = up;
        } else {
            b[i] -= up*topFrontier(i*hx);
        }
    }

    likwid_markerInit();
    // SOR
    double tSor = 0.0;
    double tRes = 0.0;
    double t;
    for(int iter=0;iter<maxIter;++iter) {
        t = timestamp();
        likwid_markerStartRegion("SOR");
        for(int i=0; i < points; ++i) {
            double r = 0;
            int mod = i % (nx);
            if(mod > 0) {
                r += a[i].lt*x[i-1];
            }

            if(mod < (nx-1)) {
                r += a[i].rt*x[i+1];
            }

            if(i >= nx) {
                r += a[i].dw*x[i-nx];
            }

            if(i < (points - nx)) {
//comment only to conflict
                r += a[i].up*x[i+nx];
            }
            double residual = ((b[i]-r)/a[i].dg-x[i]);
            x[i] = x[i] + omega*residual;
        }
        likwid_markerStopRegion("SOR");
        tSor += timestamp() - t;
        t = timestamp();
        likwid_markerStartRegion("Residual");
        resNorms[iter] = calculateResidual(&a, &b, &x, nx, ny);
        likwid_markerStopRegion("Residual");
        tRes += timestamp() - t;
    }
    likwid_markerClose();

    // printf("#INFO: tempo = %f\n", t1-t0);

    writeData(path, tSor/maxIter, tRes/maxIter, maxIter, &resNorms, nx, ny, hx, hy, &x);

    free(a);
    free(b);
    free(x);

    return 0;
}

double calculateResidual(Point** a, double** b, double** x, int nx, int ny){
    double residual = 0;
    int points = nx*ny;
    for (int i = 0; i < points; ++i){
        double r = 0;
        int mod = i % (nx);
        if(mod > 0) {
            r += (*a)[i].lt * (*x)[i-1];
        }

        if(mod < (nx-1)) {
            r += (*a)[i].rt * (*x)[i+1];
        }

        if(i >= nx) {
            r += (*a)[i].dw * (*x)[i-nx];
        }

        if(i < (points - nx)) {
            r += (*a)[i].up * (*x)[i+nx];
        }
        double res = ((*b)[i] - r);
        residual += res*res;
    }
    return sqrt(residual);
}

void parseArgs(int argc, char*** argv, double* hx, double* hy, int* maxIter,
    char** path){
    int i;
    char *ptr;
    int argSet[4] = {0,0,0,0};
    for(i=1; i < argc; i+=2){
        if(!strcmp((*argv)[i],"-hx")){
            *hx = strtod((*argv)[i+1], &ptr);
            if(*hx == 0.0){
                fputs("Valor inválido para a opção -hx.\n", stderr);
                exit(-1);
            }
            argSet[0] = 1;
        }else if(!strcmp((*argv)[i],"-hy")){
            *hy = strtod((*argv)[i+1], &ptr);
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

void writeData(char* path, double sorTime, double resTime, int maxIter,
    double** resNorms, int nx, int ny, double hx, double hy, double** x){
    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo %s.\n", path);
        exit(1);
    }
    fprintf(f, "###########\n");
    fprintf(f, "# Tempo Método SOR: %.17g\n", sorTime);
    fprintf(f, "# Tempo Resíduo: %.17g\n", resTime);
    fprintf(f, "#\n");
    fprintf(f, "# Norma do Resíduo\n");
    for(int i=0;i<maxIter;++i){
        fprintf(f, "# i= %d: %f\n", i+1, (*resNorms)[i]);
    }
    fprintf(f, "###########\n");
    fprintf(f, "set terminal wxt persist\n");
    fprintf(f, "set hidden3d\n");
    fprintf(f, "set xlabel 'Y'\n");
    fprintf(f, "set ylabel 'X'\n");
    fprintf(f, "set zlabel 'u(x,y)'\n");
    fprintf(f, "splot '-' u 1:2:3 w l\n");
    fprintf(f, "# X Y Z\n");
    for(int i=0; i < ny; ++i)
        for(int j=0; j < nx; ++j)
            fprintf(f, "%.17g %.17g %.17g\n", i*hx, j*hy, (*x)[(i*nx)+j]);
    fclose(f);
}

double timestamp(void) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec + tp.tv_usec/1000000.0));
}

double f(double x, double y) {
    // f(x, y) = 4pi²[sin(2pix)sinh(piy)+sin(2pi(pi-x))sinh(pi(pi-y))]
    double r = 4*PI_SQUARE*(sin(2*PI*x)*sinh(PI*y)+sin(2*PI*(PI-x))*sinh(PI*(PI-y)));
    return r;
}

double topFrontier(double x) {
    return sin(2*PI*x)*sinh(PI_SQUARE);
}

double bottomFrontier(double x) {
    return sin(2*PI*PI-2*PI*x)*sinh(PI*PI);
}
