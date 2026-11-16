#include <stdio.h>

/* Print a triangle of 5 rows: the well-behaved version */
int main(void)
{
    int row, i;

    for (row = 1; row <= 5; row++) {
        for (i = 0; i < row; i++) {
            putchar('*');
        }
        putchar('\n');
    }
    return 0;
}
