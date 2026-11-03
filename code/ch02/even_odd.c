#include <stdio.h>

int main(void)
{
    int n;

    printf("请输入一个整数：");
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    if (n % 2 == 0) {
        printf("%d 是偶数\n", n);
    } else {
        printf("%d 是奇数\n", n);
    }
    return 0;
}
