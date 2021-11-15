#include "NLSLsolver.h"
#include "NLSLutils.h"
#include "NLSLmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
// APP_SELECTOR = 0: Nelder-Mead solver
// APP_SELECTOR = 1: QR-decomposition, which is used for camera calibration
// APP_SELECTOR = 2: invert matrix
#define APP_SELECTOR    2

#define EQUSETUP 0
#ifndef ARRAYSIZE
#define ARRAYSIZE(a) sizeof(a)/sizeof((a)[0])
#endif
#if (EQUSETUP == 0)
#define YTARGET { -1.4f, -1.8f, -10.6f, -24.4f, 1.0f, -26.1f, 0.0f, 15.2f, 1.2f, -4.9f, 11.56f, 0.55f }
#define XIDEAL { 1.0f, 2.2f, -3.2f, 0.5f, 3.0f, 7.0f, -2.0f, -4.0f, -2.0f, -1.0f, 1.0f, -3.3f }
// #define XINITIAL { 2.0f, 2.2f, -3.2f, 0.5f, 3.0f, 7.0f, -2.0f, -4.0f, -2.0f, -1.0f, 0.0f, 0.0f }
#define XINITIAL { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f }
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
    NLSLmatrix_t x0 = { 2, 2, { x } };
    NLSLmatrix_t x1 = { 2, 2, { x + 4 } };
    NLSLmatrix_t x2 = { 2, 2, { x + 8}  };
    NLSLmatrix_t y0 = { 2, 2, y };
    NLSLmatrix_t y1 = { 2, 2, { y + 4 } };
    NLSLmatrix_t y2 = { 2, 2, { y + 8 } };
    NLSLmatrix_mult(&x0, &x1, &y0);
    NLSLmatrix_mult(&x1, &x2, &y1);
    NLSLmatrix_mult(&x2, &x0, &y2);
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
#ifdef NDEBUG
    conf->trace = (FILE*)NULL;
#else
    conf->trace = stderr;
#endif
    memcpy(conf->yTarget, yTarget, sizeof(yTarget));
    memcpy(conf->xInitial, xInitial, sizeof(xInitial));
    NLSL_FILLFLOATS(conf->xInitialSteps, 1.0f, conf->cx);
    NLSL_FILLFLOATS(conf->xEps, 1.0e-4f, conf->cx);
    memcpy(conf->params, params, sizeof(params));
    conf->thObjective = 1.0e-8f;
    conf->objective  = objective;
}

void showresult(pNLSLsolver_t solver)
{
    static const char* StatusLabels[] = { "IterLimit", "XConverged", "YConverged", "ErrorAbort", "NotRun", "Running" };
    printf("status = %s\n", StatusLabels[(int)NLSLsolver_status(solver)]);
    printf("eval final = %f\n", NLSLsolver_evalfinal(solver));
    pcNLSLsolverconf_t conf = NLSLsolver_conf(solver);
    const float* x = NLSLsolver_xfinal(solver);
    const float* y = NLSLsolver_yfinal(solver);
    for (int i = 0; i < conf->cx; i++)
    {
        printf("\tx[%d] = %f\n", i, x[i]);
    }
    printf("\n");
    for (int i = 0; i < conf->cy; i++)
    {
        printf("\ty[%d] = %f\n", i, y[i]);
    }
}

#if (APP_SELECTOR == 0)
int main(int argc, const char* *argv)
{
    int err = EXIT_SUCCESS;
    pNLSLsolver_t solver = NULL;
    int iter = (argc >= 2) ? atoi(argv[1]) : 5;
    do {
        // Step 1: create the solver object
        solver = NLSLsolver_new(ARRAYSIZE(xInitial), ARRAYSIZE(yTarget), ARRAYSIZE(params));

        // Step 2: set configuration
        pNLSLsolverconf_t conf = NLSLsolver_conf(solver);
        setconf(conf);

        // Step 3: initialize the solver with the configuration
        if (EXIT_SUCCESS != (err = NLSLsolver_init(solver)))
        {
            break;
        }

        // Step 4: run optimization
        if (EXIT_SUCCESS != (err = NLSLsolver_exec(solver, iter)))
        {
            break;
        }

        // Step 5: retrieve and show the results.
        showresult(solver);
    } while (0);
    NLSL_SAFEFREE(&solver);
    return err;
}

#elif (APP_SELECTOR == 1)
int main(int argc, const char* *argv)
{
    static const float Asrc[] = {
        1.0f, 2.0f, 3.0f,
        1.0f, 3.0f, 5.0f,
        2.0f, 4.0f, 7.0f
    };
    static const float Qsrc[] = {
        -4.0825e-1f, 1.8257e-1f, -8.9443e-1f,
        -4.0825e-1f, -9.1287e-1f, 7.7716e-16f,
        -8.1650e-1f, 3.6515e-1f, 4.4721e-1f
    };
    static const float Rsrc[] = {
        -2.4495f, -5.3072f, -8.9815f,
        0.0f, -0.9129f, -1.4606f,
        0.0f, 0.0f, 0.4472f
    };
    int err = EXIT_SUCCESS;
    float QRsrc[27];
    do {
        printf("QR-decomp test app enter.\n");
        NLSLmatrix_t a = { 3,3, {Asrc} };
        NLSLmatrix_t q = { 3,3, {QRsrc} };
        NLSLmatrix_t r = { 3,3, {q.elements.c + 9} };
        NLSLmatrix_t qr = { 3,3, {r.elements.c + 9} };
        NLSLmatrix_QRdecomp(&a, &q, &r);
        NLSLmatrix_print(stdout, &q);
        NLSLmatrix_print(stdout, &r);
        NLSLmatrix_mult(&q, &r, &qr);
        NLSLmatrix_print(stdout, &qr);
    } while (0);
    return err;
}
#elif (APP_SELECTOR == 2)
int main(int argc, const char* *argv)
{
    static const float Asrc[] = {
        1.0f, 2.0f, 3.0f,
        1.0f, 3.0f, 5.0f,
        2.0f, 4.0f, 7.0f
    };
    int err = EXIT_SUCCESS;
    float invAsrc[18];
    NLSLmatrix_t matA = { 3,3, {Asrc} };
    NLSLmatrix_t matAinv = { 3,3, {invAsrc} };
    NLSLmatrix_t matI = { 3,3, {invAsrc + 9} };
    NLSLmatrix_inv(&matA, &matAinv);
    NLSLmatrix_mult(&matA, &matAinv, &matI);
    NLSLmatrix_print(stdout, &matI);
    return err;
}
#else
int main(int argc, const char* *argv)
{
    fprintf(stderr, "Invalid app was selected.\n");
    return -1;
}
#endif
