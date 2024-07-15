#include <stdlib.h>

#include "frac.h"


int gcd(int a, int b){
    if (b == 0) return a;
    return gcd(b, a%b);
}

int lcm(int a, int b){
    return a * b / gcd(a, b);
}

void simplify(frac* f){
    if (f->n == 0){
        f->d = 1;
        return;
    }
    if (f->d < 0){
        f->n *= -1;
        f->d *= -1;
    }
    int g = gcd(abs(f->n), abs(f->d));
    f->n /= g;
    f->d /= g;
}

frac fr_add(frac a, frac b){
    if (a.d != b.d){
        int denom = a.d;
        a.n *= b.d;
        a.d *= b.d;
        b.n *= denom;
        b.d *= denom;
    }
    // both a and b have the same denominator now
    // so might as well just use a.d WLOG
    frac result = { .n=(a.n + b.n), .d=a.d };
    simplify(&result);
    return result;
}

frac fr_sub(frac a, frac b){
    b.n *= -1;
    return fr_add(a, b);
}

frac fr_mul(frac a, frac b){
    frac result = { .n = (a.n * b.n), .d = (a.d * b.d) };
    simplify(&result);
    return result;
}
frac fr_div(frac a, frac b){
    frac result = { .n = (a.n * b.d), .d = (a.d * b.n) };
    simplify(&result);
    return result;
}

frac reciprocal(frac f){
    return (frac){ .n=f.d, .d=f.n };
}

frac itofrac(int n){
    return (frac){ .n=n, .d=1 };
}

double ftod(frac f){
    return (double)f.n / (double)f.d;
}

int get_common_denom(frac* list, size_t n){
    if (n == 1) return list[0].d;
    int a = list[0].d;
    int b = list[1].d;
    int denom_lcm = a*b / gcd(a, b);
    for (size_t i = 2; i < n; i++){
        denom_lcm *= list[i].d / gcd(denom_lcm, list[i].d);
    }
    return denom_lcm;
}

int lcm_list(int* list, size_t n){
    if (n == 1) return list[0];
    int cumulative_lcm = list[0] * list[1] / gcd(list[0], list[1]);
    for (size_t i = 2; i < n; i++){
        cumulative_lcm *= list[i] / gcd(cumulative_lcm, list[i]);
    }
    return cumulative_lcm;
}

int gcd_list(int* list, size_t n){
    if (n == 1) return list[0];
    int cumulative_gcd = gcd(list[0], list[1]);
    for (size_t i = 2; i < n; i++){
        cumulative_gcd = gcd(cumulative_gcd, list[i]);
    }
    return cumulative_gcd;
}
