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

#pragma region leiturasArquivos

void leNumeroDeLinhas(FILE *file, char *barra, int *nBarras, int *nPQ, int *nPV) // nBarras, nPQ, nPV, j
{
	int i, linha;
	double tipo, par1, par2, tensao;

	fopen_s(&file, barra, "r");
	if (file == NULL)
	{ // Open source file.
		perror("fopen source-file");
		return;
	}

	fscanf_s(file, "%d", nBarras);
	*nPQ = 0;
	*nPV = 0;

	for (i = 0; i < *nBarras; i++) {
		fscanf_s(file, "%d", &linha);
		fscanf_s(file, "%lf", &tipo);
		fscanf_s(file, "%lf", &tensao);
		fscanf_s(file, "%lf", &par1);
		fscanf_s(file, "%lf", &par2);
		if(tipo == 0){
			*nPQ = *nPQ + 1;
		}
		else if(tipo == 1){
			*nPV = *nPV + 1;
		}
	}

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

#pragma endregion leiturasArquivos

#pragma region inicializaVetoresAuxiliares

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
			pEsp[j] = matriz[i][2];
			j++;
		}
	}
}

void inicializaJ(double *j, int tamanho, double **matriz) {
	int k = 0;
	for (int i = 0; i < tamanho; ++i)
	{
		if(matriz[i][0] == 0) { // colocar primeiro índices de PQ
			j[k] = i;
			k++;
		}
	}
	for (int i = 0; i < tamanho; ++i)
	{
		if(matriz[i][0] == 1) { // colocar depois índices de PV
			j[k] = i;
			k++;
		}
	}
}

#pragma endregion inicializaVetoresAuxiliares

#pragma region imprimeMatrizesEVetores

