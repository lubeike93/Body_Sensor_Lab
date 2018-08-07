/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: triaxial2svm.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "triaxial2svm.h"

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

/*
 * TRIAXIAL2SVM Convert xyz-acceleration to single vector magnitude
 * Arguments    : const double triaxial_data[]
 *                const int triaxial_size[2]
 *                double svm_data[]
 *                int svm_size[1]
 * Return Type  : void
 */
void triaxial2svm(const double triaxial_data[], const int triaxial_size[2],
                  double svm_data[], int svm_size[1])
{
  int loop_ub;
  int i1;
  creal_T x_data[64];
  double absxi;
  double absxr;
  loop_ub = triaxial_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    x_data[i1].re = (triaxial_data[i1] * 2.0 + triaxial_data[i1 + triaxial_size
                     [0]] * 2.0) + triaxial_data[i1 + (triaxial_size[0] << 1)] *
      2.0;
    x_data[i1].im = 0.0;
  }

  for (loop_ub = 0; loop_ub + 1 <= triaxial_size[0]; loop_ub++) {
    if (x_data[loop_ub].im == 0.0) {
      if (x_data[loop_ub].re < 0.0) {
        absxi = 0.0;
        absxr = sqrt(-x_data[loop_ub].re);
      } else {
        absxi = sqrt(x_data[loop_ub].re);
        absxr = 0.0;
      }
    } else if (x_data[loop_ub].re == 0.0) {
      if (x_data[loop_ub].im < 0.0) {
        absxi = sqrt(-x_data[loop_ub].im / 2.0);
        absxr = -absxi;
      } else {
        absxi = sqrt(x_data[loop_ub].im / 2.0);
        absxr = absxi;
      }
    } else if (rtIsNaN(x_data[loop_ub].re)) {
      absxi = x_data[loop_ub].re;
      absxr = x_data[loop_ub].re;
    } else if (rtIsNaN(x_data[loop_ub].im)) {
      absxi = x_data[loop_ub].im;
      absxr = x_data[loop_ub].im;
    } else if (rtIsInf(x_data[loop_ub].im)) {
      absxi = fabs(x_data[loop_ub].im);
      absxr = x_data[loop_ub].im;
    } else if (rtIsInf(x_data[loop_ub].re)) {
      if (x_data[loop_ub].re < 0.0) {
        absxi = 0.0;
        absxr = x_data[loop_ub].im * -x_data[loop_ub].re;
      } else {
        absxi = x_data[loop_ub].re;
        absxr = 0.0;
      }
    } else {
      absxr = fabs(x_data[loop_ub].re);
      absxi = fabs(x_data[loop_ub].im);
      if ((absxr > 4.4942328371557893E+307) || (absxi > 4.4942328371557893E+307))
      {
        absxr *= 0.5;
        absxi *= 0.5;
        absxi = rt_hypotd_snf(absxr, absxi);
        if (absxi > absxr) {
          absxi = sqrt(absxi) * sqrt(1.0 + absxr / absxi);
        } else {
          absxi = sqrt(absxi) * 1.4142135623730951;
        }
      } else {
        absxi = sqrt((rt_hypotd_snf(absxr, absxi) + absxr) * 0.5);
      }

      if (x_data[loop_ub].re > 0.0) {
        absxr = 0.5 * (x_data[loop_ub].im / absxi);
      } else {
        if (x_data[loop_ub].im < 0.0) {
          absxr = -absxi;
        } else {
          absxr = absxi;
        }

        absxi = 0.5 * (x_data[loop_ub].im / absxr);
      }
    }

    x_data[loop_ub].re = absxi;
    x_data[loop_ub].im = absxr;
  }

  svm_size[0] = triaxial_size[0];
  loop_ub = triaxial_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    svm_data[i1] = x_data[i1].re;
  }
}

/*
 * File trailer for triaxial2svm.c
 *
 * [EOF]
 */
