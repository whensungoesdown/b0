#include <stdio.h>
#include <stdlib.h>

int counter = 100;      /* 有初值的全局变量 */
int table[1000];        /* 没有初值的全局变量 */

int main(void)
{
    int local = 5;                          /* 局部变量 */
    int *heap = malloc(sizeof(int));        /* 向系统借来的内存（int * 是什么，后面再讲） */
    FILE *maps;
    int ch;

    printf("代码        main      %p\n", (void *)main);
    printf("只读数据    \"hello\"   %p\n", (void *)"hello");
    printf("数据        counter   %p\n", (void *)&counter);
    printf("BSS         table     %p\n", (void *)table);
    printf("堆          malloc    %p\n", (void *)heap);
    printf("栈          local     %p\n", (void *)&local);

    /* 再请操作系统给出这个程序的“内存地图”：把文件 /proc/self/maps 原样打印出来 */
    printf("\n");
    maps = fopen("/proc/self/maps", "r");
    if (maps != NULL) {
        while ((ch = fgetc(maps)) != EOF) {
            putchar(ch);
        }
        fclose(maps);
    }

    free(heap);
    return 0;
}
