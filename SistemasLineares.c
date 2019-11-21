/**
 * \file SistemasLineares.c
 * \author Jackson Borguezani & Roberta Tomigian
 * \mainpage Funções de alocação, inicialiazação e resolução de um sistema linear com o Cálculo da NormaL2
 * \section introSec ReadMe
 * Trabalho 1 | ICC - Prof. Armando Nicolui. |
 * Aluno: Jackson Rossi Borguezani GRR20176573. \n Roberta Tomigian GRR20171631.
 * \brief Arquivo que define os métodos de calcular uma solução para um sistema linear e calcular a norma do resíduo L2.
 *\ date 22 out 2019
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "SistemasLineares.h"
#include "utils.h"
#include "pdeSolver.h"

/*!
	\fn SistLinear_t *alocaSistLinear(unsigned int nx, unsigned int ny)
	*
	\brief Alocação do Sistema Linear Pentadiagonal
	*
	\param nx: Número de pontos a serem calculados no eixo x
	*
	\param ny: Número de pontos a serem calculados no eixo y
	*
	\details Função responsável por alocar dinamicamente os vetores do SL.
	*
*/
SistLinear_t *alocaSistLinear(unsigned int nx, unsigned int ny)
{
	SistLinear_t *SL = (SistLinear_t *)malloc(sizeof(SistLinear_t));
	if (SL)
	{
		SL->b = (real_t *)aligned_alloc(8, ((nx+2)*(ny+2)) * sizeof(real_t));
		SL->x = (real_t *)aligned_alloc(8, ((nx+2)*(ny+2)) * sizeof(real_t));
		
		if (!(SL->b) || !(SL->x))
		{
			liberaSistLinear(SL);
		}
	}
	return SL;
}

/*!
	\fn Metrica *alocaMetrica(unsigned int nx, unsigned int ny, int maxIter)
	*
	\brief Aloca e inicia a estrutura de dados métrica
	*
	\param nx: Número de pontos a serem calculados no eixo x
	*
	\param ny: Número de pontos a serem calculados no eixo y
	*
	\param maxIter: Número máximo de iterações
	*
*/
Metrica *alocaMetrica(unsigned int nx, unsigned int ny, int maxIter){
	Metrica *P = (Metrica *)malloc(sizeof(Metrica));
	int t = maxIter;
	if(P){
		P->norma = (double*)malloc(t * sizeof(double));
		if(!(P->norma)){
			free(P);
		} 
		for(int i = 0; i <= t; ++i){
			P->norma[i] = 0.0;
		}
		P->iter = 0;
		P->mediaTempo = 0.0;
	}
	return P;
}

/*!
	\fn void inicializaSistLinear(SistLinear_t *SL, int x, int y)
	*
	\brief Inicialização do Sistema Linear com zeros
	*
	\param SL: Ponteiro do Sistema Linear
	*
	\param x: Número de pontos a serem calculados no eixo x
	*
	\param y: Número de pontos a serem calculados no eixo y
	*
*/
void inicializaSistLinear(SistLinear_t *SL, int x, int y)
{
	SL->nx = x;
	SL->ny = y;
	int tam = (x+2) * (y+2);
	for (unsigned int i = 0; i < tam; ++i)
	{
		SL->b[i] = 0.0;
	}
	return;
}

/*!
	\fn void liberaSistLinear(SistLinear_t *SL)
	*
	\brief Liberação dos Ponteiros do Sistema Linear.
	*
	\param SL: Ponteiro do Sistema Linear
	*
	\details A função libera todos os ponteiros da estrutura do Sistema Linear.
	*
*/
void liberaSistLinear(SistLinear_t *SL)
{
	free(SL->b);
	free(SL->x);
	free(SL);
	return;
}

/*!
	\fn int gaussSeidel(SistLinear_t *SL, int maxIter)
	*
	\brief Método da Gauss-Seidel
	*
	\param SL: Ponteiro do Sistema Linear
	*
	\param maxIter: Número máximo de iterações
	*
	\param P: É um ponteiro para uma estrutura de dados que guarda o vetor norma, a média do tempo e o número de iterações de cada iteração do Gauss-Seidel.
	*
	\details Esta função calcula a solução de um sistema linear pentadiagonal, com estrutura de dados em vetores.
	*
*/

