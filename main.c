#include <stdio.h>
#include<math.h>
#include <string.h>

#define bufSize 1024
#define colunaBarra 4
#define colunaNodal 2
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
    	strcpy(nodal, "Redes/2_Reticulada/2_Reticulada_Ynodal.txt");
    }
    else if (n == 3) {
    	strcpy(barra, "Redes/3_DiPrimaria/3_DiPrimaria_DadosBarras.txt");
    	strcpy(nodal, "Redes/3_DiPrimaria/3_DiPrimaria_Ynodal.txt");
    }
    else {
    	strcpy(barra, "Redes/4_Secundaria/4_Secundaria_DadosBarras.txt");
    	strcpy(nodal, "Redes/4_Secundaria/4_Secundaria_Ynodal.txt");
    }
}


void leNumeroDeLinhas(FILE *file, int *linhasBarra, char *barra)
{
	fopen_s(&file, barra, "r");
	if (file == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(file, "%d", linhasBarra);

	fclose(file);
}

void leDadosBarra(FILE *file, double **matriz,  char *barra)
{
	int numeroBarras = 0;
	int i, j;
	int barraNumero;	

	fopen_s(&file, barra, "r");
	if (file == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(file, "%d", &numeroBarras);

	for (i = 0; i < numeroBarras; i++) {
		fscanf_s(file, "%d", &barraNumero);
		for (j = 0; j < colunaBarra; j++) {
			fscanf_s(file, "%lf", &matriz[i][j]);
		}
	}

	fclose(file);
}

void leDadosTrecho(FILE *file, double **matrizG, double **matrizB, char *nodal)
{
	int numeroTrechos = 0;
	int i, j, linha, coluna;

	fopen_s(&file, nodal, "r");
	if (file == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(file, "%d", &numeroTrechos);

	for (i = 0; i < numeroTrechos; i++) {
		fscanf_s(file, "%d", &linha);
		fscanf_s(file, "%d", &coluna);
		fscanf_s(file, "%lf", &matrizG[linha][coluna]);
		fscanf_s(file, "%lf", &matrizB[linha][coluna]);
	}

	fclose(file);
}

void inicializaTensao(double *tensao, double *angulo, int tamanho, double **matriz) {
	for (int i = 0; i < tamanho; ++i)
	{
		if(matriz[i][0] == 2) {
			//printf("67 patinete\n");
			tensao[i] = matriz[i][2];
			angulo[i] = matriz[i][3];
		}
		else if (matriz[i][0] == 1)
		{
			//printf("dig dig joy dig joy popoy\n");
			tensao[i] = matriz[i][3];
			angulo[i] = 0;
		}
		else {
			//printf("e pede uma tequila\n");
			tensao[i] = matriz[i][1];
			angulo[i] = 0;
		}
	}
}

void inicializaPesp(double *pEsp, int tamanho, double **matriz) {
	int j = 0;
	for (int i = 0; i < tamanho; ++i)
	{
		if(matriz[i][0] == 1) {
			printf("67 patinete  ");
			pEsp[j] = matriz[i][2];
			printf("%.lf\n", matriz[i][2]);
			j++;
		}
	}
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

void imprimir_matrizAdmitancias(double **matriz, int linhas, int colunas) {
	int i, j;
	for (i = 0; i < linhas; i++) {
		for (j = 0; j < colunas; j++) {
			printf("%.15lf ", matriz[i][j]);
		}
		printf("\n");
	}
}

void imprimir_vetor(double *vetor, int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		printf("%.3lf ", vetor[i]);
	}
}

double * inicializaVetor(int tamanho) {
	double * vetor = (double *)malloc(tamanho * sizeof(double));
	for (int i = 0; i < tamanho; ++i)
	{
		vetor[i] = 0;
	}
	return vetor;
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
}

void calculaF(int nPQ, int nPV, double ** matrizB,  double ** matrizG, double *tensao, double *fase, double *pEsp, double *f, int tamanho) {
	for (int i = 0; i < nPQ+nPV; i++)
	{
		double fp = 0;
		for (int j = 0; j < tamanho; j++)
		{
			double thetaKJ = fase[j] - fase[i];
			fp += tensao[j]*(matrizG[i][j]*cos(thetaKJ) - matrizB[i][j]*sin(thetaKJ));
		}
		f[i] = tensao[i]*fp;
	}
	// Como os nPV ultimos correspondem às barras PV, em que a potência especificada não é necessariamente nula
	for (int i = 0; i < nPV; ++i)
	{
		f[nPQ+i] = f[nPQ+nPV] - pEsp[i];
	}

	for (int i = 0; i < nPQ; i++)
	{
		double fq = 0;
		for (int j = 0; j < tamanho; j++)
		{
			double thetaKJ = fase[j] - fase[i];
			fq += tensao[j]*(matrizG[i][j]*sin(thetaKJ) + matrizB[i][j]*cos(thetaKJ));
		}
		f[nPQ+nPV+i] = (-1)*tensao[i]*fq;
	}
}

void calculaDel(int nPQ, int nPV, double ** matrizB,  double ** matrizG, double *tensao, double *fase, double **del, int tamanho) {
	for (int i = 0; i < nPQ+nPV; i++)
	{
		double dtheta = 0;
		for (int j = 0; j < 2*nPQ + nPV; j++)
		{
			if(i == j) { // Eq (13) 1 -> dfpj/dthetaj
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					if(k != i) {
						double thetaKJ = fase[k] - fase[i];
						dtheta+= tensao[k]*(matrizG[i][k]*sin(thetaKJ) + matrizB[i][k]*cos(thetaKJ));
					}
				}
				del[i][j] = tensao[j]*dtheta;
			}
			else if (j == nPV + nPQ + i) // Eq (13) 2 -> dfpj/dVj
			{
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					if(k != i) {
						double thetaKJ = fase[k] - fase[i];
						dtheta+= tensao[k]*(matrizG[i][k]*cos(thetaKJ) - matrizB[i][k]*sin(thetaKJ));
					}
				}
				del[i][j] = dtheta + 2*tensao[i]*matrizG[i][i];
			}
			else if (j < nPV + nPQ) // Eq (13) 3 -> dpfj/dthetak
			{
				int k = j;
				double thetaKJ = fase[k] - fase[i];
				del[i][j] = (-1)*tensao[i]*tensao[k]*(matrizG[i][k]*sin(thetaKJ) + matrizB[i][k]*cos(thetaKJ));
			}
			else // Eq (13) 4 -> dfpj/dVk
			{
				int k = j - nPQ - nPV;
				double thetaKJ = fase[k] - fase[i];
				del[i][j] = tensao[i]*(matrizG[i][k]*cos(thetaKJ) - matrizB[i][k]*sin(thetaKJ));
			}
		}
	}

	for (int i = 0; i < nPQ; i++)
	{
		double dtheta = 0;
		for (int j = 0; j < 2*nPQ + nPV; j++)
		{
			if(i == j) { // Eq (14) 1 -> dfqj/dthetaj
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					if(k != i) {
						double thetaKJ = fase[k] - fase[i];
						dtheta+= tensao[k]*(matrizG[i][k]*cos(thetaKJ) - matrizB[i][k]*sin(thetaKJ));
					}
				}
				del[i][j] = tensao[i]*dtheta;
			}
			else if (j == nPV + nPQ + i) // Eq (14) 2 -> dfpj/dVj
			{
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					if(k != i) {
						double thetaKJ = fase[k] - fase[i];
						dtheta+= tensao[k]*(matrizG[i][k]*sin(thetaKJ) + matrizB[i][k]*cos(thetaKJ));
					}
				}
				del[i][j] = -dtheta - 2*tensao[i]*matrizB[i][i];
			}
			else if (j < nPV + nPQ) // Eq (14) 3 -> dpfj/dthetak
			{
				int k = j;
				double thetaKJ = fase[k] - fase[i];
				del[i][j] = (-1)*tensao[i]*tensao[k]*(matrizG[i][k]*cos(thetaKJ) - matrizB[i][k]*sin(thetaKJ));
			}
			else // Eq (14) 4 -> dfpj/dVk
			{
				int k = j - nPQ - nPV;
				double thetaKJ = fase[k] - fase[i];
				del[i][j] = (-1)*tensao[i]*(matrizG[i][k]*sin(thetaKJ) + matrizB[i][k]*cos(thetaKJ));
			}
		}
	}
}

