#ifndef NLSLMATRIX_H_
#define NLSLMATRIX_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    int rows;
    int columns;
    float* elements;
} NLSLmatrix_t, *pNLSLmatrix_t;
typedef const NLSLmatrix_t *pcNLSLmatrix_t;

void NLSLmatrix_add(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B);

void NLSLmatrix_sub(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B);

void NLSLmatrix_mult(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B);

void NLSLmatrix_transpose(pcNLSLmatrix_t matA, pNLSLmatrix_t matA_t);

#ifdef __cplusplus
}
#endif
#endif /* NLSLMATRIX_H_ */