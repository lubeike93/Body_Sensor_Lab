/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: conv.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "conv.h"

/* Function Definitions */

/*
 * Arguments    : const double A_data[]
 *                const int A_size[1]
 *                const double B_data[]
 *                const int B_size[2]
 *                double C_data[]
 *                int C_size[1]
 * Return Type  : void
 */
void conv(const double A_data[], const int A_size[1], const double B_data[],
          const int B_size[2], double C_data[], int C_size[1])
{
  int nA;
  int iC0;
  int iA0;
  int nBd2;
  int k;
  int n;
  int b_k;
  nA = A_size[0] - 2;
  C_size[0] = A_size[0];
  iC0 = A_size[0];
  for (iA0 = 0; iA0 < iC0; iA0++) {
    C_data[iA0] = 0.0;
  }

  if ((A_size[0] > 0) && (B_size[1] > 0)) {
    nBd2 = B_size[1] >> 1;
    for (k = 1; k <= nBd2; k++) {
      iA0 = nBd2 - k;
      n = nA - iA0;
      for (b_k = 0; b_k <= n; b_k++) {
        C_data[b_k] += B_data[k - 1] * A_data[(iA0 + b_k) + 1];
      }
    }

    for (k = nBd2; k + 1 <= B_size[1]; k++) {
      iC0 = k - nBd2;
      n = nA - iC0;
      for (b_k = 0; b_k <= n + 1; b_k++) {
        C_data[iC0 + b_k] += B_data[k] * A_data[b_k];
      }
    }
  }
}

/*
 * File trailer for conv.c
 *
 * [EOF]
 */
