#ifndef NLSLVARS_H_
#define NLSLVARS_H_
#include "NLSLsolver.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief vectors of independent variables and function result variables
 * 
 */
typedef struct {
    int cx, cy; // vector sizes
    float* x; // independent variables
    float* y; // function results
    float evalResult; // error agains target y
    float buffer[0]; // dummy buffer variable, don't access by user program
} NLSLvars_t, *pNLSLvars_t;
typedef const NLSLvars_t *pcNLSLvars_t;

pNLSLvars_t NLSLvars_new(int cx, int cy);

/**
 * @brief comparator callback used by qsort()
 * 
 * @param vars0 
 * @param vars1 
 * @return int -1: vars0->evalResult < vars1->evalResult, 0: vars0->evalResult == vars1->evalResult,
 *  1: vars0->evalResult > vars1->evalResult
 */
int NLSLvars_compare(const void* vars0, const void* vars1);

/**
 * @brief objective function evaluator
 * 
 */
typedef struct {
    int cy; // vector size
    const float* yTarget; // target of y
    NLSL_OBJECTIVE objective; // objective function
    float yWork[0]; // work area
} NLSLevaluator_t, *pNLSLevaluator_t;
typedef const NLSLevaluator_t *pcNLSLevaluator_t;

pNLSLevaluator_t NLSLevaluator_new(int cy, const float* yTarget, NLSL_OBJECTIVE objective);

/**
 * @brief execute the objective function and evaluate y against yTarget.
 * 
 * @param eval [in,out] evaluator object
 * @param vars [in,out] variables under evaluation
 * @return int objective function return code, usually unix errno compatible code but not limited.
 */
int NLSLevaluator_eval(pNLSLevaluator_t eval, pNLSLvars_t vars, int cp, const float* params);

#ifdef __cplusplus
}
#endif
#endif /*  NLSLVARS_H_ */