void imprimir_matriz(double **matriz, int linhas, int colunas) {
	int i, j;
	for (i = 0; i < linhas; i++) {
		for (j = 0; j < colunas; j++) {
			printf("%11.3lf ", matriz[i][j]);
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

#pragma endregion imprimeMatrizesEVetores

#pragma region criaMatrizesEVetores

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

#pragma endregion criaMatrizesEVetores

#pragma region calculoMatrizesMetodoNewton

void calculaF(int nPQ, int nPV, double*jIndice, double ** matrizB,  double ** matrizG, double *tensao, double *fase, double *pEsp, double **f, int tamanho) {
	
	// Cálculo de fpj
	for (int i = 0; i < nPQ+nPV; i++)
	{
		int j = (int) jIndice[i];
		//printf("j = %d\n", j);
		double fp = 0;
		for (int k = 0; k < tamanho; k++)
		{
			double thetaKJ = fase[k] - fase[j];
			fp += tensao[k]*(matrizG[j][k]*cos(thetaKJ) - matrizB[j][k]*sin(thetaKJ));
		}
		f[i][0] = tensao[j]*fp;
		
	}
	// Como os nPV ultimos correspondem às barras PV, em que a potência especificada não é necessariamente nula
	for (int i = 0; i < nPV; ++i)
	{
		f[nPQ+i][0] = f[nPQ+i][0] - pEsp[i];
	}

	// Cálculo de fqj
	for (int i = 0; i < nPQ; i++)
	{
		int j = (int) jIndice[i];
		double fq = 0;
		for (int k = 0; k < tamanho; k++)
		{
			double thetaKJ = fase[k] - fase[j];
			//printf("fatorG[%d][%d] = %lf\n", j, k, matrizG[j][k]*sin(thetaKJ));
			//printf("fatorB[%d][%d] = %lf\n", j, k, matrizB[j][k]*cos(thetaKJ));
			fq += tensao[k]*(matrizG[j][k]*sin(thetaKJ) + matrizB[j][k]*cos(thetaKJ));
		}
		//printf("somaFQ = %lf\n", fq);
		f[nPQ+nPV+i][0] = (-1)*tensao[j]*fq;
		//printf("f[%d][0] %lf\n", nPQ+nPV+i, f[nPQ+nPV+i][0]);
	}
}

void calculaJacobiano(int nPQ, int nPV, double*jIndice, double ** matrizB,  double ** matrizG, double *tensao, double *fase, double **del, int tamanho) {
	
	// Cálculo de dfp (dfp/dtheta e dfp/dV)
	for (int i = 0; i < nPQ+nPV; i++)
	{
		int j = (int) jIndice[i];
		//printf("j = %d\n", j);
		double dtheta;
		for (int c = 0; c < 2*nPQ + nPV; c++)
		{
			if(c == i) { // Eq (13) 1 -> dfpj/dthetaj
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					double thetaKJ;
					if(k != j) {
						thetaKJ = fase[k] - fase[j];
						dtheta += tensao[k]*(matrizG[j][k]*sin(thetaKJ) + matrizB[j][k]*cos(thetaKJ));
						
					}
				}
				del[i][c] = tensao[j]*dtheta;
				//printf("del[%d][%d] = %lf\n", i, c, del[i][c]);
			}

			else if (c == nPV + nPQ + i) // Eq (13) 2 -> dfpj/dVj 
			{
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					double thetaKJ;
					if(k != j) {
						//printf("k = %d\n", k);
						thetaKJ = fase[k] - fase[j];
						dtheta+= tensao[k]*(matrizG[j][k]*cos(thetaKJ) - matrizB[j][k]*sin(thetaKJ));
					}
				}
				//printf("soma = %lf\n", dtheta);
				//printf("tensao = %lf\n", tensao[j]);
				del[i][c] = dtheta + 2*tensao[j]*matrizG[j][j];
				//printf("del[%d][%d] = %lf\n", i, c, del[i][c]);
			}

			else if (c < nPV + nPQ) // Eq (13) 3 -> dpfj/dthetak
			{
				int k = (int)jIndice[c];
				double thetaKJ = fase[k] - fase[j];
				del[i][c] = (-1)*tensao[j]*tensao[k]*(matrizG[j][k]*sin(thetaKJ) + matrizB[j][k]*cos(thetaKJ));
				//printf("del[%d][%d] = %lf\n", i, c, del[i][c]);
			}
			else // Eq (13) 4 -> dfpj/dVk
			{
				int k = (int)jIndice[c - nPQ - nPV];
				double thetaKJ = fase[k] - fase[j];
				del[i][c] = tensao[j]*(matrizG[j][k]*cos(thetaKJ) - matrizB[j][k]*sin(thetaKJ));
				//printf("del[%d][%d] = %lf\n", i, c, del[i][c]);
			}
		}
	}

	// Cálculo de dfq (dfq/dtheta e dfq/dV)
	for (int i = 0; i < nPQ; i++)
	{
		int j = (int) jIndice[i];
		//printf("j = %d\n", j);
		double dtheta;
		for (int c = 0; c < 2*nPQ + nPV; c++)
		{
			if(c == i) { // Eq (14) 1 -> dfqj/dthetaj
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					if(k != j) {
						double thetaKJ = fase[k] - fase[j];
						dtheta+= tensao[k]*(matrizG[j][k]*cos(thetaKJ) - matrizB[j][k]*sin(thetaKJ));
					}
				}
				del[nPQ+nPV+i][c] = tensao[j]*dtheta;
				//printf("del[%d][%d] = %lf\n", nPQ+nPV+i, c, del[nPQ+nPV+i][c]);
			}

			else if (c == nPV + nPQ + i) // Eq (14) 2 -> dfqj/dVj
			{
				dtheta = 0;
				for (int k = 0; k < tamanho; k++)
				{
					if(k != j) {
						//printf("k = %d\n", k);
						double thetaKJ = fase[k] - fase[j];
						dtheta+= tensao[k]*(matrizG[j][k]*sin(thetaKJ) + matrizB[j][k]*cos(thetaKJ));
					}
				}
				//printf("soma = %lf\n", dtheta);
				del[nPQ+nPV+i][c] = -dtheta - 2*tensao[j]*matrizB[j][j];
				//printf("del[%d][%d] = %lf\n", nPQ+nPV+i, c, del[nPQ+nPV+i][c]);
			}

			else if (c < nPV + nPQ) // Eq (14) 3 -> dfqj/dthetak
			{
				int k = (int)jIndice[c];
				//printf("k = %d\n", k);
				double thetaKJ = fase[k] - fase[j];
				del[nPQ+nPV+i][c] = (-1)*tensao[j]*tensao[k]*(matrizG[j][k]*cos(thetaKJ) - matrizB[j][k]*sin(thetaKJ));
				//printf("del[%d][%d] = %lf\n", nPQ+nPV+i, c, del[nPQ+nPV+i][c]);
			}

			else // Eq (14) 4 -> dfpj/dVk
			{
				int k = (int)jIndice[c - nPQ - nPV];
				double thetaKJ = fase[k] - fase[j];
				del[nPQ+nPV+i][c] = (-1)*tensao[j]*(matrizG[j][k]*sin(thetaKJ) + matrizB[j][k]*cos(thetaKJ));
				//printf("del[%d][%d] = %lf\n", nPQ+nPV+i, c, del[nPQ+nPV+i][c]);
			}
		}
	}
}

