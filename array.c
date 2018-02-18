#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

long min(TYPE *arr, long len) {
	long i;
	long n = arr[0];

	for (i = 1; i < len; i++) {
		if (arr[i] < n)
			n = arr[i];
	}
	return n;
}

long max(TYPE *arr, long len) {
    long i;
    long n = arr[0];

    for (i = 1; i < len; i++) {
        if (arr[i] > n)
            n = arr[i];
    }
    return n;
}

long sum(TYPE *arr, long len) {
	long i;
	long s = 0;

	for (i = 0; i < len; i++) {
		s += arr[i];
	}
	return s;
}

void rotate(TYPE **matrix, long rows, long cols) {
    TYPE **tmp;
    long i;
    long j;
    TYPE n;

    tmp = (TYPE **)malloc(rows * sizeof(TYPE *));
    for (i = 0; i < rows; i++) {
        tmp[i] = (TYPE *)malloc(cols * sizeof(TYPE));
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            n = matrix[j][rows-1 - i]; 
            tmp[i][j] = n;
        }
    }

    for (i = 0; i < rows; i++) {
        memcpy(matrix[i], tmp[i], cols * sizeof(TYPE));
    }

    for (i = 0; i < rows; i++) {
        free(tmp[i]);
    }
    free(tmp);
}

void printArr(TYPE *arr, long len) {
    long i;
    
    fprintf(stderr, "\narray:\n");
    for (i = 0; i < len; i++) {
        fprintf(stderr, "%i\n", arr[i]);
    }
    fprintf(stderr, "[end]\n\n");
}

void printMatrix(TYPE **matrix, long rows, long cols) {
    int i;
    int j;
    fprintf(stderr, "\nmatrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fprintf(stderr, "%lli", matrix[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "[end]\n\n");
}

long readArr(TYPE *arr) {
    long len;
    long i;
    
    scanf("%li", &len);
    for (i = 0; i < len; i++) {
        scanf("%lli", &arr[i]);
    }
    
    return len;
}

int cmp(const void *x, const void *y) {
    return *(TYPE *)x - *(TYPE *)y;
}

int rcmp(const void *x, const void *y) {
    return *(TYPE *)y - *(TYPE *)x;
}

void sort(TYPE *arr, long len, int reverse) {
    qsort(arr, len, sizeof(TYPE), (reverse) ? rcmp : cmp);
}

void reverseArr(TYPE *arr, long len) {
    TYPE *tmp = (TYPE *)malloc(len * sizeof(TYPE));
    long i;

    if (tmp == NULL)
        perror("malloc");
    
    for (i = 0; i < len; i++) {
        tmp[len - i - 1] = arr[i];
    }

    memcpy(arr, tmp, len * sizeof(TYPE));
    free(tmp);
}
