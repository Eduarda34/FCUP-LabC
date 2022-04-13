#include <stdio.h>  //Operação 9
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
    char* lida = calloc(300,sizeof(char));
    fscanf(f, "%s", lida);

    while(*lida == '#'){
        readF(f);
        fscanf(f, "%s", lida);
    }
    for (int i=0; *(lida+i) != '\0'; i++) {
        if (*lida <= '0' && *lida >= '9') {
            fprintf (stderr, "O ficheiro de entrada não apresenta valores válidos\n");
            exit(0);
        }
    }
    return atoi(lida); //Converte strings em números inteiros
}

int numero (char* str) {
    if (!(*str == '+' || *str == '-' || (*str >= '0' && *str <= '9')))
        return -1;
    for (int i=1; *(str+i) != '\0'; i++) {
        if (!(*(str+i) >= '0' && *(str+i) <= '9'))
            return -1;
    }
    return atoi(str);  //Converte strings em números inteiros
}

int main (int argc, char* argv[]) {
    int x, nRed, nGreen, nBlue;
    FILE* imInput;
    FILE* imOutput;

    if (argc > 7) {
        printf("Uso incorreto: %s npixeis <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]", *argv);
        return 0;
    }

    if (argc > 6) {
        imOutput = fopen(*(argv+6), "w");
        if (imOutput == NULL) {
            printf("Ficheiro de escrita impossível de abrir: %s\n", *(argv+5));
            return 0;
        }
    }

    else
        imOutput = stdout;

    if (argc > 5) {
        imInput = fopen(*(argv+5), "r");
        if (imInput == NULL) {
            printf("Ficheiro não encontrado: %s\n", *(argv+4));
            return 0;
        }
    }

    else
        imInput = stdin;

    if (argc > 4) {
        x = numero(*(argv+1));
        nRed = numero(*(argv+2));
        nGreen = numero(*(argv+3));
        nBlue = numero(*(argv+4));

        if (x == -1 || nRed == -1 || nGreen == -1 || nBlue == -1) {
            printf ("Uso incorreto: %s npixeis <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]", *argv);
            return 0;
        }
    }

    else {
        printf("Uso incorreto: %s npixeis <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]", *argv);
        return 0;
    }

    char* RGB = getString(imInput);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    fprintf (imOutput, "%s\n", RGB);
    free(RGB); //Desaloca uma porção de memória

    int altura = getInt(imInput) + 2*x; //O correspondeste às colunas de uma matriz  
    fprintf(imOutput, "%d ",altura);

    int largura = getInt(imInput) + 2*x; //O correspondente às linhas de uma matriz
    fprintf(imOutput, "%d\n",largura);
    

    int maxRGB = getInt(imInput);
    fprintf(imOutput, "%d\n", maxRGB);

    PIXEL matrix[largura][altura];
    for (int i=x; i<largura-x; i++) {
        for (int j=x; j<altura-x; j++) {
            matrix[i][j].red = getInt(imInput);
            matrix[i][j].green = getInt(imInput);
            matrix[i][j].blue = getInt(imInput);
        }
    }

    for (int i=0; i<largura; i++) {
        for (int j=0; j<altura; j++) {
           if (!(i == 0 && j == 0))
                fprintf(imOutput, "\n");
            if (i < x || i > largura -1  - x || j < x || j > altura - 1 - x)
                fprintf(imOutput, "%d %d %d", nRed, nGreen, nBlue);
            else
                fprintf(imOutput, "%d %d %d", matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue);
        }
    }

    fprintf(imOutput, "\n");
    fclose(imInput);
    fclose(imOutput);

    return 0;
}