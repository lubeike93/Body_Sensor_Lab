/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "findpeaks.h"
#include "sort1.h"
#include "sortIdx.h"
#include "eml_setop.h"

/* Function Declarations */
static void c_findPeaksSeparatedByMoreThanM(const double y_data[], const double
  x_data[], const int iPk_data[], const int iPk_size[1], int idx_data[], int
  idx_size[1]);
static void getAllPeaksCodegen(const double y_data[], const int y_size[1], int
  iPk_data[], int iPk_size[1], int iInf_data[], int iInf_size[1], int
  iInflect_data[], int iInflect_size[1]);
static void keepAtMostNpPeaks(int idx_data[], int idx_size[1], double Np);
static void parse_inputs(const double Yin_data[], const int Yin_size[1], double
  varargin_4, double y_data[], int y_size[1], double x_data[], int x_size[1],
  double *Ph, double *NpOut);
static void removeSmallPeaks(const double y_data[], const int iFinite_data[],
  const int iFinite_size[1], double minH, int iPk_data[], int iPk_size[1]);

/* Function Definitions */

/*
 * Arguments    : const double y_data[]
 *                const double x_data[]
 *                const int iPk_data[]
 *                const int iPk_size[1]
 *                int idx_data[]
 *                int idx_size[1]
 * Return Type  : void
 */
static void c_findPeaksSeparatedByMoreThanM(const double y_data[], const double
  x_data[], const int iPk_data[], const int iPk_size[1], int idx_data[], int
  idx_size[1])
{
  int loop_ub;
  int partialTrueCount;
  int y_size[1];
  double locs_data[128];
  double b_y_data[128];
  int sortIdx_data[128];
  int sortIdx_size[1];
  double locs_temp_data[128];
  int i;
  boolean_T idelete_data[128];
  int trueCount;
  boolean_T tmp_data[128];
  unsigned char b_tmp_data[128];
  if (iPk_size[0] == 0) {
    idx_size[0] = 0;
  } else {
    loop_ub = iPk_size[0];
    for (partialTrueCount = 0; partialTrueCount < loop_ub; partialTrueCount++) {
      locs_data[partialTrueCount] = x_data[iPk_data[partialTrueCount] - 1];
    }

    y_size[0] = iPk_size[0];
    loop_ub = iPk_size[0];
    for (partialTrueCount = 0; partialTrueCount < loop_ub; partialTrueCount++) {
      b_y_data[partialTrueCount] = y_data[iPk_data[partialTrueCount] - 1];
    }

    sortIdx(b_y_data, y_size, sortIdx_data, sortIdx_size);
    loop_ub = sortIdx_size[0];
    for (partialTrueCount = 0; partialTrueCount < loop_ub; partialTrueCount++) {
      locs_temp_data[partialTrueCount] = locs_data[sortIdx_data[partialTrueCount]
        - 1];
    }

    loop_ub = (unsigned char)sortIdx_size[0];
    for (partialTrueCount = 0; partialTrueCount < loop_ub; partialTrueCount++) {
      idelete_data[partialTrueCount] = false;
    }

    for (i = 0; i < sortIdx_size[0]; i++) {
      if (!idelete_data[i]) {
        loop_ub = sortIdx_size[0];
        for (partialTrueCount = 0; partialTrueCount < loop_ub; partialTrueCount
             ++) {
          tmp_data[partialTrueCount] = ((locs_temp_data[partialTrueCount] >=
            locs_data[sortIdx_data[i] - 1] - 35.0) &&
            (locs_temp_data[partialTrueCount] <= locs_data[sortIdx_data[i] - 1]
             + 35.0));
        }

        loop_ub = (unsigned char)sortIdx_size[0];
        for (partialTrueCount = 0; partialTrueCount < loop_ub; partialTrueCount
             ++) {
          idelete_data[partialTrueCount] = (idelete_data[partialTrueCount] ||
            tmp_data[partialTrueCount]);
        }

        idelete_data[i] = false;
      }
    }

    loop_ub = (unsigned char)sortIdx_size[0] - 1;
    trueCount = 0;
    for (i = 0; i <= loop_ub; i++) {
      if (!idelete_data[i]) {
        trueCount++;
      }
    }

    partialTrueCount = 0;
    for (i = 0; i <= loop_ub; i++) {
      if (!idelete_data[i]) {
        b_tmp_data[partialTrueCount] = (unsigned char)(i + 1);
        partialTrueCount++;
      }
    }

    idx_size[0] = trueCount;
    for (partialTrueCount = 0; partialTrueCount < trueCount; partialTrueCount++)
    {
      idx_data[partialTrueCount] = sortIdx_data[b_tmp_data[partialTrueCount] - 1];
    }

    sort(idx_data, idx_size);
  }
}

