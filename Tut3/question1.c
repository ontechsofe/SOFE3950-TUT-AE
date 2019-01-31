#include <stdlib.h>
#include <stdio.h>
int main(void)
{
    char* name;
    int age, height;
    printf("Please enter first name: ");
    scanf("%s", name);
    printf("Please enter age: ");
    scanf("%d", &age);
    printf("Please enter height in centimeters: ");
    scanf("%d", &height);
    printf("Hi %s, you are %d, and you are %dcm tall\n", name, age, height);
    return 0;
}   