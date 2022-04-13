#include <stdio.h>  //Operação 12
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
        if (*lida <= '0' && *lida >= '9') {
            fprintf (stderr, "O ficheiro de entrada não apresenta valores válidos\n");
            exit(0);
        }
    }
    return atoi(lida); //Converte strings em números inteiros
}

int numero (char* str) {
    for (int i=0; *(str+i) != '\0'; i++) {
        if (!(*(str+i) >= '0' && *(str+i) <= '9'))
            return -1;
    }
    return atoi(str);  //Converte strings em números inteiros
}

int main (int argc, char* argv[]){
    FILE* imInput1;
    FILE* imInput2;
    FILE* imOutput;
    int proxX, proxY;

    if (argc > 6) {
        printf("Uso incorreto: %s npixeis <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]", *argv);
        return 0;
    }

    if (argc > 5) {
        imOutput = fopen(*(argv+5), "w");
        if (imOutput == NULL) {
            printf("Ficheiro de escrita impossível de abrir: %s\n", *(argv+5));
            return 0;
        }
    }

    else
        imOutput = stdout;

    if (argc > 4){
        imInput1 = fopen(*(argv+4), "r");
        if (imInput1 == NULL) {
            printf("Ficheiro não encontrado: %s\n", *(argv+4));
            return 0;
        }
    }

    else
        imInput1 = stdin;

    if (argc > 3) {
        imInput2 = fopen(*(argv+3), "r");
        if (imInput2 == NULL) {
            printf("Ficheiro não encontrado: %s\n", *(argv+4));
            return 0;
        }
    }

    else
        imInput2 = stdin;

    if (argc > 2) {
        proxX = numero(*(argv+1));
        proxY = numero(*(argv+2));

        if (proxX == -1 || proxY == -1) {
            printf ("Uso incorreto: %s npixeis <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]", *argv);
            return 0;
        }
    }
    
    
    else{
        fprintf(stderr, "Uso incorreto: %s [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

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
    int altura2 = getInt(imInput2);
    fprintf(imOutput, "%d ", altura1);

    int largura1 = getInt(imInput1);
    int largura2 = getInt(imInput2);
    fprintf(imOutput, "%d\n", largura1);

    int maxRGB = getInt(imInput1);
    getInt(imInput2);
    fprintf(imOutput, "%d\n", maxRGB);

    PIXEL matrix1[largura1][altura1];
    PIXEL matrix2[largura2][altura2];

    for (int i=0; i<largura1; i++) {
        for (int j=0; j<altura1; j++) {
            matrix1[i][j].red = getInt(imInput1);
            matrix1[i][j].green = getInt(imInput1);
            matrix1[i][j].blue = getInt(imInput1);
        }
    }

    for (int i=0; i<largura2; i++) {
        for (int j=0; j<altura2; j++) {
            matrix2[i][j].red = getInt(imInput2);
            matrix2[i][j].green = getInt(imInput2);
            matrix2[i][j].blue = getInt(imInput2);
        }
    }

    for (int i=0; i<largura1; i++) {
        for (int j=0; j<altura1; j++) {
           if (!(i == 0 && j == 0))
                fprintf(imOutput, "\n");
            if (i >= proxY && (i-proxY) < largura2 && j >= proxX && (j-proxX) < altura2)
                fprintf(imOutput, "%d %d %d", matrix2[i-proxY][j-proxX].red, matrix2[i-proxY][j-proxX].green, matrix2[i-proxY][j-proxX].blue);
            else
                fprintf(imOutput, "%d %d %d", matrix1[i][j].red, matrix1[i][j].green, matrix1[i][j].blue);
        }
    }

    fprintf(imOutput, "\n");
    fclose(imInput1);
    fclose(imOutput);

    return 0;
}