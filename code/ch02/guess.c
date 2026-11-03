#include <stdio.h>

int main(void)
{
    int secret = 42;    /* 心里想好的数 */
    int guess = 0;
    int tries = 0;

    printf("我想好了一个 1 到 100 之间的数，你来猜。\n");
    while (guess != secret) {
        printf("> ");
        if (scanf("%d", &guess) != 1) {
            printf("不玩了？好吧。\n");
            return 1;
        }
        tries = tries + 1;

        if (guess > secret) {
            printf("太大了\n");
        } else if (guess < secret) {
            printf("太小了\n");
        }
    }
    printf("猜对了！你一共猜了 %d 次。\n", tries);
    return 0;
}
