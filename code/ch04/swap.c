#include <stdio.h>

/* 想交换两个数，但是换不了 */
void swap_wrong(int x, int y)
{
    int t = x;
    x = y;
    y = t;
}

/* 拿到的是地址，就能换了 */
void swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

int main(void)
{
    int a = 3;
    int b = 4;

    swap_wrong(a, b);
    printf("swap_wrong 之后：a = %d，b = %d\n", a, b);

    swap(&a, &b);
    printf("swap 之后：      a = %d，b = %d\n", a, b);
    return 0;
}
