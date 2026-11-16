#include <stdio.h>

void third(void)
{
    int c = 3;
    printf("    c in third  lives at %p\n", (void *)&c);
}

void second(void)
{
    int b = 2;
    printf("  b in second lives at %p\n", (void *)&b);
    third();
}

void first(void)
{
    int a = 1;
    printf(" a in first  lives at %p\n", (void *)&a);
    second();
}

int main(void)
{
    int m = 0;
    printf("m in main   lives at %p\n", (void *)&m);
    first();
    return 0;
}
