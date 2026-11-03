#include <stdio.h>

int main(void)
{
    int a = 3;
    int b = 4;

    printf("a 的值是 %d，住在 %p\n", a, (void *)&a);
    printf("b 的值是 %d，住在 %p\n", b, (void *)&b);
    printf("一个 int 占 %zu 个字节\n", sizeof(int));
    return 0;
}
