#include <stdio.h>  //Operação 10
#include <stdlib.h>
#include <string.h>
#define MAXSTR 15

typedef struct pixel { //Componentes de cor RGB de um pixel
    int red;
    int green;
    int blue;
} PIXEL;

void readF (FILE* f) {  //Leitura dos caracteres
    char lida;
    do {
        fscanf(f, "%c", &lida);
    } while(lida != '\n');
}

int getMinimo (int a, int b) {
    if (a > b)
        return b;

    return a;
}

char* getString (FILE* f) {
    char* lida = calloc(300, sizeof(char)); //Indica o tamanho limite que pode atingir
    fscanf (f, "%s", lida); //Leitura da string

    while (*lida == '#') {
        readF(f);
        fscanf (f, "%s", lida);
    }
    return lida;
}

int getInt (FILE* f) {
    char* lida = getString(f);

    for (int i=0; *(lida+i) != '\0'; i++) {
        if (!(*lida >= '0' && *lida <= '9')) {
            fprintf (stderr, "O ficheiro de entrada não apresenta valores válidos\n");
            exit(0);
        }
    }
    return atoi(lida); //Converte strings em números inteiros
}

int main (int argc, char* argv[]){
    FILE* imInput1;
    FILE* imInput2;
    FILE* imOutput;

    if (argc > 4) {
        fprintf(stderr, "Uso incorreto: %s npixeis <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    if (argc > 3) {
        imOutput = fopen(*(argv+3), "w");
        if (imOutput == NULL) {
            printf("Ficheiro de escrita impossível de abrir: %s\n", *(argv+3));
            return 0;
        }
    }

    else
        imOutput = stdout;

    if (argc > 2) {
        imInput2 = fopen(*(argv+2), "r");
        if (imInput2 == NULL) {
            printf("Ficheiro não encontrado: %s\n", *(argv+2));
            return 0;
        }
    }

    else
        imInput2 = stdin;

    if (argc > 1) {
        imInput1 = fopen(*(argv+1), "r");
        if (imInput1 == NULL) {
            printf("Ficheiro não encontrado: %s\n", *(argv+1));
            return 0;
        }
    }

    else
        imInput1 = stdin;

    char* RGB = getString(imInput1);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    RGB = getString(imInput2);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    fprintf(imOutput, "%s\n", RGB);
    free(RGB);

    int altura1 = getInt(imInput1);
    int altura2 = altura1 + getInt(imInput2);
    fprintf(imOutput, "%d ", altura2);

    int largura = getMinimo(getInt(imInput1), getInt(imInput2));
    fprintf(imOutput, "%d\n", largura);

    int maxRGB = getInt(imInput1);
    getInt(imInput2);
    fprintf(imOutput, "%d\n", maxRGB);

    PIXEL matrix[largura][altura2];
    for (int i=0; i<largura; i++) {
        int j=0;
        for (; j<altura1; j++) {
            matrix[i][j].red = getInt(imInput1);
            matrix[i][j].green = getInt(imInput1);
            matrix[i][j].blue = getInt(imInput1);
        }
        for(; j<altura2; j++){
            matrix[i][j].red = getInt(imInput2);
            matrix[i][j].green = getInt(imInput2);
            matrix[i][j].blue = getInt(imInput2);
        }
    }

    for(int i=0; i<largura; i++){
        for(int j=0; j<altura2; j++){
            if(!(i == 0 && j == 0))
                fprintf(imOutput, "\n");
        fprintf(imOutput, "%d %d %d", matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue);
        }
    }

    fclose(imInput1);
    fclose(imOutput);

    return 0;
}