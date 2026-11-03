#!/bin/sh
# 运行每个程序，检查输出是否符合书中的说法。用法：make test
# 为了让地址可以比较，程序都在关闭了地址随机化的情况下运行（setarch -R）。

fail=0
R="setarch $(uname -m) -R"

# check 程序 输入 预期文字...
check() {
    prog=$1; input=$2; shift 2
    out=$(printf '%b' "$input" | timeout 5 $R ./"$prog" 2>&1)
    for want in "$@"; do
        case "$out" in
            *"$want"*) ;;
            *) echo "FAIL $prog [$input]: 输出中没有“$want”"; fail=1 ;;
        esac
    done
}

# check_exit 程序 输入 预期的退出状态
check_exit() {
    status=$( { printf '%b' "$2" | timeout 5 $R ./"$1" >/dev/null 2>&1; echo $?; } 2>/dev/null )
    if [ "$status" -ne "$3" ]; then
        echo "FAIL $1 [$2]: 退出状态是 $status，预期是 $3"; fail=1
    fi
}

# 取出输出中的所有地址，换成十进制，每行一个
addrs() {
    grep -o '0x[0-9a-f]*' | while read -r a; do printf '%d\n' "$a"; done
}

# 检查一列数是递增（up）还是递减（down），相邻两个数的差是否为 step（0 表示不检查）
check_order() {
    name=$1; dir=$2; step=$3
    if ! awk -v dir="$dir" -v step="$step" '
        NR > 1 {
            d = (dir == "up") ? $1 - prev : prev - $1
            if (d <= 0 || (step > 0 && d != step)) bad = 1
        }
        { prev = $1 }
        END { exit bad }'; then
        echo "FAIL $name: 地址的顺序不对（应当$dir，间隔 $step）"; fail=1
    fi
}

check addr     ""  "a 的值是 3" "b 的值是 4" "一个 int 占 4 个字节"
$R ./addr | addrs | check_order addr up 4

check layout   ""  "[heap]" "[stack]" "/layout"
$R ./layout | head -6 | addrs | check_order layout up 0

check func     ""  "add(3, 4) = 7" "add(100, 23) = 123"

check stack    ""  "main   的 m" "third  的 c"
$R ./stack | addrs | check_order stack down 0

# kernel 的输出不检查：输出到管道时 printf 会先攒在缓冲区里，程序一崩溃就丢了
check_exit kernel "" 139            # 128 + 11（SIGSEGV，段错误）

check pointer  ""  "*p = 3" "执行 *p = 10 之后，a = 10"
$R ./pointer | addrs | uniq | wc -l | grep -qx 1 || { echo "FAIL pointer: &a 和 p 应当相同"; fail=1; }

check swap     ""  "swap_wrong 之后：a = 3，b = 4" "swap 之后：      a = 4，b = 3"

check heap     "3\n90\n75\n60\n" "90" "75" "60" "平均分：75"
printf '3\n90\n75\n60\n' | $R ./heap | addrs | check_order heap up 4
check_exit heap "0\n"         1
check_exit heap "x\n"         1
# 用 AddressSanitizer 版本跑正常路径和中途出错的路径：有泄漏或越界会报错
check_exit heap_asan "3\n90\n75\n60\n" 0
check_exit heap_asan "3\n90\nx\n"      1

if [ $fail -eq 0 ]; then
    echo "全部通过"
fi
exit $fail
