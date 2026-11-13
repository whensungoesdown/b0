# 附录 C 的示例程序。
#   make         编译全部程序
#   make test    检查每个程序的输出
#   make book    重新生成书中引用的运行结果（out/*.txt）
#   make clean   删除生成的文件
#
# 注意：这里故意不加 -Wall。admin.c 和 vote.c 里藏着的“坏心眼”，-Wall 一眼就能看穿，见附录 C。
# anonymous.c 是 1984 年第一届 IOCCC 的作品，是老式的 C 语言，要按 32 位、gnu89 编译。
CC     = gcc
CFLAGS = -std=c99 -pedantic
PROGS  = triangle_good triangle_evil admin vote anonymous redact inspect

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# 2008 年冠军（原作）：用 -w 编译，原作有一些无关紧要的警告
redact: redact.c
	$(CC) $(CFLAGS) -w -o $@ $<

# 2015 年冠军（原作）：match.c 和 spectral_contrast.c 对 float_t 的理解不一样
inspect: inspect.c match.c spectral_contrast.c match.h
	$(CC) $(CFLAGS) -o $@ inspect.c match.c spectral_contrast.c -lm

anonymous: anonymous.c
	$(CC) -m32 -std=gnu89 -w -o $@ $<

test: all
	sh ./test.sh

book: all
	mkdir -p out
	./triangle_good > out/triangle.txt
	./admin > out/admin.txt
	./vote > out/vote.txt
	./anonymous > out/anonymous.txt
	./redact secret.ppm 0 0 4 2 > out/redacted.ppm
	./inspect > out/inspect.txt
	{ $(CC) -std=c99 -Wall -c admin.c -o /dev/null 2>&1; \
	  $(CC) -std=c99 -Wall -c vote.c -o /dev/null 2>&1; } | grep 'warning:' \
	  | sed -e 's/ \[-W[a-z-]*\]$$//' > out/wall.txt

clean:
	rm -f $(PROGS)

.PHONY: all test book clean
