/* Tests every version of load_file: run make test in this directory */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "load_file.h"

static int failures;
static int fail_next_malloc;

/* Linked with -Wl,--wrap=malloc: calls to malloc in the code under test
   come here, so that we can fake an allocation failure */
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

    /* 1. Normal read: read this test program's own source */
    err = load_file("test_main.c", &buf, &len);
    expect(err == 0, "existing file returns 0");
    expect(stat("test_main.c", &st) == 0 && (size_t)st.st_size == len,
           "length matches file size");
    expect(buf != NULL && buf[len] == '\0', "buffer is NUL-terminated");
    free(buf);

    /* 2. Missing file: the very first step fails */
    err = load_file("no-such-file", &buf, &len);
    expect(err == -ENOENT, "missing file returns -ENOENT");
    expect(buf == NULL && len == 0, "outputs cleared on failure");

    /* 3. A directory: opening works, a later step fails and the cleanup
      path runs (ASan reports any leak) */
    err = load_file(".", &buf, &len);
    expect(err < 0, "directory returns an error");
    expect(buf == NULL, "no buffer on failure");

    /* 4. Allocation failure: the file is open, malloc returns NULL */
    fail_next_malloc = 1;
    err = load_file("test_main.c", &buf, &len);
    expect(err == -ENOMEM, "malloc failure returns -ENOMEM");
    expect(buf == NULL, "no buffer after malloc failure");

    /* 5. Short read: a sysfs file reports a size (4096) larger than its
      real contents, so fread comes up short */
    if (stat("/sys/class/net/lo/address", &st) == 0) {
        err = load_file("/sys/class/net/lo/address", &buf, &len);
        expect(err == -EIO, "short read returns -EIO");
        expect(buf == NULL, "no buffer after short read");
    }

    printf("%-22s %s\n", argv[0], failures ? "FAILED" : "ok");
    return failures != 0;
}
