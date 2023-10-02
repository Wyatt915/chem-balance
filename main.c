#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "frac.h"
#include "parse.h"

int* chem_balance(matrix* m){
    rref(m);
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

    free(freecolumn);
    return coefficients;
}

int main(int argc, char* argv[]){
    char* def_str = "KNO3 + C12H22O11 = N2 + CO2 + H2O + K2CO3";
    // char* def_str = "Al(OH)3 + H2SO4 = Al2(SO4)3 + H2O";
    //char* def_str = "Al(OH)3 ++ ....... ++ H2SO4";
    char* eqn;
    if (argc == 1) eqn = def_str;
    else eqn = argv[1];
    printf("\n%s\n", eqn);
    matrix* mymatrix = eqn_to_matrix(eqn);
    if (!mymatrix) return 1;

    int* coefficients = chem_balance(mymatrix);
    StrArray reactants = EMPY_STRARRAY;
    StrArray products = EMPY_STRARRAY;
    get_reactants_products(eqn, &reactants, &products);
    size_t idx = 0;
    for (size_t i = 0; i < reactants.len; i++){
        printf("%i\t%s\n", coefficients[idx++], reactants.data[i]);
    }
    for (size_t i = 0; i < products.len; i++){
        printf("%i\t%s\n", coefficients[idx++], products.data[i]);
    }
    free(coefficients);
    freeMatrix(mymatrix);
    free_strarray(&reactants);
    free_strarray(&products);
    return 0;
}
