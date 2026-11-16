#include <stdio.h>

int main(void)
{
    int a = 3;
    size_t n = sizeof(&a);      /* how many bytes an address takes */

    printf("a lives at %p\n", (void *)&a);
    printf("An address takes %zu bytes, that is %zu bits\n", n, n * 8);
    return 0;
}
