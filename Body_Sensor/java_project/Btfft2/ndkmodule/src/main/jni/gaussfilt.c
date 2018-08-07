/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: gaussfilt.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "gaussfilt.h"
#include "conv.h"
#include "nullAssignment.h"
#include "exp.h"
#include "power.h"
#include "mean.h"
#include "sum.h"
#include "abs.h"
#include "diff.h"

/* Function Definitions */

/*
 * Apply a Gaussian filter to a time series
 *    Inputs: t = independent variable, z = data at points t, and
 *        sigma = standard deviation of Gaussian filter to be applied.
 *    Outputs: zfilt = filtered data.
 * Arguments    : const double t_data[]
 *                const int t_size[2]
 *                const double z_data[]
 *                const int z_size[1]
 *                double zfilt_data[]
 *                int zfilt_size[1]
 * Return Type  : void
 */
void gaussfilt(const double t_data[], const int t_size[2], const double z_data[],
               const int z_size[1], double zfilt_data[], int zfilt_size[1])
{
  boolean_T uniform;
  double dt_data[63];
  int dt_size[2];
  double tmp_data[63];
  double b_tmp_data[63];
  int tmp_size[2];
  int loop_ub;
  int i0;
  int ixstart;
  double w_data[64];
  int n;
  double mtmp;
  boolean_T exitg1;
  int b_t_size[2];
  double b_t_data[64];
  int w_size[2];
  int x_size[2];
  boolean_T x_data[64];
  int i;
  int c_t_size[2];
  double b_data[64];
  int ii_size_idx_1;
  int ii_data[64];
  int iedge_data[64];
  int b_loop_ub;
  int nx;
  double t;
  int d_t_size[2];
  int include_data[64];
  int c_loop_ub;
  int b_nx;

  /*  number of data */
  /*  height of Gaussian */
  /*  check for uniform spacing */
  /*  if so, use convolution. if not use numerical integration */
  uniform = false;
  diff(t_data, t_size, dt_data, dt_size);
  diff(t_data, t_size, tmp_data, dt_size);
  diff(tmp_data, dt_size, b_tmp_data, tmp_size);
  b_abs(b_tmp_data, tmp_size, tmp_data, dt_size);
  loop_ub = dt_size[0] * dt_size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    w_data[i0] = tmp_data[i0];
  }

  ixstart = 1;
  n = dt_size[1];
  mtmp = w_data[0];
  if (dt_size[1] > 1) {
    if (rtIsNaN(w_data[0])) {
      loop_ub = 2;
      exitg1 = false;
      while ((!exitg1) && (loop_ub <= n)) {
        ixstart = loop_ub;
        if (!rtIsNaN(w_data[loop_ub - 1])) {
          mtmp = w_data[loop_ub - 1];
          exitg1 = true;
        } else {
          loop_ub++;
        }
      }
    }

    if (ixstart < dt_size[1]) {
      while (ixstart + 1 <= n) {
        if (w_data[ixstart] > mtmp) {
          mtmp = w_data[ixstart];
        }

        ixstart++;
      }
    }
  }

  if (mtmp / dt_data[0] < 0.0001) {
    uniform = true;
  }

  if (uniform) {
    b_t_size[0] = 1;
    b_t_size[1] = t_size[1];
    mtmp = mean(t_data, t_size);
    loop_ub = t_size[0] * t_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      b_t_data[i0] = t_data[i0] - mtmp;
    }

    power(b_t_data, b_t_size, w_data, w_size);
    mtmp = dt_data[0] * 0.099735570100358176;
    w_size[0] = 1;
    loop_ub = w_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      w_data[i0] = -0.5 * w_data[i0] / 16.0;
    }

    b_exp(w_data, w_size);
    w_size[0] = 1;
    loop_ub = w_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      w_data[i0] *= mtmp;
    }

    x_size[0] = 1;
    x_size[1] = w_size[1];
    mtmp = dt_data[0] * 0.099735570100358176 * 1.0E-6;
    loop_ub = w_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      x_data[i0] = (w_data[i0] < mtmp);
    }

    nullAssignment(w_data, w_size, x_data, x_size);
    conv(z_data, z_size, w_data, w_size, zfilt_data, zfilt_size);
  } else {
    /* %% get distances between points for proper weighting */
    loop_ub = t_size[0] * t_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      w_data[i0] = 0.0 * t_data[i0];
    }

    if (3 > t_size[1]) {
      i0 = 0;
      ixstart = 0;
    } else {
      i0 = 2;
      ixstart = t_size[1];
    }

    n = !(2 > t_size[1] - 1);
    loop_ub = ixstart - i0;
    for (ixstart = 0; ixstart < loop_ub; ixstart++) {
      w_data[n + ixstart] = 0.5 * (t_data[i0 + ixstart] - t_data[ixstart]);
    }

    w_data[0] = t_data[1] - t_data[0];
    w_data[t_size[1] - 1] = t_data[t_size[1] - 1] - t_data[t_size[1] - 2];

    /* %% check if sigma smaller than data spacing */
    loop_ub = t_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      x_data[i0] = (w_data[i0] > 8.0);
    }

    ixstart = t_size[1];
    if (1 < ixstart) {
      ixstart = 1;
    }

    n = 0;
    loop_ub = 1;
    exitg1 = false;
    while ((!exitg1) && (loop_ub <= t_size[1])) {
      if (x_data[loop_ub - 1]) {
        n = 1;
        exitg1 = true;
      } else {
        loop_ub++;
      }
    }

    if (ixstart == 1) {
      if (n == 0) {
        ixstart = 0;
      }
    } else {
      ixstart = !(1 > n);
    }

    if (!(ixstart == 0)) {
      ixstart = t_size[1];
      for (i = 0; i < ixstart; i++) {
        mtmp = w_data[i];
        if (w_data[i] > 10.0) {
          mtmp = 10.0;
        }

        w_data[i] = mtmp;
      }

      /*  this correction leaves some residual for spacing between 2-3sigma. */
      /*  otherwise ok. */
      /*  In general, using a Gaussian filter with sigma less than spacing is */
      /*  a bad idea anyway... */
    }

    /* %% loop over points */
    zfilt_size[0] = z_size[0];
    loop_ub = z_size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      zfilt_data[i0] = 0.0 * z_data[i0];
    }

    /*  initalize output vector */
    for (i = 0; i < z_size[0]; i++) {
      c_t_size[0] = 1;
      c_t_size[1] = t_size[1];
      loop_ub = t_size[0] * t_size[1];
      for (i0 = 0; i0 < loop_ub; i0++) {
        b_t_data[i0] = t_data[i0] - t_data[i];
      }

      power(b_t_data, c_t_size, b_data, dt_size);
      dt_size[0] = 1;
      loop_ub = dt_size[1];
      for (i0 = 0; i0 < loop_ub; i0++) {
        b_data[i0] = -0.5 * b_data[i0] / 16.0;
      }

      b_exp(b_data, dt_size);
      w_size[0] = 1;
      w_size[1] = dt_size[1];
      mtmp = w_data[0] * z_data[0];
      loop_ub = dt_size[0] * dt_size[1];
      for (i0 = 0; i0 < loop_ub; i0++) {
        b_t_data[i0] = mtmp * (0.099735570100358176 * b_data[i0]);
      }

      zfilt_data[i] = sum(b_t_data, w_size);
    }

    /* %% clean-up edges - mirror data for correction */
    /*  distance from edge that needs correcting */
    /*  left edge */
    ixstart = 1;
    n = t_size[1];
    mtmp = t_data[0];
    if (t_size[1] > 1) {
      if (rtIsNaN(t_data[0])) {
        loop_ub = 2;
        exitg1 = false;
        while ((!exitg1) && (loop_ub <= n)) {
          ixstart = loop_ub;
          if (!rtIsNaN(t_data[loop_ub - 1])) {
            mtmp = t_data[loop_ub - 1];
            exitg1 = true;
          } else {
            loop_ub++;
          }
        }
      }

      if (ixstart < t_size[1]) {
        while (ixstart + 1 <= n) {
          if (t_data[ixstart] < mtmp) {
            mtmp = t_data[ixstart];
          }

          ixstart++;
        }
      }
    }

    x_size[1] = t_size[1];
    loop_ub = t_size[0] * t_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      x_data[i0] = (t_data[i0] < mtmp + 9.6);
    }

    ixstart = t_size[1];
    n = 0;
    ii_size_idx_1 = t_size[1];
    loop_ub = 1;
    exitg1 = false;
    while ((!exitg1) && (loop_ub <= ixstart)) {
      if (x_data[loop_ub - 1]) {
        n++;
        ii_data[n - 1] = loop_ub;
        if (n >= ixstart) {
          exitg1 = true;
        } else {
          loop_ub++;
        }
      } else {
        loop_ub++;
      }
    }

    if (t_size[1] == 1) {
      if (n == 0) {
        ii_size_idx_1 = 0;
      }
    } else if (1 > n) {
      ii_size_idx_1 = 0;
    } else {
      ii_size_idx_1 = n;
    }

    ixstart = ii_size_idx_1;
    for (i0 = 0; i0 < ii_size_idx_1; i0++) {
      iedge_data[i0] = ii_data[i0];
    }

    if (0 <= ii_size_idx_1 - 1) {
      x_size[1] = t_size[1];
      b_loop_ub = t_size[0] * t_size[1];
      nx = t_size[1];
    }

    for (i = 0; i < ixstart; i++) {
      t = t_data[iedge_data[i] - 1] + (t_data[iedge_data[i] - 1] - mtmp);
      for (i0 = 0; i0 < b_loop_ub; i0++) {
        x_data[i0] = (t_data[i0] > t);
      }

      n = 0;
      ii_size_idx_1 = x_size[1];
      loop_ub = 1;
      exitg1 = false;
      while ((!exitg1) && (loop_ub <= nx)) {
        if (x_data[loop_ub - 1]) {
          n++;
          ii_data[n - 1] = loop_ub;
          if (n >= nx) {
            exitg1 = true;
          } else {
            loop_ub++;
          }
        } else {
          loop_ub++;
        }
      }

      if (x_size[1] == 1) {
        if (n == 0) {
          ii_size_idx_1 = 0;
        }
      } else if (1 > n) {
        ii_size_idx_1 = 0;
      } else {
        ii_size_idx_1 = n;
      }

      for (i0 = 0; i0 < ii_size_idx_1; i0++) {
        include_data[i0] = ii_data[i0];
      }

      d_t_size[0] = 1;
      d_t_size[1] = ii_size_idx_1;
      for (i0 = 0; i0 < ii_size_idx_1; i0++) {
        b_t_data[i0] = t_data[include_data[i0] - 1] - t_data[iedge_data[i] - 1];
      }

      power(b_t_data, d_t_size, b_data, dt_size);
      dt_size[0] = 1;
      loop_ub = dt_size[1];
      for (i0 = 0; i0 < loop_ub; i0++) {
        b_data[i0] = -0.5 * b_data[i0] / 16.0;
      }

      b_exp(b_data, dt_size);
      zfilt_data[iedge_data[i] - 1] += w_data[include_data[0] - 1] *
        (0.099735570100358176 * b_data[0]) * z_data[include_data[0] - 1];
    }

    /*  right edge */
    ixstart = 1;
    n = t_size[1];
    mtmp = t_data[0];
    if (t_size[1] > 1) {
      if (rtIsNaN(t_data[0])) {
        loop_ub = 2;
        exitg1 = false;
        while ((!exitg1) && (loop_ub <= n)) {
          ixstart = loop_ub;
          if (!rtIsNaN(t_data[loop_ub - 1])) {
            mtmp = t_data[loop_ub - 1];
            exitg1 = true;
          } else {
            loop_ub++;
          }
        }
      }

      if (ixstart < t_size[1]) {
        while (ixstart + 1 <= n) {
          if (t_data[ixstart] > mtmp) {
            mtmp = t_data[ixstart];
          }

          ixstart++;
        }
      }
    }

    x_size[1] = t_size[1];
    loop_ub = t_size[0] * t_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      x_data[i0] = (t_data[i0] > mtmp - 9.6);
    }

    ixstart = t_size[1];
    n = 0;
    ii_size_idx_1 = t_size[1];
    loop_ub = 1;
    exitg1 = false;
    while ((!exitg1) && (loop_ub <= ixstart)) {
      if (x_data[loop_ub - 1]) {
        n++;
        ii_data[n - 1] = loop_ub;
        if (n >= ixstart) {
          exitg1 = true;
        } else {
          loop_ub++;
        }
      } else {
        loop_ub++;
      }
    }

    if (t_size[1] == 1) {
      if (n == 0) {
        ii_size_idx_1 = 0;
      }
    } else if (1 > n) {
      ii_size_idx_1 = 0;
    } else {
      ii_size_idx_1 = n;
    }

    ixstart = ii_size_idx_1;
    for (i0 = 0; i0 < ii_size_idx_1; i0++) {
      iedge_data[i0] = ii_data[i0];
    }

    if (0 <= ii_size_idx_1 - 1) {
      x_size[1] = t_size[1];
      c_loop_ub = t_size[0] * t_size[1];
      b_nx = t_size[1];
    }

    for (i = 0; i < ixstart; i++) {
      t = t_data[iedge_data[i] - 1] - (mtmp - t_data[iedge_data[i] - 1]);
      for (i0 = 0; i0 < c_loop_ub; i0++) {
        x_data[i0] = (t_data[i0] < t);
      }

      n = 0;
      ii_size_idx_1 = x_size[1];
      loop_ub = 1;
      exitg1 = false;
      while ((!exitg1) && (loop_ub <= b_nx)) {
        if (x_data[loop_ub - 1]) {
          n++;
          ii_data[n - 1] = loop_ub;
          if (n >= b_nx) {
            exitg1 = true;
          } else {
            loop_ub++;
          }
        } else {
          loop_ub++;
        }
      }

      if (x_size[1] == 1) {
        if (n == 0) {
          ii_size_idx_1 = 0;
        }
      } else if (1 > n) {
        ii_size_idx_1 = 0;
      } else {
        ii_size_idx_1 = n;
      }

      for (i0 = 0; i0 < ii_size_idx_1; i0++) {
        include_data[i0] = ii_data[i0];
      }

      b_t_size[0] = 1;
      b_t_size[1] = ii_size_idx_1;
      for (i0 = 0; i0 < ii_size_idx_1; i0++) {
        b_t_data[i0] = t_data[include_data[i0] - 1] - t_data[iedge_data[i] - 1];
      }

      power(b_t_data, b_t_size, b_data, dt_size);
      dt_size[0] = 1;
      loop_ub = dt_size[1];
      for (i0 = 0; i0 < loop_ub; i0++) {
        b_data[i0] = -0.5 * b_data[i0] / 16.0;
      }

      b_exp(b_data, dt_size);
      zfilt_data[iedge_data[i] - 1] += w_data[include_data[0] - 1] *
        (0.099735570100358176 * b_data[0]) * z_data[include_data[0] - 1];
    }
  }

  /*  uniform vs non-uniform */
}

/*
 * File trailer for gaussfilt.c
 *
 * [EOF]
 */
