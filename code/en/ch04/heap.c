#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i;
    int sum = 0;
    int *scores;

    printf("How many students? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    /* borrow a block of memory as big as n ints */
    scores = malloc(n * sizeof(int));
    if (scores == NULL) {
        printf("Out of memory!\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Score of student %d: ", i + 1);
        if (scanf("%d", scores + i) != 1) {
            free(scores);
            return 1;
        }
    }

    for (i = 0; i < n; i++) {
        printf("%p holds %d\n", (void *)(scores + i), *(scores + i));
        sum = sum + *(scores + i);
    }
    printf("Average: %d\n", sum / n);

    free(scores);       /* done with it: give it back */
    return 0;
}
