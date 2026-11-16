#include <stdio.h>

int main(void)
{
    int secret = 42;    /* the number I'm thinking of */
    int guess = 0;
    int tries = 0;

    printf("I'm thinking of a number between 1 and 100. Can you guess it?\n");
    while (guess != secret) {
        printf("> ");
        if (scanf("%d", &guess) != 1) {
            printf("Giving up? Oh well.\n");
            return 1;
        }
        tries = tries + 1;

        if (guess > secret) {
            printf("Too big\n");
        } else if (guess < secret) {
            printf("Too small\n");
        }
    }
    printf("You got it! It took you %d tries.\n", tries);
    return 0;
}
