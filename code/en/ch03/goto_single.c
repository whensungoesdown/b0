/* Style: goto a single exit */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"

int load_file(const char *path, char **out, size_t *out_len)
{
    int   err = 0;
    FILE *fp  = NULL;           /* every resource starts out "empty" */
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

    *out = buf;                 /* hand the buffer to the caller... */
    *out_len = (size_t)size;
    buf = NULL;                 /* ...then drop it: not freed below */

out:
    free(buf);                  /* free(NULL) does nothing */
    if (fp != NULL)
        fclose(fp);
    return err;
}
