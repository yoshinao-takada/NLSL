#include "NLSLsolver.h"
#include "NLSLutils.h"
#include "NLSLvars.h"
#include <stdlib.h>
#include <errno.h>
#include <memory.h>

#define NLSLsolver_trace(solver, ...) { \
    if (solver->conf.trace) { \
        fprintf(solver->conf.trace, __VA_ARGS__); \
    } \
}

struct NLSLsolver {
    NLSLsolverconf_t conf;
    int indexR;
    int indexE;
    int indexOC;
    int indexIC;
    int indexCentroid;
    float rhoR;
    float rhoE;
    float rhoOC;
    float rhoIC;
    float rhoS;
    NLSLstatus_t status;
    pNLSLvars_t vertices[0];
};

pNLSLsolver_t NLSLsolver_new(int cx, int cy, int cp)
{
    pNLSLsolver_t p = NULL;
    do {
        int dimVertices = cx + 1 + 5;
        p = (pNLSLsolver_t)malloc(sizeof(NLSLsolver_t) + dimVertices * sizeof(pNLSLvars_t));
        for (int i = 0; i < dimVertices; i++)
        {
            p->vertices[i] = NLSLvars_new(cx, cy);
        }
        p->conf.cx = cx;
        p->conf.cy = cy;
        p->conf.cp = cp;
        p->conf.objective = NULL;
        size_t confAllocFloatCount = 3 * cx + cy + cp;
        p->conf.xInitial = (float*)calloc(confAllocFloatCount, sizeof(float));
        p->conf.xInitialSteps = p->conf.xInitial + cx;
        p->conf.xEps = p->conf.xInitialSteps + cx;
        p->conf.yTarget = p->conf.xEps + cx;
        p->conf.params = p->conf.yTarget + cy;
        p->indexR = cx + 1;
        p->indexE = cx + 2;
        p->indexOC = cx + 3;
        p->indexIC = cx + 4;
        p->indexCentroid = cx + 5;
        p->rhoR = 1.0f;
        p->rhoE = 1.5f;
        p->rhoOC = 0.5f;
        p->rhoIC = -0.5f;
        p->rhoS = 0.7f;
        p->status = NLSLstatus_notrun;
    } while (0);
    return p;
}

static void NLSLsolver_showvertex(pcNLSLsolver_t solver, int index, int withY)
{
    static const char* indexLabel[] = { "R", "E", "OC", "IC", "CENT" };
    if (index <= solver->conf.cx)
    {
        NLSLsolver_trace(solver, "vert[%d]: ", index);
    }
    else
    {
        NLSLsolver_trace(solver, "candidate[%s]: ", indexLabel[index - solver->conf.cx - 1]);
    }
    pcNLSLvars_t vars = solver->vertices[index];
    NLSLsolver_trace(solver, "%f:", vars->evalResult);
    for (int i = 0; i < solver->conf.cx; i++)
    {
        NLSLsolver_trace(solver, "%c%f", (i == 0) ? ' ': ',', vars->x[i]);
    }
    if (withY > 0)
    {
        NLSLsolver_trace(solver, ": y:");
        for (int i = 0; i < solver->conf.cy; i++)
        {
            NLSLsolver_trace(solver, "%c%f", (i == 0) ? ' ': ',', vars->y[i]);
        }
    }
    NLSLsolver_trace(solver, "\n");
}

static void NLSLsolver_showvertices(pcNLSLsolver_t solver, int withY, int iBegin, int iEnd)
{
    if (solver->conf.trace)
    {
        for (int index = iBegin; index < iEnd; index++)
        {
            NLSLsolver_showvertex(solver, index, withY);
        }
    }
}

static int NLSLsolver_fillvars(pNLSLsolver_t solver)
{
    int err = EXIT_SUCCESS;
    pNLSLevaluator_t evalulator = NULL;
    do {
        if (NULL == (evalulator = NLSLevaluator_new(solver->conf.cy, solver->conf.yTarget, solver->conf.objective)))
        {
            err = ENOMEM;
            break;
        }
        for (int vertIndex = 0; vertIndex <= solver->conf.cx; vertIndex++) // loop from 0 to n
        {
            pNLSLvars_t vars = solver->vertices[vertIndex];
            memcpy(vars->x, solver->conf.xInitial, solver->conf.cx * sizeof(float));
            if (vertIndex != 0)
            {
                vars->x[vertIndex - 1] += solver->conf.xInitialSteps[vertIndex - 1];
            }
            if (EXIT_SUCCESS != (err = NLSLevaluator_eval(evalulator, vars, solver->conf.cp, solver->conf.params)))
            {
                NLSLsolver_trace(solver, "%s,%d,fail in NLSLevaluator_eval() = %d\n", __FUNCTION__, __LINE__, err);
                break;
            }
        }
    } while (0);
    NLSL_SAFEFREE(&evalulator);
    return err;
}

int NLSLsolver_init(pNLSLsolver_t solver)
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = NLSLsolver_fillvars(solver)))
        {
            break;
        }
    } while (0);
    return err;
}

