/* Winner of the 2008 Underhanded C Contest, by John Meacham (original).
 * The posted code left out the #includes and BUFSIZE below; this book adds them. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024

/*
 * This is a simple redactor, it accepts a plain text ppm file, a set of
 * coordinates defining a rectangle, and produces a ppm file with said
 * rectangle blacked out.
 *
 * Usage: redact in.ppm x y width height > out.ppm
 */

int
main(int argc, char *argv[])
    {
        if(argc != 6) {
            fprintf(stderr, "usage: redact in.ppm x y width height > out.ppm\n");
            exit(1);
        }

    // process command line arguments
    int rx = atoi(argv[2]), ry = atoi(argv[3]), rwidth = atoi(argv[4]), rheight = atoi(argv[5]);

    FILE *ppm = fopen(argv[1],"r");
    if(!ppm) {
        perror(argv[1]); exit(1);
    }

    //read the ppm header
    unsigned width,height,maxdepth;
    fscanf(ppm,"P3\n%u %u\n%u\n", &width, &height, &maxdepth);
    printf("P3\n%u %u\n%u\n", width, height, maxdepth);

    //current locations
    int x = 0, y = 0, ws = 0;

    //fixed buffer size to avoid overflow
    char buf[BUFSIZE], *c;

    while(fgets(buf,BUFSIZE,ppm)) {
        for(c = buf;*c;c++) {
            if(isdigit(*c)) {
                if(!ws) {   // new number, increment location.
                    ws = 1; x++;
                    if(x >= width * 3) {
                        y++; x = 0;
                    }
                }
                if(x > rx * 3 && x <= (rx + rwidth) * 3 && y > ry && y < ry + rheight)
                    putchar('0');
                else
                    putchar(*c);
            }  else {
               ws = 0;
               putchar(*c);
            }
        }
    }
    return 0;
}
