# Example programs for Chapter 2.
#   make         compile all the programs
#   make test    run each program with prepared input and check its output
#   make book    regenerate the program output shown in the book (out/*.txt)
#   make clean   delete the generated files
CC     = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -g
PROGS  = var arith add even_odd grade leap countdown guess sum triangle first_square dragon

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	sh ./test.sh

book: all
	sh ./sessions.sh

clean:
	rm -f $(PROGS)

.PHONY: all test book clean
