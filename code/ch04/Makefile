# 第 4 章的示例程序。
#   make         编译全部程序
#   make test    运行每个程序，检查输出是否符合书中的说法
#   make book    重新生成书中引用的运行结果（out/*.txt）
#   make clean   删除生成的文件
#
# 书中只写最简单的命令（gcc -m32 addr.c -o addr），这里多加的选项是为了书中的结果：
#   -std=c99 -pedantic   检查代码符合 C99 标准
#   -Wall -Wextra -Werror 打开警告，有警告就编译不过去
#   -m32                 编译成 32 位（需要安装 gcc-multilib），地址短一些、好读一些
#   -no-pie              让程序的代码装在 32 位 Linux 的传统位置 0x08048000
# 运行时用 setarch i686 -3 -R（见 test.sh 和 sessions.sh）：
#   -3 只用下面 3 GB 的地址，和真正的 32 位 Linux 一样；-R 关闭地址随机化。
# 下面三个是 64 位的：kernel（要访问 64 位的内核地址）、layout64 和 bits64（和 32 位对比）。
CC     = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -g
M32    = -m32 -no-pie
PROGS  = addr layout func stack pointer swap heap bits32 kernel layout64 bits64

all: $(PROGS)

# 默认编译成 32 位
%: %.c
	$(CC) $(M32) $(CFLAGS) -o $@ $<

bits32: bits.c
	$(CC) $(M32) $(CFLAGS) -o $@ $<

# 64 位的程序
kernel: kernel.c
	$(CC) $(CFLAGS) -o $@ $<

layout64: layout.c
	$(CC) $(CFLAGS) -o $@ $<

bits64: bits.c
	$(CC) $(CFLAGS) -o $@ $<

# heap 再用 AddressSanitizer 编译一份，检查有没有内存泄漏和越界访问
heap_asan: heap.c
	$(CC) $(M32) $(CFLAGS) -fsanitize=address,undefined -o $@ $<

test: all heap_asan
	sh ./test.sh

book: all
	sh ./sessions.sh

clean:
	rm -f $(PROGS) heap_asan

.PHONY: all test book clean
