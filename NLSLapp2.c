#include "NLSLgnsolver.h"
#include "NLSLmatrix.h"
#include "NLSLutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#define MAT_SRC { \
    { 1.0f, 2.2f, -0.5f, -1.2f }, { 0.0f, -0.5f, -1.2f, 1.5f }, { 1.1f, 2.0f, -5.0f, 0.2f } \
}
static const float MatSrc[3][4] = MAT_SRC;

#pragma region objective_and_jacobian
/**
 * @brief skew symmetrical products of three matrices
 * 
 * @param cx [in]
 * @param x  [in] expanded three 2x2 matrices, MatX0, MatX1, MatX2
 * @param cy [in]
 * @param y  [in] expanded three 2x2 matrices, MatX0 * MatX1 - MatY0, MatX1 * MatX2 - MatY1, MatX2 * MatX0 - MatY2
 * @param cp [in]
 * @param params [in] expanded three 2x2 matrices MatY0, MatY1, MatY2
 */
static int objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSLmatrix_t MatX0 = { 2, 2, { x }};
    NLSLmatrix_t MatX1 = { 2, 2, { x + 4 }};
    NLSLmatrix_t MatX2 = { 2, 2, { x + 8 }};
    float matProducts[12];
    NLSLmatrix_t MatX0MatX1 = { 2, 2, { matProducts }};
    NLSLmatrix_t MatX1MatX2 = { 2, 2, { matProducts + 4 }};
    NLSLmatrix_t MatX2MatX0 = { 2, 2, { matProducts + 8 }};
    float matYRef[12];
    NLSLmatrix_t MatY0Ref = { 2, 2, { params }};
    NLSLmatrix_t MatY1Ref = { 2, 2, { params + 4 }};
    NLSLmatrix_t MatY2Ref = { 2, 2, { params + 8 }};
    NLSLmatrix_t MatResult0 = { 2, 2, { y }};
    NLSLmatrix_t MatResult1 = { 2, 2, { y + 4 }};
    NLSLmatrix_t MatResult2 = { 2, 2, { y + 8 }};
    NLSLmatrix_mult(&MatX0, &MatX1, &MatX0MatX1);
    NLSLmatrix_mult(&MatX1, &MatX2, &MatX1MatX2);
    NLSLmatrix_mult(&MatX2, &MatX0, &MatX2MatX0);
    NLSLmatrix_sub(&MatX0MatX1, &MatY0Ref, &MatResult0);
    NLSLmatrix_sub(&MatX1MatX2, &MatY1Ref, &MatResult1);
    NLSLmatrix_sub(&MatX2MatX0, &MatY2Ref, &MatResult2);
    return 0;
}

/**
 * @brief The No.0 (1st) column of Jacobian
 */
static int J_0(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[0] = x1[0]; y[1] = x1[1]; y[8] = x2[0]; y[10] = x2[2];
    return 0;
}

/**
 * @brief The No.1 (2nd) column of Jacobian
 */
static int J_1(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[0] = x1[2]; y[1] = x1[3]; y[9] = x2[0]; y[11] = x2[2];
    return 0;
}

/**
 * @brief The No.2 (3rd) column of Jacobian
 */
static int J_2(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[2] = x1[0]; y[3] = x1[1]; y[8] = x2[1]; y[10] = x2[3];
    return 0;
}

/**
 * @brief The No.3 (4th) column of Jacobian
 */
static int J_3(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[2] = x1[2]; y[3] = x1[3]; y[9] = x2[1]; y[11] = x2[3];
    return 0;
}

static int J_4(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[0] = x[0]; y[2] = x[2]; y[4] = x2[0]; y[5] = x2[1];
    return 0;
}

static int J_5(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[1] = x[0]; y[3] = x[2]; y[4] = x2[2]; y[5] = x2[3];
    return 0;
}

static int J_6(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[0] = x[1]; y[2] = x[3]; y[6] = x2[0]; y[7] = x2[1];
    return 0;
}

