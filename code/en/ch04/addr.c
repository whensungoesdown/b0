#include <stdio.h>

int main(void)
{
    int a = 3;
    int b = 4;

    printf("a is %d and lives at %p\n", a, (void *)&a);
    printf("b is %d and lives at %p\n", b, (void *)&b);
    printf("An int takes %zu bytes\n", sizeof(int));
    return 0;
}
