/* 写法：每个出错点各自清理（不推荐） */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"

int load_file(const char *path, char **out, size_t *out_len)
{
    FILE *fp;
    char *buf;
    long  size;
    int   err;

    *out = NULL;
    *out_len = 0;

    fp = fopen(path, "rb");
    if (fp == NULL)
        return -errno;

    if (fseek(fp, 0, SEEK_END) != 0) {
        err = -errno;           /* fclose 可能改写 errno，必须先保存 */
        fclose(fp);
        return err;
    }

    size = ftell(fp);
    if (size < 0) {
        err = -errno;
        fclose(fp);
        return err;
    }
    rewind(fp);

    buf = malloc((size_t)size + 1);
    if (buf == NULL) {
        fclose(fp);
        return -ENOMEM;
    }

    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        free(buf);
        fclose(fp);
        return -EIO;
    }
    buf[size] = '\0';

    fclose(fp);
    *out = buf;
    *out_len = (size_t)size;
    return 0;
}
