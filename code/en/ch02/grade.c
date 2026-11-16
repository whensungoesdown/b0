#include <stdio.h>

int main(void)
{
    int score;

    printf("What was your score? ");
    if (scanf("%d", &score) != 1) {
        return 1;
    }

    if (score >= 90) {
        printf("Excellent! Extra ice cream tonight.\n");
    } else if (score >= 75) {
        printf("Good. Keep it up.\n");
    } else if (score >= 60) {
        printf("A pass. Phew, that was close.\n");
    } else {
        printf("A fail... no cartoons tonight.\n");
    }
    return 0;
}
