#pragma once

#include "matrix.h"

matrix* eqn_to_matrix(const char* eqn);

char** list_elements(char const* eqn, size_t* count);

