# 第 1 章：把 hello.c 一步一步变成可执行文件。
#   make        运行全部四步，并运行程序
#   make hey    编译并运行“先别急着合上书”一节中的 hey.c
#   make book   重新生成书中引用的摘录（hello_*.s、hello_*.objdump、hello_tail.i、hey.out）
#   make clean  删除生成的文件
CC = gcc

all: hello
	./hello

hey: hey.c
	$(CC) $< -o $@
	./hey

hello.i: hello.c            # 1. 预处理
	$(CC) -E $< -o $@

hello.s: hello.i            # 2. 编译成汇编代码
	$(CC) -S $< -o $@

hello.o: hello.s            # 3. 汇编成目标文件
	$(CC) -c $< -o $@

hello: hello.o              # 4. 链接
	$(CC) $< -o $@

book: hello.i hello.s hello.o hey
	./hey > hey.out
	n=$$(grep -n '^# [0-9]* "hello.c"$$' hello.i | tail -1 | cut -d: -f1); \
	tail -n +$$n hello.i > hello_tail.i
	awk '/^\t\.ident/ { exit } !/cfi/ && !/^\t\.(file|size)/ { print }' hello.s > hello_main.s
	objdump -d -r hello.o | sed -n '/<main>:/,$$p' > hello_main.objdump

clean:
	rm -f hello.i hello.s hello.o hello hey

.PHONY: all book clean
