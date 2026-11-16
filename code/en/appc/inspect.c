/* Test program written for this book (not part of the original entry): use the
 * 2015 winner's match() to inspect two "warheads", one real and one fake.
 * The spectrum has 256 bins; real fissile material has peaks at bins 30 and 80. */
#include <stdio.h>
#include "match.h"

#define BINS 256

/* Add a triangular peak of the given height, centred on bin center */
static void peak(float_t *v, int center, double height)
{
    int i;

    for (i = -3; i <= 3; i++)
        v[center + i] += height * (4 - (i < 0 ? -i : i)) / 4;
}

int main(void)
{
    float_t reference[BINS] = { 0 }, real[BINS] = { 0 }, fake[BINS] = { 0 };

    peak(reference, 30, 1000);     /* the reference spectrum */
    peak(reference, 80, 600);

    peak(real, 30, 1000);          /* real warhead: same as the reference */
    peak(real, 80, 600);

    peak(fake, 30, 30);            /* fake warhead: only 3% of the material... */
    peak(fake, 80, 18);
    peak(fake, 200, 20000);        /* ...plus a strong, unrelated peak in the second half */

    printf("Real warhead: %s\n", match(real, reference, BINS, 0.6) ? "PASS" : "FAIL");
    printf("Fake warhead: %s\n", match(fake, reference, BINS, 0.6) ? "PASS" : "FAIL");
    return 0;
}
