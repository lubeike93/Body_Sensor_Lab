/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: exp.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "exp.h"

/* Function Definitions */

/*
 * Arguments    : double x_data[]
 *                int x_size[2]
 * Return Type  : void
 */
void b_exp(double x_data[], int x_size[2])
{
  int nx;
  int k;
  nx = x_size[1];
  for (k = 0; k + 1 <= nx; k++) {
    x_data[k] = exp(x_data[k]);
  }
}

/*
 * File trailer for exp.c
 *
 * [EOF]
 */