pNLSLsolverconf_t NLSLsolver_conf(pNLSLsolver_t solver)
{
    return &solver->conf;
}

void NLSLsolver_delete(pNLSLsolver_t* ppsolver)
{
    if (ppsolver && *ppsolver)
    {
        pNLSLsolver_t p = *ppsolver;
        free(p->conf.xInitial);
        free(p);
        *ppsolver = NULL;
    }
}

const float* NLSLsolver_xfinal(pcNLSLsolver_t solver)
{
    return solver->vertices[0]->x;
}

const float* NLSLsolver_yfinal(pcNLSLsolver_t solver)
{
    return solver->vertices[0]->y;
}

const float NLSLsolver_evalfinal(pcNLSLsolver_t solver)
{
    return solver->vertices[0]->evalResult;
}

static int NLSLsolver_evalcentroid(pNLSLsolver_t solver, pNLSLevaluator_t evaluator)
{
    // Step 1: calculate centroid of vertices[0] .. vertices[conf.cx-1], i.e. N-dimensional simplex
    NLSL_FILLFLOATS(solver->vertices[solver->indexCentroid]->x, 0.0f, solver->conf.cx);
    for (int i = 0; i < solver->conf.cx; i++)
    {
        NLSLutils_addfloats(solver->conf.cx, solver->vertices[solver->indexCentroid]->x, solver->vertices[i]->x);
    }
    const float rcpItemCount = 1.0f / (float)solver->conf.cx;
    NLSLutils_multfloatsscalar(solver->conf.cx, solver->vertices[solver->indexCentroid]->x, rcpItemCount);

    // Step 2: evaluate objective function for the centroid point.
    return NLSLevaluator_eval(evaluator, solver->vertices[solver->indexCentroid], solver->conf.cp, solver->conf.params);
}

// Apply shirink operation to vertices[1]..vertices[conf.cx]. vertices[0] is excluded from the operation.
static int NLSLsolver_shrink(pNLSLsolver_t solver, pNLSLevaluator_t evalulator)
{
    int err = EXIT_SUCCESS;
    int cp = solver->conf.cp;
    const float* params = solver->conf.params;
    for (int i = 1; i <= solver->conf.cx; i++)
    {
        NLSLutils_subfloats(solver->conf.cx, solver->vertices[i]->x, solver->vertices[0]->x);
        NLSLutils_multfloatsscalar(solver->conf.cx, solver->vertices[i]->x, solver->rhoS);
        NLSLutils_addfloats(solver->conf.cx, solver->vertices[i]->x, solver->vertices[0]->x);
        if (EXIT_SUCCESS != (err = NLSLevaluator_eval(evalulator, solver->vertices[i], cp, params)))
        {
            NLSLsolver_trace(solver, "%s,%d,fail in NLSLsolver_eval(,vert[%d],,) = %d\n", __FUNCTION__, __LINE__, i, err);
            break;
        }
    }
    return err;
}

/**
 * @brief check if best y variable vector is sufficiently close to yTarget.
 * 
 * @param solver [in,out]
 * @return int 1: converged, 0: not yet
 */
static int NLSLsolver_yconverged(pNLSLsolver_t solver)
{
    int result = 0;
    if (solver->vertices[0]->evalResult < solver->conf.thObjective)
    {
        solver->status = NLSLstatus_yconverged;
        result = 1;
    }
    return result;
}

/**
 * @brief check if best, worst, and centroid x variable vectors are sufficiently close within xEps.
 * This means that x variation between iteration is too small to improve y and yTarget distance.
 * 
 * @param solver 
 * @return int 1: converged, 0: not yet
 */
static int NLSLsolver_xconverged(pNLSLsolver_t solver)
{
    int result = 1;
    do {
        pcNLSLvars_t best = solver->vertices[0];
        pcNLSLvars_t worst = solver->vertices[solver->conf.cx];
        pcNLSLvars_t centroid = solver->vertices[solver->indexCentroid];
        result *= NLSLutils_iscloserthan(solver->conf.cx, best->x, worst->x, solver->conf.xEps);
        if (result == 0)
        {
            break;
        }
        result *= NLSLutils_iscloserthan(solver->conf.cx, best->x, centroid->x, solver->conf.xEps);
        if (result == 0)
        {
            break;
        }
        result *= NLSLutils_iscloserthan(solver->conf.cx, centroid->x, worst->x, solver->conf.xEps);
        if (result == 0)
        {
            break;
        }
        solver->status = NLSLstatus_xconverged;
    } while (0);
    return result;
}

