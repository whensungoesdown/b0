#include <stdio.h>

/* Tries to swap two numbers, but can't */
void swap_wrong(int x, int y)
{
    int t = x;
    x = y;
    y = t;
}

/* Gets the addresses, so it can */
void swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

int main(void)
{
    int a = 3;
    int b = 4;

    swap_wrong(a, b);
    printf("after swap_wrong: a = %d, b = %d\n", a, b);

    swap(&a, &b);
    printf("after swap:       a = %d, b = %d\n", a, b);
    return 0;
}
