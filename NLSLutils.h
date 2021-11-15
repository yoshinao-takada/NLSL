#ifndef NLSLUTILS_H_
#define NLSLUTILS_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief vector add src to dst.
 * 
 * @param size [in] vector size
 * @param dst [in,out]
 * @param src [in]
 */
void NLSLutils_addfloats(int size, float* dst, const float* src);

/**
 * @brief vector subtract src from dst
 * 
 * @param size [in] vector size
 * @param dst [in,out]
 * @param src [in]
 */
void NLSLutils_subfloats(int size, float* dst, const float* src);

/**
 * @brief multiply vector with scalar
 * 
 * @param size [in] vector size
 * @param dst [in,out] multiplicant
 * @param scalar [in] multiplier
 */
void NLSLutils_multfloatsscalar(int size, float* dst, float scalar);

/**
 * @brief dot product
 * 
 * @param size 
 * @param v0 
 * @param v1 
 * @return float number of dot product
 */
float NLSLutils_dot(int size, const float* v0, const float* v1);

/**
 * @brief transform vertex of simplex
 * 
 * @param size [in] vector size
 * @param centroid [in] centroid of vertex0 .. n-1
 * @param worst [in] the worst vertex in the previous iteration
 * @param tranformed [out]
 * @param rho [in] transform parameter
 */
void NLSLutils_tranformvertex(int size, const float* centroid, const float* worst, float rho, float* transformed);

/**
 * @brief check if x0 is closer to x1 within the threshold xth.
 * 
 * @param size 
 * @param x0 
 * @param x1 
 * @param xth 
 * @return int 1: closer, 0: not closer
 */
int NLSLutils_iscloserthan(int size, const float* x0, const float* x1, const float* xth);

#define NLSL_SAFEFREE(ppobj) if ((ppobj) && *(ppobj)) { free(*(ppobj)); *(ppobj) = NULL; }

#define NLSL_ARRAYSIZE(_a_) sizeof(_a_)/sizeof(_a_[0])

/**
 * @brief set _a_ with const array values in _ca_
 * 
 * @param _a_ [out] array or non const pointer
 * @param _ca_ [in] const array
 */
#define NLSL_SETFLOATS(_a_,_ca_) for (int a_index = 0; a_index < ARRAYSIZE(_ca_); a_index++) { _a_[a_index] = _ca_[a_index]; }

/**
 * @brief fill array with a value
 * 
 * @param _a_ [out] array or non-const pointer
 * @param _val_ [in] scalar value
 * @param _size_ [in] array size
 */
#define NLSL_FILLFLOATS(_a_,_val_,_size_) for (int a_index = 0; a_index < _size_; a_index++) { (_a_)[a_index] = _val_; }

#define NLSL_SWAP(a, b, temp) { temp = a; a = b; b = temp; }

#define NLSL_SIGNF(_n_) (((_n_) >= 0.0f) ? 1.0f : -1.0f)
#ifdef __cplusplus
}
#endif
#endif /* NLSLUTILS_H_ */