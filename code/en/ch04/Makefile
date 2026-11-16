# Example programs for Chapter 4.
#   make         build all programs
#   make test    run each program and check that the output matches the book
#   make book    regenerate the program output quoted in the book (out/*.txt)
#   make clean   delete the generated files
#
# The book only shows the simplest command (gcc -m32 addr.c -o addr). The extra
# options here are for the results in the book:
#   -std=c99 -pedantic    check that the code follows the C99 standard
#   -Wall -Wextra -Werror turn on warnings; any warning stops the build
#   -m32                  build a 32-bit program (needs gcc-multilib): shorter, easier addresses
#   -no-pie               load the program's code at 0x08048000, the classic place on 32-bit Linux
# At run time we use setarch i686 -3 -R (see test.sh and sessions.sh):
#   -3 uses only the lower 3 GB of addresses, just like real 32-bit Linux;
#   -R turns off address randomization.
# These three are 64-bit: kernel (it touches a 64-bit kernel address), and
# layout64 and bits64 (to compare with the 32-bit versions).
CC     = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -g
M32    = -m32 -no-pie
PROGS  = addr layout func stack pointer swap heap bits32 kernel layout64 bits64

all: $(PROGS)

# 32-bit by default
%: %.c
	$(CC) $(M32) $(CFLAGS) -o $@ $<

bits32: bits.c
	$(CC) $(M32) $(CFLAGS) -o $@ $<

# 64-bit programs
kernel: kernel.c
	$(CC) $(CFLAGS) -o $@ $<

layout64: layout.c
	$(CC) $(CFLAGS) -o $@ $<

bits64: bits.c
	$(CC) $(CFLAGS) -o $@ $<

# Build heap once more with AddressSanitizer, to catch memory leaks and out-of-bounds access
heap_asan: heap.c
	$(CC) $(M32) $(CFLAGS) -fsanitize=address,undefined -o $@ $<

test: all heap_asan
	sh ./test.sh

book: all
	sh ./sessions.sh

clean:
	rm -f $(PROGS) heap_asan

.PHONY: all test book clean