static int J_7(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[1] = x[1]; y[3] = x[3]; y[6] = x2[2]; y[7] = x2[3];
    return 0;
}

static int J_8(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[4] = x1[0]; y[6] = x1[2]; y[8] = x[0]; y[9] = x[1];
    return 0;
}

static int J_9(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[5] = x1[0]; y[7] = x1[2]; y[8] = x[2]; y[9] = x[3];
    return 0;
}

static int J10(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[4] = x1[1]; y[6] = x1[3]; y[10] = x[0]; y[11] = x[1];
    return 0;
}

static int J11(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    NLSL_FILLFLOATS(y, 0.0f, cy);
    const float *x1 = x + 4;
    const float *x2 = x1 + 4;
    y[5] = x1[1]; y[7] = x1[3]; y[10] = x[2]; y[11] = x[3];
    return 0;
}

static const NLSL_OBJECTIVE JacobianColumns[] = { J_0, J_1, J_2, J_3, J_4, J_5, J_6, J_7, J_8, J_9, J10, J11 };
#pragma endregion objective_and_jacobian
#pragma region InitParams
static const float xInit[] = {
    1.0f, 0.1f, 1.0f, 0.5f,
    1.0f, 0.7f, 1.0f, 0.3f,
    1.0f, 0.1f, 1.0f, 0.2f
};
static const float xEps[] = {
    1.0e-4f, 1.0e-4f, 1.0e-4f, 1.0e-4f, 
    1.0e-4f, 1.0e-4f, 1.0e-4f, 1.0e-4f,
    1.0e-4f, 1.0e-4f, 1.0e-4f, 1.0e-4f
};

static const float xRef[] = {
    -1.5f, -0.5f, 0.5f, 1.2f,
    0.3f, 0.7f, -2.2f, 0.0f,
    1.5f, 1.0f, 0.0f, -2.1f
};

static float yRef[12];

static void InitYRef()
{
    NLSLmatrix_t x0 = { 2, 2, { xRef }};
    NLSLmatrix_t x1 = { 2, 2, { xRef + 4 }};
    NLSLmatrix_t x2 = { 2, 2, { xRef + 8 }};
    NLSLmatrix_t y0 = { 2, 2, { yRef }};
    NLSLmatrix_t y1 = { 2, 2, { yRef + 4 }};
    NLSLmatrix_t y2 = { 2, 2, { yRef + 8 }};
    NLSLmatrix_mult(&x0, &x1, &y0);
    NLSLmatrix_mult(&x1, &x2, &y1);
    NLSLmatrix_mult(&x2, &x0, &y2);
}

#pragma endregion InitParams
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
        InitYRef();
        if ((pNLSLgnsolver_t)NULL == (solver = NLSLgnsolver_new(12, 12, 0)))
        {
            fprintf(stderr, "fail in NLSLgnsolver_new(12, 12, 12) @ %s, %d\n", __FILE__, __LINE__);
            break;
        }
        memcpy(NLSLgnsolver_jacobian(solver), JacobianColumns, sizeof(JacobianColumns));
        pNLSLsolverconf_t conf = NLSLgnsolver_conf(solver);
        memcpy(conf->xInitial, xInit, sizeof(xInit));
        memcpy(conf->xEps, xEps, sizeof(xEps));
        conf->objective = objective;
        conf->trace = stderr;
        conf->thObjective = 1.0e-6f;
        memcpy(conf->params, yRef, conf->cp * sizeof(float));
        NLSLgnsolver_init(solver);
        err = NLSLgnsolver_exec(solver, 2);
        if (err)
        {
            fprintf(stderr, "fail in NLSLgnsolver_new(12, 12, 0) @ %s, %d\n", __FILE__, __LINE__);
        }
        ShowX(solver);
    } while (0);
    NLSLgnsolver_delete(&solver);
    return err;
}