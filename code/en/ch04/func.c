#include <stdio.h>

/* A function: takes two integers x and y and returns their sum */
int add(int x, int y)
{
    int sum = x + y;
    return sum;
}

int main(void)
{
    int c;

    c = add(3, 4);
    printf("add(3, 4) = %d\n", c);
    printf("add(100, 23) = %d\n", add(100, 23));
    return 0;
}
