#include <stdio.h>

int main(void)
{
    int i;

    /* 找出第一个平方大于 2000 的正整数 */
    for (i = 1; i <= 100; i++) {
        if (i * i > 2000) {
            break;
        }
    }
    printf("%d * %d = %d，比 2000 大\n", i, i, i * i);
    return 0;
}
