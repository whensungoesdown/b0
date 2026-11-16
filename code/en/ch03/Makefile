# Build and test the examples of Chapter 3: make test
# nt_read.c is Windows driver code; it needs the WDK and is not built here.
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Werror -g -fsanitize=address,undefined
CPPFLAGS = -D_POSIX_C_SOURCE=200809L
LDFLAGS  = -Wl,--wrap=malloc
VARIANTS = nested dup_cleanup goto_single goto_ladder

test: $(VARIANTS:%=out/test_%)
	@for v in $(VARIANTS); do ./out/test_$$v || exit 1; done

out/test_%: %.c test_main.c load_file.h | out
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $*.c test_main.c

out:
	mkdir -p out

clean:
	rm -rf out

.PHONY: test clean
