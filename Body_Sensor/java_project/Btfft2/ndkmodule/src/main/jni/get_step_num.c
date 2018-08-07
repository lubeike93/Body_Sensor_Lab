/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: get_step_num.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:27:19
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_step_num.h"
#include "gaussfilt.h"
#include "findpeaks.h"
#include "mean.h"
#include "triaxial2svm.h"

/* Function Definitions */

/*
 * GET_STEP_NUM Summary of this function goes here
 *    Detailed explanation goes here
 *  0 for walking, 1 for running, 2 for jumping
 * Arguments    : const double triaxial_sequence_data[]
 *                const int triaxial_sequence_size[2]
 *                double status
 * Return Type  : double
 */
double get_step_num(const double triaxial_sequence_data[], const int
                    triaxial_sequence_size[2], double status)
{
  double num_steps;
  int gaussian_smoothed_size[2];
  int ixstart;
  int n;
  int numSeq;
  static double gaussian_smoothed_data[7602];
  int b_triaxial_sequence_size[1];
  int i;
  static double training_svm_data[2534];
  int y_size[2];
  int loop_ub;
  static double y_data[2534];
  static double smoothed_svm_data[2534];
  int smoothed_svm_size[1];
  double mph;
  static double peaks_data[5068];
  boolean_T exitg1;
  gaussian_smoothed_size[0] = triaxial_sequence_size[0];
  gaussian_smoothed_size[1] = 3;
  ixstart = triaxial_sequence_size[0] * triaxial_sequence_size[1];
  for (n = 0; n < ixstart; n++) {
    gaussian_smoothed_data[n] = triaxial_sequence_data[n];
  }

  numSeq = triaxial_sequence_size[0];
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
              training_svm_data, smoothed_svm_size);
    loop_ub = smoothed_svm_size[0];
    for (n = 0; n < loop_ub; n++) {
      gaussian_smoothed_data[n + gaussian_smoothed_size[0] * i] =
        training_svm_data[n];
    }
  }

  triaxial2svm(gaussian_smoothed_data, gaussian_smoothed_size, training_svm_data,
               b_triaxial_sequence_size);
  switch ((int)status) {
   case 0:
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
    mph = b_mean(smoothed_svm_data, smoothed_svm_size);
    findpeaks(smoothed_svm_data, smoothed_svm_size, mph, peaks_data,
              b_triaxial_sequence_size);
    ixstart = (short)b_triaxial_sequence_size[0];
    if (1 > (short)b_triaxial_sequence_size[0]) {
      ixstart = 1;
    }

    num_steps = ixstart;
    break;

   case 1:
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
    mph = b_mean(smoothed_svm_data, smoothed_svm_size);

    /*  + ((max(smoothed_svm) - mean(smoothed_svm)) / 2); */
    findpeaks(smoothed_svm_data, smoothed_svm_size, mph, peaks_data,
                b_triaxial_sequence_size);
    ixstart = (short)b_triaxial_sequence_size[0];
    if (1 > (short)b_triaxial_sequence_size[0]) {
      ixstart = 1;
    }

    num_steps = ixstart;
    break;

   case 2:
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
    mph = smoothed_svm_data[0];
    if (smoothed_svm_size[0] > 1) {
      if (rtIsNaN(smoothed_svm_data[0])) {
        numSeq = 2;
        exitg1 = false;
        while ((!exitg1) && (numSeq <= n)) {
          ixstart = numSeq;
          if (!rtIsNaN(smoothed_svm_data[numSeq - 1])) {
            mph = smoothed_svm_data[numSeq - 1];
            exitg1 = true;
          } else {
            numSeq++;
          }
        }
      }

      if (ixstart < smoothed_svm_size[0]) {
        while (ixstart + 1 <= n) {
          if (smoothed_svm_data[ixstart] > mph) {
            mph = smoothed_svm_data[ixstart];
          }

          ixstart++;
        }
      }
    }

    mph -= b_mean(smoothed_svm_data, smoothed_svm_size);
    mph = b_mean(smoothed_svm_data, smoothed_svm_size) + mph / 2.0;
    findpeaks(smoothed_svm_data, smoothed_svm_size, mph, peaks_data,
              b_triaxial_sequence_size);
    ixstart = (short)b_triaxial_sequence_size[0];
    if (1 > (short)b_triaxial_sequence_size[0]) {
      ixstart = 1;
    }

    num_steps = ixstart;
    break;

   default:
    num_steps = 0.0;
    break;
  }

  return num_steps;
}

/*
 * File trailer for get_step_num.c
 *
 * [EOF]
 */
