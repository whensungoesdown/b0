/* Three ways to print a line, each one closer to the OS */
#define _DEFAULT_SOURCE         /* needed for syscall() */
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
    const char msg2[] = "2. Hello from write\n";
    const char msg3[] = "3. Hello from syscall\n";

    /* 1. C standard library: printf first puts the text in a
     *    buffer, and hands it to the OS when there is enough */
    printf("1. Hello from printf\n");
    fflush(stdout);             /* hand it over now, don't wait */

    /* 2. POSIX function write: ask the OS to write right away.
     *    1 means "standard output". sizeof(msg2) - 1 is the
     *    number of bytes (not counting the final '\0') */
    write(1, msg2, sizeof(msg2) - 1);

    /* 3. Make the system call ourselves: SYS_write is the
     *    number of the write system call */
    syscall(SYS_write, 1, msg3, sizeof(msg3) - 1);
    return 0;
}
