/* 写法：分层标签（Linux 内核风格） */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"

int load_file(const char *path, char **out, size_t *out_len)
{
    int   err;
    FILE *fp;
    char *buf;
    long  size;

    *out = NULL;
    *out_len = 0;

    fp = fopen(path, "rb");
    if (fp == NULL)
        return -errno;          /* 还没有任何资源，直接返回 */

    if (fseek(fp, 0, SEEK_END) != 0) {
        err = -errno;
        goto err_close;
    }

    size = ftell(fp);
    if (size < 0) {
        err = -errno;
        goto err_close;
    }
    rewind(fp);

    buf = malloc((size_t)size + 1);
    if (buf == NULL) {
        err = -ENOMEM;
        goto err_close;
    }

    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        err = -EIO;
        goto err_free;
    }
    buf[size] = '\0';

    fclose(fp);
    *out = buf;
    *out_len = (size_t)size;
    return 0;

err_free:
    free(buf);
err_close:
    fclose(fp);
    return err;
}
