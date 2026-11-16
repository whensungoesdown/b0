#include <stdio.h>

int main(void)
{
    int seconds;

    printf("17 + 5 = %d\n", 17 + 5);
    printf("17 - 5 = %d\n", 17 - 5);
    printf("17 * 5 = %d\n", 17 * 5);
    printf("17 / 5 = %d\n", 17 / 5);    /* integer division: no fraction */
    printf("17 %% 5 = %d\n", 17 % 5);   /* the remainder; %% prints a % */

    seconds = 24 * 60 * 60;
    printf("A day has %d seconds\n", seconds);
    printf("A year has %d seconds\n", seconds * 365);
    return 0;
}
