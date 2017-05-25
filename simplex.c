#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "gauss.h"

bool precisaFaseI(size_t numRestricoes, int sinalRestricoes[numRestricoes]) {
    // Passar verificando o sinal de todas as restricoes e caso encontre
    // algum sinal igual a 1 --> >, 2 --> >=, 3 --> =, -3 --> = sera necessario
    // ter fase e portanto retornar true, caso contrario retornar false
    for (int i = 0; i < numRestricoes; i++) {
        if (sinalRestricoes[i] == 1 || sinalRestricoes[i] == 2 ||
            sinalRestricoes[i] == 3 || sinalRestricoes[i] == -3) {
                return true;
                break;
            }
    }

    return false;
}

void verificarECorrigirVetorB(size_t numRestricoes, size_t numVariaveis, float *b, float matriz[][numVariaveis],
    int sinalRestricoes[numRestricoes]) {

    // Passar verificando a existencia de valores negativos no vetor b e caso
    // encontrar inverter esse valor (multiplicar por -1) e os valores da matriz
    // e sinal da restrição
    for (int i = 0; i < numRestricoes; i++) {
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

int formaPadraoMatriz(size_t numRestricoes, size_t numVariaveis,
    float matriz[numRestricoes][numVariaveis + numRestricoes], int sinalRestricoes[numRestricoes]){

    // Passar verificando quantas colunas a mais terão que ser adicionadas porque caso
    // a restrição seja 3 ou -3 que é (=) não deve ser adicionado uma coluna para essa
    // restrição
    int tam = numVariaveis;
    for (int i = 0; i < numRestricoes; i++) {
        if (sinalRestricoes[i] != -3 && sinalRestricoes[i] != 3) {
            tam++;
        }
    }

    // Passar em cada linha da matriz adicionando a coluna necessária de acordo com a restrição
    // onde :
    // (<=, <) é 1,
    // (>=, >) é -1 e
    // (=) não é adicionado uma coluna
    float x;
    for (int i = 0; i < numRestricoes; i++) {
        if (sinalRestricoes[i] == -3 || sinalRestricoes[i] == 3) {
            continue;
        }
        if (sinalRestricoes[i] < 0) {
            x = 1;
        }
        else {
            x = -1;
        }

        int j = numVariaveis + i;
        matriz[i][j] = x;
    }

    // Retornar o numero total de colunas da matriz na forma padrão
    return tam;
}

int formaPadraoFuncObj(float funcObjetivo[], int numRestricoes, int numVariaveis, int *tipoProblema) {
    int tam = numVariaveis + numRestricoes;

    // Verificar se o tipo do problema onde 1 = MAX e 2 = MIN e caso o problema for de MAX
    // inverter a funçao objetivo para resolver o problema como MIN
    if (*(tipoProblema) == 1) {
        for (int i = 0; i < numVariaveis; i++) {
            funcObjetivo[i] *= -1;
        }
        *(tipoProblema) = 2;
    }

    // Passar nas variaveis de folga e colocar o seu índice como zero
    for (int i = numVariaveis; i < tam; i++) {
        funcObjetivo[i] = 0;
    }

    // Retornar o numero total de variaveis da funcão objetivo na forma padrão
    return tam;
}


float *simplex(int tipoProblema, float funcObjetivo[], int numVariaveis,
    int numRestricoes, float matrizOriginal[numRestricoes][numVariaveis],
    int sinalRestricoes[numRestricoes], float b[numRestricoes]) {

    // Arrumar o vetor B caso haja numeros negativos
    verificarECorrigirVetorB(numRestricoes, numVariaveis, b, matrizOriginal, sinalRestricoes);

    int colFuncObj = formaPadraoFuncObj(funcObjetivo, numRestricoes, numVariaveis, &tipoProblema);
    int colMatriz = formaPadraoMatriz(numRestricoes, numVariaveis, matrizOriginal, sinalRestricoes);

    // Verificar se precisa de fase I
    if (precisaFaseI(numRestricoes, sinalRestricoes)) {
        // Adicionar novas X colunas onde X = numRestricoes
        // Criar funcao objetivo artificial onde os indices originais sao 0 e os novos sao 1
        // simplex(tipoProblema, funcObjetivoA, numVariaveis + numRestricoes, numRestricoes, matrizArtificial, b);
    }

    // Fase II
    // Variaveis basicas e nao basicas
    float base[colMatriz - numVariaveis];
    float nBase[colMatriz - numRestricoes];

    // Metodo simplex
    int nIteracao = 0;
    bool parar = false;
    while (!parar) {

    }
}

int main() {

    // Numero de variaveis e numero de restricoes
    int numVariaveis, numRestricoes;

    printf ("Digite o num. de variaves e restricoes\n");
    scanf("%d %d", &numVariaveis, &numRestricoes);

    // Vetor com os indices da funcao objetivo e o tipo (MAX ou MIN)
    float funcObjetivo[numVariaveis + numRestricoes];

    // Tipo de funcao
    // MAX --> 1
    // MIN --> 2
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
    float matriz[numRestricoes][numVariaveis + numRestricoes];
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

    printf("Matriz na forma padrao: \n");
    int tam = formaPadraoMatriz(numRestricoes, numVariaveis, matriz, sinalRestricoes);
    for (int i = 0; i < numRestricoes; i++) {
        for (int j = 0; j < tam; j++) {
            printf("%f ", matriz[i][j]);
        }
        printf("\n");
    }
    // Fim teste

	return 0;
}