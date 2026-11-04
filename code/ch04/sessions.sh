#!/bin/sh
# 生成书中的运行结果 out/*.txt。用法：make book
# 程序在伪终端（script 命令）中运行，并关闭地址随机化（setarch -R），
# 这样每次生成的地址都一样。32 位的程序还加上 -3，只用下面 3 GB 的地址，
# 看到的内存布局和真正的 32 位 Linux 一样。在你的电脑上直接运行，看到的地址会不同。
mkdir -p out
ARCH=$(uname -m)

# 按书中的方式运行一个程序的命令
cmd() {
    case $1 in
        kernel|layout64|bits64) echo "setarch $ARCH -R ./$1" ;;
        *)                      echo "setarch i686 -3 -R ./$1" ;;
    esac
}

# 环境变量放在栈的最顶上，环境变量多一点少一点，栈上的地址就会跟着移动。
# 为了每次生成的地址都一样，运行时只保留一个固定的 PATH。
ENV="env -i PATH=/usr/bin:/bin"

# run 程序 输出文件 [输入的行...]
run() {
    prog=$1; file=$2; shift 2
    {
        sleep 0.5
        for line in "$@"; do
            printf '%s\n' "$line"
            sleep 0.3
        done
        sleep 0.5
    } | script -q -e -c "$ENV $(cmd "$prog")" /dev/null | tr -d '\r' > "out/$file"
}

run addr    addr.txt
run func    func.txt
run stack   stack.txt
run pointer pointer.txt
run swap    swap.txt
run heap    heap.txt "3" "90" "75" "60"

# layout：前 6 行是程序自己打印的地址；后面是内存地图，为了排版只保留
# 程序本身、[heap]、libc 的第一行和 [stack]，每行只留地址范围、权限和名字
run layout  layout_all.txt
head -6 out/layout_all.txt > out/layout.txt
awk 'NR > 7 {
         n = split($NF, part, "/"); name = part[n]
         if (name == "layout" || name == "[heap]") print $1, $2, name
         else if (name ~ /^libc\./ && !libc) { print "……"; print $1, $2, name; libc = 1 }
         else if (name == "[stack]") { print "……"; print $1, $2, name }
     }' out/layout_all.txt > out/layout_maps.txt
rm -f out/layout_all.txt

# layout64：64 位的版本，只要前 6 行
run layout64 layout64_all.txt
head -6 out/layout64_all.txt > out/layout64.txt
rm -f out/layout64_all.txt

# bits：同一个程序，分别编译成 32 位和 64 位
{
    # 书中只显示简单的命令；实际的编译、运行选项见 Makefile 和上面的 cmd
    echo '$ gcc -m32 bits.c -o bits32'
    echo '$ ./bits32'
    $ENV $(cmd bits32)
    echo '$ gcc bits.c -o bits64'
    echo '$ ./bits64'
    $ENV $(cmd bits64)
} > out/bits.txt

# kernel：在交互式的 shell 中运行，这样才能看到 shell 打印的 “Segmentation fault”
{
    sleep 1;   printf './kernel\n'
    sleep 1;   printf 'echo $?\n'
    sleep 0.5; printf 'exit\n'
    sleep 0.3
} | script -q -c 'env PS1="$ " bash --norc --noprofile -i' /dev/null \
  | tr -d '\r' | sed 's/\x1b\[[0-9;?]*[a-zA-Z]//g' | sed '/^\$ exit$/,$d' > out/kernel.txt
