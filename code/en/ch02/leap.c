#include <stdio.h>

int main(void)
{
    int y;

    printf("Enter a year: ");
    if (scanf("%d", &y) != 1) {
        return 1;
    }

    /* divisible by 4 but not by 100, or divisible by 400: a leap year */
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
        printf("%d is a leap year; February has 29 days\n", y);
    } else {
        printf("%d is not a leap year; February has 28 days\n", y);
    }
    return 0;
}
