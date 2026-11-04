#include <stdio.h>

int main(void)
{
    int a = 3;
    size_t n = sizeof(&a);      /* 一个地址占几个字节 */

    printf("a 住在 %p\n", (void *)&a);
    printf("一个地址占 %zu 个字节，也就是 %zu 位\n", n, n * 8);
    return 0;
}
