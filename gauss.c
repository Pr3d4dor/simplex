#include "gauss.h"
#include <stdio.h>
#include <stdlib.h>

float modulo(float x){
  if (x < 0)
    x = -x;
  return x;
}

float *gauss(size_t ordem, float **matrizOriginal, float bOriginal[ordem]) {
    int n = ordem;
	float mult = 0, temp = 0;
	float matriz[ordem][ordem];
	float b[ordem];
	
	float *vetorResposta =  malloc(sizeof (float) * ordem);
	
	// Copiar para vetor e matriz local para as alterações não refletirem na matriz original
	for (int i = 0; i < ordem; i++) {
		for (int j = 0; j < ordem; j++) {
			matriz[i][j] = matrizOriginal[i][j];
		}
		b[i] = bOriginal[i];
	}
		
	/* Método de gauss */
	for (int k = 0; k < n; k++){
		/* Pivoteamento parcial */
		float maxEl = modulo(matriz[k][k]);
		int linhaDoMax = k;
		
		// Procura o maximo em modulo na coluna //
		for (int i = k + 1; i < n; i++) {
            if (modulo(matriz[i][k]) > maxEl) {
                maxEl = modulo(matriz[i][k]);
                linhaDoMax = i;
            }
        }
		
		// Se o maximo for zero nao tem como calcular a resposta
		if (maxEl == 0){
            return NULL;
		}
		
		// Troca linha atual com a linha de maior pivo //
		for (int i = 0; i < n; i++){
			float temp = matriz[linhaDoMax][i];
			matriz[linhaDoMax][i] = matriz[k][i];
			matriz[k][i] = temp;
		}
		
		// Trocar também no vetor resposta
		float temp = b[linhaDoMax];
		b[linhaDoMax] = b[k];
		b[k] = temp;
		
		/* Triangularizacao */
		for (int i = k + 1; i < n; i++){
			mult = matriz[i][k] / matriz[k][k];		
			matriz[i][k] = 0;
			b[i] -= mult * b[k];
			for (int j = k+1; j <= n; j++){
				matriz[i][j] -= mult * matriz[k][j];
			}	
		}
	}
		
	/* Substituição de cima para baixo */
	vetorResposta[n-1] = b[n-1] / matriz[n-1][n-1];
	for(int i = (n - 2);i >= 0; i--) {
		temp = b[i];
		for(int j=(i+1);j<n;j++){
			temp -= (matriz[i][j] * vetorResposta[j]);
		}
		vetorResposta[i] = temp / matriz[i][i];
	}

    return vetorResposta;
}  
