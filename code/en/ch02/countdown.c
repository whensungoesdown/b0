#include <stdio.h>

int main(void)
{
    int n = 10;

    while (n > 0) {
        printf("%d...\n", n);
        n = n - 1;
    }
    printf("Liftoff!\n");
    return 0;
}
