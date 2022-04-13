#include <stdio.h>
#include <stdlib.h>

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

// Le o header
void read_header(FILE *fO, FILE *fD, int *lins, int *cols, char *line){
	// Copiar primeira linha
  	fgets(line, 200, fO);
        fprintf(fD, "%s", line);
	
	// Ignorar comentarios
	while(fscanf(fO, "%s", line) && line[0] == '#')
		fgets(line, 200, fO);
	
	// Encontrar dimensoes e valor maximo
	*cols = atoi(line);
	fscanf(fO, "%d %d", lins, &MAX_VAL);
	fprintf(fD, "%d %d\n%d\n", *cols, *lins, MAX_VAL);
	
	// Ignorar comentarios
	while(fscanf(fO, "%s", line) && line[0] == '#')
	fgets(line, 200, fO);
}

// Le a matrix
void h_bars(FILE *fO, FILE *fD, char *line, int lins, int cols, int width, int color[]){
	int r, g , b;
	r = atoi(line);
	fscanf(fO, "%d %d", &g, &b);
	free(line);
	fprintf(fD, "%d %d %d\n", r, g, b);
	
	int l, c=1, w=0, wdt = width * 2;
	for(l=0; l < lins; l++){
		if(l % wdt < width)
			while(c < cols){
				fscanf(fO, "%d %d %d\n", &r, &g, &b);
				fprintf(fD, "%d %d %d\n", r, g, b);
				c++;
			}
		else
			while(c < cols){
				fscanf(fO, "%d %d %d\n", &r, &g, &b);
				fprintf(fD, "%d %d %d\n", color[0], color[1], color[2]);
				c++;
			}
		w++;
		c=0;
	}
}

int main(int argc, char* argv[]){
    char *line;
    line = (char*) malloc(200);
	int lins, cols, width, color[3];
	
	// Propriedades das barras
	width = atoi(argv[1]);
	color[0] = atoi(argv[2]);
	color[1] = atoi(argv[3]);
	color[2] = atoi(argv[4]);
	
	// Abrir ficheiros
	FILE *fO, *fD;
	if(argc > 5){
		fO = fopen(argv[5], "r");
		if(fO == NULL)
			throw(argv[5]);
		if(argc > 6)
			fD = fopen(argv[6], "w");
		else
			fD = stdout;
	}else{
		fO = stdin;
		fD = stdout;
	}
	
	// Ler o header
	read_header(fO, fD, &lins, &cols, line);
	
	// Ler e imprimir imagem com barras
	h_bars(fO, fD, line, lins, cols, width, color);
	
	// Fechar ficheiros e sair do programa
	fclose(fO);
	fclose(fD);
	return 0;
}