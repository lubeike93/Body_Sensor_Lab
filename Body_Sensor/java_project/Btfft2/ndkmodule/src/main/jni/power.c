/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: power.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "power.h"

/* Function Definitions */

/*
 * Arguments    : const double a_data[]
 *                const int a_size[2]
 *                double y_data[]
 *                int y_size[2]
 * Return Type  : void
 */
void power(const double a_data[], const int a_size[2], double y_data[], int
           y_size[2])
{
  int k;
  y_size[0] = 1;
  y_size[1] = (signed char)a_size[1];
  for (k = 0; k + 1 <= a_size[1]; k++) {
    y_data[k] = a_data[k] * a_data[k];
  }
}

/*
 * File trailer for power.c
 *
 * [EOF]
 */
