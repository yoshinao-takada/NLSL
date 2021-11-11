#include "NLSLsolver.h"
#include "NLSLutils.h"
#include "NLSLmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#define EQUSETUP 0
#ifndef ARRAYSIZE
#define ARRAYSIZE(a) sizeof(a)/sizeof((a)[0])
#endif
#if (EQUSETUP == 0)
#define YTARGET { -1.4f, -1.8f, -10.6f, -24.4f, 1.0f, -26.1f, 0.0f, 15.2f, 1.2f, -4.9f, 11.56f, 0.55f, 1.0f }
#define XIDEAL { 1.0f, 2.2f, -3.2f, 0.5f, 3.0f, 7.0f, -2.0f, -4.0f, -2.0f, -1.0f, 1.0f, -3.3f }
#define XINITIAL { 1.0f, 2.2f, -3.2f, 0.5f, 3.0f, 7.0f, -2.0f, -4.0f, -2.0f, -1.0f, 0.0f, 0.0f }
#define PARAMS { 0.0f }
#elif (EQUSETUP == 1)
#define YTARGET { 0.0f }
#define XINITIAL { 0.0f, 0.0f, 0.0f }
#define PARAMS { 1.0f, 2.0f, -3.0f }
#endif
static const float yTarget[] = YTARGET;
static const float xInitial[] = XINITIAL;
// static const float xInitial[] = XIDEAL;
static const float params[] = PARAMS;

// test objective function
#if (EQUSETUP == 0)
int objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    int err = 0;
    assert(cx == 12 && cy == 13);
    float x_[12];
    memcpy(x_, x, sizeof(x_));
    NLSLmatrix_t x0 = { 2, 2, x_ };
    NLSLmatrix_t x1 = { 2, 2, x_ + 4 };
    NLSLmatrix_t x2 = { 2, 2, x_ + 8 };
    NLSLmatrix_t y0 = { 2, 2, y };
    NLSLmatrix_t y1 = { 2, 2, y + 4 };
    NLSLmatrix_t y2 = { 2, 2, y + 8 };
    NLSLmatrix_mult(&x0, &x1, &y0);
    NLSLmatrix_mult(&x1, &x2, &y1);
    NLSLmatrix_mult(&x2, &x0, &y2);
    y[12] = x[0];
    return err;
}
#elif (EQUSETUP == 1)
int objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    int err = 0;
    assert(cx == 3 && cy == 1 && cp == 3);
    float d[3] = { x[0] - params[0], x[1] - params[1], x[2] - params[2] };
    *y = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
    return err;
}
#endif

void setconf(pNLSLsolverconf_t conf)
{
    conf->trace = stderr;
    memcpy(conf->yTarget, yTarget, sizeof(yTarget));
    memcpy(conf->xInitial, xInitial, sizeof(xInitial));
    NLSL_FILLFLOATS(conf->xInitialSteps, 1.0f, conf->cx);
    NLSL_FILLFLOATS(conf->xEps, 1.0e-4f, conf->cx);
    memcpy(conf->params, params, sizeof(params));
    conf->thObjective = 1.0e-4f;
    conf->objective  = objective;
}

int main(int argc, const char* *argv)
{
    int err = EXIT_SUCCESS;
    pNLSLsolver_t solver = NULL;
    int iter = (argc >= 2) ? atoi(argv[1]) : 5;
    do {
        solver = NLSLsolver_new(ARRAYSIZE(xInitial), ARRAYSIZE(yTarget), ARRAYSIZE(params));
        pNLSLsolverconf_t conf = NLSLsolver_conf(solver);
        setconf(conf);
        if (EXIT_SUCCESS != (err = NLSLsolver_init(solver)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = NLSLsolver_exec(solver, iter)))
        {
            break;
        }
    } while (0);
    NLSL_SAFEFREE(&solver);
    return err;
}