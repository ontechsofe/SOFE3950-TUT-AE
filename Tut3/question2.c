#include <stdlib.h>
#include <stdio.h>
int main(void)
{
    int arr[9999];
    int i = 0;
    FILE *fp = fopen("./question2.txt", "r");
    while (!feof(fp)) {
       fscanf(fp,"%d",&arr[i]);
       printf("%d\n",arr[i]);
       i++;
    }
    fclose(fp);
    return 0;
}