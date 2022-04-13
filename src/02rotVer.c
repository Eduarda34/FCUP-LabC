#include <stdio.h>  //Operação 2
#include <stdlib.h>
#include <string.h>
#define MAXSTR 15

typedef struct pixel { //Componentes de cor RGB de um pixel
    int red;
    int green;
    int blue;
} PIXEL;

void readF (FILE* f){ //leitura do ficheiro e palavra
    char lida;
    do{
        fscanf (f, "%c", &lida); //Leitura de caracteres
    } while (lida != '\n');
}

char* getStr (FILE* f) {  //Strings
    char* lida = calloc (300, sizeof(char)); //Aloca o número específico de bytes e inicializa-os a zero
    fscanf (f, "%s", lida); //Leitura da string
    while (*lida == '#'){
        readF (f);
        fscanf (f, "%s", lida);
    }
    return lida;
}

int getInt (FILE* f) { //Gerar mensagem de erro
    char* lida = getStr(f);
    for (int i=0; *(lida+i) != '\0'; i++){
        if (!(*lida >= '0' && *lida <= '9')) {
            fprintf (stderr, "O ficheiro de entrada não apresenta valores válidos\n"); //Refere-se ao ecrã (stderr)
            exit(0);
        }
    }
    return atoi(lida); //Converte strings em números inteiros
}

int main (int argc, char* argv[]) { //Função principal
    FILE* imInput; //Imagem de input
    FILE* imOutput; //Imagem de output

    if (argc > 3) {
        fprintf (stderr, "Uso incorreto: %s [<ficheiro de entrada> [ficheiro de saída]]\n", *argv);
        return 0;
    }

    if (argc > 2) {
        imOutput = fopen (*(argv+2), "w");
        if (imOutput == NULL) {
            fprintf (stderr, "Ficheiro impossível de abrir: %s\n", *(argv+2));
            return 0;
        }
    }

    else
        imOutput = stdout; //Associado ao terminal (Standard output)

    if (argc > 1) {
        imInput = fopen (*(argv+1), "r");
        if (imInput == NULL) {
            fprintf (stderr, "Ficheiro não encontrado: %s\n", *(argv+1));
            return 0;
        }
    }

    else 
        imInput = stdin; //Associado ao teclado (Standard input)

    char* RGB = getStr (imInput); //P3
    if (strcmp(RGB, "P3")) { //Comparar strings
        fprintf (stderr, "O ficheiro de entrada não apresenta valores válidos\n");
        return 0;
    }
    fprintf (imOutput, "%s\n", RGB);
    free (RGB); //Desaloca a porção de memória

    int altura = getInt(imInput);  //O correspondente às colunas de uma matriz
    fprintf (imOutput, "%d ", altura);

    int largura = getInt(imInput); //O corresponde às linhas de uma matriz
    fprintf (imOutput, "%d\n", largura);

    int maxRGB = getInt(imInput);
    fprintf (imOutput, "%d\n", maxRGB);

    PIXEL matrix[largura][altura];
    for (int i=0; i < largura; i++) { //Fazer a matriz com as cores RGB
        for (int j=0; j < altura; j++) {
            matrix[i][j].red = getInt(imInput);
            matrix[i][j].green = getInt(imInput);
            matrix[i][j].blue = getInt(imInput);
        }
    }

    for (int i=largura-1; i>=0; i--) {
        for (int j=0; j<altura; j++) {
            if (!(i==largura-1 && j==0))
                fprintf (imOutput, "\n");
            fprintf (imOutput, "%d %d %d", matrix[i][j].red, matrix[i][j].green, matrix[i][j].blue);
        }
    }

    fclose(imInput);  //Fecha arquivo que foi aberto pela função open
    fclose(imOutput);  //Fecha o arquivo que foi aberto pela função open

    return 0;
}