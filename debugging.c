#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugging.h"

long min(int *arr, long len) {
	long i;
	long n = arr[0];

	for (i = 1; i < len; i++) {
		if (arr[i] < n)
			n = arr[i];
	}
	return n;
}

long max(int *arr, long len) {
    long i;
    long n = arr[0];

    for (i = 1; i < len; i++) {
        if (arr[i] > n)
            n = arr[i];
    }
    return n;
}

long sum(int *arr, long len) {
	long i;
	long s = 0;

	for (i = 0; i < len; i++) {
		s += arr[i];
	}
	return s;
}

void rotate(int **matrix, long rows, long cols) {
    int **tmp;
    long i;
    long j;
    int n;

    tmp = (int **)malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++) {
        tmp[i] = (int *)malloc(cols * sizeof(int));
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            n = matrix[j][rows-1 - i]; 
            tmp[i][j] = n;
        }
    }

    for (i = 0; i < rows; i++) {
        memcpy(matrix[i], tmp[i], cols * sizeof(int));
    }

    for (i = 0; i < rows; i++) {
        free(tmp[i]);
    }
    free(tmp);
}

void printArr(int *arr, long len) {
    long i;
    
    fprintf(stderr, "\narray:\n");
    for (i = 0; i < len; i++) {
        fprintf(stderr, "%i\n", arr[i]);
    }
    fprintf(stderr, "[end]\n\n");
}

void printMatrix(int **matrix, long rows, long cols) {
    int i;
    int j;
    fprintf(stderr, "\nmatrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fprintf(stderr, "%i", matrix[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "[end]\n\n");
}

long readArr(int *arr) {
    long len;
    long i;
    
    scanf("%li", &len);
    for (i = 0; i < len; i++) {
        scanf("%i", &arr[i]);
    }
    
    return len;
}

int cmp(const void *x, const void *y) {
    return *(int *)x - *(int *)y;
}

int rcmp(const void *x, const void *y) {
    return *(int *)y - *(int *)x;
}

void sort(int *arr, long len, int reverse) {
    qsort(arr, len, sizeof(int), (reverse) ? rcmp : cmp);
}

void reverseArr(int *arr, long len) {
    int *tmp = (int *)malloc(len * sizeof(int));
    long i;

    if (tmp == NULL)
        perror("malloc");
    
    for (i = 0; i < len; i++) {
        tmp[len - i - 1] = arr[i];
    }

    memcpy(arr, tmp, len * sizeof(int));
    free(tmp);
}
