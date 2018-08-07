/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nonSingletonDim.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:27:19
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_step_num.h"
#include "nonSingletonDim.h"

/* Function Definitions */

/*
 * Arguments    : const int x_size[1]
 * Return Type  : int
 */
int nonSingletonDim(const int x_size[1])
{
  int dim;
  dim = 2;
  if (x_size[0] != 1) {
    dim = 1;
  }

  return dim;
}

/*
 * File trailer for nonSingletonDim.c
 *
 * [EOF]
 */
