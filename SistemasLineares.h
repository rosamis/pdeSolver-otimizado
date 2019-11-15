#ifndef __SISTLINEAR_H__
#define __SISTLINEAR_H__
/**
 * \file SistemasLineares.h
 * \author Jackson Borguezani & Roberta Tomigian
 * \mainpage Biblioteca de Funções e Estruturas do Sistema Linear
 * \section introSec ReadMe
 * Trabalho 1 | ICC - Prof. Armando Nicolui. |
 * Aluno: Jackson Borguezani GRR20176573 | Roberta Tomigian GRR20171631
 * \brief Arquivo que define as funções e estruturas do sistema linear.
 *\ date 22 out 2019
 *
 */
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "utils.h"

#include <likwid.h>
//#define EPS 1.0e-4

/*============================= Estruturas de Dados =============================*/

typedef double real_t;

/*! \struct SistLinear_t
    \brief Estrutura de dados do Sistema Linear Pentadiagonal
*/
typedef struct {
  real_t dp; /**< Constante da diagonal principal. */
  real_t ds; /**< Constante da diagonal superior. */
  real_t di; /**< Constante da diagonal inferior. */
  real_t dia; /**< Constante da diagonal inferior afastada. */
  real_t dsa; /**< Constante da diagonal superior afastada. */
  real_t* restrict b; /**< Vetor de termos independentes. */
  real_t* restrict x; /**< Vetor solução. */
  unsigned int nx; /**< Quantidade de pontos em x. */
  unsigned int ny; /**< Quantidade de pontos em y. */
} SistLinear_t;

/*! \struct Metrica
    \brief Estrutura de dados das métricas usadas na escrita do arquivo
*/
typedef struct{
    double * restrict norma; /**< Vetor das normas de cada iteração do Gauss-Seidel */
    double mediaTempo; /**< Média de tempo de cada iteração do Gauss-Seidel */
    int iter; /**< Quantidade total de iterações da entrada */
}Metrica;

/*=========================== Funções do Sistema Linear ===========================*/

SistLinear_t* alocaSistLinear (unsigned int nx, unsigned int ny);
Metrica* alocaMetrica(unsigned int nx, unsigned int ny, int maxIter);
void inicializaSistLinear (SistLinear_t *SL, int x, int y);
void liberaSistLinear (SistLinear_t *SL);
int gaussSeidel (SistLinear_t *SL, int maxIter, Metrica *P);
double normaL2Residuo(SistLinear_t *SL);

#endif //__SISTLINEAR_H__