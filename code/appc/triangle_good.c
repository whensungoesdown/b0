#include <stdio.h>

/* 打印 5 行的三角形：好好写的版本 */
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
