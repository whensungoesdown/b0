#!/bin/sh
# Generate the program output shown in the book, out/*.txt. Usage: make book
# Interactive programs run inside a pseudo-terminal (the script command), and the
# input is "typed" slowly, one line at a time, so that it appears right after the
# prompt, just as it does on the screen.
mkdir -p out

# run program output-file [input lines...]
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
