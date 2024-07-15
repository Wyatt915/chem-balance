#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "matrix.h"
#include "frac.h"
#include "parse.h"
//#include <emscripten/emscripten.h>

int* chem_balance(matrix* m){
    rref(m);
    int* freecolumn   = malloc(m->r * sizeof(int));
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

void print_balanced_equation(const char* eqn, const int* coeff){
    StrArray reactants = EMPTY_STRARRAY;
    StrArray products = EMPTY_STRARRAY;
    get_reactants_products(eqn, &reactants, &products);
    size_t idx = 0;
    for (size_t i = 0; i < reactants.len - 1; i++){
        if (coeff[idx] > 1) printf("%i", coeff[idx]);
        printf("%s + ", reactants.data[i]);
        idx++;
    }
    if (coeff[idx] > 1) printf("%i", coeff[idx]);
    printf("%s = ", reactants.data[reactants.len - 1]);
    idx++;
    for (size_t i = 0; i < products.len - 1; i++){
        if (coeff[idx] > 1) printf("%i", coeff[idx]);
        printf("%s + ", products.data[i]);
        idx++;
    }
    if (coeff[idx] > 1) printf("%i", coeff[idx]);
    printf("%s\n", products.data[products.len - 1]);
    free_strarray(&reactants);
    free_strarray(&products);
}

size_t make_digit_str(char* cursor, char* buffer){
    size_t n = 0;
    do{
        buffer[n] = cursor[n];
        n++;
    }while (isdigit(cursor[n]));
    return n;
}

void print_balanced_equation_html(const char* eqn, const int* coeff){
    StrArray reactants = EMPTY_STRARRAY;
    StrArray products = EMPTY_STRARRAY;
    get_reactants_products(eqn, &reactants, &products);
    // need room for <sub></sub>. Assume there are at most 16 subscripts, allocate 16 chars for each pair of tags.
    char* result = calloc(strlen(eqn) * (reactants.len + products.len) * 16 * 16, sizeof(char));
    size_t idx = 0;
    char* res_cursor;
    char* temp_cursor;
    char* digits = calloc(32, sizeof(char));
    res_cursor = result;
    for (size_t i = 0; i < reactants.len; i++){
        temp_cursor = reactants.data[i];
        if (coeff[idx] > 1) res_cursor += sprintf(res_cursor, "%i", coeff[idx]);
        while (*temp_cursor){
            if (isdigit(*temp_cursor)){
                temp_cursor += make_digit_str(temp_cursor, digits);
                res_cursor += sprintf(res_cursor, "<sub>%s</sub>", digits);
                memset(digits, 0, sizeof(char)*32);
            }
            else *(res_cursor++) = *(temp_cursor++);
        }
        if (i < reactants.len - 1) res_cursor += sprintf(res_cursor, " + ");
        else res_cursor += sprintf(res_cursor, " = ");
        idx++;
    }
    for (size_t i = 0; i < products.len; i++){
        temp_cursor = products.data[i];
        if (coeff[idx] > 1) res_cursor += sprintf(res_cursor, "%i", coeff[idx]);
        while (*temp_cursor){
            if (isdigit(*temp_cursor)){
                temp_cursor += make_digit_str(temp_cursor, digits);
                res_cursor += sprintf(res_cursor, "<sub>%s</sub>", digits);
                memset(digits, 0, sizeof(char)*32);
            }
            else *(res_cursor++) = *(temp_cursor++);
        }
        if (i < products.len - 1) res_cursor += sprintf(res_cursor, " + ");
        idx++;
    }
    free_strarray(&reactants);
    free_strarray(&products);
    free(digits);
    printf("%s\n", result);
    free(result);
}
int main(int argc, char* argv[]){
    char* def_str = "KNO3 + C12H22O11 = N2 + CO2 + H2O + K2CO3";
    // char* def_str = "Al(OH)3 + H2SO4 = Al2(SO4)3 + H2O";
    char* eqn = def_str;
    //if (argc == 1) eqn = def_str;
    //eqn = argv[1];

    matrix* mymatrix = eqn_to_matrix(eqn);
    if (!mymatrix) return 1;

    int* coefficients = chem_balance(mymatrix);

    print_balanced_equation_html(eqn, coefficients);
    //printf("%s\n", print_balanced_equation_html(eqn, coefficients));

    free(coefficients);
    freeMatrix(mymatrix);
    // matrix* mymatrix = createMatrix(3,3);
    // int vals[] = {1,2,3,4,5,6,7,8,9};
    // loadIntValues(mymatrix, vals);
    // printMatrix(mymatrix);
    // matrix* squared = matmul(mymatrix, mymatrix);
    // matrix* t = transpose(mymatrix);
    // printMatrix(t);
    // printMatrix(squared);
    // freeMatrix(squared);
    // freeMatrix(mymatrix);
    // freeMatrix(t);

    return 0;
}


//#ifdef __cplusplus
//#define EXTERN extern "C"
//#else
//#define EXTERN
//#endif
//
//
//EXTERN EMSCRIPTEN_KEEPALIVE void dothething(int argc, char** argv) {
//    matrix* mymatrix = eqn_to_matrix(argv[0]);
//    if (!mymatrix) return;
//
//    int* coefficients = chem_balance(mymatrix);
//
//    print_balanced_equation_html(argv[0], coefficients);
//
//}
