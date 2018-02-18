#ifndef VARIOUS_H
#define VARIOUS_H

extern int getEndianness(void);
extern void binPrint(void *ptr, int len, int original);
extern long rnd(double number);
extern int sgn(double n);
extern long findPeak(long (*func)(long), long a, long b, int sens);
extern double solve(double (*func)(double), double a, double b, double accuracy);

#endif
