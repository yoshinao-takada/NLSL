#include "NLSLgnsolver.h"
#include "NLSLutils.h"
#include "NLSLmatrix.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#pragma region TestData_Definitions
static float pk[4][2] = {
    { 100.0f, 100.0f }, { 220.0f, 100.0f }, { 100.0f, 200.0f }, { 0.0f, 0.0f }
};

static float pj[4][3] = {
    { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }
};

static void FillLastPoints()
{
    float pkCenter[] = { pk[1][0] + pk[2][0], pk[1][1] + pk[2][1] };
    float pkCenter_P0[] = { pkCenter[0] - pk[0][0], pkCenter[1] - pk[0][1] };
    float pk3[] = { pkCenter[0] + pkCenter_P0[0], pkCenter[1] + pkCenter_P0[1] };
    pk[3][0] = pk3[0];
    pk[3][1] = pk3[1];

    float pjCenter[] = { pj[1][0] + pj[2][0], pj[1][1] + pj[2][1], pj[1][2] + pj[2][2] };
    float pjCenter_P0[] = {
        pjCenter[0] - pj[0][0], pjCenter[1] - pj[0][1], pjCenter[2] - pj[0][2]
    };
    float pj3[] = { pjCenter[0] + pjCenter_P0[0], pjCenter[1] + pjCenter_P0[1], pjCenter[2] + pjCenter_P0[2] };
    pj[3][0] = pj3[0];
    pj[3][1] = pj3[1];
    pj[3][2] = pj3[2];
}

/**
 * @brief fill 8x9 matrix of objective function parameter
 */
static void ObjectiveParamMatrix(float* m)
{
    float* mRowHead = m;
    FillLastPoints();
    NLSL_FILLFLOATS(mRowHead, 0.0f, 72);
    mRowHead[0] = pj[0][0]; mRowHead[1] = pj[0][1]; mRowHead[2] = pj[0][2];
    mRowHead[6] = -pk[0][0] * pj[0][0]; mRowHead[7] = -pk[0][0] * pj[0][1]; mRowHead[8] = -pk[0][0] * pj[0][2];
    mRowHead += 9;
    mRowHead[3] = pj[0][0]; mRowHead[4] = pj[0][1]; mRowHead[5] = pj[0][2]; 
    mRowHead[6] = -pk[0][1] * pj[0][0]; mRowHead[7] = -pk[0][1] * pj[0][1]; mRowHead[8] = -pk[0][1] * pj[0][2];
    mRowHead += 9;
    mRowHead[0] = pj[1][0]; mRowHead[1] = pj[1][1]; mRowHead[2] = pj[1][2];
    mRowHead[6] = -pk[1][0] * pj[1][0]; mRowHead[7] = -pk[1][0] * pj[1][1]; mRowHead[8] = -pk[1][0] * pj[1][2];
    mRowHead += 9;
    mRowHead[3] = pj[1][0]; mRowHead[4] = pj[1][1]; mRowHead[5] = pj[1][2]; 
    mRowHead[6] = -pk[1][1] * pj[1][0]; mRowHead[7] = -pk[1][1] * pj[1][1]; mRowHead[8] = -pk[1][1] * pj[1][2];
    mRowHead += 9;
    mRowHead[0] = pj[2][0]; mRowHead[1] = pj[2][1]; mRowHead[2] = pj[2][2];
    mRowHead[6] = -pk[2][0] * pj[2][0]; mRowHead[7] = -pk[2][0] * pj[2][1]; mRowHead[8] = -pk[2][0] * pj[2][2];
    mRowHead += 9;
    mRowHead[3] = pj[2][0]; mRowHead[4] = pj[2][1]; mRowHead[5] = pj[2][2]; 
    mRowHead[6] = -pk[2][1] * pj[2][0]; mRowHead[7] = -pk[2][1] * pj[2][1]; mRowHead[8] = -pk[2][1] * pj[2][2];
    mRowHead += 9;
    mRowHead[0] = pj[3][0]; mRowHead[1] = pj[3][1]; mRowHead[2] = pj[3][2];
    mRowHead[6] = -pk[3][0] * pj[3][0]; mRowHead[7] = -pk[3][0] * pj[3][1]; mRowHead[8] = -pk[3][0] * pj[3][2];
    mRowHead += 9;
    mRowHead[3] = pj[3][0]; mRowHead[4] = pj[3][1]; mRowHead[5] = pj[3][2]; 
    mRowHead[6] = -pk[3][1] * pj[3][0]; mRowHead[7] = -pk[3][1] * pj[3][1]; mRowHead[8] = -pk[3][1] * pj[3][2];
    NLSLmatrix_t mat = { 8, 9, { m }};
    NLSLmatrix_t matT = { 9, 8, { m + 72 }};
    NLSLmatrix_transpose(&mat, &matT);
}

#pragma endregion TestData_Definitions

#pragma region ObjectiveAndJacobian
/**
 * @brief Objective function
 * 
 * @param cx 
 * @param x 
 * @param cy 
 * @param y 
 * @param cp 
 * @param params 
 * @return int 
 */
