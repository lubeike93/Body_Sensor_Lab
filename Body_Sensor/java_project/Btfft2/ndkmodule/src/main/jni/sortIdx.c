/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortIdx.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 04-Jan-2018 15:43:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "get_feature_array.h"
#include "sortIdx.h"

/* Function Declarations */
static void merge(int idx_data[], int x_data[], int offset, int np, int nq, int
                  iwork_data[], int xwork_data[]);
static void merge_block(int idx_data[], int x_data[], int n, int iwork_data[],
  int xwork_data[]);

/* Function Definitions */

/*
 * Arguments    : int idx_data[]
 *                int x_data[]
 *                int offset
 *                int np
 *                int nq
 *                int iwork_data[]
 *                int xwork_data[]
 * Return Type  : void
 */
static void merge(int idx_data[], int x_data[], int offset, int np, int nq, int
                  iwork_data[], int xwork_data[])
{
  int n;
  int qend;
  int p;
  int iout;
  int exitg1;
  if (nq != 0) {
    n = np + nq;
    for (qend = 0; qend + 1 <= n; qend++) {
      iwork_data[qend] = idx_data[offset + qend];
      xwork_data[qend] = x_data[offset + qend];
    }

    p = 0;
    n = np;
    qend = np + nq;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[p] <= xwork_data[n]) {
        idx_data[iout] = iwork_data[p];
        x_data[iout] = xwork_data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[n];
        x_data[iout] = xwork_data[n];
        if (n + 1 < qend) {
          n++;
        } else {
          n = (iout - p) + 1;
          while (p + 1 <= np) {
            idx_data[n + p] = iwork_data[p];
            x_data[n + p] = xwork_data[p];
            p++;
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

/*
 * Arguments    : int idx_data[]
 *                int x_data[]
 *                int n
 *                int iwork_data[]
 *                int xwork_data[]
 * Return Type  : void
 */
static void merge_block(int idx_data[], int x_data[], int n, int iwork_data[],
  int xwork_data[])
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> 2;
  bLen = 4;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx_data, x_data, tailOffset, bLen, nTail - bLen, iwork_data,
              xwork_data);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 1; nTail <= nPairs; nTail++) {
      merge(idx_data, x_data, (nTail - 1) * tailOffset, bLen, bLen, iwork_data,
            xwork_data);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    merge(idx_data, x_data, 0, bLen, n - bLen, iwork_data, xwork_data);
  }
}

/*
 * Arguments    : int x_data[]
 *                int x_size[1]
 *                int idx_data[]
 *                int idx_size[1]
 * Return Type  : void
 */
void b_sortIdx(int x_data[], int x_size[1], int idx_data[], int idx_size[1])
{
  unsigned char unnamed_idx_0;
  int nQuartets;
  int nDone;
  int n;
  int i;
  int x4[4];
  short idx4[4];
  int iwork_data[128];
  int xwork_data[128];
  int nLeft;
  signed char perm[4];
  int i2;
  int i3;
  int i4;
  unnamed_idx_0 = (unsigned char)x_size[0];
  idx_size[0] = unnamed_idx_0;
  nQuartets = unnamed_idx_0;
  for (nDone = 0; nDone < nQuartets; nDone++) {
    idx_data[nDone] = 0;
  }

  n = x_size[0];
  for (i = 0; i < 4; i++) {
    x4[i] = 0;
    idx4[i] = 0;
  }

  nQuartets = unnamed_idx_0;
  for (nDone = 0; nDone < nQuartets; nDone++) {
    iwork_data[nDone] = 0;
  }

  nQuartets = (unsigned char)x_size[0];
  for (nDone = 0; nDone < nQuartets; nDone++) {
    xwork_data[nDone] = 0;
  }

  nQuartets = x_size[0] >> 2;
  for (nDone = 1; nDone <= nQuartets; nDone++) {
    i = (nDone - 1) << 2;
    idx4[0] = (short)(i + 1);
    idx4[1] = (short)(i + 2);
    idx4[2] = (short)(i + 3);
    idx4[3] = (short)(i + 4);
    x4[0] = x_data[i];
    x4[1] = x_data[i + 1];
    x4[2] = x_data[i + 2];
    x4[3] = x_data[i + 3];
    if (x_data[i] <= x_data[i + 1]) {
      nLeft = 1;
      i2 = 2;
    } else {
      nLeft = 2;
      i2 = 1;
    }

    if (x_data[i + 2] <= x_data[i + 3]) {
      i3 = 3;
      i4 = 4;
    } else {
      i3 = 4;
      i4 = 3;
    }

    if (x4[nLeft - 1] <= x4[i3 - 1]) {
      if (x4[i2 - 1] <= x4[i3 - 1]) {
        perm[0] = (signed char)nLeft;
        perm[1] = (signed char)i2;
        perm[2] = (signed char)i3;
        perm[3] = (signed char)i4;
      } else if (x4[i2 - 1] <= x4[i4 - 1]) {
        perm[0] = (signed char)nLeft;
        perm[1] = (signed char)i3;
        perm[2] = (signed char)i2;
        perm[3] = (signed char)i4;
      } else {
        perm[0] = (signed char)nLeft;
        perm[1] = (signed char)i3;
        perm[2] = (signed char)i4;
        perm[3] = (signed char)i2;
      }
    } else if (x4[nLeft - 1] <= x4[i4 - 1]) {
      if (x4[i2 - 1] <= x4[i4 - 1]) {
        perm[0] = (signed char)i3;
        perm[1] = (signed char)nLeft;
        perm[2] = (signed char)i2;
        perm[3] = (signed char)i4;
      } else {
        perm[0] = (signed char)i3;
        perm[1] = (signed char)nLeft;
        perm[2] = (signed char)i4;
        perm[3] = (signed char)i2;
      }
    } else {
      perm[0] = (signed char)i3;
      perm[1] = (signed char)i4;
      perm[2] = (signed char)nLeft;
      perm[3] = (signed char)i2;
    }

    idx_data[i] = idx4[perm[0] - 1];
    idx_data[i + 1] = idx4[perm[1] - 1];
    idx_data[i + 2] = idx4[perm[2] - 1];
    idx_data[i + 3] = idx4[perm[3] - 1];
    x_data[i] = x4[perm[0] - 1];
    x_data[i + 1] = x4[perm[1] - 1];
    x_data[i + 2] = x4[perm[2] - 1];
    x_data[i + 3] = x4[perm[3] - 1];
  }

  nDone = (nQuartets << 2) - 1;
  nLeft = (x_size[0] - nDone) - 1;
  if (nLeft > 0) {
    for (nQuartets = 0; nQuartets + 1 <= nLeft; nQuartets++) {
      idx4[nQuartets] = (short)((nDone + nQuartets) + 2);
      x4[nQuartets] = x_data[(nDone + nQuartets) + 1];
    }

    for (i = 0; i < 4; i++) {
      perm[i] = 0;
    }

    if (nLeft == 1) {
      perm[0] = 1;
    } else if (nLeft == 2) {
      if (x4[0] <= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] <= x4[1]) {
      if (x4[1] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }

    for (nQuartets = 1; nQuartets <= nLeft; nQuartets++) {
      idx_data[nDone + nQuartets] = idx4[perm[nQuartets - 1] - 1];
      x_data[nDone + nQuartets] = x4[perm[nQuartets - 1] - 1];
    }
  }

  if (n > 1) {
    merge_block(idx_data, x_data, n, iwork_data, xwork_data);
  }
}

/*
 * Arguments    : const double x_data[]
 *                const int x_size[1]
 *                int idx_data[]
 *                int idx_size[1]
 * Return Type  : void
 */
void sortIdx(const double x_data[], const int x_size[1], int idx_data[], int
             idx_size[1])
{
  int n;
  int loop_ub;
  int i;
  boolean_T p;
  int i2;
  int j;
  int pEnd;
  int b_p;
  int q;
  int qEnd;
  int kEnd;
  int iwork_data[128];
  n = x_size[0] + 1;
  idx_size[0] = (unsigned char)x_size[0];
  loop_ub = (unsigned char)x_size[0];
  for (i = 0; i < loop_ub; i++) {
    idx_data[i] = 0;
  }

  for (loop_ub = 1; loop_ub <= n - 2; loop_ub += 2) {
    if ((x_data[loop_ub - 1] >= x_data[loop_ub]) || rtIsNaN(x_data[loop_ub - 1]))
    {
      p = true;
    } else {
      p = false;
    }

    if (p) {
      idx_data[loop_ub - 1] = loop_ub;
      idx_data[loop_ub] = loop_ub + 1;
    } else {
      idx_data[loop_ub - 1] = loop_ub + 1;
      idx_data[loop_ub] = loop_ub;
    }
  }

  if ((x_size[0] & 1) != 0) {
    idx_data[x_size[0] - 1] = x_size[0];
  }

  i = 2;
  while (i < n - 1) {
    i2 = i << 1;
    j = 1;
    for (pEnd = 1 + i; pEnd < n; pEnd = qEnd + i) {
      b_p = j - 1;
      q = pEnd;
      qEnd = j + i2;
      if (qEnd > n) {
        qEnd = n;
      }

      loop_ub = 0;
      kEnd = qEnd - j;
      while (loop_ub + 1 <= kEnd) {
        if ((x_data[idx_data[b_p] - 1] >= x_data[idx_data[q - 1] - 1]) ||
            rtIsNaN(x_data[idx_data[b_p] - 1])) {
          p = true;
        } else {
          p = false;
        }

        if (p) {
          iwork_data[loop_ub] = idx_data[b_p];
          b_p++;
          if (b_p + 1 == pEnd) {
            while (q < qEnd) {
              loop_ub++;
              iwork_data[loop_ub] = idx_data[q - 1];
              q++;
            }
          }
        } else {
          iwork_data[loop_ub] = idx_data[q - 1];
          q++;
          if (q == qEnd) {
            while (b_p + 1 < pEnd) {
              loop_ub++;
              iwork_data[loop_ub] = idx_data[b_p];
              b_p++;
            }
          }
        }

        loop_ub++;
      }

      for (loop_ub = 0; loop_ub + 1 <= kEnd; loop_ub++) {
        idx_data[(j + loop_ub) - 1] = iwork_data[loop_ub];
      }

      j = qEnd;
    }

    i = i2;
  }
}

/*
 * File trailer for sortIdx.c
 *
 * [EOF]
 */
