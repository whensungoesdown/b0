# 编译并测试第 3 章的示例代码：make test
# nt_read.c 是 Windows 驱动代码，需要 WDK，不在这里编译。
CFLAGS   = -std=c11 -Wall -Wextra -Werror -g -fsanitize=address,undefined
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
