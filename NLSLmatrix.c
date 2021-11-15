#include "NLSLmatrix.h"
#include "NLSLutils.h"
#include <malloc.h>
#include <assert.h>
#include <memory.h>
#include <math.h>

pNLSLmatrix_t NLSLmatrix_new(int rows, int columns)
{
    size_t dataBytes = rows * columns * sizeof(float);
    pNLSLmatrix_t p = (pNLSLmatrix_t)malloc(sizeof(NLSLmatrix_t) + dataBytes);
    p->rows = rows;
    p->columns = columns;
    NLSL_FILLFLOATS(p->elements.v, 0.0f, rows * columns);
    return p;
}

void NLSLmatrix_add(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B)
{
    assert((matA->rows == matB->rows) && (matA->columns == matB->columns));
    assert((matA->rows == matA_B->rows) && (matA->columns == matA_B->columns));
    int elementCount = matA->rows * matA->columns;
    memcpy(matA_B->elements.v, matA->elements.c, elementCount * sizeof(float));
    NLSLutils_addfloats(elementCount, matA_B->elements.v, matB->elements.c);
}

void NLSLmatrix_sub(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B)
{
    assert((matA->rows == matB->rows) && (matA->columns == matB->columns));
    assert((matA->rows == matA_B->rows) && (matA->columns == matA_B->columns));
    int elementCount = matA->rows * matA->columns;
    memcpy(matA_B->elements.v, matA->elements.c, elementCount * sizeof(float));
    NLSLutils_subfloats(elementCount, matA_B->elements.v, matB->elements.c);
}

void NLSLmatrix_mult(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B)
{
    assert(matA->columns == matB->rows);
    assert((matA->rows == matA_B->rows) && (matB->columns == matA_B->columns));
    for (int rowA_B = 0; rowA_B != matA_B->rows; rowA_B++)
    {
        for (int columnA_B = 0; columnA_B != matA_B->columns; columnA_B++)
        {
            int indexA_B = columnA_B + rowA_B * matA_B->columns;
            matA_B->elements.v[indexA_B] = 0.0f;
            for (int columnA = 0; columnA != matA->columns; columnA++)
            {
                int indexA = columnA + rowA_B * matA->columns;
                int rowB = columnA;
                int columnB = columnA_B;
                int indexB = columnB + rowB * matB->columns;
                matA_B->elements.v[indexA_B] += matA->elements.c[indexA] * matB->elements.c[indexB];
            }
        }
    }
}

void NLSLmatrix_transpose(pcNLSLmatrix_t matA, pNLSLmatrix_t matA_t)
{
    assert((matA->rows * matA->columns) == (matA_t->rows * matA_t->columns));
    matA_t->rows = matA->columns;
    matA_t->columns = matA->rows;
    for (int row = 0; row != matA->rows; row++)
    {
        for (int column = 0; column != matA->columns; column++)
        {
            int indexA = column + row * matA->columns;
            int indexA_t = row + column * matA_t->columns;
            matA_t->elements.v[indexA_t] = matA->elements.c[indexA];
        }
    }
}

static void NLSLmatrix_getcolumnvector(pcNLSLmatrix_t m, int column, float* v)
{
    const float* psrc = m->elements.c + column;
    for (int row = 0; row != m->rows; row++)
    {
        v[row] = psrc[row * m->columns];
    }
}

static void NLSLmatrix_setcolumnvector(pNLSLmatrix_t m, int column, const float* v)
{
    float* pdst = m->elements.v + column;
    for (int row = 0; row != m->rows; row++)
    {
        pdst[row * m->columns] = v[row];
    }
}

