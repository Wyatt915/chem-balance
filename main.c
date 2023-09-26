#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "frac.h"
#include "parse.h"

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

int main(int argc, char* argv[]){
//    char* def_str = "KNO3 + C12H22O11 = N2 + CO2 + H2O + K2CO3";
    // char* def_str = "Al(OH)3 + H2SO4 = Al2(SO4)3 + H2O";
    char* def_str = "Al(OH)3 ++ ....... ++ H2SO4";
    char* eqn;
    if (argc == 1) eqn = def_str;
    else eqn = argv[1];
    printf("\n%s\n", eqn);
    matrix* mymatrix = eqn_to_matrix(eqn);
    if (!mymatrix) return 1;
    printMatrix(mymatrix);
    rref(mymatrix);
    printMatrix(mymatrix);
    chem_balance(mymatrix);
    freeMatrix(mymatrix);
    return 0;
}
