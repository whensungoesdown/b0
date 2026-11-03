#!/bin/sh
# 生成书中的运行结果 out/*.txt。用法：make book
# 程序在伪终端（script 命令）中运行，并关闭地址随机化（setarch -R），
# 这样每次生成的地址都一样。在你的电脑上直接运行，看到的地址会不同。
mkdir -p out
ARCH=$(uname -m)

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
    } | script -q -e -c "setarch $ARCH -R ./$prog" /dev/null | tr -d '\r' > "out/$file"
}

run addr    addr.txt
run func    func.txt
run stack   stack.txt
run pointer pointer.txt
run swap    swap.txt
run heap    heap.txt "3" "90" "75" "60"

# layout：前 6 行是程序自己打印的地址；后面是内存地图，
# 为了排版只保留程序本身、[heap] 和 [stack] 这几行，每行只留地址范围、权限和名字
run layout  layout_all.txt
head -6 out/layout_all.txt > out/layout.txt
awk 'NR > 7 {
         n = split($NF, part, "/"); name = part[n]
         if (name == "layout" || name == "[heap]") print $1, $2, name
         else if (name == "[stack]") { print "……"; print $1, $2, name }
     }' out/layout_all.txt > out/layout_maps.txt
rm -f out/layout_all.txt

# kernel：在交互式的 shell 中运行，这样才能看到 shell 打印的 “Segmentation fault”
{
    sleep 1;   printf './kernel\n'
    sleep 1;   printf 'echo $?\n'
    sleep 0.5; printf 'exit\n'
    sleep 0.3
} | script -q -c 'env PS1="$ " bash --norc --noprofile -i' /dev/null \
  | tr -d '\r' | sed 's/\x1b\[[0-9;?]*[a-zA-Z]//g' | sed '/^\$ exit$/,$d' > out/kernel.txt
