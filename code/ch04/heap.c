#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i;
    int sum = 0;
    int *scores;

    printf("有几个同学？");
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    /* 向系统借 n 个 int 那么大的一块内存 */
    scores = malloc(n * sizeof(int));
    if (scores == NULL) {
        printf("内存不够了！\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("第 %d 个同学的分数：", i + 1);
        if (scanf("%d", scores + i) != 1) {
            free(scores);
            return 1;
        }
    }

    for (i = 0; i < n; i++) {
        printf("%p 里放着 %d\n", (void *)(scores + i), *(scores + i));
        sum = sum + *(scores + i);
    }
    printf("平均分：%d\n", sum / n);

    free(scores);       /* 用完了，还给系统 */
    return 0;
}
