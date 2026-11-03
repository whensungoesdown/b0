#include <stdio.h>

int main(void)
{
    int seconds;

    printf("17 + 5 = %d\n", 17 + 5);
    printf("17 - 5 = %d\n", 17 - 5);
    printf("17 * 5 = %d\n", 17 * 5);
    printf("17 / 5 = %d\n", 17 / 5);    /* 整数除法：只要商，不要小数 */
    printf("17 %% 5 = %d\n", 17 % 5);   /* 取余数；%% 用来打印一个 % */

    seconds = 24 * 60 * 60;
    printf("一天有 %d 秒\n", seconds);
    printf("一年有 %d 秒\n", seconds * 365);
    return 0;
}
