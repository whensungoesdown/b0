#include <stdio.h>
#include "greet.h"

int main(void)
{
    greet("小明", 2);
    printf("1 + 2 + ... + 100 = %d\n", sum_to(100));
    return 0;
}
