# Objective function design
## Matrix library
I intended to design `NLSL` not to depend on external third party libraries.
Then I made a very simple matrix library, `NLSLmatrix.c/.h`.
It has only four API functions.

function name | brief description
--|--
NLSLmatrix_add | addition of two matrices
NLSLmatrix_sub | difference of two matrices
NLSLmatrix_mult | multiplication of two matrices
NLSLmatrix_transpose | transpose of a matrix.
NLSLmatrix_QRdecomp | QR-decomposition of a matrix
NLSLmatrix_inv | invert a matrix
NLSLmatrix_print | print a matrix in a human readable format
<br>

## Matrix objective function
The function signature of objective functions is defined in `NLSLsolver.h` as shown below.
```
typedef int (*NLSL_OBJECTIVE)(int cx, const float* x, int cy, float* y, int cp, const float* params);
```
,where  
`cx, x` : input vector of independent variables,  
`cy, y` : output vector of dependent variables,  
`cp, params` : fixed parameters of the objective function  

The sample objective function is implemented in `NLSLapp.c`. The code snippet is shown below.
```
int objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    int err = 0;
    float x_[ARRAYSIZE(xInitial)];
    memcpy(x_, x, sizeof(x_));
    NLSLmatrix_t x0 = { 2, 2, x_ };
    NLSLmatrix_t x1 = { 2, 2, x_ + 4 };
    NLSLmatrix_t x2 = { 2, 2, x_ + 8 };
    NLSLmatrix_t y0 = { 2, 2, y };
    NLSLmatrix_t y1 = { 2, 2, y + 4 };
    NLSLmatrix_t y2 = { 2, 2, y + 8 };
    NLSLmatrix_mult(&x0, &x1, &y0);
    NLSLmatrix_mult(&x1, &x2, &y1);
    NLSLmatrix_mult(&x2, &x0, &y2);
    return err;
}
```
