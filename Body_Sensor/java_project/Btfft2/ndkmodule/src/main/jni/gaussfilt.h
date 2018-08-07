/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: gaussfilt.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

#ifndef GAUSSFILT_H
#define GAUSSFILT_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "get_feature_array_types.h"

/* Function Declarations */
extern void gaussfilt(const double t_data[], const int t_size[2], const double
                      z_data[], const int z_size[1], double zfilt_data[], int
                      zfilt_size[1]);

#endif

/*
 * File trailer for gaussfilt.h
 *
 * [EOF]
 */
