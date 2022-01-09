#include "NLSLgnsolver.h"
#include "NLSLutils.h"
#include "NLSLmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

static const float Yref[] = { 1.0f, 2.2f };
static const float M[] = { 1.0f, 2.5f, -1.2f, 0.5f };
static const float xInit[] = { 0.0f, 0.0f };
static const float xEps[] = { 1.0e-4f, 1.0e-4f };
static const float th = 1.0e-6f;

static int objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSLmatrix_t m = { 2, 2, {params}};
    NLSLmatrix_t mx = { 2, 1, { y }};
    NLSLmatrix_t xv = { 2, 1, { x }};
    NLSLmatrix_mult(&m, &xv, &mx);
    const float* yRef = params + 4;
    y[0] -= yRef[0];
    y[1] -= yRef[1];
    return 0;
}

static int J_0(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    y[0] = params[0];
    y[1] = params[2];
    return 0;
}

static int J_1(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    y[0] = params[1];
    y[1] = params[3];
    return 0;
}

static const NLSL_OBJECTIVE JacobianSource[] = { J_0, J_1 };

static void ShowX(pNLSLgnsolver_t solver)
{
    pcNLSLsolverconf_t conf = NLSLgnsolver_conf(solver);
    const float* x = NLSLgnsolver_xfinal(solver);
    for (int i = 0; i < conf->cx; i++)
    {
        fprintf(stderr, "%c%f", (i == 0) ? ' ' : ',', x[i]);
    }
    fprintf(stderr, "\n");
}

int main()
{
    int err = EXIT_SUCCESS;
    pNLSLgnsolver_t solver = (pNLSLgnsolver_t)NULL;
    do {
        solver = NLSLgnsolver_new(2, 2, 6);
        memcpy(NLSLgnsolver_jacobian(solver), JacobianSource, sizeof(JacobianSource));
        pNLSLsolverconf_t conf = NLSLgnsolver_conf(solver);
        memcpy(conf->xInitial, xInit, sizeof(xInit));
        memcpy(conf->xEps, xEps, sizeof(xEps));
        memcpy(conf->params, M, sizeof(M));
        conf->params[4] = Yref[0];
        conf->params[5] = Yref[1];
        conf->thObjective = th;
        conf->trace = stderr;
        conf->objective = objective;
        NLSLgnsolver_init(solver);
        NLSLgnsolver_exec(solver, 10);
        ShowX(solver);
        fprintf(stderr, "status = %d\n", NLSLgnsolver_status(solver));
    } while (0);
    NLSLgnsolver_delete(&solver);
    return err;
}