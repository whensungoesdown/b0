#include <stdio.h>

/* 一个函数：接受两个整数 x 和 y，返回它们的和 */
int add(int x, int y)
{
    int sum = x + y;
    return sum;
}

int main(void)
{
    int c;

    c = add(3, 4);
    printf("add(3, 4) = %d\n", c);
    printf("add(100, 23) = %d\n", add(100, 23));
    return 0;
}
