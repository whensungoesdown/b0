/* 写法：goto 到单一出口 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"

int load_file(const char *path, char **out, size_t *out_len)
{
    int   err = 0;
    FILE *fp  = NULL;           /* 所有资源先设为“空” */
    char *buf = NULL;
    long  size;

    *out = NULL;
    *out_len = 0;

    fp = fopen(path, "rb");
    if (fp == NULL) {
        err = -errno;
        goto out;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        err = -errno;
        goto out;
    }

    size = ftell(fp);
    if (size < 0) {
        err = -errno;
        goto out;
    }
    rewind(fp);

    buf = malloc((size_t)size + 1);
    if (buf == NULL) {
        err = -ENOMEM;
        goto out;
    }

    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        err = -EIO;
        goto out;
    }
    buf[size] = '\0';

    *out = buf;                 /* 把缓冲区交给调用者…… */
    *out_len = (size_t)size;
    buf = NULL;                 /* ……之后置空，下面就不会释放它 */

out:
    free(buf);                  /* free(NULL) 什么也不做 */
    if (fp != NULL)
        fclose(fp);
    return err;
}
