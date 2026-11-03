#include <stdio.h>

void third(void)
{
    int c = 3;
    printf("    third  的 c 住在 %p\n", (void *)&c);
}

void second(void)
{
    int b = 2;
    printf("  second 的 b 住在 %p\n", (void *)&b);
    third();
}

void first(void)
{
    int a = 1;
    printf(" first  的 a 住在 %p\n", (void *)&a);
    second();
}

int main(void)
{
    int m = 0;
    printf("main   的 m 住在 %p\n", (void *)&m);
    first();
    return 0;
}
