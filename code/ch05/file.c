#include <stdio.h>

int main(void)
{
    FILE *fp;

    fp = fopen("diary.txt", "w");       /* 打开（没有就新建）一个文件 */
    if (fp == NULL) {
        printf("打不开文件！\n");
        return 1;
    }
    fprintf(fp, "Dear diary: today I learned C.\n");
    fclose(fp);

    printf("写好了，看看 diary.txt\n");
    return 0;
}
