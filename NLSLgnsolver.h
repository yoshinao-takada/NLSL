#ifndef NLSLGNSOLVER_H_
#define NLSLGNSOLVER_H_
#include "NLSLsolver.h"
/**
 * @brief Gauss-Newton solver
 * 
 */
#ifdef __cplusplus
extern "C" {
#endif
struct NLSLgnsolver;
typedef struct NLSLgnsolver NLSLgnsolver_t;
typedef NLSLgnsolver_t *pNLSLgnsolver_t;
typedef const NLSLgnsolver_t *pcNLSLgnsolver_t;

/**
 * @brief constructor
 * 
 * @param cx [in] variable x dimension
 * @param cy [in] function result vector dimension
 * @param cp [in] function const parameter dimension
 * @return pNLSLgnsolver_t  is newly created object
 */
pNLSLgnsolver_t NLSLgnsolver_new(int cx, int cy, int cp);

/**
 * @brief accessor to configuration
 * 
 * @param solver [in]
 * @return pNLSLsolverconf_t  configuration info of the solver
 */
pNLSLsolverconf_t NLSLgnsolver_conf(pNLSLgnsolver_t solver);

/**
 * @brief accessor to Jacobian function array
 * 
 * @param solver [in]
 * @return NLSL_OBJECTIVE* function array of cx dimension
 */
NLSL_OBJECTIVE* NLSLgnsolver_jacobian(pNLSLgnsolver_t solver);

/**
 * @brief initialize internal data after setting configuration parameters and Jacobian array
 * 
 * @param solver 
 * @return int unix errno compatible error code
 */
int NLSLgnsolver_init(pNLSLgnsolver_t solver);

/**
 * @brief delete an existing object
 * 
 * @param ppsolver is a pointer-pointer which is filled with NULL after delete. 
 */
void NLSLgnsolver_delete(pNLSLgnsolver_t* ppsolver);

const float* NLSLgnsolver_xfinal(pcNLSLgnsolver_t solver);

const float* NLSLgnsolver_yfinal(pcNLSLgnsolver_t solver);

const float* NLSLgnsolver_deltaxfinal(pcNLSLgnsolver_t solver);

const float* NLSLgnsolver_jacobianfinal(pcNLSLgnsolver_t solver);

int NLSLgnsolver_exec(pNLSLgnsolver_t solver, int iterMax);

NLSLstatus_t NLSLgnsolver_status(pcNLSLgnsolver_t solver);
#ifdef __cplusplus
}
#endif
#endif /* NLSLGNSOLVER_H_ */
