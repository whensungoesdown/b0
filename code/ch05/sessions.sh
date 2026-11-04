#!/bin/sh
# 生成书中的运行结果 out/*.txt。用法：make book（需要 strace 和 MinGW）
# 程序在伪终端（script 命令）中运行，看到的就和在终端里一样。
mkdir -p out
STRACE=${STRACE:-strace}
SYS=/mnt/c/Windows/System32          # WSL 中看到的 Windows 系统目录

# run 输出文件 命令：在伪终端中运行命令
run() {
    file=$1; shift
    script -q -e -c "$*" /dev/null < /dev/null | tr -d '\r' > "out/$file"
}

# 1. 自己做的函数库
{
    echo '$ gcc -c greet.c -o greet.o'
    echo '$ ar rcs libgreet.a greet.o'
    echo '$ ar t libgreet.a'
    ar t libgreet.a
    echo '$ nm libgreet.a'
    nm libgreet.a | grep ' [TU] '
} > out/build_lib.txt
run use_greet.txt ./use_greet

# 2. 三种打印方法，以及它们发出的系统调用
run layers.txt ./layers
run strace_layers.txt "$STRACE -o /tmp/ch05_strace.txt -e trace=write ./layers > /dev/null; cat /tmp/ch05_strace.txt"

# 3. Hello, world 一共发出了多少个系统调用
# 统计表的表头和最后的合计行（中间每种系统调用一行，太长，不要）
run strace_hello_count.txt "$STRACE -c -o /tmp/ch05_strace.txt ./hello > /dev/null; head -2 /tmp/ch05_strace.txt; tail -1 /tmp/ch05_strace.txt"
run strace_hello_tail.txt  "$STRACE -o /tmp/ch05_strace.txt ./hello > /dev/null; tail -3 /tmp/ch05_strace.txt"

# 4. 写文件：fopen、fprintf、fclose 变成了 openat、write、close
# （只保留从打开 diary.txt 开始的部分，前面是加载 libc 时的系统调用；去掉最后打印提示的那个 write）
run strace_file.txt "$STRACE -o /tmp/ch05_strace.txt -e trace=openat,write,close ./file > /dev/null; sed -n '/diary.txt/,\$p' /tmp/ch05_strace.txt | grep -v 'write(1,'"
rm -f diary.txt /tmp/ch05_strace.txt

# 5. Windows 版
run win_hello.txt ./win_hello.exe

# 每个 .exe 从哪些 DLL 里引用了哪些函数（只列出书中讲到的几个）
imports() {
    objdump -p "$1" | awk '/DLL Name/ { dll = $3 }
        /<none>/ && $NF ~ /^(GetStdHandle|WriteFile|puts|MessageBoxW)$/ { printf "  %-14s %s\n", dll, $NF }'
}
{
    for e in win_hello win_printf win_msgbox; do
        echo "$e.exe:"
        imports $e.exe
    done
} > out/win_imports.txt

# WriteFile 在 Windows 中的旅程：反汇编这台电脑上真正的系统 DLL
# rva DLL 函数名：查出函数在 DLL 中的地址（加上 ImageBase）
#   导出表中有两种行：“[ 681] +base[ 689] 00161e40 Export RVA”（序号和地址）
#   和“[ 681] +base[ 689]  02a8 NtWriteFile”（序号和名字），用方括号里的序号把它们对上
rva() {
    objdump -p "$1" | awk -v f="$2" '
        /^ImageBase/ { base = $2 }
        /^\t\[ *[0-9]+\] \+base/ {
            line = $0; sub(/^\t\[ */, "", line); idx = line; sub(/\].*/, "", idx)
            if ($0 ~ /Export RVA$/) { addr[idx] = $(NF - 2) }
            else if ($NF == f) { want = idx }
        }
        END { printf "0x%s %s\n", base, addr[want] }'
}
# dis DLL 函数名 字节数 行数：反汇编函数开头的几条指令，只保留地址、机器码和指令
dis() {
    set -- "$1" "$2" "$3" "$4" $(rva "$1" "$2")
    start=$(( $5 + 0x$6 ))
    objdump -d -M intel --start-address=$start --stop-address=$((start + $3)) "$1" \
        | grep -E '^ +[0-9a-f]+:' | head -"$4" | sed -e 's/^ *1800*//' -e 's/ *#.*//'
}
{
    echo "kernel32.dll!WriteFile:"
    dis $SYS/kernel32.dll WriteFile 6 1
    echo
    echo "KernelBase.dll!WriteFile 中调用 NtWriteFile 的那一条指令:"
    objdump -p $SYS/KernelBase.dll | awk '/DLL Name/ { dll = $3 } dll == "ntdll.dll" && $NF == "NtWriteFile" { print "  （导入表：" $1 " = ntdll.dll!NtWriteFile）" }'
    nt=$(objdump -p $SYS/KernelBase.dll | awk '/DLL Name/ { dll = $3 } dll == "ntdll.dll" && $NF == "NtWriteFile" { print $1 }')
    set -- $(rva $SYS/KernelBase.dll WriteFile)
    start=$(( $1 + 0x$2 ))
    target=$(printf '0x%x' $(( $1 + 0x$nt )))      # 导入表中 NtWriteFile 那一格的地址
    objdump -d -M intel --start-address=$start --stop-address=$((start + 0x400)) $SYS/KernelBase.dll \
        | grep -m1 "call.*# $target\$" | sed -e 's/^ *1800*//' -e 's/ *#.*//'
    echo
    echo "ntdll.dll!NtWriteFile:"
    dis $SYS/ntdll.dll NtWriteFile 24 8
} > out/win_chain.txt
