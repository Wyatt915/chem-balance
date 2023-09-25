#include "parse.h"
#include "matrix.h"
#include "frac.h"

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <stdio.h>

#define PARSE_OK 0
#define PARSE_ERROR 1

struct element {
    char* name;
    size_t count;
};

typedef struct element element;


// For use with qsort, taken directly form the man page
static int strcmpWrapper(const void * p1, const void * p2){
    return strcmp(*(const char **) p1, *(const char **) p2);
}

int get_next_element(const char* eqn, size_t* beg, size_t* end){
    const char* p = eqn;
    //find next capital letter
    while (*p && (*p < 'A' || *p > 'Z')) p++;
    if (!*p) return 0;
    *beg = p - eqn;
    p++;
    *end = *beg + 1;
    // continue as long as *p is lowercase
    while (*p && (*p >= 'a' && *p <= 'z')) { p++; (*end)++; }
    return 1;
}

int bin_search(const char** lst, size_t len, const char* key){
    if (len == 0) return -1;
    size_t a = 0;
    size_t b = len-1;
    int idx;
    int cmp;
    while (b>a){
        idx = (a+b) / 2;
        cmp = strcmp(lst[idx], key);
        if (cmp == 0) return idx;
        else if (cmp < 0) b = idx;
        else a = idx;
    }
    if (strcmp(lst[a], key) == 0) return a;
    return -1;
}

// returns the index of `key` if `key` is in str, -1 otherwise
int char_in_str(const char* str, size_t n, char key){
    const char* p = str;
    while (*p && p < str + n){
        if (*p == key) return p - str;
    }
    return -1;
}

size_t count_in_str(const char* str, char key){
    size_t count = 0;
    while (*str){
        if (*(str++) == key) count++;
    }
    return count;
}

// doesn't check for invalid parens
// TODO: make it chack for invalid parens
size_t get_matching_paren(const char* str){
    const char* p = str;
    int sum = 1;
    while (sum && *(++p)){
        switch (*p){
            case '(':
                sum++;
                break;
            case ')':
                sum--;
                break;
            default:
                break;
        }
    }
    if (sum == 0) return p - str;
    return 0;
}

// generates one column of the matrix for each species
// size_t len - length of element list (total number of elements in the reaction)
// char** elements - actual list of elements (sorted)
// int* col - the resultant column. Index corresponds to that in `elements`.
// returns PARSE_ERROR on failure, PARSE_OK on success
int count_in_species(const char* species, size_t len, const char** elements, int* col){
    size_t sym_beg, sym_end, sym_len, paren_beg, paren_end;
    const char* p = species;
    char temp[16];
    long int subscript;
    for (size_t i = 0; i < len; i++){
        col[i] = 0;
    }
    while (get_next_element(p, &sym_beg, &sym_end)){
        paren_beg = char_in_str(p, sym_end - sym_beg, '(');
        // check for opening parentheses between current element and next element
        if (paren_beg >= 0){
            paren_end = get_matching_paren(p+paren_beg);
            if (paren_end == 0) return PARSE_ERROR; //ERROR
            int* subcount = calloc(len, sizeof(int));
            // guaranteed to be enough to includd null terminator
            char* substring = calloc(paren_end - paren_beg, sizeof(char));
            for (size_t i = paren_beg + 1, j = 0; i < paren_end; i++, j++){
                substring[j] = p[i];
            }
            if(count_in_species(substring, len, elements, subcount) == 0) {
                free(substring);
                free(subcount);
                return PARSE_ERROR;
            }
            free(substring);

            errno = 0;
            subscript = *p ? strtol(p, NULL, 10) : 1;
            if (errno){
                free(subcount);
                return PARSE_ERROR;
            }
            // an unwritten subscript means 1.
            subscript = subscript == 0 ? 1 : subscript;
            for (size_t i = 0; i < len; i++){
                col[i] += subcount[i] * subscript;
            }
            free(subcount);
            p += paren_end + 1;
        }
        else {
            // truncate symbol to 15 chars
            sym_len = sym_end - sym_beg < 16 ? sym_end - sym_beg : 15;
            memset(temp, 0, 16); // fill w/ nulls because I don't trust strncpy
            strncpy(temp, p + sym_beg, sym_len);
            p += sym_end - sym_beg;
            int idx = bin_search(elements, len, temp);
            if (idx < 0) return PARSE_ERROR;
            errno = 0;
            subscript = *p ? strtol(p, NULL, 10) : 1;
            if (errno) return PARSE_ERROR;
            col[idx] += subscript;
        }
    }
    return PARSE_OK;
}

