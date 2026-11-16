#include <stdio.h>

int main(void)
{
    int a = 3;
    int *p;             /* p is a pointer: it holds an int's address */

    p = &a;             /* put a's address into p: p points to a */
    printf("a  = %d, the address of a is &a = %p\n", a, (void *)&a);
    printf("p  = %p\n", (void *)p);
    printf("*p = %d\n", *p);

    *p = 10;            /* follow p's address, put 10 into that box */
    printf("after *p = 10, a = %d\n", a);
    return 0;
}
