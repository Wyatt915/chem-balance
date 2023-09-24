# A note on the sign of matrix entries

Consider the following unbalanced equation
    αKNO₃ + βC₁₂H₂₂O₁₁ → ɣN₂ + δCO₂ + εH₂O + ζK₂O₃
which can be represented by the matrix
```
        α  β   ɣ  δ  ε  ζ
      +-------------------
    K | 1  0   0  0  0  2
    N | 1  0   2  0  0  0
    O | 3  11  0  2  1  3
    C | 0  12  0  1  0  1
    H | 0  22  0  0  2  0
```
where each column refers to a coefficient in the original equation and each row records the influence each coefficient
has on all the individual elements in the reaction. After row reduction, we will find that in column ζ of rows 1 and 2,
the values are negative. This is because solving the system effectively puts all species on the product side (RHS) and
hence the resulting values of α and β will be negative. To avoid this, in a reaction with n reactants and m products, we
simply make the first n columns of the matrix negative. We can equivalently make the last m columns negative to the same
effect. In reduced row echelon form, our matrix becomes
```
    1  0  0  0  0  -2
    0  1  0  0  0  -5/24
    0  0  1  0  0  -1
    0  0  0  1  0  -3/2
    0  0  0  0  1  -55/24
```
and to recover the coefficients, we make the final column positive and multiply all entries in the final column by the
greatest common denominator (24 in this case), giving the column vector [48, 5, 24, 36, 55]ᵀ. Finally, we append the GCD (24) to the vector and we have

    α = 48
    β = 5
    ɣ = 24
    δ = 36
    ε = 55
    ζ = 24
