#include <stdio.h>  //Operação 6
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

int numero (char* str) {
    for (int i=0; *(str+i) != '\0'; i++) {
        if (!(*(str+i) >= '0' && *(str+i) <= '9'))
            return -1;
    }
    return atoi (str);
}

int converter (int red, int green, int blue, int x, int maxRGB) {
    int auxiliar = (float) (red*(0.2126) + green*(0.7152) + blue*(0.0722) + 0.5);
    if (auxiliar > x)
        return maxRGB;
    return 0;
}

int main (int argc, char* argv[]) {
    int x;
    FILE* imInput; //Imagem de input
    FILE* imOutput; //Imagem de output

    if (argc > 4) {
        printf ("Uso incorreto: %s <limite> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    if (argc > 3) {
        imOutput = fopen(*(argv+3), "w");
        if (imOutput == NULL) {
            printf ("Ficheiro de escrita impossível de abrir: %s\n", *(argv+3));
            return 0;
        }
    }

    else
        imOutput = stdout;

    if (argc > 2) {
        imInput = fopen(*(argv+2), "r");
        if (imInput == NULL) {
            printf ("Ficheiro não encontrado: %s\n", *(argv+2));
            return 0;
        }
    }

    else
        imInput = stdin;

    if (argc > 1) {
        x = numero(*(argv+1));
        if (x == -1) {
            fprintf (stderr, "O ficheiro de entrada não apresenta valores válidos\n");
            return 0;
        }
    }

    else {
        printf ("Uso incorreto: %s <limite> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    char* RGB = getString(imInput);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    fprintf (imOutput, "%s\n", RGB);
    free(RGB); //Desaloca uma porção de memória

    int altura = getInt(imInput); //O correspondeste às colunas de uma matriz
    fprintf(imOutput, "%d ", altura);

    int largura = getInt(imInput); //O correspondente às linhas de uma matriz
    fprintf(imOutput, "%d\n", largura);

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

    for (int i=0; i<largura; i++) {
        for (int j=0; j<altura; j++) {
            if (!(i == 0 && j == 0))
                fprintf (imOutput, "\n");
            int auxiliar = converter(matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue, x,maxRGB);
            fprintf (imOutput, "%d %d %d", auxiliar, auxiliar, auxiliar);
        }
    }

    fclose(imInput);
    fclose(imOutput);

    return 0;
}