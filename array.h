#ifndef ARRAY_H
#define ARRAY_H

extern void printArr(TYPE *arr, long len);
extern long readArr(TYPE *arr);
extern void sort(TYPE *arr, long len, int reverse);
extern void reverseArr(TYPE *arr, long len);
extern long sum(TYPE *arr, long len);
extern long min(TYPE *arr, long len);
extern long max(TYPE *arr, long len);
extern void rotate(TYPE **matrix, long rows, long cols);
extern void printMatrix(TYPE **matrix, long rows, long cols);

#endif
