#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "gauss.h"

// Funcao que verifica se algum valor esta no vetor passado por parametro
bool contemValor(size_t tam, int *vet, int valor) {
	bool resultado = false;
		
	for (int i = 0; i < tam; i++) {
		if (vet[i] == valor) {
			resultado = true;
			break;
		}
	}
	
	return resultado;
}

// Funcao que verifca a condicao de parada do vetor y
bool condicaoDeParadaY(float *vetorY, int tam) {
    int count = 0;

    for (int i = 0; i < tam; i++) {
        if (vetorY[i] <= 0) {
            count++;
        }
    }

    return count == tam;
}

// Funcao que retornar o indice do menor valor do vetor y
int minVetorY(float *vetorY, float *xB, size_t tam) {
    float resultados[tam];

    // Realizar o calculo do epsilon, ignorando os y que forem menores que zero
	// onde se o o y quebrar essa condicao, o valor nessa posição recebera -1
	// e sera ignorado no calculo do menor valor do vetor
	for (int i = 0; i < tam; i++) {
        if (vetorY[i] > 0) {
            resultados[i] = xB[i] / vetorY[i];	
        }
        else {
            resultados[i] = -1.0;
        }
		printf("resultados %d: %f\n", i, resultados[i]);
    }

    // Iniciar o minimo como sendo o primeiro valor nao negativo
	int k = 0;
    float min;
	for (int i = 0; i < tam; i++) {
		if (resultados[i] >= 0.0) {
			min = resultados[i];
			k = i;
			break;
		}
	}
	
	// Encontrar o menor valor no vetor Y ignorando os valores negativos
	for (int i = 0; i < tam; i++) {
		if (resultados[i] < 0.0) {
			continue;
		}	
        if (resultados[i] < min) {
            k = i;
            min = resultados[i];
        }
    }

	// Retornar a posicao no vetor do menor valor encontrado
    return k;
}

// Funcao que retorna o indice da variavel com menor custo relativo
int minCustoRelativo(float *custosRelativos, size_t tam) {
    float min = custosRelativos[0];
    int k = 0;

    for (int i = 1; i < tam; i++) {
        if (custosRelativos[i] < min) {
            k = i;
            min = custosRelativos[i];
        }
    }

    return k;
}

// Funcao que cria e retorna uma nova matriz com as colunas escolhidas a partir da matriz passada
// por parametro
float *retiraColuna(size_t ordem, float **matriz, size_t nCol) {
    float *coluna = malloc(ordem * sizeof(float));

    for (int i = 0; i < ordem; i++) {
        coluna[i] = matriz[i][nCol];
    }

    return coluna;
}

// Funcao que multiplica dois vetores
float multVetor(size_t ordem, float *v1, float *v2) {
    float resultado = 0;

    for (int i = 0; i < ordem; i++) {
        resultado += v1[i] * v2[i];
    }

    return resultado;
}

// Funcao que cria e retorna a matriz transposta da matriz passada por parametro
float **criarMatrizTransposta(size_t ordem, float **matrizOriginal) {
    
	// Copiar para uma matriz local para as alterações não refletirem na matriz original
	float matriz[ordem][ordem];
	for (int i = 0; i < ordem; i++) {
		for (int j = 0; j < ordem; j++) {
			matriz[i][j] = matrizOriginal[i][j];
		}
	}
	
	// Alocar a matriz transposta
    float **novaMatriz = (float**)(malloc(ordem * sizeof(float*)));
	for (int i = 0; i < ordem; i++) {
        novaMatriz[i] = (float *)(malloc(ordem * sizeof(float)));
    }

    // Colocar os valores transpostos na nova matriz
	for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            novaMatriz[i][j] = matriz[j][i];
        }
    }

	// Retornar a matriz transposta
    return novaMatriz;
}

