#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

matrix* createMatrix(size_t rows, size_t cols){
    matrix* m = malloc(sizeof(matrix));
    m->r = rows;
    m->c = cols;
    m->data = malloc(rows * sizeof(frac*));
    for (size_t i = 0; i < rows; i++){
        m->data[i] = malloc(cols*sizeof(frac));
    }
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j <cols; j++){
            m->data[i][j] = ZEROFRAC;
        }
    }
    return m;
}

void freeMatrix(matrix* m){
    for (size_t r = 0; r < m->r; r++){
            free(m->data[r]);
    }
    free(m->data);
    m->r = 0;
    m->c = 0;
    m->data = 0x0;
    free(m);
}

void printMatrix(const matrix* m){
    for (size_t r = 0; r < m->r; r++){
        for (size_t c = 0; c < m->c; c++){
            if (m->data[r][c].d == 1) printf(" %i \t", m->data[r][c].n);
            else printf("%i/%i\t", m->data[r][c].n, m->data[r][c].d);
        }
        printf("\n");
    }
    printf("\n");
}

void loadIntValues(matrix* m, int* values){
    for (size_t r = 0; r < m->r; r++){
        for (size_t c = 0; c < m->c; c++){
            m->data[r][c] = itofrac(values[r*m->c + c]);
        }
    }
}

void swapRows(matrix* m, size_t r1, size_t r2){
    frac* row = m->data[r1];
    m->data[r1] = m->data[r2];
    m->data[r2] = row;
}

// r = r + (mult * other)
void replaceRow(matrix* m, size_t r, size_t other, frac mult){
    for (size_t i = 0; i < m->c; i++){
        m->data[r][i] = fr_add(m->data[r][i], fr_mul(mult, m->data[other][i]));
    }
}

// sets pivots to 1, potentially resulting in noninteger entries
void normalize_pivots(matrix* m){
    size_t c;
    for (size_t r = 0; r < m->r; r++){
        c = 0;
        while (c < m->c && m->data[r][c].n == 0) { c++; }
        if (c == m->c) continue; // no pivots
        frac mult = reciprocal(m->data[r][c]);
        for (; c < m->c ; c++){
            m->data[r][c] = fr_mul(m->data[r][c], mult);
        }
    }
}

// Gives a matrix of whole numbers
void normalize_rows(matrix* m){
    if (m->c < 2) return; // nothing to do!
    int* numerators = malloc(m->c * sizeof(int));
    size_t count, c;
    for (size_t r = 0; r < m->r; r++){
        int com_denom = get_common_denom(m->data[r], m->c);
        for (c = 0; c < m->c; c++){
            m->data[r][c].n *= com_denom / m->data[r][c].d;
            m->data[r][c].d = 1;
        }
        count = 0;
        for (c = 0; c < m->c; c++){
            if (m->data[r][c].n == 0) continue; // ignore zeroes
            numerators[count++] = m->data[r][c].n;
        }
        int divisor = gcd_list(numerators, count);
        for (c = 0; c < m->c; c++){
            m->data[r][c].n /= divisor;
            simplify(&(m->data[r][c]));
        }
        // Now we want all pivots to be positive
        // c = 0;
        // while (c < m->c && m->data[r][c].n == 0) { c++; }
        // if (c == m->c) continue; // no pivots
        // int mult = m->data[r][c].n < 0 ? -1 : 1;
        // for (; c < m->c ; c++){
        //     m->data[r][c].n *= mult;
        // }
    }
    free(numerators);
}

void rref(matrix* m){
    size_t cols = m->c;
    size_t rows = m->r;
    size_t pivot_col = 0;
    size_t pivot_row = 0;
    // Clear columns below pivots
    for (pivot_col = 0; pivot_col < cols && pivot_row < rows; pivot_col++){
        size_t r = pivot_row;
        while (r < rows-1 && m->data[r][pivot_col].n == 0) { r++; }
        if (m->data[r][pivot_col].n == 0) continue; // no pivot in this column
        if (r != pivot_row) swapRows(m, r, pivot_row); //move first nonzero pivot to top
        // clear all values in the column below the pivot
        for (r = pivot_row + 1; r < rows ; r++){
            if (m->data[r][pivot_col].n == 0) continue;
            frac mult = fr_div(m->data[r][pivot_col], m->data[pivot_row][pivot_col]);
            mult.n *= -1;
            replaceRow(m, r, pivot_row, mult);
        }
        pivot_row++;
    }
    // Clear columns above pivots
    for (pivot_row = rows - 1; pivot_row > 0; pivot_row--){
        pivot_col = 0;
        while (pivot_col < cols && m->data[pivot_row][pivot_col].n == 0) { pivot_col++; }
        if (pivot_col >= cols) continue; //no pivot in this row
        // int instead of size_t because we are doing subtraction!!
        for (int r = pivot_row - 1; r >= 0; r--){
            if (m->data[r][pivot_col].n == 0) continue;
            frac mult = fr_div(m->data[r][pivot_col], m->data[pivot_row][pivot_col]);
            mult.n *= -1;
            replaceRow(m, r, pivot_row, mult);
        }
    }
    normalize_rows(m);
}
