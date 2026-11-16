#!/bin/sh
# Generate the program output shown in the book, out/*.txt. Usage: make book
# The programs run in a pseudo-terminal (the script command) with address
# randomization turned off (setarch -R), so the addresses are the same every
# time. The 32-bit programs also get -3: they use only the lower 3 GB of
# addresses and see the same memory layout as on real 32-bit Linux. If you run
# them directly on your own computer, you will see different addresses.
mkdir -p out
ARCH=$(uname -m)

# The command that runs a program the way the book does
cmd() {
    case $1 in
        kernel|layout64|bits64) echo "setarch $ARCH -R ./$1" ;;
        *)                      echo "setarch i686 -3 -R ./$1" ;;
    esac
}

# Environment variables live at the very top of the stack: a few more or fewer
# of them and every address on the stack moves. To get the same addresses
# every time, the programs run with nothing but a fixed PATH.
ENV="env -i PATH=/usr/bin:/bin"

# run PROGRAM OUTPUT-FILE [INPUT-LINES...]
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

# layout: the first 6 lines are the addresses printed by the program itself; the
# rest is the memory map. To fit the page we keep only the program itself,
# [heap], the first line of libc and [stack], and only the columns
# address range, permissions and name
run layout  layout_all.txt
head -6 out/layout_all.txt > out/layout.txt
awk 'NR > 7 {
         n = split($NF, part, "/"); name = part[n]
         if (name == "layout" || name == "[heap]") print $1, $2, name
         else if (name ~ /^libc\./ && !libc) { print "..."; print $1, $2, name; libc = 1 }
         else if (name == "[stack]") { print "..."; print $1, $2, name }
     }' out/layout_all.txt > out/layout_maps.txt
rm -f out/layout_all.txt

# layout64: the 64-bit version; only the first 6 lines
run layout64 layout64_all.txt
head -6 out/layout64_all.txt > out/layout64.txt
rm -f out/layout64_all.txt

# bits: the same program, built as 32-bit and as 64-bit
{
    # The book shows only the simple commands; the real build and run options
    # are in the Makefile and in cmd above
    echo '$ gcc -m32 bits.c -o bits32'
    echo '$ ./bits32'
    $ENV $(cmd bits32)
    echo '$ gcc bits.c -o bits64'
    echo '$ ./bits64'
    $ENV $(cmd bits64)
} > out/bits.txt

# kernel: run in an interactive shell, so that we see the "Segmentation fault" printed by the shell
{
    sleep 1;   printf './kernel\n'
    sleep 1;   printf 'echo $?\n'
    sleep 0.5; printf 'exit\n'
    sleep 0.3
} | script -q -c 'env PS1="$ " bash --norc --noprofile -i' /dev/null \
  | tr -d '\r' | sed 's/\x1b\[[0-9;?]*[a-zA-Z]//g' | sed '/^\$ exit$/,$d' > out/kernel.txt
