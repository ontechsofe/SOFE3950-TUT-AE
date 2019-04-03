#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: question5 [nthreads]\n");
	}

	int nthreads = atoi(argv[1]);
	// set a variable n to 100 million	
	int n = 100000000;
	// define dx
    	double dx = 1.0 / (n + 1.0);    

	FILE *fp;
	fp = fopen("calculations.txt", "w+");

	#ifdef _OPENMP
	omp_set_num_threads(nthreads);
	#endif

	#pragma omp parallel for num_threads(nthreads)
	for (int i = 0; i < n; i++)
	{ 
		// lexical scope applies; x is private
	        double x = i * dx;
	        double y = exp(x) * cos(x) * sin(x) * sqrt(5 * x + 6.0);
		
		// at a millionth mark
		if (i % 1000000 == 0) {
			#pragma omp critical
			{
				// critical section to ensure safe writing
				#ifdef _OPENMP
				fprintf(fp, "i:%d\tx: %f\ty: %f\n", i, x, y);
				#endif
			}
		}	
	}
	
    	fclose(fp);
	return 0;
}
