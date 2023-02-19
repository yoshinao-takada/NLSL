#include "NLSLgnsolver.h"
#include "NLSLutils.h"
#include "NLSLmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <math.h>

//#define BAD_APPROX
//#define GOOD_APPROX

typedef struct {
    float r, cth, sth, cph, sph; // r, cos(theta), sin(theta), cos(phi), sin(phi)
} polar_components_t;

typedef union {
    float v[5];
    polar_components_t polar;
} objective_args_t;

#pragma region objective_function
static float unit_circle_condition(float x, float y)
{
    return x*x + y*y - 1.0f;
}

static int objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert((cx == 5) && (cy == 5) && (cp == 3));
    const objective_args_t *args = (const objective_args_t *)x;
    const polar_components_t *p = &args->polar;
    y[0] = p->r * p->cth * p->cph - params[0];
    y[1] = p->r * p->cth * p->sph - params[1];
    y[2] = p->r * p->sth - params[2];
    y[3] = unit_circle_condition(p->cth, p->sth);
    y[4] = unit_circle_condition(p->cph, p->sph);
    return 0;
}
#pragma endregion objective_function

#pragma region Jacobian
static int j0(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert((cx == 5) && (cy == 5) && (cp == 3));
    const objective_args_t *args = (const objective_args_t *)x;
    const polar_components_t *p = &args->polar;
    y[0] = p->cth * p->cph;
    y[1] = p->cth * p->sph;
    y[2] = p->sth;
    y[3] = y[4] = 0.0f;
    return 0;    
}

static int j1(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert((cx == 5) && (cy == 5) && (cp == 3));
    const objective_args_t *args = (const objective_args_t *)x;
    const polar_components_t *p = &args->polar;
    y[0] = p->r * p->cph;
    y[1] = p->r * p->sph;
    y[2] = y[3] = y[4] = 0.0f;
    return 0;    
}

static int j2(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert((cx == 5) && (cy == 5) && (cp == 3));
    const objective_args_t *args = (const objective_args_t *)x;
    const polar_components_t *p = &args->polar;
    y[2] = p->r;
    y[3] = 2.0f * p->sth;
    y[0] = y[1] = y[4] = 0.0f;
    return 0;    
}

static int j3(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert((cx == 5) && (cy == 5) && (cp == 3));
    const objective_args_t *args = (const objective_args_t *)x;
    const polar_components_t *p = &args->polar;
    y[0] = p->r * p->cth;
    y[4] = 2.0f * p->cph;
    y[1] = y[2] = y[3] = 0.0f;
    return 0;    
}

static int j4(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert((cx == 5) && (cy == 5) && (cp == 3));
    const objective_args_t *args = (const objective_args_t *)x;
    const polar_components_t *p = &args->polar;
    y[1] = p->r * p->cth;
    y[4] = 2.0f * p->sph;
    y[0] = y[2] = y[3] = 0.0f;
    return 0;    
}

static const NLSL_OBJECTIVE JacobianSource[] = { j0, j1, j2, j3, j4 };
#pragma endregion Jacobian

#pragma region Program_operation
static const float cartesian_ref[] = { 1.0f, 1.5f, 2.2f }; // reference cartesian coordinate
static const int cp = NLSL_ARRAYSIZE(cartesian_ref);
static float xInit[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static const float xEps[] = { 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f };
static const float th = 1.0e-8f;

float noise() 
{
    static const float offset = 0.5f * (float)RAND_MAX;
    static const float normalizer = 2.0f/(float)RAND_MAX;
    return 0.01f * ((float)rand() - offset) * normalizer;
}

void ApproxInit()
{
    #if defined(BAD_APPROX)
    xInit[0] = xInit[1] = xInit[3]= 1.0f;
    #else
    polar_components_t* polar = (polar_components_t*)xInit;
    float Rxy = sqrtf(cartesian_ref[0]*cartesian_ref[0] + cartesian_ref[1]*cartesian_ref[1]);
    polar->r = sqrtf(cartesian_ref[0]*cartesian_ref[0] + cartesian_ref[1]*cartesian_ref[1] + cartesian_ref[2]*cartesian_ref[2]);
    polar->sth = cartesian_ref[2]/polar->r;
    
    polar->cth = Rxy/polar->r;
    polar->sph = cartesian_ref[1]/Rxy;
    polar->cph = cartesian_ref[0]/Rxy;
    #endif
    #if !defined(GOOD_APPROX)
    for (int index = 0; index < NLSL_ARRAYSIZE(xInit); index++)
    {
        xInit[index] += noise();
    }
    #endif
}

void ShowX(pNLSLgnsolver_t solver)
{
    float x, y, z;
    const polar_components_t *polar = (const polar_components_t *)NLSLgnsolver_xfinal(solver);
    fprintf(stderr, "r = %f\n", polar->r);
    fprintf(stderr, "cos(theta) = %f, sin(theta) = %f\n", polar->cth, polar->sth);
    fprintf(stderr, "cos(phi) = %f, sin(phi) = %f\n", polar->cph, polar->sph);
    x = polar->r * polar->cth * polar->cph;
    y = polar->r * polar->cth * polar->sph;
    z = polar->r * polar->sth;
    fprintf(stderr, "(x, y, z) = (%f, %f, %f)\n", x, y, z);
}

void ShowDeltaX(pNLSLgnsolver_t solver)
{
    const float* deltax = NLSLgnsolver_deltaxfinal(solver);
    int cx = NLSLgnsolver_conf(solver)->cx;
    fprintf(stderr, "Delta X\n");
    for (int i = 0; i < cx; i++)
    {
        fprintf(stderr, "Deilta X[%d] = %f\n", i, deltax[i]);
    }
}
int main()
{
    int err = EXIT_SUCCESS;
    pNLSLgnsolver_t solver = (pNLSLgnsolver_t)NULL;
    do {
        ApproxInit();
        solver = NLSLgnsolver_new(5, 5, 3);
        memcpy(NLSLgnsolver_jacobian(solver), JacobianSource, sizeof(JacobianSource));
        pNLSLsolverconf_t conf = NLSLgnsolver_conf(solver);
        memcpy(conf->xInitial, xInit, sizeof(xInit));
        memcpy(conf->xEps, xEps, sizeof(xEps));
        memcpy(conf->params, cartesian_ref, sizeof(cartesian_ref));
        conf->thObjective = th;
        conf->trace = stderr;
        conf->objective = objective;
        NLSLgnsolver_init(solver);
        NLSLgnsolver_exec(solver, 30);
        ShowX(solver);
        fprintf(stderr, "status = %d\n", NLSLgnsolver_status(solver));
        ShowDeltaX(solver);
    } while (0);
    NLSLgnsolver_delete(&solver);
}
#pragma endregion Program_operation
