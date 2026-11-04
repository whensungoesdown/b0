# 第 5 章的示例程序。
#   make         编译全部程序（Linux 版和 Windows 版）
#   make test    运行每个程序，检查输出
#   make book    重新生成书中引用的运行结果（out/*.txt）
#   make clean   删除生成的文件
#
# Windows 版的程序用 MinGW 交叉编译器在 Linux 上编译（sudo apt install gcc-mingw-w64-x86-64），
# 生成的 .exe 可以拷到 Windows 上运行；在 WSL 中也可以直接运行。
# make book 还需要 strace（sudo apt install strace）。
CC      = gcc
MINGW  ?= x86_64-w64-mingw32-gcc
STRACE ?= strace
CFLAGS  = -std=c99 -pedantic -Wall -Wextra -Werror -g
PROGS   = use_greet layers file hello
WINPROGS = win_hello.exe win_printf.exe win_msgbox.exe

all: $(PROGS) $(WINPROGS)

# 自己做一个函数库：先把 greet.c 编译成目标文件，再用 ar 打包成 libgreet.a
greet.o: greet.c greet.h
	$(CC) $(CFLAGS) -c greet.c -o greet.o

libgreet.a: greet.o
	ar rcs libgreet.a greet.o

# 使用函数库：-L. 表示在当前目录里找库，-lgreet 表示使用 libgreet.a
use_greet: use_greet.c greet.h libgreet.a
	$(CC) $(CFLAGS) use_greet.c -L. -lgreet -o use_greet

layers: layers.c
	$(CC) $(CFLAGS) layers.c -o layers

file: file.c
	$(CC) $(CFLAGS) file.c -o file

hello: ../ch01/hello.c
	$(CC) $(CFLAGS) ../ch01/hello.c -o hello

win_hello.exe: win_hello.c
	$(MINGW) $(CFLAGS) win_hello.c -o win_hello.exe

win_printf.exe: win_printf.c
	$(MINGW) $(CFLAGS) win_printf.c -o win_printf.exe

# -mwindows：这是一个有窗口的程序，运行时不要打开黑色的命令行窗口
win_msgbox.exe: win_msgbox.c
	$(MINGW) $(CFLAGS) -mwindows win_msgbox.c -o win_msgbox.exe

test: all
	sh ./test.sh

book: all
	STRACE="$(STRACE)" sh ./sessions.sh

clean:
	rm -f $(PROGS) $(WINPROGS) greet.o libgreet.a diary.txt

.PHONY: all test book clean
