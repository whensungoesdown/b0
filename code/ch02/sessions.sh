#!/bin/sh
# 生成书中的运行结果 out/*.txt。用法：make book
# 交互的程序放在伪终端（script 命令）里运行，并且一行一行慢慢地“敲”进输入，
# 这样输入的内容会像在屏幕上一样，出现在提示文字的后面。
mkdir -p out

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
    } | script -q -e -c "./$prog" /dev/null | tr -d '\r' > "out/$file"
}

run var          var.txt
run arith        arith.txt
run add          add.txt          "3 4"
run even_odd     even_odd.txt     "7"
run grade        grade.txt        "59"
run leap         leap.txt         "2024"
run countdown    countdown.txt
run guess        guess.txt        "50" "25" "37" "43" "40" "42"
run sum          sum.txt
run triangle     triangle.txt     "5"
run first_square first_square.txt
run dragon       dragon.txt       "1" "2" "2" "5000" "5050"
