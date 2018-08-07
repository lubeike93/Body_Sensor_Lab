/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nullAssignment.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "nullAssignment.h"

/* Function Definitions */

/*
 * Arguments    : double x_data[]
 *                int x_size[2]
 *                const boolean_T idx_data[]
 *                const int idx_size[2]
 * Return Type  : void
 */
void nullAssignment(double x_data[], int x_size[2], const boolean_T idx_data[],
                    const int idx_size[2])
{
  int nxin;
  int nxout;
  int k;
  int k0;
  nxin = x_size[1];
  nxout = 0;
  for (k = 1; k <= idx_size[1]; k++) {
    nxout += idx_data[k - 1];
  }

  nxout = x_size[1] - nxout;
  k0 = -1;
  for (k = 1; k <= nxin; k++) {
    if ((k > idx_size[1]) || (!idx_data[k - 1])) {
      k0++;
      x_data[k0] = x_data[k - 1];
    }
  }

  if (1 > nxout) {
    x_size[1] = 0;
  } else {
    x_size[1] = nxout;
  }
}

/*
 * File trailer for nullAssignment.c
 *
 * [EOF]
 */
