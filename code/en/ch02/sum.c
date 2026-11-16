#include <stdio.h>

int main(void)
{
    int i;
    int sum = 0;

    for (i = 0; i <= 100; i++) {
        sum = sum + i;
    }
    printf("0 + 1 + 2 + ... + 100 = %d\n", sum);
    return 0;
}