// Criar matriz com colunas especificas
float **criarMatriz(size_t numRestricoes, size_t colMatriz, 
    float **matriz, size_t numColunas, int colunas[numColunas]) {
    
	// Alocando a matriz que contera as colunas selecionadas de outra matriz
    float **novaMatriz = (float**)(malloc(numRestricoes * sizeof(float*)));
    for (int i = 0; i < numRestricoes; i++) {
        novaMatriz[i] = (float *)(malloc(numColunas * sizeof(float)));
    }
    
    // Copiar as colunas selecionadas para a novaMatriz
	for (int i = 0; i < numColunas; i++) {
        int coluna = colunas[i];

        for (int j = 0; j < numRestricoes; j++) {
            novaMatriz[j][i] = matriz[j][coluna];
        }
    }

	// Retornar a matriz com as colunas selecionadas
    return novaMatriz;
}

// Funcao que verifica a necessidade de fase I
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

// Funcao que transforma a matriz na forma padrao
float **formaPadraoMatriz(size_t numRestricoes, size_t numVariaveis,
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

    // Alocar nova matriz e copiar os valores da matriz que nao esta na forma padrao
    float **novaMatriz = (float**)(malloc(numRestricoes * sizeof(float*)));
    for (int i = 0; i < numRestricoes; i++) {
        novaMatriz[i] = (float *)(malloc(tam * sizeof(float)));
        for (int j = 0; j < tam; j++) {
            novaMatriz[i][j] = matriz[i][j];
        }
    }

    // Passar em cada linha da matriz adicionando a coluna necessária de acordo com a restrição
    // onde :
    // (<=, <) é 1,
    // (>=, >) é -1 e
    // (=) não é adicionado uma coluna
	
	// Zerar todos os valores das colunas da identidade
    for (int i = 0; i < numRestricoes; i++) {
        for (int j = numVariaveis; j < numRestricoes + numVariaveis; j++) {
			novaMatriz[i][j] = 0;
        }
    }
	
    // Colocar os 1's e -1's nas posicoes corretas
	float x;
	int indice = 0;
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

        int j = numVariaveis + indice;
        novaMatriz[i][j] = x;
		indice++;
    }

    // Retornar o numero total de colunas da matriz na forma padrão
    return novaMatriz;
}

// Funcao que transforma a funcao objetivo na forma padrao
int formaPadraoFuncObj(float funcObjetivo[], int numRestricoes, int numVariaveis, int *tipoProblema) {
    int tam = numVariaveis + numRestricoes;

    // Verificar se o tipo do problema onde 1 = MAX e 2 = MIN e caso o problema for de MAX
    // inverter a funçao objetivo para resolver o problema como MIN
    if (*(tipoProblema) == 1) {
        for (int i = 0; i < numVariaveis; i++) {
            funcObjetivo[i] *= -1;
        }
    }

    for (int i = numVariaveis; i < tam; i++) {
        funcObjetivo[i] = 0;
    }

    // Retornar o numero total de variaveis da funcão objetivo na forma padrão
    return tam;
}

