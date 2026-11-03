#include <stdio.h>

int main(void)
{
    int a, b;

    printf("请输入两个整数，用空格隔开：");
    if (scanf("%d %d", &a, &b) != 2) {
        printf("输入的不是两个整数。\n");
        return 1;
    }
    printf("%d + %d = %d\n", a, b, a + b);
    return 0;
}
