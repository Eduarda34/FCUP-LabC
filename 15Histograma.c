#include <stdio.h>
#include <stdlib.h>
#include <Math.h>

int MAX_VAl;

typedef struct PIXEL{
	int r;
	int g;
	int b;
}Pixel;

void throw(char *fileName){
	printf("Error: \"%s\" could not be open!\n", fileName);
	printf("Check file name and make sure it ends with \".ppm\"\n");
	exit(1);
}

// Le o header
void read_header(FILE *fO, FILE *fD, int *lins, int *cols){
	int p;
	fscanf(fO, "P%d %d %d %d", &p, cols, lins, &MAX_VAl);
	fprintf(fD, "P%d\n%d %d\n%d\n", p, *cols, *lins, MAX_VAl);
}

// Le a matrix
void read_matrix(FILE *fO, int lins, int cols, Pixel matrix[][cols], int histo[]){
	for(int l=0; l < lins; l++){
		for(int c=0; c < cols; c++){
			fscanf(fO, "%d %d %d",
			&matrix[l][c].r, &matrix[l][c].g, &matrix[l][c].b);
			// printf("%d   %d\n", matrix[l][c].r, histo[matrix[l][c].r]);
			histo[matrix[l][c].r]++;
		}
	}
}

int main(int argc, char* argv[]){
	int lins, cols, *histo, *cdf;
	// cdf = (int*) malloc((MAX_VAl+1) * sizeof(int));
	
	// Abrir ficheiros
	FILE *fO, *fD;
	if(argc > 1){
		fO = fopen(argv[1], "r");
		if(fO == NULL)
			throw(argv[1]);
		if(argc > 2)
			fD = fopen(argv[2], "w");
		else
			fD = stdout;
	}else{
		fO = stdin;
		fD = stdout;
	}
	
	// Ler o header
	read_header(fO, fD, &lins, &cols);
	
	//Inicializar histograma a 0
	histo = (int*) malloc((MAX_VAl+1) * sizeof(int));
	for(int i=0; i<(MAX_VAl+1); i++)
		histo[i] = 0;
	
	// Criar e ler matrix de pixels
	Pixel (*matrix)[cols] = malloc(sizeof(Pixel[lins][cols]));
	read_matrix(fO, lins, cols, matrix, histo);
	
	// Calcular cdf
	int acc = 0;
	for(int i=0; i<(MAX_VAl+1); i++)
		if(histo[i] != 0){
			histo[i] += acc;
			acc = histo[i];
		}		
	
	// Equalizar histograma
	int cdfMin=-1;
	for(int i=0; (cdfMin = histo[i]) == -1; i++);
	
	for(int l=0; l<lins; l++)
		for(int c=0; c<cols; c++)
			matrix[l][c].r = round((histo[matrix[l][c].r] - cdfMin) *
			(MAX_VAl-1) / (lins * cols - cdfMin));
	
	// Imprimir nova matriz
	for(int l=0; l<lins; l++)
		for(int c=0; c<cols; c++)
			fprintf(fD, "%d %d %d\n", matrix[l][c].r, matrix[l][c].r, matrix[l][c].r);
	
	// Fechar ficheiros e sair do programa
	free(matrix);
	free(histo);
	fclose(fO);
	fclose(fD);
	
	return 0;
}