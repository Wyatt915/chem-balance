#include <stdio.h>
#include "matrix.h"

int main(){
    int mydata[18] = {
        2,-1,0,1,0,0,
        -1,2,-1,0,1,0,
        0,-1,2,0,0,1
    };
    matrix* mymatrix = createMatrix(3,6);
    loadIntValues(mymatrix, mydata);
    printMatrix(mymatrix);
    rref(mymatrix);
    printMatrix(mymatrix);
    normalize_pivots(mymatrix);
    printMatrix(mymatrix);
    freeMatrix(mymatrix);
    return 0;
}
