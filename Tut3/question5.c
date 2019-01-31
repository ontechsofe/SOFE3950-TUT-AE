#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct grade{
    int student_id;
    int mark;
}grade;

void grade_students(grade *grades, int num_students){
    FILE* fp = fopen("grades.txt", "w");
    int sum = 0;
    for(int i = 0; i < num_students; i++){
        sum += grades[i].mark;
        fprintf(fp, "%d, %d\n", grades[i].student_id, grades[i].mark);
    }
    int mean = sum / num_students;
    int sd = 0;
    for(int i = 0; i < num_students; i++)
        sd += pow(grades[i].mark - mean, 2);
    sd = sqrt(sd / num_students);

    fprintf(fp, "%d, %d\n", mean, sd);
}

int main(void)
{
    char *professor;
    grade *grades;
    int num_students;

    professor = (char*)calloc(256, sizeof(char));

    printf("Please input name: ");
    scanf("%s", professor);
    printf("Please input number of students to mark: ");
    scanf("%d", &num_students);

    grades = (grade*)malloc(num_students * sizeof(grades));

    for (int i = 0; i < num_students; i++){
        printf("Please input student id and mark: ");
        scanf("%d %d", &grades[i].student_id, &grades[i].mark);
    }

    grade_students(grades, num_students);

    free(professor);
    free(grades);
}