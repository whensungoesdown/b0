# 第 2 章的示例程序。
#   make         编译全部程序
#   make test    用预先准备好的输入运行每个程序，检查输出是否符合预期
#   make book    重新生成书中引用的运行结果（out/*.txt）
#   make clean   删除生成的文件
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g
PROGS  = var arith add even_odd grade leap countdown guess sum triangle first_square dragon

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	sh ./test.sh

book: all
	sh ./sessions.sh

clean:
	rm -f $(PROGS)

.PHONY: all test book clean
