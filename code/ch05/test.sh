#!/bin/sh
# 运行每个程序，检查输出。用法：make test
fail=0

# check 预期文字 命令...
check() {
    want=$1; shift
    out=$(timeout 10 "$@" 2>&1)
    case "$out" in
        *"$want"*) ;;
        *) echo "FAIL $*: 输出中没有“$want”"; fail=1 ;;
    esac
}

check "你好，小明！"                   ./use_greet
check "1 + 2 + ... + 100 = 5050"      ./use_greet
check "1. Hello from printf"                ./layers
check "2. Hello from write"                 ./layers
check "3. Hello from syscall"               ./layers
# 三行的顺序必须对
[ "$(./layers | cut -c1-2 | tr -d '\n')" = "1.2.3." ] || { echo "FAIL layers: 顺序不对"; fail=1; }

rm -f diary.txt
check "写好了"                         ./file
grep -q "Dear diary: today I learned C." diary.txt 2>/dev/null || { echo "FAIL file: diary.txt 的内容不对"; fail=1; }
rm -f diary.txt

# libgreet.a 里应当有 greet 和 sum_to 两个函数
nm libgreet.a | grep -q " T greet$"  || { echo "FAIL libgreet.a: 没有 greet";  fail=1; }
nm libgreet.a | grep -q " T sum_to$" || { echo "FAIL libgreet.a: 没有 sum_to"; fail=1; }

# Windows 版：在 WSL 中可以直接运行 .exe（不在 WSL 中就跳过）
if grep -qi microsoft /proc/version 2>/dev/null; then
    check "Hello, Win32!"                  ./win_hello.exe
    check "Hello from printf on Windows!"  ./win_printf.exe
fi
x86_64-w64-mingw32-objdump -p win_hello.exe 2>/dev/null | grep -q "WriteFile" \
    || objdump -p win_hello.exe | grep -q "WriteFile" \
    || { echo "FAIL win_hello.exe: 没有引用 WriteFile"; fail=1; }

if [ $fail -eq 0 ]; then
    echo "全部通过"
fi
exit $fail
