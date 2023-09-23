#include <stdio.h>
#include "matrix.h"

int main(){
    int mydata[12] = {2,1,-1,8,-3,-1,2,-11,-2,1,2,-3};
    matrix* mymatrix = createMatrix(3,4);
    loadIntValues(mymatrix, mydata);
    printMatrix(mymatrix);
    rref(mymatrix);
    printMatrix(mymatrix);
    freeMatrix(mymatrix);
    return 0;
}
