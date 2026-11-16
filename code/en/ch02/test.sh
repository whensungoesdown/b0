#!/bin/sh
# Run each program with prepared input and check that the output contains the expected text.
# Each program gets at most 5 seconds, in case of an infinite loop. Usage: make test

fail=0

# check program input expected-text...
check() {
    prog=$1; input=$2; shift 2
    out=$(printf '%b' "$input" | timeout 5 ./"$prog" 2>&1)
    if [ $? -eq 124 ]; then
        echo "FAIL $prog [$input]: timed out (infinite loop?)"; fail=1; return
    fi
    for want in "$@"; do
        case "$out" in
            *"$want"*) ;;
            *) echo "FAIL $prog [$input]: output does not contain \"$want\""; fail=1 ;;
        esac
    done
}

check var          ""        "3 + 4 = 7" "10 + 4 = 14"
check arith        ""        "17 / 5 = 3" "17 % 5 = 2" "A day has 86400 seconds" "A year has 31536000 seconds"
check add          "3 4\n"   "3 + 4 = 7"
check add          "x\n"     "That was not two integers"
check even_odd     "8\n"     "8 is even"
check even_odd     "7\n"     "7 is odd"
check even_odd     "-3\n"    "-3 is odd"
check grade        "95\n"    "Excellent"
check grade        "80\n"    "Good"
check grade        "60\n"    "A pass"
check grade        "59\n"    "A fail"
check leap         "2024\n"  "2024 is a leap year"
check leap         "1900\n"  "1900 is not a leap year"
check leap         "2000\n"  "2000 is a leap year"
check leap         "2026\n"  "2026 is not a leap year"
check countdown    ""        "10..." "1..." "Liftoff!"
check guess        "50\n25\n42\n" "Too big" "Too small" "You got it! It took you 3 tries."
check guess        ""        "Giving up"
check sum          ""        "= 5050"
check triangle     "3\n"     "  *" " ***" "*****"
check first_square ""        "45 * 45 = 2025"

# The Dragon's Cave: try every path through the game
check dragon "2\n"                 "You go home to bed" "Game over"
check dragon "1\n1\n1\n2\n2\n1\n"  "key in your pocket" "Gold!" "You took 6 steps"
check dragon "1\n1\n1\n1\n"        "already in your pocket"
check dragon "1\n2\n2\n5000\n5050\n" "Too small" "Not bad" "You took 3 steps"
check dragon "1\n2\n2\n9999\n"     "Too big"
check dragon "1\n2\n1\n5050\n"     "CRACK" "Not bad"
check dragon "1\n2\n2\n1\n2\n3\n"  "out of the cave" "Game over"
check dragon "1\n2\n3\n"           "Tiptoe back" "You reach a fork"
check dragon "1\n3\n1\n3\n1\n3\n1\n3\n1\n3\n1\n3\n" "torch goes out" "Game over"
check dragon "1\n9\n"              "no such choice" "don't understand your choice"
check dragon "abc\n"               "don't understand your choice"
check dragon ""                    "don't understand your choice"
check dragon "1\n2\n2\n"           "out of the cave"

if [ $fail -eq 0 ]; then
    echo "All tests passed"
fi
exit $fail
