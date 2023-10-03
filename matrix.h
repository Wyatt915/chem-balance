#pragma once
#include <stddef.h>

#include "frac.h"


struct matrix {
    size_t r;
    size_t c;
    // a 1d array is probably more performant but fuck it.
    frac** data;
};
typedef struct matrix matrix;

void printMatrix(const matrix* m);

matrix* createMatrix(size_t rows, size_t cols);
void freeMatrix(matrix* m);
//fill matrix row-by-row
void loadIntValues(matrix* m, int* values);
void rref(matrix* m);
void normalize_pivots(matrix* m);
void scaleRow(matrix* m, size_t r, frac mult);

matrix* transpose(const matrix* m);

matrix* matmul(const matrix* m1, const matrix* m2);
