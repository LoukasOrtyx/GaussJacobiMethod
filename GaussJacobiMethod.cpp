#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define XCHAR 88

double *FindInitialK(int n, double **vet) {
	
	double *Ko = (double *) malloc(n * sizeof(double));
	
	for (int i = 0; i < n; i++) {
		
		Ko[i] = vet[i][n]/vet[i][i];
	}
	
	return Ko;
}

double **CreateSystem(int n) {
	
	double **vet;
	
	vet = (double **) malloc(n * sizeof(double *));
	
	for (int i = 0; i < n; i++) {
		
		vet[i] = (double *) malloc((n + 1) * sizeof(double));
	}
	
	return vet;
}

double *RefineK(int n, double **vet, double *Ko, double *error) {
	
	double *Ki = (double *) malloc(n * sizeof(double));
	
	for (int i = 0; i < n; i++) {
		
		Ki[i] = vet[i][n];
		
		for (int j = 0; j < n; j++) {
			
			if (j == i) continue;
			
			Ki[i] -= vet[i][j] * Ko[j];
		}
		
		Ki[i] /= vet[i][i];
		error[i] = fabs(Ki[i] - Ko[i]);
	}
	
	return Ki;
}

bool isErrorSmaller(double *error, double limit, int n) {
	
	double round;
	int timesten;
	
	for (int i = 0; i < n; i++) {
		
		round = error[i];
		
		while (round < 1) {
		
			round *= 10;
			timesten++;
		}
		
		round = (round > floor(round) + 0.5)? ceil(round) : floor(round);
		round /= pow(10, timesten);
		
		if(round >= limit) {
			
			return false;
		}
		
		timesten = 0;
	}
	
	return true;
}

main() {
	
	int n, letter = XCHAR;
	double *error, limit;
	
	setlocale(LC_ALL, "Portuguese");

	printf("Digite a quantidade de equações do sistema (use vírgula em vez de ponto):\n");
	scanf("%d", &n);	
	
	printf("Digite os parâmetros das equações seguindo a ordem: \naX + bY + cZ... = d\n");
	
	printf("Digite o erro:\n");
	scanf("%lf", &limit);
	
	double **vet = CreateSystem(n);
	error = (double *) malloc(n * sizeof(double));	
	
	for(int i = 0; i < n; i++) {
		
		error[i] = 1.0;
	}

	for (int i = 0; i < n; i++) {
		
		printf("Equação %d: \n", i + 1);
		
		for (int j = 0; j <= n; j++) {
			
			scanf("%lf", &vet[i][j]);			
		}
	}
	
	system("cls");
	
	double *Kvet = FindInitialK(n, vet);
	
	printf("\nIteração 0\n......................................................\n");
	
	for (int i = 0; i < n; i++) {
		
		if(i > 2) letter = 63;
		
		printf("Ki de %c: %lf\t                             |\n", (letter + i), Kvet[i]);
	}
	
	printf("\t\t\t                             |\n......................................................\n");
	
	int i = 0;

	while (!isErrorSmaller(error, limit, n)) {
		
		Kvet = RefineK(n, vet, Kvet, error);
		printf("\nIteração %d\n......................................................\n", i + 1);
		
		for(int j = 0; j < n; j++) {
			
			if(j > 2) letter = 63;
			
			printf("K de %c\t = %.4f\t                             |\n",(j + letter), Kvet[j]);
			printf("Err de %c = %.4f\t                             |\n\t\t\t                             |\n", (j + letter), error[j]);
		}
		
		printf("......................................................\n");
		i++;
	}
}
