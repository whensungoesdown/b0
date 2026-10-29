#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <stddef.h>

/*
 * 把 path 指向的文件全部读入新分配的内存。
 * 成功：返回 0，*out 指向以 '\0' 结尾的缓冲区（调用者负责 free），*out_len 为文件长度。
 * 失败：返回负的错误码（如 -ENOENT），*out 为 NULL，*out_len 为 0。
 */
int load_file(const char *path, char **out, size_t *out_len);

#endif
