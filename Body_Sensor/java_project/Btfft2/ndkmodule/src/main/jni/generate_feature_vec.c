/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: generate_feature_vec.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "generate_feature_vec.h"
#include "std.h"
#include "mean.h"

/* Function Definitions */

/*
 * GENERATE_FEATURE_VEC extracts feature values from data sequence
 * Arguments    : const double triaxial_data[]
 *                const int triaxial_size[2]
 *                const double svm_data[]
 *                const int svm_size[1]
 *                double feature_vec[16]
 * Return Type  : void
 */
void generate_feature_vec(const double triaxial_data[], const int triaxial_size
  [2], const double svm_data[], const int svm_size[1], double feature_vec[16])
{
  double meanSVMAcc;
  int ixstart;
  int b_triaxial_size[1];
  int n;
  double meanXAcc;
  double b_triaxial_data[64];
  int c_triaxial_size[1];
  double meanYAcc;
  int d_triaxial_size[1];
  double meanZAcc;
  double mtmp;
  int ix;
  boolean_T exitg1;
  double b_mtmp;
  double c_mtmp;
  double d_mtmp;
  double e_mtmp;
  double f_mtmp;
  double g_mtmp;
  double h_mtmp;
  double stdSVMAcc;
  int e_triaxial_size[1];
  double stdXAcc;
  int f_triaxial_size[1];
  double stdYAcc;
  int g_triaxial_size[1];
  double stdZAcc;
  meanSVMAcc = b_mean(svm_data, svm_size);

  /*  Mean svm acceleration */
  ixstart = triaxial_size[0];
  b_triaxial_size[0] = triaxial_size[0];
  for (n = 0; n < ixstart; n++) {
    b_triaxial_data[n] = triaxial_data[n];
  }

  meanXAcc = b_mean(b_triaxial_data, b_triaxial_size);

  /*  Mean x acceleration */
  ixstart = triaxial_size[0];
  c_triaxial_size[0] = triaxial_size[0];
  for (n = 0; n < ixstart; n++) {
    b_triaxial_data[n] = triaxial_data[n + triaxial_size[0]];
  }

  meanYAcc = b_mean(b_triaxial_data, c_triaxial_size);

  /*  Mean y acceleration */
  ixstart = triaxial_size[0];
  d_triaxial_size[0] = triaxial_size[0];
  for (n = 0; n < ixstart; n++) {
    b_triaxial_data[n] = triaxial_data[n + (triaxial_size[0] << 1)];
  }

  meanZAcc = b_mean(b_triaxial_data, d_triaxial_size);

  /*  Mean z acceleration */
  ixstart = 1;
  n = svm_size[0];
  mtmp = svm_data[0];
  if (svm_size[0] > 1) {
    if (rtIsNaN(svm_data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(svm_data[ix - 1])) {
          mtmp = svm_data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < svm_size[0]) {
      while (ixstart + 1 <= n) {
        if (svm_data[ixstart] > mtmp) {
          mtmp = svm_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = triaxial_size[0];
  b_mtmp = triaxial_data[0];
  if (triaxial_size[0] > 1) {
    if (rtIsNaN(triaxial_data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(triaxial_data[ix - 1])) {
          b_mtmp = triaxial_data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < triaxial_size[0]) {
      while (ixstart + 1 <= n) {
        if (triaxial_data[ixstart] > b_mtmp) {
          b_mtmp = triaxial_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = triaxial_size[0];
  c_mtmp = triaxial_data[triaxial_size[0]];
  if (triaxial_size[0] > 1) {
    if (rtIsNaN(triaxial_data[triaxial_size[0]])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(triaxial_data[(ix + triaxial_size[0]) - 1])) {
          c_mtmp = triaxial_data[(ix + triaxial_size[0]) - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < triaxial_size[0]) {
      while (ixstart + 1 <= n) {
        if (triaxial_data[ixstart + triaxial_size[0]] > c_mtmp) {
          c_mtmp = triaxial_data[ixstart + triaxial_size[0]];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = triaxial_size[0];
  d_mtmp = triaxial_data[triaxial_size[0] << 1];
  if (triaxial_size[0] > 1) {
    if (rtIsNaN(triaxial_data[triaxial_size[0] << 1])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(triaxial_data[(ix + (triaxial_size[0] << 1)) - 1])) {
          d_mtmp = triaxial_data[(ix + (triaxial_size[0] << 1)) - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < triaxial_size[0]) {
      while (ixstart + 1 <= n) {
        if (triaxial_data[ixstart + (triaxial_size[0] << 1)] > d_mtmp) {
          d_mtmp = triaxial_data[ixstart + (triaxial_size[0] << 1)];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = svm_size[0];
  e_mtmp = svm_data[0];
  if (svm_size[0] > 1) {
    if (rtIsNaN(svm_data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(svm_data[ix - 1])) {
          e_mtmp = svm_data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < svm_size[0]) {
      while (ixstart + 1 <= n) {
        if (svm_data[ixstart] < e_mtmp) {
          e_mtmp = svm_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = triaxial_size[0];
  f_mtmp = triaxial_data[0];
  if (triaxial_size[0] > 1) {
    if (rtIsNaN(triaxial_data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(triaxial_data[ix - 1])) {
          f_mtmp = triaxial_data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < triaxial_size[0]) {
      while (ixstart + 1 <= n) {
        if (triaxial_data[ixstart] < f_mtmp) {
          f_mtmp = triaxial_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = triaxial_size[0];
  g_mtmp = triaxial_data[triaxial_size[0]];
  if (triaxial_size[0] > 1) {
    if (rtIsNaN(triaxial_data[triaxial_size[0]])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(triaxial_data[(ix + triaxial_size[0]) - 1])) {
          g_mtmp = triaxial_data[(ix + triaxial_size[0]) - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < triaxial_size[0]) {
      while (ixstart + 1 <= n) {
        if (triaxial_data[ixstart + triaxial_size[0]] < g_mtmp) {
          g_mtmp = triaxial_data[ixstart + triaxial_size[0]];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = triaxial_size[0];
  h_mtmp = triaxial_data[triaxial_size[0] << 1];
  if (triaxial_size[0] > 1) {
    if (rtIsNaN(triaxial_data[triaxial_size[0] << 1])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(triaxial_data[(ix + (triaxial_size[0] << 1)) - 1])) {
          h_mtmp = triaxial_data[(ix + (triaxial_size[0] << 1)) - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < triaxial_size[0]) {
      while (ixstart + 1 <= n) {
        if (triaxial_data[ixstart + (triaxial_size[0] << 1)] < h_mtmp) {
          h_mtmp = triaxial_data[ixstart + (triaxial_size[0] << 1)];
        }

        ixstart++;
      }
    }
  }

  stdSVMAcc = b_std(svm_data, svm_size);
  ixstart = triaxial_size[0];
  e_triaxial_size[0] = triaxial_size[0];
  for (n = 0; n < ixstart; n++) {
    b_triaxial_data[n] = triaxial_data[n];
  }

  stdXAcc = b_std(b_triaxial_data, e_triaxial_size);
  ixstart = triaxial_size[0];
  f_triaxial_size[0] = triaxial_size[0];
  for (n = 0; n < ixstart; n++) {
    b_triaxial_data[n] = triaxial_data[n + triaxial_size[0]];
  }

  stdYAcc = b_std(b_triaxial_data, f_triaxial_size);
  ixstart = triaxial_size[0];
  g_triaxial_size[0] = triaxial_size[0];
  for (n = 0; n < ixstart; n++) {
    b_triaxial_data[n] = triaxial_data[n + (triaxial_size[0] << 1)];
  }

  stdZAcc = b_std(b_triaxial_data, g_triaxial_size);
  feature_vec[0] = meanSVMAcc;
  feature_vec[1] = meanXAcc;
  feature_vec[2] = meanYAcc;
  feature_vec[3] = meanZAcc;
  feature_vec[4] = mtmp;
  feature_vec[5] = b_mtmp;
  feature_vec[6] = c_mtmp;
  feature_vec[7] = d_mtmp;
  feature_vec[8] = e_mtmp;
  feature_vec[9] = f_mtmp;
  feature_vec[10] = g_mtmp;
  feature_vec[11] = h_mtmp;
  feature_vec[12] = stdSVMAcc;
  feature_vec[13] = stdXAcc;
  feature_vec[14] = stdYAcc;
  feature_vec[15] = stdZAcc;

  /* feature_vec = feature_vec'; */
}

/*
 * File trailer for generate_feature_vec.c
 *
 * [EOF]
 */
