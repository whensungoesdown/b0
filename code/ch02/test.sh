#!/bin/sh
# 用预先准备好的输入运行每个程序，检查输出中是否有预期的文字。
# 每个程序最多运行 5 秒，防止死循环。用法：make test

fail=0

# check 程序 输入 预期文字...
check() {
    prog=$1; input=$2; shift 2
    out=$(printf '%b' "$input" | timeout 5 ./"$prog" 2>&1)
    if [ $? -eq 124 ]; then
        echo "FAIL $prog [$input]: 超时（死循环？）"; fail=1; return
    fi
    for want in "$@"; do
        case "$out" in
            *"$want"*) ;;
            *) echo "FAIL $prog [$input]: 输出中没有“$want”"; fail=1 ;;
        esac
    done
}

check var          ""        "3 + 4 = 7" "10 + 4 = 14"
check arith        ""        "17 / 5 = 3" "17 % 5 = 2" "一天有 86400 秒" "一年有 31536000 秒"
check add          "3 4\n"   "3 + 4 = 7"
check add          "x\n"     "输入的不是两个整数"
check even_odd     "8\n"     "8 是偶数"
check even_odd     "7\n"     "7 是奇数"
check even_odd     "-3\n"    "-3 是奇数"
check grade        "95\n"    "优秀"
check grade        "80\n"    "良好"
check grade        "60\n"    "及格"
check grade        "59\n"    "不及格"
check leap         "2024\n"  "2024 年是闰年"
check leap         "1900\n"  "1900 年不是闰年"
check leap         "2000\n"  "2000 年是闰年"
check leap         "2026\n"  "2026 年不是闰年"
check countdown    ""        "10..." "1..." "发射！"
check guess        "50\n25\n42\n" "太大了" "太小了" "猜对了！你一共猜了 3 次。"
check guess        ""        "不玩了"
check sum          ""        "= 5050"
check triangle     "3\n"     "  *" " ***" "*****"
check first_square ""        "45 * 45 = 2025"

# 龙洞寻宝：每一条路线都走一遍
check dragon "2\n"                 "回家睡了一觉" "游戏结束"
check dragon "1\n1\n1\n2\n2\n1\n"  "揣进了口袋" "金币闪闪发光" "一共走了 6 步"
check dragon "1\n1\n1\n1\n"        "钥匙已经在你口袋里了"
check dragon "1\n2\n2\n5000\n5050\n" "太小了" "不错嘛" "一共走了 3 步"
check dragon "1\n2\n2\n9999\n"     "太大了"
check dragon "1\n2\n1\n5050\n"     "咔嚓" "不错嘛"
check dragon "1\n2\n2\n1\n2\n3\n"  "吹出了山洞" "游戏结束"
check dragon "1\n2\n3\n"           "退回" "你来到一个岔路口"
check dragon "1\n3\n1\n3\n1\n3\n1\n3\n1\n3\n1\n3\n" "火把灭了" "游戏结束"
check dragon "1\n9\n"              "没有这个选项" "看不懂你的选择"
check dragon "abc\n"               "看不懂你的选择"
check dragon ""                    "看不懂你的选择"
check dragon "1\n2\n2\n"           "吹出了山洞"

if [ $fail -eq 0 ]; then
    echo "全部通过"
fi
exit $fail
