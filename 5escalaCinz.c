#include <stdio.h>  //Operação 5
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

int converter (int R, int G, int B) {
    return (float) (R*(0.2126) + G*(0.7152) + B*(0.0722) + 0.5);
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

int main (int argc, char* argv[]) {
    FILE* imInput; //Imagem de input
    FILE* imOutput; //Imagem de output

    if (argc > 3) {
        printf ("Uso incorreto: %s <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    if (argc > 2) {
        imOutput = fopen(*(argv+2), "w");
        if (imOutput == NULL) {
            printf ("Impossível abrir o ficheio para escrita: %s\n", *(argv+2));
            return 0;
        }
    }

    else
        imOutput = stdout; //Associado ao terminal

    if (argc > 1) {
        imInput = fopen(*(argv+1), "r");
        if (imInput == NULL) {
            printf ("Ficheiro não encontrado: %s\n", *(argv+1));
            return 0;
        }
    }

    else
        imInput = stdin; //Associado ao teclado

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

            int x = converter(matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue);
            fprintf (imOutput, "%d %d %d", x, x, x);
        }
    }

    fclose(imInput); //Fecha arquivo que foi aberto pela função open
    fclose(imOutput);

    return 0;
}