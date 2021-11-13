#include "NLSLmatrix.h"
#include "NLSLutils.h"
#include <malloc.h>
#include <assert.h>
#include <memory.h>

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
