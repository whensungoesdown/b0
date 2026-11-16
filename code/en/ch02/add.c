#include <stdio.h>

int main(void)
{
    int a, b;

    printf("Enter two integers, separated by a space: ");
    if (scanf("%d %d", &a, &b) != 2) {
        printf("That was not two integers.\n");
        return 1;
    }
    printf("%d + %d = %d\n", a, b, a + b);
    return 0;
}
