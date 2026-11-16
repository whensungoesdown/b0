#!/bin/sh
# Run each program and check that the output matches what the book says. Usage: make test
# So that addresses can be compared, the programs run with address randomization
# turned off (setarch -R). The 32-bit programs also get -3: use only the lower
# 3 GB of addresses, just like real 32-bit Linux.

fail=0

# run PROGRAM: run a program the way the book does, for at most 5 seconds
run() {
    case $1 in
        kernel|layout64|bits64|heap_asan)
            timeout 5 setarch "$(uname -m)" -R ./"$1" ;;
        *)
            timeout 5 setarch i686 -3 -R ./"$1" ;;
    esac
}

# check PROGRAM INPUT EXPECTED-TEXT...
check() {
    prog=$1; input=$2; shift 2
    out=$(printf '%b' "$input" | run "$prog" 2>&1)
    for want in "$@"; do
        case "$out" in
            *"$want"*) ;;
            *) echo "FAIL $prog [$input]: \"$want\" not found in the output"; fail=1 ;;
        esac
    done
}

# check_exit PROGRAM INPUT EXPECTED-EXIT-STATUS
check_exit() {
    status=$( { printf '%b' "$2" | run "$1" >/dev/null 2>&1; echo $?; } 2>/dev/null )
    if [ "$status" -ne "$3" ]; then
        echo "FAIL $1 [$2]: exit status is $status, expected $3"; fail=1
    fi
}

# Take every address in the output, convert it to decimal, one per line
addrs() {
    grep -o '0x[0-9a-f]*' | while read -r a; do printf '%d\n' "$a"; done
}

# Check that a column of numbers goes up or down, and (if step is not 0)
# that neighbouring numbers differ by exactly step
check_order() {
    name=$1; dir=$2; step=$3
    if ! awk -v dir="$dir" -v step="$step" '
        NR > 1 {
            d = (dir == "up") ? $1 - prev : prev - $1
            if (d <= 0 || (step > 0 && d != step)) bad = 1
        }
        { prev = $1 }
        END { exit bad }'; then
        echo "FAIL $name: addresses in the wrong order (expected $dir, step $step)"; fail=1
    fi
}

# Check the addresses printed by a 32-bit program: at most 8 hex digits,
# and all below 0xc0000000 (user space)
check_32() {
    out=$(run "$1" 2>/dev/null | head -6)
    if echo "$out" | grep -o '0x[0-9a-f]*' | grep -q '0x[0-9a-f]\{9,\}'; then
        echo "FAIL $1: should be a 32-bit program, but printed a longer address"; fail=1
    fi
    if echo "$out" | grep -o '0x[0-9a-f]*' | grep -q '0x[c-f][0-9a-f]\{7\}'; then
        echo "FAIL $1: addresses should be below 0xc0000000"; fail=1
    fi
}
for p in addr layout stack pointer bits32; do check_32 $p; done

check addr     ""  "a is 3" "b is 4" "An int takes 4 bytes"
run addr | addrs | check_order addr up 4

check layout   ""  "[heap]" "[stack]" "/layout" "main      0x804"
run layout | head -6 | addrs | check_order layout up 0
run layout | grep -q '^08048000-.*/layout$' || { echo "FAIL layout: the program should start at 0x08048000"; fail=1; }
run layout | grep -q -- '-c0000000 .*\[stack\]$' || { echo "FAIL layout: the top of the stack should be 0xc0000000"; fail=1; }
check layout64 ""  "[heap]" "[stack]" "/layout64"
run layout64 | head -6 | addrs | check_order layout64 up 0
run layout64 | head -6 | grep -q '0x7f[0-9a-f]\{10\}' || { echo "FAIL layout64: the stack address should be a 12-digit hex number starting with 0x7f"; fail=1; }

check bits32   ""  "An address takes 4 bytes, that is 32 bits"
check bits64   ""  "An address takes 8 bytes, that is 64 bits"

check func     ""  "add(3, 4) = 7" "add(100, 23) = 123"

check stack    ""  "m in main" "c in third"
run stack | addrs | check_order stack down 0

# kernel's output is not checked: when writing to a pipe, printf keeps the text
# in a buffer first, and the buffer is lost when the program crashes
check_exit kernel "" 139            # 128 + 11 (SIGSEGV, segmentation fault)

check pointer  ""  "*p = 3" "after *p = 10, a = 10"
run pointer | addrs | uniq | wc -l | grep -qx 1 || { echo "FAIL pointer: &a and p should be the same"; fail=1; }

check swap     ""  "after swap_wrong: a = 3, b = 4" "after swap:       a = 4, b = 3"

check heap     "3\n90\n75\n60\n" "90" "75" "60" "Average: 75"
printf '3\n90\n75\n60\n' | run heap | addrs | check_order heap up 4
check_exit heap "0\n"         1
check_exit heap "x\n"         1
# Run the AddressSanitizer build on the normal path and on the early-error path:
# any leak or out-of-bounds access is reported as an error
check_exit heap_asan "3\n90\n75\n60\n" 0
check_exit heap_asan "3\n90\nx\n"      1

if [ $fail -eq 0 ]; then
    echo "All tests passed"
fi
exit $fail
