#include <stdio.h>

int main(void)
{
    int score;

    printf("考了多少分？");
    if (scanf("%d", &score) != 1) {
        return 1;
    }

    if (score >= 90) {
        printf("优秀！晚上加鸡腿。\n");
    } else if (score >= 75) {
        printf("良好，继续加油。\n");
    } else if (score >= 60) {
        printf("及格了，好险。\n");
    } else {
        printf("不及格……今晚的动画片没了。\n");
    }
    return 0;
}
