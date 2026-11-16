#!/bin/sh
# Run each program and check its output. Usage: make test
fail=0

# check expected-text command...
check() {
    want=$1; shift
    out=$(timeout 10 "$@" 2>&1)
    case "$out" in
        *"$want"*) ;;
        *) echo "FAIL $*: output does not contain \"$want\""; fail=1 ;;
    esac
}

check "Hello, Sam!"                    ./use_greet
check "1 + 2 + ... + 100 = 5050"      ./use_greet
check "1. Hello from printf"                ./layers
check "2. Hello from write"                 ./layers
check "3. Hello from syscall"               ./layers
# The three lines must come in the right order
[ "$(./layers | cut -c1-2 | tr -d '\n')" = "1.2.3." ] || { echo "FAIL layers: wrong order"; fail=1; }

rm -f diary.txt
check "Done!"                          ./file
grep -q "Dear diary: today I learned C." diary.txt 2>/dev/null || { echo "FAIL file: wrong contents in diary.txt"; fail=1; }
rm -f diary.txt

# libgreet.a should contain the two functions greet and sum_to
nm libgreet.a | grep -q " T greet$"  || { echo "FAIL libgreet.a: no greet";  fail=1; }
nm libgreet.a | grep -q " T sum_to$" || { echo "FAIL libgreet.a: no sum_to"; fail=1; }

# Windows versions: in WSL the .exe files can be run directly (skipped outside WSL)
if grep -qi microsoft /proc/version 2>/dev/null; then
    check "Hello, Win32!"                  ./win_hello.exe
    check "Hello from printf on Windows!"  ./win_printf.exe
fi
x86_64-w64-mingw32-objdump -p win_hello.exe 2>/dev/null | grep -q "WriteFile" \
    || objdump -p win_hello.exe | grep -q "WriteFile" \
    || { echo "FAIL win_hello.exe: does not import WriteFile"; fail=1; }

if [ $fail -eq 0 ]; then
    echo "All tests passed"
fi
exit $fail
