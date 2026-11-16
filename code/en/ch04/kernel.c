#include <stdio.h>

int main(void)
{
    int *p = (int *)0xffffffff81000000;     /* in kernel space */

    printf("I'm going to write a number to %p...\n", (void *)p);
    *p = 42;
    printf("Done!\n");                      /* this line never runs */
    return 0;
}
