#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define N 5

int moving_sum[N] = {0};

int main() {
    for (int i = 0; i < N; i++) {
        printf("Enter #%d/%d", i, N);
        scanf("%d", &moving_sum[i]);
    }

    return 0;
}

