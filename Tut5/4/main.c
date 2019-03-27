#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef void* AtomicGrade;

AtomicGrade globalAtomicGrade;

// EWWWW WATCH OUT FOR THIS GLOBAL VARIABLE!
typedef struct atomicGrade_st {
    float total_grade;
    pthread_mutex_t gradeLock;
} atomicGrade_t;

AtomicGrade createAtomicGrade() {
    atomicGrade_t* g = (atomicGrade_t*)malloc(sizeof(atomicGrade_t));
    pthread_mutex_init((&g->gradeLock), NULL);
    g->total_grade = 0;
    return (AtomicGrade) g;
}

void addToTotal(AtomicGrade _atomicGrade, float grade) {
    atomicGrade_t* g = (atomicGrade_t*) _atomicGrade;
    pthread_mutex_lock(&(g->gradeLock));
    g->total_grade += grade;
    pthread_mutex_unlock(&(g->gradeLock));
}

void printTotal(AtomicGrade _atomicGrade) {
    atomicGrade_t* g = (atomicGrade_t*) _atomicGrade;
    pthread_mutex_lock(&(g->gradeLock));
    printf("Final Sum: %f\n", g->total_grade);
    pthread_mutex_unlock(&(g->gradeLock));
}

void* addThread(void* _grade) {
    float grade = *(float*)_grade;
    addToTotal(globalAtomicGrade, grade);
    return NULL;
}

int main() {
    int numOfStudents = 10;
    float* gradeArray;
    pthread_t *threadArray;
    globalAtomicGrade = createAtomicGrade();
    threadArray = (pthread_t*)malloc(numOfStudents * sizeof(pthread_t));
    gradeArray = (float*)malloc(numOfStudents * sizeof(float));
    for (int i = 0; i < numOfStudents; i++) {
        printf("Enter grade for student #%d: ", i+1);
        scanf("%f", &gradeArray[i]);
    }
    for (int i = 0; i < numOfStudents; i++) {
        void* grade = (void*)&gradeArray[i];
        pthread_create(&threadArray[i], NULL, addThread, grade);
    }
    for (int i = 0; i < numOfStudents; i++) {
        pthread_join(threadArray[i], NULL);
    }
    printTotal(globalAtomicGrade);
    return 0;
}