/*
 * Arguments    : const double y_data[]
 *                const int y_size[1]
 *                int iPk_data[]
 *                int iPk_size[1]
 *                int iInf_data[]
 *                int iInf_size[1]
 *                int iInflect_data[]
 *                int iInflect_size[1]
 * Return Type  : void
 */
static void getAllPeaksCodegen(const double y_data[], const int y_size[1], int
  iPk_data[], int iPk_size[1], int iInf_data[], int iInf_size[1], int
  iInflect_data[], int iInflect_size[1])
{
  int nPk;
  int nInf;
  int nInflect;
  char dir;
  int kfirst;
  double ykfirst;
  boolean_T isinfykfirst;
  int k;
  double yk;
  boolean_T isinfyk;
  char previousdir;
  int i4;
  nPk = 0;
  nInf = 0;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 0; k + 1 <= y_size[0]; k++) {
    yk = y_data[k];
    if (rtIsNaN(y_data[k])) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(y_data[k]) && (y_data[k] > 0.0)) {
      isinfyk = true;
      nInf++;
      iInf_data[nInf - 1] = k + 1;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if ('d' != previousdir) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            iPk_data[nPk - 1] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if ('i' != previousdir) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      }

      ykfirst = yk;
      kfirst = k + 1;
      isinfykfirst = isinfyk;
    }
  }

  if ((y_size[0] > 0) && (!isinfykfirst) && ((nInflect + 1 == 0) ||
       (iInflect_data[nInflect] < y_size[0]))) {
    nInflect++;
    iInflect_data[nInflect] = y_size[0];
  }

  if (1 > nPk) {
    iPk_size[0] = 0;
  } else {
    iPk_size[0] = nPk;
  }

  if (1 > nInf) {
    iInf_size[0] = 0;
  } else {
    iInf_size[0] = nInf;
  }

  if (1 > nInflect + 1) {
    i4 = -1;
  } else {
    i4 = nInflect;
  }

  iInflect_size[0] = i4 + 1;
}

/*
 * Arguments    : int idx_data[]
 *                int idx_size[1]
 *                double Np
 * Return Type  : void
 */
static void keepAtMostNpPeaks(int idx_data[], int idx_size[1], double Np)
{
  int loop_ub;
  int i7;
  int b_idx_data[128];
  if (idx_size[0] > Np) {
    loop_ub = (int)Np;
    for (i7 = 0; i7 < loop_ub; i7++) {
      b_idx_data[i7] = idx_data[i7];
    }

    idx_size[0] = (int)Np;
    loop_ub = (int)Np;
    for (i7 = 0; i7 < loop_ub; i7++) {
      idx_data[i7] = b_idx_data[i7];
    }
  }
}

/*
 * Arguments    : const double Yin_data[]
 *                const int Yin_size[1]
 *                double varargin_4
 *                double y_data[]
 *                int y_size[1]
 *                double x_data[]
 *                int x_size[1]
 *                double *Ph
 *                double *NpOut
 * Return Type  : void
 */