static int Objective(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    assert(cp == (72  + 72));
    int err = 0;
    float work[8];
    NLSLmatrix_t m = { 8, 9, { params }};
    NLSLmatrix_t h = { 9, 1, { x }};
    NLSLmatrix_t ht = { 1, 9, { x }};
    NLSLmatrix_t mh = { 8, 1, { y }};
    NLSLmatrix_t hh = { 1, 1, { y + 8 }};
    NLSLmatrix_mult(&m, &h, &mh);
    NLSLmatrix_mult(&ht, &h, &hh);
    hh.elements.v[0] -= 1.0f;
    return err;
}

#pragma region Jacobian
static int J_0(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 0;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_1(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 1;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_2(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 2;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_3(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 3;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_4(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 4;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_5(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 5;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_6(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 6;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_7(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 7;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static int J_8(int cx, const float* x, int cy, float* y, int cp, const float* params)
{
    const int row = 8;
    int err = 0;
    assert(cp == (72  + 72));
    const float *mT_row = params + 72 + row * 8; // transpose(m), corresponding row
    memcpy(y, mT_row, 8 * sizeof(float));
    y[8] = 2 * x[row];
    return err;
}

static const NLSL_OBJECTIVE jacobian_colums[] = { J_0, J_1, J_2, J_3, J_4, J_5, J_6, J_7, J_8 };
#pragma endregion Jacobian
#pragma endregion ObjectiveAndJacobian

// Initial values given to the solver
static const float xInit[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
static const float xEps[] = { 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f, 1.0e-5f };

static void ShowResults(pcNLSLgnsolver_t solver)
{
    static const char* status_labels[] = {
        "iter_limit", "xconverged", "yconverged", "errorabort", "notrun", "running"
    };
    const float* xfinal = NLSLgnsolver_xfinal(solver);
    const float* deltaxfinal = NLSLgnsolver_deltaxfinal(solver);
    printf("status = %s\n", status_labels[(int)NLSLgnsolver_status(solver)]);
    printf("xfinal: ");
    for (int i = 0; i < 9; i++)
    {
        printf("%c%f", (i == 0) ? ' ' : ',', xfinal[i]);
    }
    printf("\ndeltaxfinal: ");
    for (int i = 0; i < 9; i++)
    {
        printf("%c%f", (i == 0) ? ' ' : ',', deltaxfinal[i]);
    }
    printf("\n");
}

/**
 * @brief Using the retrieved homography matrix, convert pj[*] to their correspondences.
 *  Compare them with the original pk[*] to verify the homography matrix.
 * 
 * @param solver [in] solver instance after the equation was solved. The solver should have a valid homography matrix
 *  elements as its solution.
 */
static void VerifyResults(pcNLSLgnsolver_t solver)
{
    float pkwork[3];
    NLSLmatrix_t h = { 3, 3, { NLSLgnsolver_xfinal(solver)}}; // homography matrix
    for (int iCorner = 0; iCorner != NLSL_ARRAYSIZE(pj); iCorner++)
    {
        NLSLmatrix_t pj_ = { 3, 1, { pj[iCorner] }};
        NLSLmatrix_t pk_ = { 3, 1, { pkwork }};
        NLSLmatrix_mult(&h, &pj_, &pk_);
        pkwork[0] /= pkwork[2];
        pkwork[1] /= pkwork[2];
        printf("pk[%d]=(%f, %f), pkwork[%d]=(%f, %f)\n", iCorner, pk[iCorner][0], pk[iCorner][1],
            iCorner, pkwork[0], pkwork[1]);
    }
}

int main()
{
    int err = EXIT_SUCCESS;
    pNLSLgnsolver_t solver = NULL;
    do {
        // Step 1: Initialize a Gauss-Newton solver
        solver = NLSLgnsolver_new(9, 9, 72 + 72);
        // Step 1.1: Set Jacobian callbacks
        memcpy(NLSLgnsolver_jacobian(solver), jacobian_colums, sizeof(jacobian_colums));
        // Step 1.2: Set other configuration parameters
        pNLSLsolverconf_t conf = NLSLgnsolver_conf(solver);
        memcpy(conf->xInitial, xInit, sizeof(xInit)); // x initial value
        memcpy(conf->xEps, xEps, sizeof(xEps)); // delta-x limit between iterations
        conf->thObjective = 1.0e-7f; // threshold of innerproducts of equation value vectors
        conf->trace = stderr; // debug trace stream
        conf->objective = Objective; // objective function
        ObjectiveParamMatrix(conf->params); // setting four points in 3D world space and in 2D projected plane

        // Step 2: Execute calculation
        NLSLgnsolver_init(solver); // update internal state of the solver
        int err = NLSLgnsolver_exec(solver, 8); // iterative operation

        // Step 3: Verify results
        ShowResults(solver);
        VerifyResults(solver);
    } while (0);
    NLSLgnsolver_delete(&solver);
    return err;
}