int gaussSeidel(SistLinear_t *SL, int maxIter, Metrica *P)
{
	real_t xk, *Xi;
	real_t register Di = SL->di;
	real_t register Dia = SL->dia;
	real_t register Ds = SL->ds;
	real_t register Dsa = SL->dsa;
	real_t register Dp = SL->dp;

	int nx = SL->nx;
	int ny = SL->ny;

	Xi = SL->x;

	double somaTempo = 0.0;
	int index, index_iafast, index_safast;
	contorno_x(SL);
	int k = 1;
	do
	{
		#ifdef gaussTEMPO
		double tempo = timestamp();
		#endif
		for(unsigned int i = 1; i < (nx+1); ++i){
			for(unsigned int j = 1; j < (ny+1); ++j){
				index = (i*(ny+2));
				index_iafast = ((i-1)*(ny+2));
				index_safast = ((i+1)*(ny+2));
				Xi[index + j] = SL->b[index + j] - ((Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j])) / Dp;++j;
				Xi[index + j] = SL->b[index + j] - ((Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j])) / Dp;++j;
				Xi[index + j] = SL->b[index + j] - ((Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j])) / Dp;++j;
				Xi[index + j] = SL->b[index + j] - ((Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j])) / Dp;++j;
				
			}
		}
		#ifdef gaussTEMPO
		double tempof = timestamp();
		double tempoFim = tempof - tempo;
		somaTempo += tempoFim;
		#endif

		#ifdef normaL2TEMPO
		double tempo = timestamp();
		P->norma[k] = normaL2Residuo(SL);
		double tempof = timestamp();
		double tempoFim = tempof - tempo;
		somaTempo += tempoFim;
		#endif

		++k;
	} while (k <= maxIter);
	P->mediaTempo = somaTempo / k;
	P->iter = k-1;

	#ifdef gaussTEMPO
	printf("%d %g\n",ny ,somaTempo/maxIter);
	#endif

	#ifdef normaL2TEMPO
	printf("%d %g\n",ny ,somaTempo/maxIter);
	#endif

	return 0;
}

/*!
	\fn double normaL2Residuo(SistLinear_t * SL)
	*
	\brief Cálculo da NormaL2
	*
	\param SL: Ponteiro do Sistema Linear
	*
*/
double normaL2Residuo(SistLinear_t *SL)
{
	real_t *R;

	int i, j, k, l, tam;
	real_t xk;

	int nx = SL->nx;
	int ny = SL->ny;

	real_t register Di = SL->di;
	real_t register Dia = SL->dia;
	real_t register Ds = SL->ds;
	real_t register Dsa = SL->dsa;
	real_t register Dp = SL->dp;

	real_t *Xi = SL->x;
	real_t *Bi = SL->b;

	real_t soma = 0.0;
	int index, index_iafast, index_safast;

	for(unsigned int i = 1; i < (nx+1); ++i){
		for(unsigned int j = 1; j < (ny+1); ++j){	
			index = i*(ny+2);	
			index_iafast = ((i-1)*(ny+2));
			index_safast = ((i+1)*(ny+2));	

			xk = Bi[index + j] - (Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j]) - (Dp * Xi[index + j]);
			soma += xk*xk;
			++j;

			xk = Bi[index + j] - (Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j]) - (Dp * Xi[index + j]);
			soma += xk*xk;
			++j;
			
			xk = Bi[index + j] - (Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j]) - (Dp * Xi[index + j]);
			soma += xk*xk;
			++j;			

			xk = Bi[index + j] - (Di * Xi[index + j - 1]) - (Dia * Xi[index_iafast + j]) - (Ds * Xi[index + j + 1]) - (Dsa * Xi[index_safast + j]) - (Dp * Xi[index + j]);
			soma += xk*xk;
			++j;
									
		}
	}
	return sqrt(soma);
}
