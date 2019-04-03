#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

int main ()
{
	// create arrays
	int array_a[100][100];
	int array_b[100][100];
	int array_c[100][100];
	
	// populate the arrays
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			array_a[i][j] = i + j;	
		}
	}
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			array_b[i][j] = i + j;	
		}
	}

	// parallel matrix multiplication

	#pragma omp parallel for
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				array_c[i][j] = array_a[i][k] * array_b[k][j];						
			}
		}
	} 
	
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			printf("%d ", array_c[i][j]);
		}
		printf("\n");
	}
}