int NLSLsolver_exec(pNLSLsolver_t solver, int iterMax)
{
    int err = EXIT_SUCCESS;
    int cp = solver->conf.cp;
    const float* params = solver->conf.params;
    pNLSLvars_t temp = NULL;
    pNLSLevaluator_t evaluator = NLSLevaluator_new(solver->conf.cy, solver->conf.yTarget, solver->conf.objective);
    if (!evaluator)
    {
        return ENOMEM;
    }
    solver->status = NLSLstatus_running;
    do {
        qsort(solver->vertices, solver->conf.cx + 1, sizeof(pcNLSLvars_t), NLSLvars_compare);
        if (NLSLsolver_xconverged(solver) || NLSLsolver_yconverged(solver)) { break; }// check convergence
        NLSLsolver_showvertices(solver, 1, 0, solver->indexCentroid + 1);
        if (EXIT_SUCCESS != (err = NLSLsolver_evalcentroid(solver, evaluator)))
        {
            NLSLsolver_trace(solver, "%s,%d,fail in NLSLsolver_evalcentroid() = %d\n", __FUNCTION__, __LINE__, err);
            solver->status = NLSLstatus_errorabort;
            break;
        }
        // calc reflection point
        NLSLutils_tranformvertex(
            solver->conf.cx, solver->vertices[solver->indexCentroid]->x, solver->vertices[solver->conf.cx]->x, 
            solver->rhoR, solver->vertices[solver->indexR]->x);
        NLSLutils_tranformvertex(
            solver->conf.cx, solver->vertices[solver->indexCentroid]->x, solver->vertices[solver->conf.cx]->x, 
            solver->rhoE, solver->vertices[solver->indexE]->x);
        NLSLutils_tranformvertex(
            solver->conf.cx, solver->vertices[solver->indexCentroid]->x, solver->vertices[solver->conf.cx]->x, 
            solver->rhoOC, solver->vertices[solver->indexOC]->x);
        NLSLutils_tranformvertex(
            solver->conf.cx, solver->vertices[solver->indexCentroid]->x, solver->vertices[solver->conf.cx]->x, 
            solver->rhoIC, solver->vertices[solver->indexIC]->x);
        if ((EXIT_SUCCESS != (err = NLSLevaluator_eval(evaluator, solver->vertices[solver->indexR], cp, params))) ||
            (EXIT_SUCCESS != (err = NLSLevaluator_eval(evaluator, solver->vertices[solver->indexE], cp, params))) ||
            (EXIT_SUCCESS != (err = NLSLevaluator_eval(evaluator, solver->vertices[solver->indexOC], cp, params))) ||
            (EXIT_SUCCESS != (err = NLSLevaluator_eval(evaluator, solver->vertices[solver->indexIC], cp, params))))

        {
            NLSLsolver_trace(solver, "%s,%d,fail in NLSLevaluator_eval() = %d\n", __FUNCTION__, __LINE__, err);
            solver->status = NLSLstatus_errorabort;
            break;
        }
        const float evalBest = solver->vertices[0]->evalResult;
        const float evalWorst = solver->vertices[solver->conf.cx]->evalResult;
        const float eval2ndWorst = solver->vertices[solver->conf.cx - 1]->evalResult;
        const float evalR = solver->vertices[solver->indexR]->evalResult;
        const float evalE = solver->vertices[solver->indexE]->evalResult;
        const float evalOC = solver->vertices[solver->indexOC]->evalResult;
        const float evalIC = solver->vertices[solver->indexIC]->evalResult;
        if ((evalBest <= evalR) && (evalR < eval2ndWorst))
        { // adopt reflection
            NLSLsolver_trace(solver, "Adopt REFLECTION\n");
            NLSL_SWAP(solver->vertices[solver->indexR], solver->vertices[solver->conf.cx], temp);
        }
        else if (evalR < evalBest)
        {
            if (evalE <= evalR)
            { // adopt expansion
                NLSLsolver_trace(solver, "Adopt EXPANSION\n");
                NLSL_SWAP(solver->vertices[solver->indexE], solver->vertices[solver->conf.cx], temp);
            }
            else
            { // adopt reflection after evaluating expansion
                NLSLsolver_trace(solver, "Adopt REFLECTION after evaluating EXPANSION\n");
                NLSL_SWAP(solver->vertices[solver->indexR], solver->vertices[solver->conf.cx], temp);
            }
        }
        else if (eval2ndWorst <= evalR)
        {
            if ((evalR < evalWorst) && (evalOC <= evalR))
            { // adopt outside contraction
                NLSLsolver_trace(solver, "Adopt OUTSIDE CONTRACTION\n");
                NLSL_SWAP(solver->vertices[solver->indexOC], solver->vertices[solver->conf.cx], temp);
            }
            else if ((evalWorst <= evalR) && (evalIC < evalWorst))
            { // adopt inside contraction
                NLSLsolver_trace(solver, "Adopt INSIDE CONTRACTION\n");
                NLSL_SWAP(solver->vertices[solver->indexIC], solver->vertices[solver->conf.cx], temp);
            }
        }
        else if (EXIT_SUCCESS != (err = NLSLsolver_shrink(solver, evaluator))) // adopt shrink
        { // fail in shrink
            NLSLsolver_trace(solver, "Adopt SHRINKING\n");
            solver->status = NLSLstatus_errorabort;
            break;
        }
    } while (--iterMax);
    NLSL_SAFEFREE(&evaluator);
    if (iterMax == 0)
    {
        solver->status = NLSLstatus_iterlimit;
    }
    return err;
}

NLSLstatus_t NLSLsolver_status(pcNLSLsolver_t solver)
{
    return solver->status;
}