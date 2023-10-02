#pragma once

#include "matrix.h"

struct StrArray{
    char** data;
    size_t len;
};
typedef struct StrArray StrArray;
#define EMPY_STRARRAY ((StrArray){.data=0x0, .len=0})

void free_strarray(StrArray*);

matrix* eqn_to_matrix(const char* eqn);

char** list_elements(char const* eqn, size_t* count);