char** list_elements(const char * eqn, size_t* count){
    // worst case is that we could have a single species made up entirely of
    // elements with single-character symbols.
    printf("list_elements: %s\n", eqn);
    size_t n = strlen(eqn);
    char **list = malloc(n * sizeof(char*));
    for (size_t i = 0; i < n; i++){
        // why 16? The longest element symbols are 3 characters: Oganesson ₁₁₈Og
        // was previously given the plaseholder symbol Uuo (Ununoctium). Maybe
        // people will be weird and want to use placeholder symbols like
        // 'Unobtanium'. Looking at you, James Cameron. Anyway 16 should be
        // sufficient. Full element names could also be used since Rutherfordium
        // is the longest at 13 characters.
        list[i] = calloc(16, sizeof(char));
    }
    *count = 0;
    const char* p = eqn;
    size_t sym_beg, sym_end, sym_len;
    char temp[16];
    int is_in_list;
    while (get_next_element(p, &sym_beg, &sym_end) == 0) {
        sym_len = sym_end - sym_beg < 16 ? sym_end - sym_beg : 15; // truncate symbol to 15 chars
        memset(temp, 0, 16); // fill w/ nulls because I don't trust strncpy
        strncpy(temp, p + sym_beg, sym_len);
        p += sym_end - sym_beg;
        is_in_list = (1 == 0);
        for (size_t i = 0; i < *count; i++){
            if (strcmp(temp, list[i]) == 0){
                is_in_list = (1 == 1);
                break;
            }
        }
        if (!is_in_list) { memcpy(list[(*count)++], temp, 16); }
    }
    // shrink to fit
    char** out = malloc(*count * sizeof(char*));
    for (size_t i = 0; i < *count; i++){
        out[i] = malloc(16 * sizeof(char));
        memcpy(out[i], list[i], 16);
    }
    for (size_t i = 0; i < n; i++){
        free(list[i]);
    }
    free(list);
    // sort alphabetically
    qsort(out, *count, sizeof(char*), strcmpWrapper);
    return out;
}

int is_invalid(char** elem_r, size_t num_r, char** elem_p, size_t num_p){
    for (size_t i = 0; i < num_r; i++){
        if (bin_search(elem_p, num_p, elem_r[i]) < 0){
            fprintf(stderr, "Element %s in reactants not in products\n", elem_r[i]);
            return PARSE_ERROR;
        }
    }
    for (size_t i = 0; i < num_p; i++){
        if (bin_search(elem_r, num_r, elem_p[i]) < 0){
            fprintf(stderr, "Element %s in products not in reactants\n", elem_p[i]);
            return PARSE_ERROR;
        }
    }
    return PARSE_OK;
}

matrix* eqn_to_matrix(const char* eqn){
    size_t len = strlen(eqn);
    char* eqn_copy = calloc(len + 1, sizeof(char));
    strcpy(eqn_copy, eqn);
    char* temp;

    temp = strtok(eqn_copy, "=");
    if (!temp) return NULL;
    char* reactants = malloc((strlen(temp)+1)*sizeof(char));
    temp = strtok(NULL, "=");
    if (!temp) return NULL;
    char* products = malloc((strlen(temp)+1)*sizeof(char));

    size_t num_elem_r, num_elem_p; // count number of elements in reactants and products
    size_t num_r, num_p; //number of reactant/product species

    char** elements_r = list_elements(reactants, &num_elem_r);
    num_r = count_in_str(reactants, '+') + 1;
    char** elements_p = list_elements(products, &num_elem_p);
    num_p = count_in_str(products, '+') + 1;

    if (is_invalid(elements_r, num_elem_r, elements_p, num_elem_p)){
        free(reactants);
        free(products);
        for (size_t i = 0; i < num_elem_r; i++){
            free(elements_r[i]);
        }
        for (size_t i = 0; i < num_elem_p; i++){
            free(elements_p[i]);
        }
        free(elements_p);
        free(elements_r);
        return NULL;
    }

    //at this point we only need one element list
    for (size_t i = 0; i < num_elem_r; i++){
        free(elements_r[i]);
    }
    free(elements_r);
    char** elements = elements_p;

    char** reactant_species = malloc(num_r * sizeof(char*));
    char** product_species  = malloc(num_p * sizeof(char*));
    temp = strtok(reactants, " +");
    size_t count = 0;
    do {
        if (strlen(temp) > 0){
            reactant_species[count] = malloc((strlen(temp)+1) * sizeof(char));
            strcpy(reactant_species[count], temp);
            count++;
        }
        temp = strtok(NULL, " +");
    } while (temp);
    count = 0;
    temp = strtok(products, " +");
    do {
        if (strlen(temp) > 0){
            product_species[count] = malloc((strlen(temp)+1) * sizeof(char));
            strcpy(product_species[count], temp);
            count++;
        }
        temp = strtok(NULL, " +");
    } while (temp);

    // If we got to this point then num_elem_r == num_elem_p. The number of
    // columns is the number of reactant species plus the number of product
    // species.
    size_t num_rows = num_elem_r;
    size_t num_cols = num_p + num_r;
    matrix* eqnMatrix = createMatrix(num_rows, num_cols);
    int* column = malloc(num_rows * sizeof(int));
    for (size_t c = 0; c < num_r; c++){
        count_in_species(reactant_species[c], num_rows, elements, column);
        for (size_t r = 0; r < num_rows; r++){
            //mult by -1 only for reactants
            eqnMatrix->data[r][c] = itofrac(-1 * column[r]);
        }
    }
    for (size_t c = num_r; c < num_cols; c++){
        count_in_species(reactant_species[c-num_r], num_rows, elements, column);
        for (size_t r = 0; r < num_rows; r++){
            eqnMatrix->data[r][c] = itofrac(column[r]);
        }
    }
    return eqnMatrix;
}
