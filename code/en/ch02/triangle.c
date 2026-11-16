#include <stdio.h>

int main(void)
{
    int n, row, i;

    printf("How many rows? ");
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    for (row = 1; row <= n; row++) {
        for (i = 0; i < n - row; i++) {         /* first the spaces */
            printf(" ");
        }
        for (i = 0; i < 2 * row - 1; i++) {     /* then the stars */
            printf("*");
        }
        printf("\n");
    }
    return 0;
}
