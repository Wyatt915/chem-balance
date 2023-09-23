#pragma once

#include <stddef.h>

struct frac {
    int n;
    int d;
};
typedef struct frac frac;
#define ZEROFRAC ((frac){ .n=0, .d=1 })

frac fr_add(frac a, frac b);
frac fr_sub(frac a, frac b);
frac fr_mul(frac a, frac b);
frac fr_div(frac a, frac b);
frac reciprocal(frac f);

double ftod(frac f);
frac itofrac(int n);

void simplify(frac* f);

int gcd(int a, int b);
int lcm(int a, int b);

int get_common_denom(frac* list, size_t n);
int lcm_list(int* list, size_t n);
int gcd_list(int* list, size_t n);
