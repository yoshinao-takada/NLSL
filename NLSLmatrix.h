#ifndef NLSLMATRIX_H_
#define NLSLMATRIX_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief a matrix, row-major layout
 * 
 */
typedef struct {
    int rows;
    int columns;
    union {
        const float* c;
        float* v;
    } elements;
} NLSLmatrix_t, *pNLSLmatrix_t;
typedef const NLSLmatrix_t *pcNLSLmatrix_t;

/**
 * @brief matA_B = matA + matB, matA_B fields must be initialized before.
 * 
 * @param matA [in]
 * @param matB [in]
 * @param matA_B [out] result matrix
 */
void NLSLmatrix_add(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B);

/**
 * @brief matA_B = matA - matB
 * 
 * @param matA 
 * @param matB 
 * @param matA_B 
 */
void NLSLmatrix_sub(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B);

/**
 * @brief matA_B = matA * matB
 * 
 * @param matA 
 * @param matB 
 * @param matA_B 
 */
void NLSLmatrix_mult(pcNLSLmatrix_t matA, pcNLSLmatrix_t matB, pNLSLmatrix_t matA_B);

/**
 * @brief matA_t = transpose(matA)
 * 
 * @param matA 
 * @param matA_t 
 */
void NLSLmatrix_transpose(pcNLSLmatrix_t matA, pNLSLmatrix_t matA_t);

#ifdef __cplusplus
}
#endif
#endif /* NLSLMATRIX_H_ */