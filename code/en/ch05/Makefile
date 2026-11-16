# Example programs for Chapter 5.
#   make         build all programs (Linux and Windows versions)
#   make test    run each program and check its output
#   make book    regenerate the outputs quoted in the book (out/*.txt)
#   make clean   delete the generated files
#
# The Windows programs are built on Linux with the MinGW cross compiler
# (sudo apt install gcc-mingw-w64-x86-64). Copy the .exe files to Windows to
# run them; in WSL you can also run them directly.
# make book also needs strace (sudo apt install strace).
CC      = gcc
MINGW  ?= x86_64-w64-mingw32-gcc
STRACE ?= strace
CFLAGS  = -std=c99 -pedantic -Wall -Wextra -Werror -g
PROGS   = use_greet layers file hello
WINPROGS = win_hello.exe win_printf.exe win_msgbox.exe

all: $(PROGS) $(WINPROGS)

# Our own library: compile greet.c into an object file, then pack it into libgreet.a with ar
greet.o: greet.c greet.h
	$(CC) $(CFLAGS) -c greet.c -o greet.o

libgreet.a: greet.o
	ar rcs libgreet.a greet.o

# Using the library: -L. means "look for libraries in this directory", -lgreet means "use libgreet.a"
use_greet: use_greet.c greet.h libgreet.a
	$(CC) $(CFLAGS) use_greet.c -L. -lgreet -o use_greet

layers: layers.c
	$(CC) $(CFLAGS) layers.c -o layers

file: file.c
	$(CC) $(CFLAGS) file.c -o file

hello: ../../ch01/hello.c
	$(CC) $(CFLAGS) ../../ch01/hello.c -o hello

win_hello.exe: win_hello.c
	$(MINGW) $(CFLAGS) win_hello.c -o win_hello.exe

win_printf.exe: win_printf.c
	$(MINGW) $(CFLAGS) win_printf.c -o win_printf.exe

# -mwindows: this program has a window, so don't open a black console window when it runs
win_msgbox.exe: win_msgbox.c
	$(MINGW) $(CFLAGS) -mwindows win_msgbox.c -o win_msgbox.exe

test: all
	sh ./test.sh

book: all
	STRACE="$(STRACE)" sh ./sessions.sh

clean:
	rm -f $(PROGS) $(WINPROGS) greet.o libgreet.a diary.txt

.PHONY: all test book clean
