#include <stdio.h>

int main(void)
{
    int a = 3;
    int b = 4;
    int c;

    c = a + b;
    printf("%d + %d = %d\n", a, b, c);

    a = 10;                 /* the old value in the box is thrown out */
    c = a + b;
    printf("%d + %d = %d\n", a, b, c);
    return 0;
}