int *simplexFaseI(float *funcObjetivo, size_t numVariaveis,
    size_t numRestricoes, float **matrizOriginalFP,
    int sinalRestricoes[numRestricoes], float b[numRestricoes]) {
	
	int colMatrizArtficial = numVariaveis + numRestricoes;
	float matrizOriginalLocalFP[numRestricoes][numVariaveis + numRestricoes];
	
	for (int i = 0; i < numRestricoes; i++) {
		for (int j = 0; j < colMatrizArtficial; j++) {
			matrizOriginalLocalFP[i][j] = matrizOriginalFP[i][j];
		}
	}
	
	// Criar matriz artificial
	float **matrizArtificial = (float**)(malloc(numRestricoes * sizeof(float*)));
	for (int i = 0; i < numRestricoes; i++) {
        matrizArtificial[i] = (float *)(malloc(colMatrizArtficial * sizeof(float)));
    }
	
	// Zerar todos os valores das colunas da identidade
    for (int i = 0; i < numRestricoes; i++) {
        for (int j = numVariaveis; j < colMatrizArtficial; j++) {
            matrizArtificial[i][j] = 0;
        }
    }
	
	// Copiar os valores da matriz do problema original e colocar as colunas da identidade
    for (int i = 0; i < numRestricoes; i++) {
		for (int j = 0; j < numVariaveis; j++) {
			matrizArtificial[i][j] = matrizOriginalLocalFP[i][j];
		}
		int k = numVariaveis + i;
        matrizArtificial[i][k] = 1;
	}

	// Fase II
    // Variaveis basicas e nao basicas
    int numVarBasicas = colMatrizArtficial - numVariaveis;
    int numVarNbasicas = numVariaveis;
	int *base = malloc(numVarBasicas * sizeof(int));
    int nBase[numVarNbasicas];

    // Outras variaveis simplex
    float *xB;
    float *lambda;
    float custosRelativos[numVarNbasicas];
    float *vetorY;
    int cnk;
    int epsilon;

    // Iniciar com as variaveis de folga na base e as restantes serao nao basicas  
    for (int i = 0; i < numVarNbasicas; i++) {
        nBase[i] = i;
    }
    int indice = 0;
    for (int i = numVariaveis; i < colMatrizArtficial; i++) {
        base[indice] = i;
        indice++;
    }

    // Prints
	printf("\nFuncao objetivo artificial: \n");
	float funcObjetivoArtificial[colMatrizArtficial];
	for (int i = 0; i < colMatrizArtficial; i++) {
		if (i < numVariaveis) {
			funcObjetivoArtificial[i] = 0;
		}
		else{
			funcObjetivoArtificial[i] = 1;
		}
		printf("%.2f ", funcObjetivoArtificial[i]);
	}
	
    printf("\n\nMatriz artificial FP e vetor b:\n");
    for (int i = 0; i < numRestricoes; i++) {
        for (int j = 0; j < colMatrizArtficial; j++) {
            printf("%.2f\t", matrizArtificial[i][j]);
        }
        printf("= %.2f", b[i]);
        printf("\n");
    }

    printf("\n----- INICIO FASE 1 -------\n");
	// Metodo simplex
    int nIteracao = 0;
    bool parar = false;
    while(!parar) {
        nIteracao++;
        printf("\n\nIteracao: %d\n\n", nIteracao);

        float **matriz = criarMatriz(numRestricoes, colMatrizArtficial, matrizArtificial, numVarBasicas, base);

        printf("Matriz base:\n");
        for (int i = 0; i < numVarBasicas; i++) {
            for (int j = 0; j < numVarBasicas; j++) {
                printf("%.2f\t", matriz[i][j]);
            }
            printf("\n");
        }

        // Passo 1
        printf("\n\nPasso 1: calculo da solucao basica\n");
        xB = gauss(numVarBasicas, matriz, b);

        for (int i = 0; i < numVarBasicas; i++) {
            printf("xB%d: %f\n", i, xB[i]);
        }

        // Passo 2
        // Passo 2.1
        printf("\nPasso 2.1: vetor multiplicador simplex\n");
        // Matriz transposta
        float **bT = criarMatrizTransposta(numVarBasicas, matriz);

        // Gerar cB
        float cB[numVarBasicas];

        for (int i = 0; i < numVarBasicas; i++) {
            int colBase = base[i];
            cB[i] = funcObjetivoArtificial[colBase];
        }

		// Calcular lambda e exibir
        lambda = gauss(numVarBasicas, bT, cB);
        for (int i = 0; i < numVarBasicas; i++) {
            printf("lambda %d: %f\n", i, lambda[i]);
        }

        // Passo 2.2
        printf("\nPasso 2.2: custos relativos\n");
        for (int i = 0; i < numVarNbasicas; i++) {
            float *colNbase = retiraColuna(numVarBasicas, matrizArtificial, nBase[i]);
            custosRelativos[i] = funcObjetivoArtificial[nBase[i]] - multVetor(numVarBasicas, lambda, colNbase);
            printf("cN%d: %f\n", i, custosRelativos[i]);
        }

        // Passo 2.3
        cnk = minCustoRelativo(custosRelativos, numVarNbasicas);

        printf("\nPasso 2.3: determinacao da variavel a entrar na base\n");
        printf("cnk: %d\n", cnk);

        // Passo 3
        printf("\nPasso 3: teste de otimalidade\n");
        if (custosRelativos[cnk] >= 0) {
            parar = true;
            printf("Parou porque cnK > 0\n");
            break;
        }

        // Passo 4
        printf("\nPasso 4: calculo da direcao simplex\n");
        float *colNbasek = retiraColuna(numVarBasicas, matrizArtificial, nBase[cnk]);
		
        // Calcular By e exibir
		vetorY = gauss(numVarBasicas, matriz, colNbasek);
        for (int i = 0; i < numVarBasicas; i++) {
            printf("y%d: %f\n", i, vetorY[i]);
        }

        // Passo 5
        printf("\nPasso 5: determinacao da variavel a sair da base\n");
        if (condicaoDeParadaY(vetorY, numVarNbasicas)) {
            parar = true;
            printf("Parou porque o vetorY <= 0\n");
            break;
        }
        epsilon = minVetorY(vetorY, xB, numVarBasicas);
        printf("Variavel a sair da base: %d\n", epsilon);

        // Passo 6
        printf("\nPasso 6: atualizacao, nova particao basica\n");

        int aux = base[epsilon];
        base[epsilon] = nBase[cnk];
        nBase[cnk] = aux;

        // IMPRIMIR BASE E NBASE AQUI
		
		// Liberando memoria e apontando o ponteiro para nulo para evitar dangling pointers
		free(matriz);
		free(lambda);
		free(vetorY);
		free(bT);
		matriz = NULL;
		lambda = NULL;
		vetorY = NULL;
		bT = NULL;
    }
	
	// Liberando memoria e apontando o ponteiro para nulo para evitar dangling pointers
	free(matrizArtificial);
	matrizArtificial = NULL;
	
	// Exibindo as colunas que devem ficar na base para iniciar o simplex na fase II
	for (int i = 0; i < numRestricoes; i++) {
		printf("base[%d] = %d\n", i, base[i]);
	}
	
	printf("\n----- FIM FASE 1 -------\n");
	
	// Retornar o vetor com as colunas da base para o simplex fase II
	return base;
	
}

