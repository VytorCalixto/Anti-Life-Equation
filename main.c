#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

void parseArgs(int argc,char** argv, int* hx, int* hy, int* maxIter, 
    char** path);
bool isInt(char* str);

int int main(int argc, char** argv){
    int hx, hy, maxIter;
    char** path;
    
    parseArgs(argc,argv,&hx,&hy,&maxIter,&path);

    return 0;
}

void parseArgs(int argc,char** argv, int* hx, int* hy, int* maxIter, 
    char** path){
    
    int c;
    struct option longopts[] = {
       { "hx", required_argument, NULL, 'x' },
       { "hy", required_argument, NULL, 'y' }
       { 0, 0, 0, 0 }
    };
    while ((c = getopt_long(argc, argv, "x:y:i:o:", longopts, NULL)) != -1) {
        switch (c) {
            case 'x':
                if(isInt(optarg))
                    *hx = atoi(optarg);
                else{
                    printf("Inserir apenas inteiros para a opção -hx.\n");
                    abort();
                }
                break;
            case 'y':
                if(isInt(optarg))
                    *hy = atoi(optarg);
                else{
                    printf("Inserir apenas inteiros para a opção -hy.\n");
                    abort();
                }
                break;
            case 'i':
                if(isInt(optarg))
                    *maxIter = atoi(optarg);
                else{
                    printf("Inserir apenas inteiros para a opção -i.\n");
                    abort();
                }
                break;
            case 'o':
                *path = optarg
                break;
            default:
                printf("Opção inexistente.\n");
                abort();
        }
   }
}

bool isInt(char* str){
    if (!*str)
        return false;

    while (*str){
        if (!isdigit(*str))
            return false;
        else
            ++str;
    }
    return true;
}