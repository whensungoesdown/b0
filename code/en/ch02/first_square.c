#include <stdio.h>

int main(void)
{
    int i;

    /* find the first positive integer whose square is greater than 2000 */
    for (i = 1; i <= 100; i++) {
        if (i * i > 2000) {
            break;
        }
    }
    printf("%d * %d = %d, which is greater than 2000\n", i, i, i * i);
    return 0;
}
