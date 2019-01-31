#include <stdlib.h>
#include <stdio.h>

typedef struct STUDENT{
    int student_id;
    int age;
    int year;
} STUDENT;

void save_student(STUDENT s){
    FILE *fp = fopen("students.txt", "a");
    fprintf(fp, "%d, %d, %d\n", s.student_id, s.age, s.year);
}
int main(void)
{
    STUDENT s;
    printf("Please enter student id: ");
    scanf("%d", &s.student_id);
    printf("Please enter age: ");
    scanf("%d", &s.age);
    printf("Please enter start year at UOIT: ");
    scanf("%d", &s.year);
    save_student(s);
}