/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: generate_feature_vec.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

#ifndef GENERATE_FEATURE_VEC_H
#define GENERATE_FEATURE_VEC_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "get_feature_array_types.h"

/* Function Declarations */
extern void generate_feature_vec(const double triaxial_data[], const int
  triaxial_size[2], const double svm_data[], const int svm_size[1], double
  feature_vec[16]);

#endif

/*
 * File trailer for generate_feature_vec.h
 *
 * [EOF]
 */