int determinaQuantiaBarras(double **matriz, int linhas, int tipo) {
	int nP = 0;
	for (int i = 0; i < linhas; ++i)
	{
		if(matriz[i][0] == tipo) {
			//printf("67 patinete\n");
			nP++;
		}
	}
	return nP;
}

double determinant(double **a, int tamanho, int tamanhoOriginal)
{
double s = 1, det = 0; 
double **b = inicializa_Matrix(tamanhoOriginal, tamanhoOriginal);
int i, j, m, n, c;
if (tamanho == 1)
 return (a[0][0]);
else
 {
 det = 0;
 for (c = 0; c < tamanho; c++)
  {
  m = 0;
  n = 0;
  for (i = 0; i < tamanho; i++)
   {
   for (j = 0; j < tamanho; j++)
    {
    b[i][j] = 0;
    if (i != 0 && j != c)
     {
     b[m][n] = a[i][j];
     if (n < (tamanho - 2))
      n++;
     else
      {
      n = 0;
      m++;
      }
     }
    }
   }
  det = det + s * (a[0][c] * determinant(b, tamanho - 1, tamanhoOriginal));
  s = -1 * s;
  }
 }
return (double)det;
}

/*Finding transpose of matrix*/ 
void transpose(double **matriz,double **fac, double **inversa, int tamanho, int tamanhoOriginal)
{
  int i,j;
  double **b = inicializa_Matrix(tamanhoOriginal, tamanhoOriginal);
  double d;
 
  for (i=0;i<tamanho;i++)
    {
     for (j=0;j<tamanho;j++)
       {
         b[i][j]=fac[j][i];
        }
    }
  d=determinant(matriz,tamanho, tamanhoOriginal);
  for (i=0;i<tamanho;i++)
    {
     for (j=0;j<tamanho;j++)
       {
       	inversa[i][j]=b[i][j] / d;
        }
    }
}

