/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sqrt.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:27:19
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_step_num.h"
#include "sqrt.h"

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
 * Arguments    : creal_T x_data[]
 *                int x_size[1]
 * Return Type  : void
 */
void b_sqrt(creal_T x_data[], int x_size[1])
{
  int nx;
  int k;
  double xr;
  double xi;
  double absxi;
  double absxr;
  nx = x_size[0];
  for (k = 0; k + 1 <= nx; k++) {
    xr = x_data[k].re;
    xi = x_data[k].im;
    if (xi == 0.0) {
      if (xr < 0.0) {
        absxi = 0.0;
        xr = sqrt(-xr);
      } else {
        absxi = sqrt(xr);
        xr = 0.0;
      }
    } else if (xr == 0.0) {
      if (xi < 0.0) {
        absxi = sqrt(-xi / 2.0);
        xr = -absxi;
      } else {
        absxi = sqrt(xi / 2.0);
        xr = absxi;
      }
    } else if (rtIsNaN(xr)) {
      absxi = xr;
    } else if (rtIsNaN(xi)) {
      absxi = xi;
      xr = xi;
    } else if (rtIsInf(xi)) {
      absxi = fabs(xi);
      xr = xi;
    } else if (rtIsInf(xr)) {
      if (xr < 0.0) {
        absxi = 0.0;
        xr = xi * -xr;
      } else {
        absxi = xr;
        xr = 0.0;
      }
    } else {
      absxr = fabs(xr);
      absxi = fabs(xi);
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

      if (xr > 0.0) {
        xr = 0.5 * (xi / absxi);
      } else {
        if (xi < 0.0) {
          xr = -absxi;
        } else {
          xr = absxi;
        }

        absxi = 0.5 * (xi / xr);
      }
    }

    x_data[k].re = absxi;
    x_data[k].im = xr;
  }
}

/*
 * File trailer for sqrt.c
 *
 * [EOF]
 */
