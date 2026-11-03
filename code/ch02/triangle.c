#include <stdio.h>

int main(void)
{
    int n, row, i;

    printf("三角形要几行？");
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    for (row = 1; row <= n; row++) {
        for (i = 0; i < n - row; i++) {         /* 先打印空格 */
            printf(" ");
        }
        for (i = 0; i < 2 * row - 1; i++) {     /* 再打印星号 */
            printf("*");
        }
        printf("\n");
    }
    return 0;
}
