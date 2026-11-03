#include <stdio.h>

int main(void)
{
    int y;

    printf("请输入年份：");
    if (scanf("%d", &y) != 1) {
        return 1;
    }

    /* 能被 4 整除但不能被 100 整除，或者能被 400 整除，就是闰年 */
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
        printf("%d 年是闰年，二月有 29 天\n", y);
    } else {
        printf("%d 年不是闰年，二月有 28 天\n", y);
    }
    return 0;
}
