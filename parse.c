#include "parse.h"
#include "matrix.h"
#include "frac.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <stdio.h>

struct element {
    char* name;
    size_t count;
};

typedef struct element element;

element* count_in_species(const char* species, size_t* len){
    return 0x0;
}



char** list_elements(char const* eqn, size_t* count){
    // worst case is that we could have a single species made up entirely of
    // elements with single-character symbols.
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
    char const* p = eqn;
    char const* sym_beg;
    size_t sym_len;
    char temp[16];
    int is_in_list;
    do{
        //find next capital letter
        while (*p && (*p < 'A' || *p > 'Z')) p++;
        if (!*p) break;
        sym_beg = p++;
        // continue as long as *p is lowercase
        while (*p && (*p >= 'a' && *p <= 'z')) p++;
        sym_len = p - sym_beg < 16 ? p - sym_beg : 15; // truncate symbol to 15 chars
        memset(temp, 0, 16); // fill w/ nulls because I don't trust strncpy
        strncpy(temp, sym_beg, sym_len);
        is_in_list = (1 == 0);
        for (size_t i = 0; i < *count; i++){
            if (strcmp(temp, list[i]) == 0){
                is_in_list = (1 == 1);
                break;
            }
        }
        if (!is_in_list) { memcpy(list[(*count)++], temp, 16); }
    } while (*p);
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
    return out;
}
