#include <stdio.h>
#include <string.h>

#define bufSize 1024
#define colunaBarra 4
#define caminhoDadosBarra 49
#define caminhoDadosNodal 42

void decideRede(char *barra, char *nodal) {
	char *p, s[100];
    int n;
    printf("Decida a Rede\n");
    printf("   |-> 1) Stevenson\n");
    printf("   |-> 2) Reticulada\n");
    printf("   |-> 3) Distribiucao Primaria\n");
    printf("   |-> 4) Distribiucao Primaria Secundaria\n");
    printf("Favor escolha uma opcao: ");
    //pega do teclado
    while (fgets(s, sizeof(s), stdin)) {
        n = strtol(s, &p, 10);
        // caso não seja int
        if ((p == s || *p != '\n')) {
            printf("Favor escolha uma opcao: ");
        } 
        // não ser opção válida
        else if (n != 1 && n!= 2 && n!= 3 && n!=4) {
        	printf("Favor escolha uma opcao: ");
        } 
        // sai do loop
        else break;
    }
    if (n == 1) {
    	strcpy(barra, "Redes/1_Stevensonn/1_Stevensonn_DadosBarras.txt");
    	strcpy(nodal, "Redes/1_Stevensonn/1_Stevensonn_Ynodal.txt");
    }
    else if (n == 2) {
    	strcpy(barra, "Redes/2_Reticulada/2_Reticulada_DadosBarras.txt");
    	strcpy(barra, "Redes/2_Reticulada/2_Reticulada_Ynodal.txt");
    }
    else if (n == 3) {
    	strcpy(barra, "Redes/3_DiPrimaria/3_DiPrimaria_DadosBarras.txt");
    	strcpy(barra, "Redes/3_DiPrimaria/3_DiPrimaria_Ynodal.txt");
    }
    else {
    	strcpy(barra, "Redes/4_Secundaria/4_Secundaria_DadosBarra.txt");
    	strcpy(barra, "Redes/4_Secundaria/4_Secundaria_Ynodal.txt");
    }
}


void leNumeroDeLinhas(FILE *fp, int *linhas, char *barra)
{
	fopen_s(&fp, barra, "r");
	if (fp == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(fp, "%d", linhas);
	printf("%d\n", *linhas);

	fclose(fp);
}

void leDadosBarra(FILE *fp, double **matriz,  char *barra)
{
	int numeroBarras = 0;
	int i, j;
	int barraNumero;	

	fopen_s(&fp, barra, "r");
	if (fp == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(fp, "%d", &numeroBarras);

	for (i = 0; i < numeroBarras; i++) {
		fscanf_s(fp, "%d", &barraNumero);
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
	int linhasBarra;
	
	char arquivoBarra[caminhoDadosBarra];
	char arquivoNodal[caminhoDadosNodal];
	decideRede(arquivoBarra, arquivoNodal);
	//printf("%s\n", arquivoBarra);
	leNumeroDeLinhas(fp, &linhasBarra, arquivoBarra);
	system("pause");
	double **dadosBarra = inicializa_Matrix(linhasBarra, colunaBarra);
	
	leDadosBarra(fp, dadosBarra, arquivoBarra);
	imprimir_matriz(dadosBarra, linhasBarra, colunaBarra);

	system("pause");
	return 0;
}