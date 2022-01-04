#include "NLSLgnsolver.h"
#include "NLSLmatrix.h"
#include "NLSLutils.h"
#include <math.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define NLSLgnsolver_trace(solver, ...) { \
    if (solver->conf.trace) { \
        fprintf(solver->conf.trace, __VA_ARGS__); \
    } \
}

struct NLSLgnsolver {
    NLSLsolverconf_t conf;
    NLSLstatus_t status;
    NLSL_OBJECTIVE* dfdx;
    float *x;
    float *y;
    float *jacobian;
    float *jacobianT; // transpose of jacobian
    float *jTj; // product of jacobianT and jacobian
    float *jTjinv; // inv(jTj)
    float *deltax;
    float *temp0;
    float *temp1;
    float buf[0];
};

pNLSLgnsolver_t NLSLgnsolver_new(int cx, int cy, int cp)
{
    size_t fltCount = cx + cy // x, y
        + (2 * cy * cx) // jacobian, jacobianT
        + (2 * cx * cx) // jTj, jTjinv
        + 3 * cx // deltax, temp0, temp1
        + 2 * cx + cp; // conf.xInitial, conf.xEps, conf.params
    pNLSLgnsolver_t p = (pNLSLgnsolver_t)malloc(sizeof(NLSLgnsolver_t) + sizeof(float) * fltCount);
    p->dfdx = (NLSL_OBJECTIVE*)calloc(cx, sizeof(NLSL_OBJECTIVE));
    p->x = p->buf;
    p->y = p->x + cx;
    p->jacobian = p->y + cy;
    p->jacobianT = p->jacobian + cx * cy;
    p->jTj = p->jacobianT + cx * cy;
    p->jTjinv = p->jTj + cx * cx;
    p->deltax = p->jTjinv + cx * cx;
    p->temp0 = p->deltax + cx;
    p->temp1 = p->temp0 + cx;
    p->status = NLSLstatus_notrun;
    p->conf.xInitial = p->temp1 + cx;
    p->conf.xInitialSteps = (float*)NULL;
    p->conf.xEps = p->conf.xInitial + cx;
    p->conf.yTarget = (float*)NULL;
    p->conf.params = p->conf.xEps + cx;
    p->conf.cx = cx;
    p->conf.cy = cy;
    p->conf.cp = cp;
    return p;
}

pNLSLsolverconf_t NLSLgnsolver_conf(pNLSLgnsolver_t solver)
{
    return &solver->conf;
}

NLSL_OBJECTIVE* NLSLgnsolver_jacobian(pNLSLgnsolver_t solver)
{
    return solver->dfdx;
}

int NLSLgnsolver_init(pNLSLgnsolver_t solver)
{
    int err = EXIT_SUCCESS;
    do {
        memcpy(solver->x, solver->conf.xInitial, solver->conf.cx * sizeof(float));
    } while (0);
    return err;
}

void NLSLgnsolver_delete(pNLSLgnsolver_t* ppsolver)
{
    if (ppsolver && *ppsolver)
    {
        pNLSLgnsolver_t p = *ppsolver;
        free(p->dfdx);
        free(p);
        *ppsolver = (pNLSLgnsolver_t)NULL;
    }
    else
    {
        assert(0);
    }
}

