#include <stdio.h>
#include <stdlib.h>

int counter = 100;      /* a global variable with an initial value */
int table[1000];        /* a global variable without an initial value */

int main(void)
{
    int local = 5;                      /* a local variable */
    int *heap = malloc(sizeof(int));    /* borrowed; int * comes later */
    FILE *maps;
    int ch;

    /* C99 does not allow turning a function address into void *,
       so we turn it into an integer first and print it with %#lx */
    printf("code       main      %#lx\n", (unsigned long)main);
    printf("read-only  \"hello\"   %p\n", (void *)"hello");
    printf("data       counter   %p\n", (void *)&counter);
    printf("BSS        table     %p\n", (void *)table);
    printf("heap       malloc    %p\n", (void *)heap);
    printf("stack      local     %p\n", (void *)&local);

    /* Now ask the operating system for this program's "memory map":
       print the file /proc/self/maps exactly as it is */
    printf("\n");
    maps = fopen("/proc/self/maps", "r");
    if (maps != NULL) {
        while ((ch = fgetc(maps)) != EOF) {
            putchar(ch);
        }
        fclose(maps);
    }

    free(heap);
    return 0;
}
