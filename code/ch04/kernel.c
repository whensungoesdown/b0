#include <stdio.h>

int main(void)
{
    int *p = (int *)0xffffffff81000000;     /* 内核空间里的一个地址 */

    printf("我要往 %p 写一个数……\n", (void *)p);
    *p = 42;
    printf("写进去了！\n");                 /* 这一行永远不会执行 */
    return 0;
}
