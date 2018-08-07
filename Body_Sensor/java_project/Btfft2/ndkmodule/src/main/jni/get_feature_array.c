/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: get_feature_array.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "gaussfilt.h"
#include "generate_feature_vec.h"
#include "mean.h"
#include "findpeaks.h"
#include "triaxial2svm.h"

/* Function Definitions */

/*
 * GET_FEATURE_ARRAY matlab implementation for code generation
 * Arguments    : const double triaxial_sequence_data[]
 *                const int triaxial_sequence_size[2]
 *                double feature_array[17]
 * Return Type  : void
 */
void get_feature_array(const double triaxial_sequence_data[], const int
  triaxial_sequence_size[2], double feature_array[17])
{
  int gaussian_smoothed_size[2];
  int ixstart;
  int n;
  int numSeq;
  double gaussian_smoothed_data[192];
  int b_triaxial_sequence_size[1];
  int i;
  double training_svm_data[64];
  int y_size[2];
  int loop_ub;
  double y_data[64];
  double smoothed_svm_data[64];
  int smoothed_svm_size[1];
  int tmp_size[1];
  double mtmp;
  boolean_T exitg1;
  double abs_mean;
  double tmp_data[128];
  double feature_vector[16];
  gaussian_smoothed_size[0] = triaxial_sequence_size[0];
  gaussian_smoothed_size[1] = 3;
  ixstart = triaxial_sequence_size[0] * triaxial_sequence_size[1];
  for (n = 0; n < ixstart; n++) {
    gaussian_smoothed_data[n] = triaxial_sequence_data[n];
  }

  numSeq = triaxial_sequence_size[0];

  /*  apply Gaussian onto xyz_sequence */
  ixstart = triaxial_sequence_size[0];
  b_triaxial_sequence_size[0] = triaxial_sequence_size[0];
  for (i = 0; i < 3; i++) {
    if (numSeq < 1) {
      y_size[0] = 1;
      y_size[1] = 0;
    } else {
      y_size[0] = 1;
      y_size[1] = numSeq;
      loop_ub = numSeq - 1;
      for (n = 0; n <= loop_ub; n++) {
        y_data[n] = 1.0 + (double)n;
      }
    }

    for (n = 0; n < ixstart; n++) {
      smoothed_svm_data[n] = triaxial_sequence_data[n + triaxial_sequence_size[0]
        * i];
    }

    gaussfilt(y_data, y_size, smoothed_svm_data, b_triaxial_sequence_size,
              training_svm_data, tmp_size);
    loop_ub = tmp_size[0];
    for (n = 0; n < loop_ub; n++) {
      gaussian_smoothed_data[n + gaussian_smoothed_size[0] * i] =
        training_svm_data[n];
    }
  }

  triaxial2svm(gaussian_smoothed_data, gaussian_smoothed_size, training_svm_data,
               b_triaxial_sequence_size);
  if (b_triaxial_sequence_size[0] < 1) {
    y_size[0] = 1;
    y_size[1] = 0;
  } else {
    y_size[0] = 1;
    y_size[1] = b_triaxial_sequence_size[0];
    ixstart = b_triaxial_sequence_size[0] - 1;
    for (n = 0; n <= ixstart; n++) {
      y_data[n] = 1.0 + (double)n;
    }
  }

  gaussfilt(y_data, y_size, training_svm_data, b_triaxial_sequence_size,
            smoothed_svm_data, smoothed_svm_size);
  ixstart = 1;
  n = smoothed_svm_size[0];
  mtmp = smoothed_svm_data[0];
  if (smoothed_svm_size[0] > 1) {
    if (rtIsNaN(smoothed_svm_data[0])) {
      numSeq = 2;
      exitg1 = false;
      while ((!exitg1) && (numSeq <= n)) {
        ixstart = numSeq;
        if (!rtIsNaN(smoothed_svm_data[numSeq - 1])) {
          mtmp = smoothed_svm_data[numSeq - 1];
          exitg1 = true;
        } else {
          numSeq++;
        }
      }
    }

    if (ixstart < smoothed_svm_size[0]) {
      while (ixstart + 1 <= n) {
        if (smoothed_svm_data[ixstart] > mtmp) {
          mtmp = smoothed_svm_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = smoothed_svm_size[0];
  abs_mean = smoothed_svm_data[0];
  if (smoothed_svm_size[0] > 1) {
    if (rtIsNaN(smoothed_svm_data[0])) {
      numSeq = 2;
      exitg1 = false;
      while ((!exitg1) && (numSeq <= n)) {
        ixstart = numSeq;
        if (!rtIsNaN(smoothed_svm_data[numSeq - 1])) {
          abs_mean = smoothed_svm_data[numSeq - 1];
          exitg1 = true;
        } else {
          numSeq++;
        }
      }
    }

    if (ixstart < smoothed_svm_size[0]) {
      while (ixstart + 1 <= n) {
        if (smoothed_svm_data[ixstart] < abs_mean) {
          abs_mean = smoothed_svm_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  abs_mean = (mtmp + abs_mean) / 2.0;
  ixstart = 1;
  n = smoothed_svm_size[0];
  mtmp = smoothed_svm_data[0];
  if (smoothed_svm_size[0] > 1) {
    if (rtIsNaN(smoothed_svm_data[0])) {
      numSeq = 2;
      exitg1 = false;
      while ((!exitg1) && (numSeq <= n)) {
        ixstart = numSeq;
        if (!rtIsNaN(smoothed_svm_data[numSeq - 1])) {
          mtmp = smoothed_svm_data[numSeq - 1];
          exitg1 = true;
        } else {
          numSeq++;
        }
      }
    }

    if (ixstart < smoothed_svm_size[0]) {
      while (ixstart + 1 <= n) {
        if (smoothed_svm_data[ixstart] > mtmp) {
          mtmp = smoothed_svm_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  findpeaks(smoothed_svm_data, smoothed_svm_size, abs_mean + (mtmp - abs_mean) /
            2.0, tmp_data, tmp_size);
  abs_mean = b_mean(tmp_data, tmp_size);

  /*  extract feature values */
  generate_feature_vec(gaussian_smoothed_data, gaussian_smoothed_size,
                       training_svm_data, b_triaxial_sequence_size,
                       feature_vector);
  memcpy(&feature_array[0], &feature_vector[0], sizeof(double) << 4);
  feature_array[16] = abs_mean;
}

/*
 * File trailer for get_feature_array.c
 *
 * [EOF]
 */
