#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "gauss.h"


bool precisaFaseI(size_t numRestricoes, int sinalRestricoes[numRestricoes]) {
    for (int i = 0; i < numRestricoes; i++) {
        if (sinalRestricoes[i] == 1 || sinalRestricoes[i] == 2 ||
            sinalRestricoes[i] == 3 || sinalRestricoes[i] == -3) {
                return true;
                break;
            }
    }

    return false;
}

void verificarECorrigirVetorB(size_t tam, size_t numVariaveis, float *b, float matriz[][numVariaveis], int sinalRestricoes[tam]) {
    for (int i = 0; i < tam; i++) {
        if (b[i] < 0) {
            printf("entrou\n");   
            sinalRestricoes[i] *= -1;
            b[i] *= -1;
            for (int j = 0; j < numVariaveis; j++) {
                matriz[i][j] *= -1;
            }
        }
    } 
}

/*
float *simplex(int tipoProblema, float funcObjetivo[], int numVariaveis, 
    int numRestricoes, matrizOriginal[numRestricoes][numVariaveis], sinalRestricoes[numRestricoes], 
    float b[numRestricoes]) {
    
    // Arrumar o vetor B caso haja numeros negativos
    verificarECorrigirVetorB(numRestricoes, b, matrizOriginal);

    float *funcObjetivoFormaPadrao = formaPadraoFuncObj(funcObjetivo);
    float **matrizFormaPadrao = formaPadraoMatriz(matrizOriginal);

    // Verificar se precisa de fase I
    if (verificarFaseI()) {
        // Adicionar novas X colunas onde X = numRestricoes
        // Criar funcao objetivo artificial onde os indices originais sao 0 e os novos sao 1
        simplex(tipoProblema, funcObjetivoA, numVariaveis + numRestricoes, numRestricoes, matrizArtificial, b);
    }

    // Variaveis basicas e nao basicas
    float base[numVariaveis];
    float nBase[(numVariaveis + numRestricoes) - numVariaveis];

    // Metodo simplex
    int nIteracao = 0;
    bool parar = false;
    while (!parar) {

    }
} 
*/

int main() {
 
    // Numero de variaveis e numero de restricoes
    int numVariaveis, numRestricoes;

    printf ("Digite o num. de variaves e restricoes\n");
    scanf("%d %d", &numVariaveis, &numRestricoes);

    // Vetor com os indices da funcao objetivo e o tipo (MAX ou MIN)
    float funcObjetivo[numVariaveis];
    int tipo;

    // Prints e leitura
    printf("\n1 - MAX\n");
    printf("2 - MIN\n");
    printf("\nDigite o tipo de funcao:\n");
    scanf("%d", &tipo);
    printf("\nDigite os indices das variaveis da func. objetivo: \n");
    for (int i = 0; i < numVariaveis; i++) {
        scanf("%f", &funcObjetivo[i]);
    }

    // Vetor com o sinal das restricoes (<,>,<=,>=,=)
    // < --> -1
    // > --> 1
    // <= --> -2
    // >= --> 2
    // = --> 3 e -3
    int sinalRestricoes[numRestricoes];
    
    // Matriz e vetor b
    float matriz[numRestricoes][numVariaveis];
    float b[numRestricoes];

    // Leitura da matriz e vetor b
    printf("\nDigite as restricoes: \n");
    for (int i = 0; i < numRestricoes; i++) {
        for (int j = 0; j < numVariaveis; j++) {
            scanf("%f", &matriz[i][j]);
        }
        scanf("%d", &sinalRestricoes[i]);
        scanf("%f", &b[i]);
    }

    // Testando
    verificarECorrigirVetorB(numRestricoes, numVariaveis, b, matriz, sinalRestricoes);

    for (int i = 0; i < numRestricoes; i++) {
        printf("Restricao: %d, B: %f\n", sinalRestricoes[i], b[i]);
    }
    if (precisaFaseI(numRestricoes, sinalRestricoes)) {
        printf("Precisa de Fase I\n");
    }
    // Fim teste

	return 0;
}