#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "various.h"

/*enum {LITTLE_ENDIAN, BIG_ENDIAN};*/

long rnd(double n)
{
    return (n >= 0) ? (long)(n + 0.5) : (long)(n - 0.5);
}

int getEndianness(void) {
    unsigned int test = 1;
    unsigned char *ptr = (unsigned char *)&test;

    return (*ptr == 0) ? BIG_ENDIAN : LITTLE_ENDIAN;
}

void binPrint(void *ptr, int len, int original) {
    int i;
    int j;
    unsigned char byte;
    unsigned char mask;
    unsigned char bytes[16];

    for (i = 0; i < len; i++) {
        bytes[i] = ((unsigned char *)ptr)[i];
    }

    for (i = 0; i < len; i++) {
        byte = (getEndianness() == LITTLE_ENDIAN && original == 0) ? bytes[len-1 - i] : bytes[i];
        mask = 128;
        for (j = 0; j < 8; j++) {
            if ((i != 0 || j != 0) && j % 4 == 0)
                printf(" ");

            printf("%c", (byte & mask) ? '1' : '0');
            mask >>= 1;
        }
    }
    printf("\n");
}

long findPeak(long (*func)(long), long a, long b, int sens) {
    double x;

    while (b - a != 0) {
        x = (double)(a + b) / 2;

        /*fprintf(stderr, "%li %li %f\n", a, b, x);*/
        if (func(floor(x))*sens < func(floor(x) + 1)*sens)
            a = floor(x) + 1;
        else if (func(ceil(x) - 1)*sens > func(ceil(x))*sens)
            b = ceil(x) - 1;
        else
            a = b = x;
    }
    return a;
}

int sgn(double n) {
    assert(n != 0);

    return (n > 0) ? 1 : -1;
}

double solve(double (*func)(double), double a, double b, double accuracy) {
    double x;
    
    assert(func(a) != 0 && func(b) != 0);
    assert(sgn(func(a)) != sgn(func(b)));

    while (b - a > accuracy) {
        x = (double)(a + b) / 2;

        if (sgn(func(a)) != sgn(func(x))) {
            b = x;
        }
        else {
            a = x;
        }
    }

    return x;
}
