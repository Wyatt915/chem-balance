#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "frac.h"

void chem_balance(matrix* m){
    int* freecolumn = malloc(m->r * sizeof(int));
    int* coefficients = malloc(m->c * sizeof(int));
    size_t count = 0;
    for (size_t r = 0; r < m->r; r++){
        if (m->data[r][m->c - 1].n == 0) break;
        freecolumn[count++] = m->data[r][m->c - 1].d;
    }
    int divisor = gcd_list(freecolumn, count);
    for (size_t r = 0; r < count; r++){
        m->data[r][m->c - 1].d /= divisor;
        freecolumn[r] /= divisor;
    }
    frac scalar = itofrac(lcm_list(freecolumn, count));
    for (size_t r = 0; r < count; r++){
        coefficients[r] = -1*fr_mul(scalar, m->data[r][m->c-1]).n;
    }
    coefficients[count] = scalar.n * divisor;
    for (size_t i = 0; i < m->c; i++){
        printf("%c=%i\t", 'a'+i, coefficients[i]);
    }
    printf("\n");
    free(coefficients);
    free(freecolumn);
}

int main(){
    // int mydata[30] = {
    //     1,  0,   0,   0,   0,   -2,
    //     1,  0,   -2,  0,   0,   0,
    //     3,  11,  0,   -2,  -1,  -3,
    //     0,  12,  0,   -1,  0,   -1,
    //     0,  22,  0,   0,   -2,  0
    // };
    // matrix* mymatrix = createMatrix(5,6);
    int mydata[12] = {
        6,0,-1,0,
        14,0,0,-2,
        0,2,-2,-1
    };
    matrix* mymatrix = createMatrix(3,4);
    loadIntValues(mymatrix, mydata);
    printMatrix(mymatrix);
    rref(mymatrix);
    printMatrix(mymatrix);
    normalize_pivots(mymatrix);
    printMatrix(mymatrix);
    chem_balance(mymatrix);
    freeMatrix(mymatrix);
    return 0;
}
