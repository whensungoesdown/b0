#include <stdio.h>

/* Count the ballots: 1 is a vote for the cat, 2 for the dog */
int main(void)
{
    int ballots[] = { 1, 2, 2, 1, 2, 2, 1, 2 };
    int n = sizeof(ballots) / sizeof(ballots[0]);
    int cat = 0, dog = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (ballots[i] = 1)    /* a vote for the cat? */
            cat++;
        else
            dog++;
    }
    printf("Cat: %d votes, dog: %d votes\n", cat, dog);
    return 0;
}
