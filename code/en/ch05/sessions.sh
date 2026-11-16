#!/bin/sh
# Generate the outputs quoted in the book, out/*.txt. Usage: make book (needs strace and MinGW)
# The programs run in a pseudo-terminal (the script command), so they see the same as in a terminal.
mkdir -p out
STRACE=${STRACE:-strace}
SYS=/mnt/c/Windows/System32          # the Windows system directory as seen from WSL

# run output-file command: run the command in a pseudo-terminal
run() {
    file=$1; shift
    script -q -e -c "$*" /dev/null < /dev/null | tr -d '\r' > "out/$file"
}

# 1. Our own library
{
    echo '$ gcc -c greet.c -o greet.o'
    echo '$ ar rcs libgreet.a greet.o'
    echo '$ ar t libgreet.a'
    ar t libgreet.a
    echo '$ nm libgreet.a'
    nm libgreet.a | grep ' [TU] '
} > out/build_lib.txt
run use_greet.txt ./use_greet

# 2. Three ways to print, and the system calls they make
run layers.txt ./layers
run strace_layers.txt "$STRACE -o /tmp/ch05en_strace.txt -e trace=write ./layers > /dev/null; cat /tmp/ch05en_strace.txt"

# 3. How many system calls does Hello, world make?
# The header of the summary table and the total line at the end (the lines in between,
# one per system call, are too long to show)
run strace_hello_count.txt "$STRACE -c -o /tmp/ch05en_strace.txt ./hello > /dev/null; head -2 /tmp/ch05en_strace.txt; tail -1 /tmp/ch05en_strace.txt"
run strace_hello_tail.txt  "$STRACE -o /tmp/ch05en_strace.txt ./hello > /dev/null; tail -3 /tmp/ch05en_strace.txt"

# 4. Writing a file: fopen, fprintf, fclose become openat, write, close
# (keep only the part from opening diary.txt on; the system calls before that load libc.
#  Also drop the final write that prints the message)
run strace_file.txt "$STRACE -o /tmp/ch05en_strace.txt -e trace=openat,write,close ./file > /dev/null; sed -n '/diary.txt/,\$p' /tmp/ch05en_strace.txt | grep -v 'write(1,'"
rm -f diary.txt /tmp/ch05en_strace.txt

# 5. Windows versions
run win_hello.txt ./win_hello.exe

# Which functions each .exe imports from which DLL (only the few discussed in the book)
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

# The journey of WriteFile inside Windows: disassemble the real system DLLs on this computer
# rva DLL function: find the function's address in the DLL (plus ImageBase)
#   The export table has two kinds of lines: "[ 681] +base[ 689] 00161e40 Export RVA" (ordinal and address)
#   and "[ 681] +base[ 689]  02a8 NtWriteFile" (ordinal and name); match them by the ordinal in brackets
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
# dis DLL function bytes lines: disassemble the first few instructions of a function,
# keeping only the address, the machine code and the instruction
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
    echo "The instruction in KernelBase.dll!WriteFile that calls NtWriteFile:"
    objdump -p $SYS/KernelBase.dll | awk '/DLL Name/ { dll = $3 } dll == "ntdll.dll" && $NF == "NtWriteFile" { print "  (import table: " $1 " = ntdll.dll!NtWriteFile)" }'
    nt=$(objdump -p $SYS/KernelBase.dll | awk '/DLL Name/ { dll = $3 } dll == "ntdll.dll" && $NF == "NtWriteFile" { print $1 }')
    set -- $(rva $SYS/KernelBase.dll WriteFile)
    start=$(( $1 + 0x$2 ))
    target=$(printf '0x%x' $(( $1 + 0x$nt )))      # address of the NtWriteFile slot in the import table
    objdump -d -M intel --start-address=$start --stop-address=$((start + 0x400)) $SYS/KernelBase.dll \
        | grep -m1 "call.*# $target\$" | sed -e 's/^ *1800*//' -e 's/ *#.*//'
    echo
    echo "ntdll.dll!NtWriteFile:"
    dis $SYS/ntdll.dll NtWriteFile 24 8
} > out/win_chain.txt
