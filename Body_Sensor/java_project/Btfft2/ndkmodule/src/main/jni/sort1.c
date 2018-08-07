/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sort1.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "sort1.h"
#include "sortIdx.h"

/* Type Definitions */
#ifndef struct_emxArray_int32_T_128
#define struct_emxArray_int32_T_128

struct emxArray_int32_T_128
{
  int data[128];
  int size[1];
};

#endif                                 /*struct_emxArray_int32_T_128*/

#ifndef typedef_emxArray_int32_T_128
#define typedef_emxArray_int32_T_128

typedef struct emxArray_int32_T_128 emxArray_int32_T_128;

#endif                                 /*typedef_emxArray_int32_T_128*/

/* Function Definitions */

/*
 * Arguments    : int x_data[]
 *                int x_size[1]
 * Return Type  : void
 */
void sort(int x_data[], int x_size[1])
{
  int dim;
  int i6;
  int vwork_size[1];
  int vstride;
  int k;
  int vwork_data[128];
  emxArray_int32_T_128 b_vwork_data;
  dim = 2;
  if (x_size[0] != 1) {
    dim = 1;
  }

  if (dim <= 1) {
    i6 = x_size[0];
  } else {
    i6 = 1;
  }

  vwork_size[0] = (unsigned char)i6;
  vstride = 1;
  k = 1;
  while (k <= dim - 1) {
    vstride *= x_size[0];
    k = 2;
  }

  for (dim = 0; dim + 1 <= vstride; dim++) {
    for (k = 0; k + 1 <= i6; k++) {
      vwork_data[k] = x_data[dim + k * vstride];
    }

    b_sortIdx(vwork_data, vwork_size, b_vwork_data.data, b_vwork_data.size);
    for (k = 0; k + 1 <= i6; k++) {
      x_data[dim + k * vstride] = vwork_data[k];
    }
  }
}

/*
 * File trailer for sort1.c
 *
 * [EOF]
 */