#pragma endregion calculoMatrizesMetodoNewton

#pragma region operacoresMatriciais

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

double ** multiplyMatrix(int m1, int m2, double **matrizA,
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
    return res;
}

#pragma endregion operacoresMatriciais

int main(int argc, char *argv[])
{
	FILE* file;
	int nBarras;
	int nPQ = -1;
	int nPV = -1;
	char arquivoBarra[caminhoDadosBarra];
	char arquivoNodal[caminhoDadosNodal];
	decideRede(arquivoBarra, arquivoNodal);

	leNumeroDeLinhas(file, arquivoBarra, &nBarras, &nPQ, &nPV);

	double **dadosBarra = inicializa_Matrix(nBarras, colunaBarra);

	// matrizes de admitancia
	double **gTrechos = inicializa_Matrix(nBarras, nBarras);
	double **bTrechos = inicializa_Matrix(nBarras, nBarras);

	//vetores auxiliares
	double *tensao = inicializaVetor(nBarras);
	double *angulo = inicializaVetor(nBarras);
	double *pEsp = inicializaVetor(nPV);
	double *jIndice = inicializaVetor(nPQ+nPV);
	
	leDadosBarra(file, dadosBarra, arquivoBarra);
	leDadosTrecho(file, gTrechos, bTrechos, arquivoNodal);
	
	int nEquacoes = 2*(nPQ) + nPV;

	// Atribução de valores aos vetores auxiliares
	inicializaTensao(tensao, angulo, nBarras, dadosBarra); // conforme tabela 4
	inicializaPesp(pEsp, nBarras, dadosBarra);
	inicializaJ(jIndice, nBarras, dadosBarra);

	// Vetores para cálculo segundo método de Newton
	double ** f = inicializa_Matrix(nEquacoes, 1);
	double ** jacobiano = inicializa_Matrix(nEquacoes, nEquacoes);
	double ** delta = inicializa_Matrix(nEquacoes, 1);

	// Calcula os valores
	calculaF(nPQ, nPV, jIndice, bTrechos,  gTrechos, tensao, angulo, pEsp, f, nBarras);
	printf("\nVetor f\n");
	imprimir_matriz(f, nEquacoes, 1);
	printf("\n");
	
	calculaJacobiano(nPQ, nPV, jIndice, bTrechos,  gTrechos, tensao, angulo, jacobiano, nBarras);
	printf("\nMatriz Jacobiana\n");
	imprimir_matriz(jacobiano, nEquacoes, nEquacoes);
	printf("\n");

	// Inversa
	double **inversa = inicializa_Matrix(nEquacoes, nEquacoes);
	cofactor(jacobiano, inversa, 3, 3);
	delta = multiplyMatrix(nEquacoes, nEquacoes, inversa, 1, nEquacoes, f);
	printf("Resultado\n");
	imprimir_matriz(delta, nEquacoes, 1);
	printf("\n");
	system("pause");

	// ^ uma iteração -> implementar para fazer em várias (cada iteração recebe tensao e theta, sendo os novos como o recebido + delta)
	return 0;
}