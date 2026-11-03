# 第 4 章的示例程序。
#   make         编译全部程序
#   make test    运行每个程序，检查输出是否符合书中的说法
#   make book    重新生成书中引用的运行结果（out/*.txt）
#   make clean   删除生成的文件
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g
PROGS  = addr layout func stack kernel pointer swap heap

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# heap 再用 AddressSanitizer 编译一份，检查有没有内存泄漏和越界访问
heap_asan: heap.c
	$(CC) $(CFLAGS) -fsanitize=address,undefined -o $@ $<

test: all heap_asan
	sh ./test.sh

book: all
	sh ./sessions.sh

clean:
	rm -f $(PROGS) heap_asan

.PHONY: all test book clean
