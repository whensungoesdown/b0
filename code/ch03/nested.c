/* 写法：层层嵌套（不推荐） */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"

int load_file(const char *path, char **out, size_t *out_len)
{
    int   err = 0;
    FILE *fp;
    char *buf;
    long  size;

    *out = NULL;
    *out_len = 0;

    fp = fopen(path, "rb");
    if (fp != NULL) {
        if (fseek(fp, 0, SEEK_END) == 0) {
            size = ftell(fp);
            if (size >= 0) {
                rewind(fp);
                buf = malloc((size_t)size + 1);
                if (buf != NULL) {
                    if (fread(buf, 1, (size_t)size, fp) == (size_t)size) {
                        buf[size] = '\0';
                        *out = buf;
                        *out_len = (size_t)size;
                    } else {
                        free(buf);
                        err = -EIO;
                    }
                } else {
                    err = -ENOMEM;
                }
            } else {
                err = -errno;
            }
        } else {
            err = -errno;
        }
        fclose(fp);
    } else {
        err = -errno;
    }
    return err;
}
