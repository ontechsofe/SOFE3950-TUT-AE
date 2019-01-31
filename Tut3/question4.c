#include <stdlib.h>
#include <stdio.h>
int main(void)
{
    char *professor;
    int *student_ids;
    int *grades;
    int num_students;

    professor = (char*)calloc(256, sizeof(char));

    printf("Please input name: ");
    scanf("%s", professor);
    printf("Please print number of students to mark: ");
    scanf("%d", &num_students);

    student_ids = (int*)malloc(num_students * sizeof(int));
    grades = (int*)malloc(num_students * sizeof(int));

    free(professor);
    free(student_ids);
    free(grades);
}