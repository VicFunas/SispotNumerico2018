#include <stdio.h>
#include <string.h>

#define bufSize 1024
#define colunaBarra 4


void leDadosBarra(FILE *fp, double **matriz)
{
	int numeroBarras = 0;
	int i, j;
	int barraNumero;
	int tipoBarra;
	double tensaoNominalFase;
	double parametro1;
	double parametro2;
	//char buf[bufSize];
	

	fopen_s(&fp, "Redes/1_Stevenson/1_Stevenson_DadosBarras.txt", "r");
	if (fp == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(fp, "%d", &numeroBarras);
	printf("%d\n", numeroBarras);

	for (i = 0; i < numeroBarras; i++) {
		fscanf_s(fp, "%d", &barraNumero);
		printf("NB = %d\n", barraNumero);
		for (j = 0; j < colunaBarra; j++) {
			fscanf_s(fp, "%lf", &matriz[i][j]);
		}
	}

	fclose(fp);
}

void imprimir_matriz(double **matriz, int linhas, int colunas) {
	int i, j;
	for (i = 0; i < linhas; i++) {
		for (j = 0; j < colunas; j++) {
			printf("%.3lf ", matriz[i][j]);
		}
		printf("\n");
	}
}

double ** inicializa_Matrix(int linhasA, int colunasA) {
	int i = 0, j = 0;
	double **A = (double **) malloc(linhasA * sizeof(double*));
	for (i = 0;i<linhasA;i++) {
		A[i] =(double *) malloc(colunasA * sizeof(double));
	};
	for (i = 0; i<linhasA; i++) {
		for (j = 0; j< colunasA; j++) A[i][j] = 0;
	};
	return A;
} //*/

int main(int argc, char *argv[])
{
	FILE* fp;
	double **dadosBarra = inicializa_Matrix(5, colunaBarra);

	//leDadosBarra(fp, dadosBarra);
	
	leDadosBarra(fp, dadosBarra);
	imprimir_matriz(dadosBarra, 5, colunaBarra);

	system("pause");
	return 0;
}


