#include "NLSLvars.h"
#include "NLSLutils.h"
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>

pNLSLvars_t NLSLvars_new(int cx, int cy)
{
    pNLSLvars_t p = NULL;
    do {
        size_t dataBytes = (cx + cy) * sizeof(float);
        size_t allocBytes = sizeof(NLSLvars_t) + dataBytes;
        if (NULL == (p = (pNLSLvars_t)malloc(allocBytes)))
        {
            break;
        }
        p->cx = cx;
        p->cy = cy;
        p->evalResult = 0.0f;
        p->x = p->buffer;
        p->y = p->x + cx;
    } while (0);
    return p;
}

int NLSLvars_compare(const void* vars0, const void* vars1)
{
    pcNLSLvars_t vars0_ = *(pcNLSLvars_t*)vars0;
    pcNLSLvars_t vars1_ = *(pcNLSLvars_t*)vars1;
    int result = 0;
    if (vars0_->evalResult < vars1_->evalResult)
    {
        result = -1;
    }
    else if (vars0_->evalResult > vars1_->evalResult)
    {
        result = 1;
    }
    return result;
}

pNLSLevaluator_t NLSLevaluator_new(int cy, const float* yTarget, NLSL_OBJECTIVE objective)
{
    pNLSLevaluator_t p = NULL;
    do {
        size_t dataBytes = cy * sizeof(float);
        size_t allocBytes = sizeof(NLSLevaluator_t) + dataBytes;
        if (NULL == (p = (pNLSLevaluator_t)malloc(allocBytes)))
        {
            break;
        }
        p->cy = cy;
        p->yTarget = yTarget;
        p->objective = objective;
    } while (0);
    return p;
}

int NLSLevaluator_eval(pNLSLevaluator_t eval, pNLSLvars_t vars, int cp, const float* params)
{
    int err = EXIT_SUCCESS;
    do {
        err = eval->objective(vars->cx, vars->x, vars->cy, vars->y, cp, params);
        if (EXIT_SUCCESS != err)
        {
            break;
        }
        memcpy(eval->yWork, vars->y, sizeof(float) * vars->cy);
        NLSLutils_subfloats(eval->cy, eval->yWork, eval->yTarget);
        vars->evalResult = NLSLutils_dot(eval->cy, eval->yWork, eval->yWork);
    } while (0);
    return err;
}
