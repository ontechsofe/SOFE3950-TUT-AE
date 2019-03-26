#include <stdlib.h>
#include<stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main(int argc, char *argv[]){
    int x[10000000];
    int snorm = 0 ;
    int pnorm = 0;
    double sstart,send,pstart,pend,stime,ptime;
    int thread_num, nthreads = 16;
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    for (int i = 0; i < 10000000; i++){
        x[i] = rand();
    }
    sstart = omp_get_wtime();
    //time_t start = time(NULL);
    for(int o = 0; o<10000000; o++){
        snorm += fabs(x[o]);
    }
    send = omp_get_wtime();
    stime = send - sstart;
    //time_t end = time(NULL);
    printf("Serial time taken: %f\n", stime);
    pstart = omp_get_wtime();
    #pragma omp parallel for reduction(+: pnorm)
    for(int p = 0; p<10000000; p++){
        pnorm += fabs(x[p]);
    }
    pend = omp_get_wtime();
    ptime = pend - pstart;
    printf("parallel time taken%f\n", ptime);
    return 0;
    
}