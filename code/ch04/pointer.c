#include <stdio.h>

int main(void)
{
    int a = 3;
    int *p;             /* p 是一个指针，能装一个 int 的地址 */

    p = &a;             /* 把 a 的地址装进 p：p 指向了 a */
    printf("a  = %d，a 的地址 &a = %p\n", a, (void *)&a);
    printf("p  = %p\n", (void *)p);
    printf("*p = %d\n", *p);

    *p = 10;            /* 顺着 p 里的地址找过去，把 10 放进那个格子 */
    printf("执行 *p = 10 之后，a = %d\n", a);
    return 0;
}
