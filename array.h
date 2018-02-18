#ifndef ARRAY_H
#define ARRAY_H

extern void printArr(int *arr, long len);
extern long readArr(int *arr);
extern void sort(int *arr, long len, int reverse);
extern void reverseArr(int *arr, long len);
extern long sum(int *arr, long len);
extern long min(int *arr, long len);
extern long max(int *arr, long len);
extern void rotate(int **matrix, long rows, long cols);
extern void printMatrix(int **matrix, long rows, long cols);

#endif