static int NLSLgnsolver_calcjacobian(pNLSLgnsolver_t solver)
{
    int err = EXIT_SUCCESS;
    do {
        err = solver->conf.objective(
            solver->conf.cx, solver->x, solver->conf.cy, solver->y, solver->conf.cp, solver->conf.params
        );
        if (EXIT_SUCCESS != err)
        {
            solver->status = NLSLstatus_errorabort;
            NLSLgnsolver_trace(solver, "objective() failed, err = %d @ %s, %d\n", err, __FILE__, __LINE__);
            break;
        }
        float* ptr_jT = solver->jacobianT;
        for (int ix = 0; ix < solver->conf.cx; ix++)
        {
            err = solver->dfdx[ix](
                solver->conf.cx, solver->x, solver->conf.cy, ptr_jT, solver->conf.cp, solver->conf.params
            );
            if (EXIT_SUCCESS != err)
            {
                solver->status = NLSLstatus_errorabort;
                break;
            }
            ptr_jT += solver->conf.cy;
        }
        if (EXIT_SUCCESS != err) break;

        { // matrix operation
            NLSLmatrix_t j = { solver->conf.cy, solver->conf.cx, { solver->jacobian }};
            NLSLmatrix_t jT = { solver->conf.cy, solver->conf.cx, { solver->jacobianT }};
            NLSLmatrix_t jTj = { solver->conf.cx, solver->conf.cx, { solver->jTj }};
            NLSLmatrix_t jTjinv = { solver->conf.cx, solver->conf.cx, { solver->jTjinv }};
            NLSLmatrix_t deltax = { solver->conf.cx, 1, { solver->deltax }};
            NLSLmatrix_t y = { solver->conf.cy, 1, { solver->y }};
            NLSLmatrix_t temp0 = { solver->conf.cx, 1, { solver->temp0 }};
            NLSLmatrix_t temp1 = { solver->conf.cx, 1, { solver->temp1 }};
            NLSLmatrix_t x = { solver->conf.cx, 1, { solver->x }};
            NLSLmatrix_transpose(&jT, &j);
            NLSLmatrix_mult(&jT, &j, &jTj);
            if (solver->conf.trace)
            {
                fprintf(solver->conf.trace, "jTj @ %s, %d\n", __FUNCTION__, __LINE__);
                NLSLmatrix_print(solver->conf.trace, &jTj);
            }
            NLSLmatrix_inv(&jTj, &jTjinv);
            if (solver->conf.trace)
            {
                fprintf(solver->conf.trace, "jTjinv @ %s, %d\n", __FUNCTION__, __LINE__);
                NLSLmatrix_print(solver->conf.trace, &jTjinv);
            }
            NLSLmatrix_mult(&jT, &y, &temp0);
            NLSLmatrix_mult(&jTjinv, &temp0, &deltax);
            NLSLmatrix_sub(&x, &deltax, &temp1);
            memcpy(solver->x, solver->temp1, solver->conf.cx * sizeof(float));
        }
    } while (0);
    return err;
}

static void NLSLgnsolver_updatestatus(pNLSLgnsolver_t solver)
{
    solver->status = NLSLstatus_xconverged;
    // check deltax is less than xEps
    for (int ix = 0; ix != solver->conf.cx; ix++)
    {
        if (fabsf(solver->deltax[ix]) > solver->conf.xEps[ix])
        {
            solver->status = NLSLstatus_running;
            break;
        }
    }
    if (solver->status == NLSLstatus_xconverged)
    {
        return;
    }

    // check if |y| < thObjective
    float normY = 0.0f;
    for (int iy = 0; iy != solver->conf.cy; iy++)
    {
        normY += solver->y[iy] * solver->y[iy];
    }
    if (normY < (solver->conf.thObjective * solver->conf.thObjective))
    {
        solver->status = NLSLstatus_yconverged;

    }
}

const float* NLSLgnsolver_xfinal(pcNLSLgnsolver_t solver)
{
    return solver->x;
}

const float* NLSLgnsolver_yfinal(pcNLSLgnsolver_t solver)
{
    return solver->y;
}

const float* NLSLgnsolver_deltaxfinal(pcNLSLgnsolver_t solver)
{
    return solver->deltax;
}

const float* NLSLgnsolver_jacobianfinal(pcNLSLgnsolver_t solver)
{
    return solver->jacobian;
}

NLSLstatus_t NLSLgnsolver_status(pcNLSLgnsolver_t solver)
{
    return solver->status;
}

int NLSLgnsolver_exec(pNLSLgnsolver_t solver, int iterMax)
{
    int err = EXIT_SUCCESS;
    do {
        for (int i = 0; i < iterMax; i++)
        {
            if (EXIT_SUCCESS != (err = NLSLgnsolver_calcjacobian(solver)))
            {
                break;
            }
            NLSLgnsolver_updatestatus(solver);
            if (solver->status != NLSLstatus_running)
            {
                break;
            }
        }
        if (solver->status == NLSLstatus_running)
        {
            solver->status = NLSLstatus_iterlimit;
        }
    } while (0);
    return err;
}