static void parse_inputs(const double Yin_data[], const int Yin_size[1], double
  varargin_4, double y_data[], int y_size[1], double x_data[], int x_size[1],
  double *Ph, double *NpOut)
{
  int loop_ub;
  int i3;
  int y_size_idx_1;
  unsigned int b_y_data[64];
  y_size[0] = Yin_size[0];
  loop_ub = Yin_size[0];
  for (i3 = 0; i3 < loop_ub; i3++) {
    y_data[i3] = Yin_data[i3];
  }

  if (Yin_size[0] < 1) {
    y_size_idx_1 = 0;
  } else {
    y_size_idx_1 = Yin_size[0];
    loop_ub = Yin_size[0] - 1;
    for (i3 = 0; i3 <= loop_ub; i3++) {
      b_y_data[i3] = (unsigned int)(1 + i3);
    }
  }

  x_size[0] = y_size_idx_1;
  for (i3 = 0; i3 < y_size_idx_1; i3++) {
    x_data[i3] = b_y_data[i3];
  }

  *Ph = varargin_4;
  *NpOut = Yin_size[0];
}

/*
 * Arguments    : const double y_data[]
 *                const int iFinite_data[]
 *                const int iFinite_size[1]
 *                double minH
 *                int iPk_data[]
 *                int iPk_size[1]
 * Return Type  : void
 */
static void removeSmallPeaks(const double y_data[], const int iFinite_data[],
  const int iFinite_size[1], double minH, int iPk_data[], int iPk_size[1])
{
  int nPk;
  int k;
  double b_y_data;
  nPk = 0;
  for (k = 0; k + 1 <= iFinite_size[0]; k++) {
    if (y_data[iFinite_data[k] - 1] > minH) {
      if ((y_data[iFinite_data[k] - 2] > y_data[iFinite_data[k]]) || rtIsNaN
          (y_data[iFinite_data[k]])) {
        b_y_data = y_data[iFinite_data[k] - 2];
      } else {
        b_y_data = y_data[iFinite_data[k]];
      }

      if (y_data[iFinite_data[k] - 1] - b_y_data >= 0.0) {
        nPk++;
        iPk_data[nPk - 1] = iFinite_data[k];
      }
    }
  }

  if (1 > nPk) {
    iPk_size[0] = 0;
  } else {
    iPk_size[0] = nPk;
  }
}

/*
 * Arguments    : const double Yin_data[]
 *                const int Yin_size[1]
 *                double varargin_4
 *                double Ypk_data[]
 *                int Ypk_size[1]
 * Return Type  : void
 */
void findpeaks(const double Yin_data[], const int Yin_size[1], double varargin_4,
               double Ypk_data[], int Ypk_size[1])
{
  double y_data[64];
  int y_size[1];
  double x_data[64];
  int x_size[1];
  double minH;
  double maxN;
  int iFinite_data[64];
  int iInfinite_data[64];
  int iInfinite_size[1];
  int iInflect_data[64];
  int iInflect_size[1];
  int iPk_size[1];
  int loop_ub;
  int i2;
  int iPk_data[128];
  int c_data[128];
  int idx_data[128];
  parse_inputs(Yin_data, Yin_size, varargin_4, y_data, y_size, x_data, x_size,
               &minH, &maxN);
  getAllPeaksCodegen(y_data, y_size, iFinite_data, x_size, iInfinite_data,
                     iInfinite_size, iInflect_data, iInflect_size);
  removeSmallPeaks(y_data, iFinite_data, x_size, minH, iInflect_data,
                   iInflect_size);
  iPk_size[0] = iInflect_size[0];
  loop_ub = iInflect_size[0];
  for (i2 = 0; i2 < loop_ub; i2++) {
    iPk_data[i2] = iInflect_data[i2];
  }

  do_vectors(iPk_data, iPk_size, iInfinite_data, iInfinite_size, c_data, y_size,
             iInflect_data, iInflect_size, iFinite_data, x_size);
  c_findPeaksSeparatedByMoreThanM(y_data, x_data, c_data, y_size, idx_data,
    x_size);
  keepAtMostNpPeaks(idx_data, x_size, maxN);
  loop_ub = x_size[0];
  for (i2 = 0; i2 < loop_ub; i2++) {
    iPk_data[i2] = c_data[idx_data[i2] - 1];
  }

  Ypk_size[0] = x_size[0];
  loop_ub = x_size[0];
  for (i2 = 0; i2 < loop_ub; i2++) {
    Ypk_data[i2] = y_data[iPk_data[i2] - 1];
  }
}

/*
 * File trailer for findpeaks.c
 *
 * [EOF]
 */
