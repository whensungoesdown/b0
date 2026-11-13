/* 本书写的测试程序（不是原作）：用 2015 年冠军的 match() 检查一真一假两个“弹头”。
 * 能谱有 256 个计数格；真的核材料在第 30 格和第 80 格各有一个峰。 */
#include <stdio.h>
#include "match.h"

#define BINS 256

/* 在第 center 格放一个高 height 的三角形的峰 */
static void peak(float_t *v, int center, double height)
{
    int i;

    for (i = -3; i <= 3; i++)
        v[center + i] += height * (4 - (i < 0 ? -i : i)) / 4;
}

int main(void)
{
    float_t reference[BINS] = { 0 }, real[BINS] = { 0 }, fake[BINS] = { 0 };

    peak(reference, 30, 1000);     /* 标准能谱 */
    peak(reference, 80, 600);

    peak(real, 30, 1000);          /* 真弹头：和标准一模一样 */
    peak(real, 80, 600);

    peak(fake, 30, 30);            /* 假弹头：只有标准的 3% 的核材料…… */
    peak(fake, 80, 18);
    peak(fake, 200, 20000);        /* ……再在后一半放一个很强的、完全不相干的峰 */

    printf("真弹头：%s\n", match(real, reference, BINS, 0.6) ? "通过" : "不通过");
    printf("假弹头：%s\n", match(fake, reference, BINS, 0.6) ? "通过" : "不通过");
    return 0;
}