void NLSLmatrix_QRdecomp(pcNLSLmatrix_t matA, pNLSLmatrix_t  matQ, pNLSLmatrix_t matR)
{
    assert(matA->rows == matA->columns);
    assert((matA->rows == matQ->rows) && (matA->rows == matR->rows));
    assert((matA->columns == matQ->columns) && (matA->columns == matR->columns));
    float* u = (float*)calloc(matA->rows * 3, sizeof(float));
    float* e = u + matA->rows;
    float* a = e + matA->rows;
    int N = matA->rows; // matrix rows and columns
    int Nbytes = N * sizeof(float); // byte count of N x float variables
    // calc Q
    for (int column = 0; column != matA->columns; column++)
    {
        NLSLmatrix_getcolumnvector(matA, column, a); // get a column vector of A
        memcpy(u, a, Nbytes);
        for (int pastColumn = 0; pastColumn < column; pastColumn++)
        {
            NLSLmatrix_getcolumnvector(matQ, pastColumn, e);
            float ae = NLSLutils_dot(N, a, e); // (a, e) dot product
            NLSLutils_multfloatsscalar(N, e, ae);
            NLSLutils_subfloats(N, u, e);
        }
        float rcpULength = NLSL_SIGNF(u[column]) / sqrtf(NLSLutils_dot(N, u, u));
        NLSLutils_multfloatsscalar(N, u, rcpULength);
        NLSLmatrix_setcolumnvector(matQ, column, u);        
    }
    // calc R
    NLSL_FILLFLOATS(matR->elements.v, 0.0f, matR->rows * matR->columns);
    for (int row = 0; row != matA->rows; row++)
    {
        NLSLmatrix_getcolumnvector(matQ, row, e);
        for (int column = row; column != matA->columns; column++)
        {
            NLSLmatrix_getcolumnvector(matA, column, a);
            int indexR = column + row * matR->columns;
            matR->elements.v[indexR] = NLSLutils_dot(N, a, e);
        }
    }
    NLSL_SAFEFREE(&u);
}

static int NLSLmatrix_pvrow(float** heads, int pvrow, int rows)
{
    int pvrowcandidate = pvrow;
    float pvelement = heads[pvrow][pvrow];
    for (int row = pvrow + 1; row != rows; row++)
    {
        if (fabsf(heads[row][pvrow]) > fabsf(pvelement))
        {
            pvrowcandidate = row;
            pvelement = heads[row][pvrow];
        }
    }
    return pvrowcandidate;
}

static void NLSLmatrix_swapptr(float** a, float** b)
{
    float* temp = *a;
    *a = *b;
    *b = temp;
}

void NLSLmatrix_inv(pcNLSLmatrix_t matA, pNLSLmatrix_t matAinv)
{
    assert(matA->rows == matA->columns);
    assert(matA->rows == matAinv->rows);
    assert(matAinv->rows == matAinv->columns);
    float* work = (float*)calloc(matA->rows * matA->columns * 2, sizeof(float));
    float** heads = (float**)calloc(matA->rows, sizeof(float*));
    int stride = 2 * matA->columns;
    int copyBytes = matA->columns * sizeof(float);
    const float* aPtr = matA->elements.c;
    // init work area
    for (int row = 0; row != matA->rows; row++)
    {
        heads[row] = work + row * stride;
        memcpy(heads[row], aPtr, copyBytes);
        aPtr += matA->columns;
        NLSL_FILLFLOATS(heads[row] + matA->columns, 0.0f, matA->columns);
        *(heads[row] + matA->columns + row) = 1.0f;
    }
    // calculation
    for (int row = 0; row != matA->rows; row++)
    {
        int pvswaprow = NLSLmatrix_pvrow(heads, row, matA->rows);
        if (row != pvswaprow)
        {
            NLSLmatrix_swapptr(&heads[row], &heads[pvswaprow]);
        }
        const float rcpPivot = 1.0f / heads[row][row];
        NLSLutils_multfloatsscalar(stride - row, heads[row] + row, rcpPivot);
        for (int row1 = 0; row1 != matA->rows; row1++)
        {
            if (row == row1) continue;
            float mpy = -heads[row1][row];
            for (int column = row; column != stride; column++)
            {
                heads[row1][column] += mpy * heads[row][column];
            }
        }
    }
    // copy work to matAinv
    float* ainvPtr = matAinv->elements.v;
    for (int row = 0; row != matA->rows; row++)
    {
        memcpy(ainvPtr, heads[row] + matA->rows, copyBytes);
        ainvPtr += matA->columns;
    }
    NLSL_SAFEFREE(&work);
    NLSL_SAFEFREE(&heads);
}

void NLSLmatrix_print(FILE* pf, pcNLSLmatrix_t m)
{
    const float* elementPtr = m->elements.c;
    fprintf(pf, "(rows, columns) = (%d, %d)\n", m->rows, m->columns);
    for (int row = 0; row < m->rows; row++)
    {
        for (int column = 0; column < m->columns; column++)
        {
            fprintf(pf, "%c%f", (column == 0) ? ' ' : ',', elementPtr[column]);
        }
        fprintf(pf, "\n");
        elementPtr += m->columns;
    }
}