void cofactor(double **matriz, double **inversa, int tamanho, int tamanhoOriginal)
{
 double **b = inicializa_Matrix(tamanhoOriginal, tamanhoOriginal);
 double **fac = inicializa_Matrix(tamanhoOriginal, tamanhoOriginal);
 int p,q,m,n,i,j;
 for (q=0;q<tamanho;q++)
 {
   for (p=0;p<tamanho;p++)
    {
     m=0;
     n=0;
     for (i=0;i<tamanho;i++)
     {
       for (j=0;j<tamanho;j++)
        {
          if (i != q && j != p)
          {
            b[m][n]=matriz[i][j];
            if (n<(tamanho-2))
             n++;
            else
             {
               n=0;
               m++;
               }
            }
        }
      }
      fac[q][p]=(double)pow(-1,q + p) * determinant(b,tamanho-1, tamanhoOriginal);
    }
  }
  transpose(matriz,fac, inversa, tamanho, tamanhoOriginal);
}

void multiplyMatrix(int m1, int m2, double **matrizA,
              int n1, int n2, double **matrizB)
{
    int x, i, j;
    double **res = inicializa_Matrix(m1,  n2);
    for (i = 0; i < m1; i++) {
        for (j = 0; j < n2; j++) {
            res[i][j] = 0;
            for (x = 0; x < m2; x++) {
                *(*(res + i) + j) += *(*(matrizA + i) + x) *
                                     *(*(matrizB + x) + j);
            }
        }
    }
    for (i = 0; i < m1; i++) {
        for (j = 0; j < n2; j++) {
            printf("%lf ", *(*(res + i) + j));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
	FILE* file;
	int linhasBarra;
	char arquivoBarra[caminhoDadosBarra];
	char arquivoNodal[caminhoDadosNodal];
	decideRede(arquivoBarra, arquivoNodal);

	printf("%s\n", arquivoNodal);
	printf("%s\n", arquivoBarra);

	leNumeroDeLinhas(file, &linhasBarra, arquivoBarra);

	double **dadosBarra = inicializa_Matrix(linhasBarra, colunaBarra);

	// matrizes de admitancia
	double **gTrechos = inicializa_Matrix(linhasBarra, linhasBarra);
	double **bTrechos = inicializa_Matrix(linhasBarra, linhasBarra);

	//vetores de tensao nominal de fase
	double *tensao = inicializaVetor(linhasBarra);
	double *angulo = inicializaVetor(linhasBarra);
	
	leDadosBarra(file, dadosBarra, arquivoBarra);
	leDadosTrecho(file, gTrechos, bTrechos, arquivoNodal);

	inicializaTensao(tensao, angulo, linhasBarra, dadosBarra); // conforme tabela 4
	int nPQ = determinaQuantiaBarras(dadosBarra, linhasBarra, 0);
	int nPV = determinaQuantiaBarras(dadosBarra, linhasBarra, 1);
	int nEquacoes = 2*nPQ + nPV;

	// potência ativa especificada (apenas para as barras PV)
	double *pEsp = inicializaVetor(nPV);
	inicializaPesp(pEsp, linhasBarra, dadosBarra);

	// Vetores para cálculo segundo método de Newton
	double * f = inicializaVetor(nEquacoes);
	double ** del = inicializa_Matrix(nEquacoes, nEquacoes);
	double * delta = inicializaVetor(nEquacoes);

	// Calcula os valores
	calculaF(nPQ, nPV, bTrechos,  gTrechos, tensao, angulo, pEsp, f, linhasBarra);
	calculaDel(nPQ, nPV, bTrechos,  gTrechos, tensao, angulo, del, linhasBarra);
	
	// Inversa
	double **inversa = inicializa_Matrix(3, 3);

	// Resultados -> Delta = (del^-1)*f
	double **identidade = inicializa_Matrix(3, 3);
	identidade[0][0] = 2;
	identidade[0][1] = 1;
	identidade[0][2] = 3;
	identidade[1][0] = 1;
	identidade[1][1] = 5;
	identidade[1][2] = 9;
	identidade[2][0] = 1;
	identidade[2][1] = -7;
	identidade[2][2] = 2;
	imprimir_matriz(identidade, 3, 3);

	double **vec = inicializa_Matrix(3, 1);
	vec[0][0] = 4;
	vec[1][0] = -2;
	vec[2][0] = 3;

	multiplyMatrix(3, 3, identidade, 3, 1, vec);

	cofactor(identidade, inversa, 3, 3);
	imprimir_matriz(inversa, 3, 3);

	system("pause");
	return 0;
}