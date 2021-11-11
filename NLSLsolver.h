#ifndef NLSLSOLVER_H_
#define NLSLSOLVER_H_
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
struct NLSLsolver;
typedef struct NLSLsolver NLSLsolver_t;
typedef NLSLsolver_t *pNLSLsolver_t;
typedef const NLSLsolver_t *pcNLSLsolver_t;

typedef int (*NLSL_OBJECTIVE)(int cx, const float* x, int cy, float* y, int cp, const float* params);

typedef struct {
    FILE* trace; // trace logger output stream
    int cx, cy, cp; // dims of variables and parameters
    float* xInitial;
    float* xInitialSteps;
    float* xEps;
    float* yTarget;
    float* params;
    float thObjective;
    NLSL_OBJECTIVE objective;
} NLSLsolverconf_t, *pNLSLsolverconf_t;
typedef const NLSLsolverconf_t *pcNLSLsolverconf_t;

/**
 * @brief constructor
 * 
 * @param cx [in] variable x dimension
 * @param cy [in] function result vector dimension
 * @param cp [in] function const parameter dimension
 * @return pNLSLsolver_t is newly created object
 */
pNLSLsolver_t NLSLsolver_new(int cx, int cy, int cp);

/**
 * @brief accessor to configuration
 * 
 * @param solver [in]
 * @return pNLSLsolverconf_t configuration info of the solver
 */
pNLSLsolverconf_t NLSLsolver_conf(pNLSLsolver_t solver);

/**
 * @brief initialize internal data after setting configuration parameters.
 * 
 * @param solver 
 * @return int 
 */
int NLSLsolver_init(pNLSLsolver_t solver);
/**
 * @brief delete an existing object
 * 
 * @param ppsolver is a pointer-pointer which is filled with NULL after delete.
 */
void NLSLsolver_delete(pNLSLsolver_t* ppsolver);

/**
 * @brief get the optimized x vector after convergence
 * 
 * @param solver 
 * @return const float* 
 */
const float* NLSLsolver_xfinal(pcNLSLsolver_t solver);

/**
 * @brief execute optimization
 * 
 * @param solver 
 * @param iterMax [in] iteration limit; optmization can be end earlier if converges before maxIter completed.
 * @return int unix errno compatible code
 */
int NLSLsolver_exec(pNLSLsolver_t solver, int iterMax);

typedef enum {
    NLSLstatus_converged,
    NLSLstatus_iterlimit,
    NLSLstatus_errorabort,
    NLSLstatus_notrun
} NLSLstatus_t;

NLSLstatus_t NLSLsolver_status(pNLSLsolver_t solver);
#ifdef __cplusplus
}
#endif
#endif /* NLSLSOLVER_H_ */