#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* bellcurve(void* _grade) {
    float* grade = (float*)_grade;
    printf("%f\n", *grade * 1.50);
}

int main() {
    int numOfStudents = 5;

    pthread_t *threadArray;
    float* gradeArray;

    threadArray = (pthread_t*)malloc(numOfStudents * sizeof(pthread_t));
    gradeArray = (float*)malloc(numOfStudents * sizeof(float));

    for (int i = 0; i < numOfStudents; i++) {
        printf("Enter grade for student #%d\n", i+1);
        scanf("%f", &gradeArray[i]);
    }

    for (int i = 0; i < numOfStudents; i++) {
        void* grade = (void*)&gradeArray[i];
        pthread_create(&threadArray[i], NULL, bellcurve, grade);
    }

    for (int i = 0; i < numOfStudents; i++) {
        pthread_join(threadArray[i], NULL);
    }

    return 0;
}