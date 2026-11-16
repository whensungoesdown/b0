#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <stddef.h>

/*
 * Reads the whole file named by path into newly allocated memory.
 * Success: returns 0; *out points to a '\0'-terminated buffer (the caller
 *          must free it) and *out_len is the file length.
 * Failure: returns a negative error code (such as -ENOENT); *out is NULL
 *          and *out_len is 0.
 */
int load_file(const char *path, char **out, size_t *out_len);

#endif
