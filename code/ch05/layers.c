/* 打印一行字的三种方法：从高到低，一层比一层更接近操作系统 */
#define _DEFAULT_SOURCE         /* 使用 syscall() 需要这一行 */
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
    const char msg2[] = "2. Hello from write\n";
    const char msg3[] = "3. Hello from syscall\n";

    /* 1. C 标准库：printf 先把字放进缓冲区，攒够了再交给操作系统 */
    printf("1. Hello from printf\n");
    fflush(stdout);             /* 现在就交出去，不要再攒了 */

    /* 2. POSIX 函数 write：直接请操作系统写。1 表示“标准输出”，
     *    sizeof(msg2) - 1 是字符串的字节数（不算最后的 '\0'） */
    write(1, msg2, sizeof(msg2) - 1);

    /* 3. 直接发起系统调用：SYS_write 是 write 系统调用的编号 */
    syscall(SYS_write, 1, msg3, sizeof(msg3) - 1);
    return 0;
}
