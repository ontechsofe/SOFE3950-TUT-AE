#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NAME_BUFFER 256

typedef void* Student;

typedef struct student_st {
    char name[NAME_BUFFER];
    int student_id;
    float grade;
} student_t;

void printStudent(Student _student) {
    student_t* student = (student_t*) _student;
    printf("%s, %d, %f\n", student->name, student->student_id, student->grade);
}

Student createStudent(char name[NAME_BUFFER], int id, float grade) {
    student_t* student;
    student = (student_t*)malloc(sizeof(student_t));
    strcpy(student->name, name);
    student->student_id = id;
    student->grade = grade;
    return (Student) student;
}

void* bellcurve(Student _student) {
    printStudent(_student);
    return NULL;
}

int main() {
    int numOfStudents = 5;

    pthread_t *threadArray;
    Student* studentArray;

    threadArray = (pthread_t*)malloc(numOfStudents * sizeof(pthread_t));
    studentArray = (Student*)malloc(numOfStudents * sizeof(student_t));

    for (int i = 0; i < numOfStudents; i++) {
        char name[NAME_BUFFER]; int id; float grade;
        printf("Enter name, ID, and grade for student #%d: ", i + 1);
        scanf("%s %d %f", &name, &id, &grade);
        studentArray[i] = createStudent(name, id, grade);
    }

    for (int i = 0; i < numOfStudents; i++) {
        void* stud = (void*)studentArray[i];
        pthread_create(&threadArray[i], NULL, bellcurve, stud);
    }

    for (int i = 0; i < numOfStudents; i++) {
        pthread_join(threadArray[i], NULL);
    }

    return 0;
}