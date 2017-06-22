#include <iostream>
#include <fstream>
#include <math.h>

#include "eiquadprog.hpp"
#include "mex.h"

using namespace std;
using namespace Eigen;


enum eiqpStatus
{
    EIQP_OK = 0,
    EIQP_INFEASIBLE = 1,
    EIQP_FAILED = 2
};



void mexFunction( int num_output, mxArray *output[], int num_input, const mxArray *input[] )
{
    const mxArray *H = input[0];
    const mxArray *g = input[1];
    const mxArray *A = input[2];
    const mxArray *b = input[3];
    const mxArray *Ain = input[4];
    const mxArray *bin = input[5];


    mxArray *x = NULL;


    int num_var = mxGetM(H);
    int num_eq = mxGetM(A);
    int num_ineq = mxGetM(Ain);
    x = mxCreateDoubleMatrix(num_var, 1, mxREAL);


    MatrixXd eH     = Map<MatrixXd>  ((double*) mxGetPr(H),   num_var, num_var);
    VectorXd eg     = Map<VectorXd>  ((double*) mxGetPr(g),   num_var);
    MatrixXd eA     = Map<MatrixXd>  ((double*) mxGetPr(A),   num_eq, num_var);
    VectorXd eb     = Map<VectorXd>  ((double*) mxGetPr(b),   num_eq);
    MatrixXd eAin   = Map<MatrixXd>  ((double*) mxGetPr(Ain), num_ineq, num_var);
    VectorXd ebin   = Map<VectorXd>  ((double*) mxGetPr(bin), num_ineq);
    VectorXd ex     = Map<VectorXd>  ((double*) mxGetPr(x),   num_var);


// solve the problem
    eiqpStatus eiqp_status;

    double qp_status = solve_quadprog(eH, eg, eA.transpose(), eb, eAin.transpose(), ebin, ex);
    Map<VectorXd>  ((double*) mxGetPr(x), num_var) = ex;

    if (qp_status == numeric_limits<double>::infinity())
    {
        eiqp_status = EIQP_INFEASIBLE;
    }
    else
    {
        if (isnan(qp_status))
        {
            eiqp_status = EIQP_FAILED;
        }
        else
        {
            eiqp_status = EIQP_OK;
        }
    }


// process results
    // solution
    output[0] = mxDuplicateArray(x);


    // info
    int num_info_fields = 2;
    const char *info_field_names[] = {
        "status",
        "obj"
    };

    output[1] = mxCreateStructMatrix(1, 1, num_info_fields, info_field_names);

    mxArray *info_status = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    ((INT32_T *) mxGetData (info_status))[0] = static_cast <int> (eiqp_status);
    mxSetField (output[1], 0, "status", info_status);

    mxArray *info_obj = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
    ((double *) mxGetData (info_obj))[0] = qp_status;
    mxSetField (output[1], 0, "obj", info_obj);

    return;
}
