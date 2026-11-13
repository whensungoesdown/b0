#include <stdio.h>

/* 数选票：1 投给猫，2 投给狗 */
int main(void)
{
    int ballots[] = { 1, 2, 2, 1, 2, 2, 1, 2 };
    int n = sizeof(ballots) / sizeof(ballots[0]);
    int cat = 0, dog = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (ballots[i] = 1)    /* 是投给猫的票吗？ */
            cat++;
        else
            dog++;
    }
    printf("猫：%d 票，狗：%d 票\n", cat, dog);
    return 0;
}
