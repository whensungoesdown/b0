/* 小游戏：龙洞寻宝
 * 只用到本章介绍的东西：int 变量、算术、printf/scanf、
 * if/else、while、for 和 break */
#include <stdio.h>

int main(void)
{
    int room = 1;       /* 现在在哪里：1 洞口，2 岔路口，3 小房间，4 龙穴 */
    int choice;         /* 玩家的选择 */
    int has_key = 0;    /* 有没有钥匙：0 没有，1 有 */
    int torch = 12;     /* 火把还能烧几步 */
    int playing = 1;    /* 游戏是否还在进行：1 是，0 否 */
    int answer;
    int won = 0;
    int i;

    printf("====== 龙洞寻宝 ======\n");
    printf("传说山里的龙洞中藏着一箱金币。你举着火把，站在了洞口。\n");

    while (playing) {
        /* 第一步：描述现在的位置，列出可以做的事 */
        printf("\n（火把还能烧 %d 步）\n", torch);
        if (room == 1) {
            printf("你在洞口。洞里黑乎乎的，吹出一阵凉风。\n");
            printf("1. 走进山洞   2. 算了，回家\n");
        } else if (room == 2) {
            printf("你来到一个岔路口。左边有一扇小木门，右边传来呼噜声。\n");
            printf("1. 推开左边的小门   2. 往右走   3. 回洞口\n");
        } else if (room == 3) {
            printf("一个落满灰尘的小房间。");
            if (has_key == 0) {
                printf("墙上挂着一把生锈的钥匙。\n");
            } else {
                printf("墙上空空的。\n");
            }
            printf("1. 拿钥匙   2. 回岔路口\n");
        } else {
            printf("龙穴！一条大红龙趴在一个宝箱上，睡得正香。\n");
            printf("1. 偷偷打开宝箱   2. 叫醒龙   3. 悄悄退回岔路口\n");
        }

        /* 第二步：读入玩家的选择 */
        printf("> ");
        if (scanf("%d", &choice) != 1) {
            printf("看不懂你的选择，游戏结束。\n");
            break;
        }
        torch = torch - 1;

        /* 第三步：根据位置和选择，改变游戏的状态 */
        if (room == 1) {
            if (choice == 1) {
                room = 2;
            } else if (choice == 2) {
                printf("你回家睡了一觉。宝藏？明天再说吧。\n");
                playing = 0;
            } else {
                printf("没有这个选项。\n");
            }
        } else if (room == 2) {
            if (choice == 1) {
                room = 3;
            } else if (choice == 2) {
                room = 4;
            } else if (choice == 3) {
                room = 1;
            } else {
                printf("没有这个选项。\n");
            }
        } else if (room == 3) {
            if (choice == 1 && has_key == 0) {
                printf("你把钥匙揣进了口袋。\n");
                has_key = 1;
            } else if (choice == 1) {
                printf("钥匙已经在你口袋里了。\n");
            } else if (choice == 2) {
                room = 2;
            } else {
                printf("没有这个选项。\n");
            }
        } else {
            if (choice == 1 && has_key == 1) {
                printf("你用钥匙轻轻打开宝箱，金币闪闪发光！龙还在打呼噜。\n");
                won = 1;
                playing = 0;
            } else if (choice == 1 || choice == 2) {
                if (choice == 1) {
                    printf("宝箱是锁着的。你使劲一掰，咔嚓！龙醒了。\n");
                } else {
                    printf("你戳了戳龙的鼻子。龙睁开了一只眼睛。\n");
                }
                printf("龙说：“我只喜欢数学好的人。1 加到 100 等于几？给你 3 次机会。”\n");
                for (i = 1; i <= 3; i++) {
                    answer = 0;
                    printf("第 %d 次 > ", i);
                    if (scanf("%d", &answer) != 1) {
                        break;
                    }
                    if (answer == 5050) {
                        printf("龙说：“不错嘛！”它挪开身子，让你拿走了宝箱。\n");
                        won = 1;
                        break;
                    } else if (answer > 5050) {
                        printf("龙说：“太大了。”\n");
                    } else {
                        printf("龙说：“太小了。”\n");
                    }
                }
                if (won == 0) {
                    printf("龙打了个大哈欠，一口气把你吹出了山洞。\n");
                }
                playing = 0;
            } else if (choice == 3) {
                room = 2;
            } else {
                printf("没有这个选项。\n");
            }
        }

        /* 火把烧完了，游戏也就结束了 */
        if (playing && torch == 0) {
            printf("火把灭了……四周一片漆黑。你摸着墙爬出了山洞，什么也没带回来。\n");
            playing = 0;
        }
    }

    if (won) {
        printf("\n*** 恭喜你，满载而归！一共走了 %d 步。***\n", 12 - torch);
    } else {
        printf("\n游戏结束。下次再来吧！\n");
    }
    return 0;
}
