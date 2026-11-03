#include <stdio.h>

int main(void)
{
    int a = 3;
    int b = 4;
    int c;

    c = a + b;
    printf("%d + %d = %d\n", a, b, c);

    a = 10;                 /* 盒子里的旧值被换掉了 */
    c = a + b;
    printf("%d + %d = %d\n", a, b, c);
    return 0;
}
