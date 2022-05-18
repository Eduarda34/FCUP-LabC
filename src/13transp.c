#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int MAX_VAL;

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

// Le os headers
void read_header(FILE *fO, int *lins, int *cols, char *line){
	int MAX_VAL; // So serve para avanÃ§ar
	
	// Ler primeira linha
  	fgets(line, 200, fO);
	
	// Ignorar comentarios
	while(fscanf(fO, "%s", line) && line[0] == '#')
		fgets(line, 200, fO);
	
	// Encontrar dimensoes
	*cols = atoi(line);
	fscanf(fO, "%d %d", lins, &MAX_VAL);
	
	// Ignorar comentarios
	while(fscanf(fO, "%s", line) && line[0] == '#')
	fgets(line, 200, fO);
}

// Le a matrix
void read_matrix(FILE *fO, char *line, int lins, int cols, Pixel matrix[][cols]){
	matrix[0][0].r = atoi(line);
	fscanf(fO, "%d %d", &matrix[0][0].g, &matrix[0][0].b);
	free(line);
	
	for(int c=1; c < cols; c++)
		fscanf(fO, "%d %d %d",
		&matrix[0][c].r, &matrix[0][c].g, &matrix[0][c].b);
	for(int l=1; l < lins; l++){
		for(int c=0; c < cols; c++)
			fscanf(fO, "%d %d %d",
			&matrix[l][c].r, &matrix[l][c].g, &matrix[l][c].b);
	}
}

// Aplica o overlay
void overlay(FILE *fO, char *line, int lins2, int cols2, int lins1, int cols1,
Pixel matrix1[][cols1], Pixel matrix2[][cols2], int coord[], float alpha){
	int l1, l2, c1, c2, lstart=0, cstart=0;
	
	// Ajustar aos limites da imagem original
	if(coord[0] < 0) {cstart = -coord[0]; coord[0] = 0;}
	if(coord[1] < 0) {lstart = -coord[1]; coord[1] = 0;}
	if(coord[2] > cols1) cols2 = cols1 - coord[0];
	if(coord[3] > lins1) lins2 = lins1 - coord[1];
	
	for(l1=coord[1], l2=lstart; l2<lins2; l1++, l2++)
		for(c1=coord[0], c2=cstart; c2<cols2; c1++, c2++){
			matrix1[l1][c1].r = round (alpha * matrix2[l2][c2].r + (1 - alpha) * matrix1[l1][c1].r);
			matrix1[l1][c1].g = round(alpha * matrix2[l2][c2].g + (1 - alpha) * matrix1[l1][c1].g);
			matrix1[l1][c1].b = round(alpha * matrix2[l2][c2].b + (1 - alpha) * matrix1[l1][c1].b);
		}
	}

int main(int argc, char* argv[]){
    int  lins1, lins2, cols1, cols2, *coord;
	coord = (int*) malloc(4*sizeof(int));
	char *line1, *line2;
    line1 = (char*) malloc(200);
	line2 = (char*) malloc(200);
	
	// Ler posicao para sobrepor
	float alpha = atof(argv[1]);
	coord[0] = atoi(argv[2]);
	coord[1] = atoi(argv[3]);
	
	// Abrir ficheiros
	FILE *f1, *f2, *f3;
	f2 = fopen(argv[4], "r");
	if(f2 == NULL) throw(argv[4]);
	if(argc > 5){
		f1 = fopen(argv[5], "r");
		if(f1 == NULL)
			throw(argv[5]);
		if(argc > 6)
			f3 = fopen(argv[6], "w");
		else
			f3 = stdout;
	}
	
	else {
		f1 = stdin;
		f3 = stdout;
	}
	
	// Ler os headers
	read_header(f1, &lins1, &cols1, line1);
	read_header(f2, &lins2, &cols2, line2);
	
	// Calcular posicao para sobrepor
	coord[2] = coord[0] + cols2 - 1;
	coord[3] = coord[1] + lins2 - 1;
	
	// Criar e ler matrizes de pixeis
	Pixel (*matrix1)[cols1] = malloc(sizeof(Pixel[lins1][cols1]));
	read_matrix(f1, line1, lins1, cols1, matrix1);
	Pixel (*matrix2)[cols2] = malloc(sizeof(Pixel[lins2][cols2]));
	read_matrix(f2, line2, lins2, cols2, matrix2);
	
	// Aplicar o overlay
	overlay(f2, line2, lins2, cols2, lins1, cols1, matrix1, matrix2, coord, alpha);
	
	// Imprimir header
	fprintf(f3, "P3\n%d %d\n255\n", cols1, lins1);
	
	// Imprimir matriz
	for(int l=0; l < lins1; l++){
		for(int c=0; c < cols1; c++){
			fprintf(f3, "%d %d %d\n", matrix1[l][c].r, matrix1[l][c].g, matrix1[l][c].b);
		}
	}
	free(matrix1);
	free(matrix2);
	
	// Fechar ficheiros e sair do programa
	fclose(f1);
	fclose(f2);
	fclose(f3);
	return 0;
}
