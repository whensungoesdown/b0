#include <stdio.h>

int main(void)
{
    FILE *fp;

    fp = fopen("diary.txt", "w");       /* open (or create) a file */
    if (fp == NULL) {
        printf("Cannot open the file!\n");
        return 1;
    }
    fprintf(fp, "Dear diary: today I learned C.\n");
    fclose(fp);

    printf("Done! Have a look at diary.txt\n");
    return 0;
}
