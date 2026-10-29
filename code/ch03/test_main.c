/* 测试 load_file 的各种实现：在本目录运行 make test */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "load_file.h"

static int failures;
static int fail_next_malloc;

/* 链接时用 -Wl,--wrap=malloc，让被测代码中的 malloc 调用到这里，以便模拟分配失败 */
void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size)
{
    if (fail_next_malloc) {
        fail_next_malloc = 0;
        return NULL;
    }
    return __real_malloc(size);
}

static void expect(int cond, const char *what)
{
    if (!cond) {
        fprintf(stderr, "  FAIL: %s\n", what);
        failures++;
    }
}

int main(int argc, char **argv)
{
    char       *buf;
    size_t      len;
    struct stat st;
    int         err;

    (void)argc;

    /* 1. 正常读取：读本测试程序的源文件 */
    err = load_file("test_main.c", &buf, &len);
    expect(err == 0, "existing file returns 0");
    expect(stat("test_main.c", &st) == 0 && (size_t)st.st_size == len,
           "length matches file size");
    expect(buf != NULL && buf[len] == '\0', "buffer is NUL-terminated");
    free(buf);

    /* 2. 文件不存在：第一步就失败 */
    err = load_file("no-such-file", &buf, &len);
    expect(err == -ENOENT, "missing file returns -ENOENT");
    expect(buf == NULL && len == 0, "outputs cleared on failure");

    /* 3. 读目录：打开成功，后面的步骤失败，走清理路径（泄漏由 ASan 报告） */
    err = load_file(".", &buf, &len);
    expect(err < 0, "directory returns an error");
    expect(buf == NULL, "no buffer on failure");

    /* 4. 分配失败：文件已打开，malloc 返回 NULL */
    fail_next_malloc = 1;
    err = load_file("test_main.c", &buf, &len);
    expect(err == -ENOMEM, "malloc failure returns -ENOMEM");
    expect(buf == NULL, "no buffer after malloc failure");

    /* 5. 读取不足：sysfs 文件报告的长度（4096）大于实际内容，fread 读不满 */
    if (stat("/sys/class/net/lo/address", &st) == 0) {
        err = load_file("/sys/class/net/lo/address", &buf, &len);
        expect(err == -EIO, "short read returns -EIO");
        expect(buf == NULL, "no buffer after short read");
    }

    printf("%-22s %s\n", argv[0], failures ? "FAILED" : "ok");
    return failures != 0;
}
