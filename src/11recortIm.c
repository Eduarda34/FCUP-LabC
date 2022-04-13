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
    char* lida = calloc(300, sizeof(char));
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
    for (int i=0; *(str+i) != '\0'; i++) {
        if (!(*(str+i) >= '0' && *(str+i) <= '9'))
            return -1;
    }
    return atoi(str);  //Converte strings em números inteiros
}

int main (int argc, char* argv[]){
    FILE* imgInput;
    FILE* imgOutput;
    int xmin, ymin, xmax, ymax;

    if (argc > 7) {
        fprintf(stderr, "Uso incorreto: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
        return 0;
    }

    if (argc > 6) {
    imgOutput = fopen(*(argv+6), "w");

        if (imgOutput == NULL) {
            fprintf(stderr, "Ficheiro de escrita impossível de abrir: %s\n", *(argv+6));
            return 0;
        }
    }
    else imgOutput = stdout;

    if (argc > 5) {
    imgInput = fopen(*(argv+5), "r");

        if (imgInput == NULL) {
            fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+5));
            return 0;
        }
    }

    else imgInput = stdin;

    if (argc > 4) {
        xmin = numero(*(argv+1)); 
        ymin = numero(*(argv+2));
        xmax = numero(*(argv+3));
        ymax = numero(*(argv+4));
    }

    char* RGB = getString(imgInput);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    fprintf(imgOutput, "%s\n", RGB);
    free(RGB);

    int altura = getInt(imgInput); //Corresponde às colunas de uma matrix
    if(xmax > altura)
        xmax = altura-1;
    fprintf(imgOutput, "%d ", xmax-xmin+1);

    int largura = getInt(imgInput);  //Corresponde às linhas de uma matriz
    if(ymax > largura)
        ymax = largura-1;
    fprintf(imgOutput, "%d\n", ymax-ymin+1);

    int maxRGB = getInt(imgInput);
    fprintf(imgOutput, "%d", maxRGB);

    PIXEL matrix[largura][altura];
    for(int i=0; i<largura; i++){
        for(int j=0; j<altura; j++){
            matrix[i][j].red = getInt(imgInput);
            matrix[i][j].green = getInt(imgInput);
            matrix[i][j].blue = getInt(imgInput);
        }
    }

    for(int i=ymin; i<=ymax; i++){
        for(int j=xmin; j<=xmax; j++)
            fprintf(imgOutput, "\n%d %d %d", matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue);
    }

    fprintf(imgOutput, "\n");
    fclose(imgInput);
    fclose(imgOutput);

    return 0;
}