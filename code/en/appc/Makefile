# Example programs for Appendix C.
#   make         build all the programs
#   make test    check the output of every program
#   make book    regenerate the program output shown in the book (out/*.txt)
#   make clean   remove the generated files
#
# Note: -Wall is left out on purpose. The "evil" tricks hidden in admin.c and vote.c
# would be spotted by -Wall at a glance; see Appendix C.
# anonymous.c is an entry from the first IOCCC in 1984, written in old-style C;
# it must be built as 32-bit gnu89.
# The original contest files (anonymous.c, match.c, match.h, spectral_contrast.c)
# are shared with the Chinese edition and live in ../../appc.
CC     = gcc
CFLAGS = -std=c99 -pedantic
ORIG   = ../../appc
PROGS  = triangle_good triangle_evil admin vote anonymous redact inspect

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# 2008 winner (original): built with -w, the original has a few harmless warnings
redact: redact.c
	$(CC) $(CFLAGS) -w -o $@ $<

# 2015 winner (original): match.c and spectral_contrast.c disagree about float_t
inspect: inspect.c $(ORIG)/match.c $(ORIG)/spectral_contrast.c $(ORIG)/match.h
	$(CC) $(CFLAGS) -I$(ORIG) -o $@ inspect.c $(ORIG)/match.c $(ORIG)/spectral_contrast.c -lm

anonymous: $(ORIG)/anonymous.c
	$(CC) -m32 -std=gnu89 -w -o $@ $<

test: all
	sh ./test.sh

book: all
	mkdir -p out
	./triangle_good > out/triangle.txt
	./admin > out/admin.txt
	./vote > out/vote.txt
	./anonymous > out/anonymous.txt
	./redact secret.ppm 0 0 4 2 > out/redacted.ppm
	./inspect > out/inspect.txt
	{ $(CC) -std=c99 -Wall -c admin.c -o /dev/null 2>&1; \
	  $(CC) -std=c99 -Wall -c vote.c -o /dev/null 2>&1; } | grep 'warning:' \
	  | sed -e 's/ \[-W[a-z-]*\]$$//' > out/wall.txt

clean:
	rm -f $(PROGS)

.PHONY: all test book clean
