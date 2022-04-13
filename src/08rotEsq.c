#include <stdio.h>  //Operaão 8
#include <stdlib.h>
#include <string.h>
#define MAXSTR 15

typedef struct pixel { //Componentes de cor RGB de um pixel
    int red;
    int green;
    int blue;
} PIXEL;

void readF (FILE* f) { //Leitura dos caracteres
    char lida;
    do {
        fscanf (f, "%c", &lida);
    } while (lida != '\n');
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

int main (int argc, char* argv[]) {
    FILE* imInput;  //Imagem de input
    FILE* imOutput;  //Imagem de output

    if (argc > 3) {
        printf ("Uso incorreto: %s <limite> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    if (argc > 2) {
        imOutput = fopen(*(argv+2), "w");
        if (imOutput == NULL) {
            printf ("Ficheiro de escrita impossível de abrir: %s\n", *(argv+3));
            return 0;
        }
    }

    else 
        imOutput = stdout;

    if (argc > 1) {
        imInput = fopen(*(argv+1), "r");
        if (imInput == NULL) {
            printf ("Ficheiro não encontrado: %s\n", *(argv+2));
            return 0;
        }
    }

    else
        imInput = stdin;

    char* RGB = getString(imInput);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    fprintf (imOutput, "%s\n", RGB);
    free(RGB); //Desaloca uma porção de memória

    int altura = getInt(imInput); //O correspondeste às colunas de uma matriz   
    int largura = getInt(imInput); //O correspondente às linhas de uma matriz
    
    fprintf(imOutput, "%d ", largura);
    fprintf(imOutput, "%d\n", altura);

    int maxRGB = getInt(imInput);
    fprintf(imOutput, "%d\n", maxRGB);

    PIXEL matrix[largura][altura];
    for (int i=0; i<largura; i++) {
        for (int j=0; j<altura; j++) {
            matrix[i][j].red = getInt(imInput);
            matrix[i][j].green = getInt(imInput);
            matrix[i][j].blue = getInt(imInput);
        }
    }

    for (int j=altura-1; j>=0; j--) {
        for (int i=0; i<largura; i++) {
            if (!(i == 0 && j == altura-1))
                fprintf(imOutput, "\n");
            fprintf(imOutput, "%d %d %d", matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue);
        }
    }

    fclose(imInput);
    fclose(imOutput);

    return 0;
}