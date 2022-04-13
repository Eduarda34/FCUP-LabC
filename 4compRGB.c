#include <stdio.h>  //Operação 4
#include <stdlib.h>
#include <string.h>
#define MAXSTR 15

typedef struct pixel { //Componentes de cor RGB de um pixel
    int red;
    int green;
    int blue;
} PIXEL;

int number (char* str) { //Verificar se são indicados os sinais corretos para fazer a alteração de cor
    if (!(*str == '+' || *str == '-' || (*str >= '0' && *str <= '9')))
        return -1;
    
    for (int i=1; *(str+i) != '\0'; i++){
        if (!(*(str+i) >= '0' && *(str+i) <= '9'))
            return -1;
    }

    return atoi (str); //Converte strings em números inteiros
}

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

int adicionar (int num, int cor, int maxRGB) {
    int auxiliar = num + cor;

    if (auxiliar > maxRGB)
        return maxRGB;
    
    else if (auxiliar < 0) 
        return 0;

    else
        return auxiliar;
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
    int nRed, nGreen, nBlue;

    FILE* imInput; //Imagem de input
    FILE* imOutput; //Imagem de output

    if (argc > 6) {
        printf ("Uso incorreto: %s <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    if (argc > 5) {
        imOutput = fopen(*(argv+5), "w");
        if (imOutput == NULL) {
            printf ("Ficheiro de escrita impossível de abrir: %s\n", *(argv+5));
            return 0;
        }
    }

    else 
        imOutput = stdout; //Associado ao terminar

    if (argc > 4) {
        imInput = fopen(*(argv+4), "r");

        if (imInput == NULL) {
            printf ("Ficheiro não encontrado: %s\n", *(argv+4));
            return 0;
        }
    }

    else 
        imInput = stdin; //Associado ao teclado

    if (argc > 3) {
        nRed = number(*(argv+1));
        nGreen = number(*(argv+2));
        nBlue = number(*(argv+3));

        if (nRed == -1 || nGreen == -1 || nBlue == -1) {
            printf ("Uso incorreto: %s <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
            return 0;
        }
    }

    else {
        printf ("Uso incorreto: %s <nRed> <nGreen> <nBlue> [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    char* RGB = getString(imInput);
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf(stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }

    fprintf(imOutput, "%s\n", RGB);
    free(RGB); //Desaloca a porção de memória

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
                fprintf(imOutput, "\n");
            fprintf(imOutput, "%d %d %d", adicionar(matrix[i][j].red,nRed,maxRGB), adicionar(matrix[i][j].green,nGreen,maxRGB), adicionar(matrix[i][j].blue,nBlue,maxRGB));
        } 
    }

    fclose(imInput); //Fecha arquivo que foi aberto pela função open
    fclose(imOutput);

    return 0;
}