// Funcao que realiza o algoritmo simplex
float *simplex(int tipoProblema, float *funcObjetivo, size_t numVariaveis,
    size_t numRestricoes, float matrizOriginal[numRestricoes][numVariaveis],
    int sinalRestricoes[numRestricoes], float b[numRestricoes]) {
	
    // Passar verificando quantas colunas a mais terão que ser adicionadas porque caso
    // a restrição seja 3 ou -3 que é (=) não deve ser adicionado uma coluna para essa
    // restrição
    int colMatriz = numVariaveis;
    for (int i = 0; i < numRestricoes; i++) {
        if (sinalRestricoes[i] != -3 && sinalRestricoes[i] != 3) {
            colMatriz++;
        }
    }

    int colFuncObj = formaPadraoFuncObj(funcObjetivo, numRestricoes, numVariaveis, &tipoProblema);
    float** matrizFP = formaPadraoMatriz(numRestricoes, numVariaveis, matrizOriginal, sinalRestricoes);
	
	// Fase II
    // Variaveis basicas e nao basicas
    int numVarBasicas = numRestricoes;
    int numVarNbasicas = colMatriz - numRestricoes;
    int base[numVarBasicas];
    int nBase[numVarNbasicas];

    // Outras variaveis simplex
    float *xB;
    float *lambda;
    float custosRelativos[numVarNbasicas];
    float *vetorY;
    int cnk;
    int epsilon;

    // Verificar se precisa de fase I
	bool faseI = precisaFaseI(numRestricoes, sinalRestricoes);
    if (faseI) {
        
		// Adicionar novas X colunas onde X = numRestricoes
        // Criar funcao objetivo artificial onde os indices originais sao 0 e os novos sao 1
        int *baseNova = simplexFaseI(funcObjetivo, colMatriz, numRestricoes, matrizFP, sinalRestricoes, b);	
		for (int i = 0; i < numVarBasicas; i++) {
			base[i] = baseNova[i];
			
			if (base[i] > colMatriz) {
				printf("Problema infactivel!");
				
				return NULL;
			}
			
			printf("base[i]: %d\n", base[i]);			
		}
		
		int indice = 0;
		for (int i = 0; i < colMatriz; i++) {
			if (!contemValor(numVarBasicas, base, i)) {
				nBase[indice] = i;
				printf("nBase[i]: %d\n", i);
				indice++;
			}
		}
    }

    // Iniciar com as variaveis de folga na base e as restantes serao nao basicas 
	// se a fase I não foi executada 
    if (!faseI) {
		for (int i = 0; i < numVarNbasicas; i++) {
			nBase[i] = i;
		}
		int indice = 0;
		for (int i = numVariaveis; i < colMatriz; i++) {
			base[indice] = i;
			indice++;
		}
	}

    // Prints
	printf("\nFuncao objetivo na forma padrao: \n");
	for (int i = 0; i < colMatriz; i++) {
		printf("%.2f\t", funcObjetivo[i]);
	}
	
    printf("\n\nMatriz na FP e vetor b:\n");
    for (int i = 0; i < numRestricoes; i++) {
        for (int j = 0; j < colMatriz; j++) {
            printf("%.2f\t", matrizFP[i][j]);
        }
        printf("= %f", b[i]);
        printf("\n");
    }
	
	

    // Metodo simplex
    int nIteracao = 0;
    bool parar = false;
    while(!parar) {
        nIteracao++;
        printf("\n\nIteracao: %d\n\n", nIteracao);

        float **matriz = criarMatriz(numRestricoes, colMatriz, matrizFP, numVarBasicas, base);

        printf("Matriz base:\n");
        for (int i = 0; i < numVarBasicas; i++) {
            for (int j = 0; j < numVarBasicas; j++) {
                printf("%.2f\t", matriz[i][j]);
            }
            printf("\n");
        }

        // Passo 1
        printf("\n\nPasso 1: calculo da solucao basica\n");
        xB = gauss(numVarBasicas, matriz, b);

        for (int i = 0; i < numVarBasicas; i++) {
            printf("xB%d: %f\n", i, xB[i]);
        }

        // Passo 2
        // Passo 2.1
        printf("\nPasso 2.1: vetor multiplicador simplex\n");
        // Matriz transposta
        float **bT = criarMatrizTransposta(numVarBasicas, matriz);

        // Gerar cB
        float cB[numVarBasicas];

        for (int i = 0; i < numVarBasicas; i++) {
            int colBase = base[i];
            cB[i] = funcObjetivo[colBase];
        }

        lambda = gauss(numVarBasicas, bT, cB);
        for (int i = 0; i < numVarBasicas; i++) {
            printf("lambda %d: %f\n", i, lambda[i]);
        }

        // Passo 2.2
        printf("\nPasso 2.2: custos relativos\n");
        for (int i = 0; i < numVarNbasicas; i++) {
            float *colNbase = retiraColuna(numVarBasicas, matrizFP, nBase[i]);
            custosRelativos[i] = funcObjetivo[nBase[i]] - multVetor(numVarBasicas, lambda, colNbase);
            printf("cN%d: %f\n", i, custosRelativos[i]);
        }

        // Passo 2.3
        cnk = minCustoRelativo(custosRelativos, numVarNbasicas);

        printf("\nPasso 2.3: determinacao da variavel a entrar na base\n");
        printf("cnk: %d\n", cnk);

        // Passo 3
        printf("\nPasso 3: teste de otimalidade\n");
        if (custosRelativos[cnk] >= 0) {
            parar = true;
            printf("Parou porque cnK > 0\n");
            break;
        }

        // Passo 4
        printf("\nPasso 4: calculo da direcao simplex\n");
        float *colNbasek = retiraColuna(numVarBasicas, matrizFP, nBase[cnk]);
		
        vetorY = gauss(numVarBasicas, matriz, colNbasek);
        for (int i = 0; i < numVarBasicas; i++) {
            printf("y%d: %f\n", i, vetorY[i]);
        }

        // Passo 5
        printf("\nPasso 5: determinacao da variavel a sair da base\n");
        if (condicaoDeParadaY(vetorY, numVarNbasicas)) {
            parar = true;
            printf("Parou porque o vetorY <= 0\n");
			printf("Solucao ilimitada, ou seja, no infinito\n");
			
			return NULL;
        }
        epsilon = minVetorY(vetorY, xB, numVarBasicas);
        printf("Variavel a sair da base: %d\n", epsilon);

        // Passo 6
        printf("\nPasso 6: atualizacao, nova particao basica\n");

        int aux = base[epsilon];
        base[epsilon] = nBase[cnk];
        nBase[cnk] = aux;

        // IMPRIMIR BASE E NBASE AQUI
		
		// Liberando memoria e apontando o ponteiro para nulo para evitar dangling pointers
		free(matriz);
		free(lambda);
		free(vetorY);
		free(bT);
		matriz = NULL;
		lambda = NULL;
		vetorY = NULL;
		bT = NULL;
    }
	
    // Liberando memoria e apontando o ponteiro para nulo para evitar dangling pointers
    free(matrizFP);
    matrizFP = NULL;

    // Alocando o vetor xC e iniciando com zero
	float *xC = malloc((numVarBasicas + numVarNbasicas) * sizeof(float));
    for (int i = 0; i < numVarBasicas + numVarNbasicas; i++) {
        xC[i] = 0;
    }

    // Colocando o valor obtido no vetor xB nas posicoes certas de acordo com a base
	for (int i = 0; i < numVarBasicas; i++) {
        xC[base[i]] = xB[i];
    }
	
	// Calcular o valor do vetor xC aplicado na funcao objetivo, ou seja, f(xC).
	float fx = multVetor(numVarBasicas + numVarNbasicas, xC, funcObjetivo);
	if (tipoProblema == 1) {
		fx = -fx;
	}
	printf("\nf(x) = %f\n", fx);
	
	// Retorna o vetor xC resposta
	return xC;
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
        scanf(" %f", &funcObjetivo[i]);
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
	
	// Passar verificando a existencia de valores negativos no vetor b e caso
    // encontrar inverter esse valor (multiplicar por -1) e os valores da matriz
    // e sinal da restrição
    for (int i = 0; i < numRestricoes; i++) {
        if (b[i] < 0) {
            sinalRestricoes[i] *= -1;
            b[i] *= -1;
            for (int j = 0; j < numVariaveis; j++) {
                matriz[i][j] *= -1;
            }
        }
    }

	float *resultado;
    resultado = simplex(tipo, funcObjetivo, numVariaveis, numRestricoes, matriz, sinalRestricoes, b);
	
    if (resultado != NULL) {
		for (int i = 0; i < numVariaveis + numRestricoes; i++) {
			printf("xC%d: %f\n", i, resultado[i]);
		}
	}
	
	// Liberando memoria e apontando o ponteiro para nulo para evitar dangling pointers
	free(resultado);
	resultado = NULL;

	return 0;
}
