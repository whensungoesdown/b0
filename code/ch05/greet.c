#include <stdio.h>
#include "greet.h"

void greet(const char *name, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        printf("你好，%s！\n", name);
    }
}

int sum_to(int n)
{
    int i;
    int sum = 0;

    for (i = 1; i <= n; i++) {
        sum = sum + i;
    }
    return sum;
}
