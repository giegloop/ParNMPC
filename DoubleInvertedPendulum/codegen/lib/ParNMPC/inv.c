/*
 * File: inv.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 21-Jan-2018 01:36:29
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "ParNMPC.h"
#include "inv.h"
#include <stdio.h>
#include "omp.h"
#include "stdio.h"

/* Function Declarations */
static void invNxN(const double x[36], double y[36]);

/* Function Definitions */

/*
 * Arguments    : const double x[36]
 *                double y[36]
 * Return Type  : void
 */
static void invNxN(const double x[36], double y[36])
{
  double A[36];
  int i5;
  signed char ipiv[6];
  int j;
  signed char p[6];
  int c;
  int jBcol;
  int ix;
  int k;
  double smax;
  double s;
  int kAcol;
  int i;
  for (i5 = 0; i5 < 36; i5++) {
    y[i5] = 0.0;
    A[i5] = x[i5];
  }

  for (i5 = 0; i5 < 6; i5++) {
    ipiv[i5] = (signed char)(1 + i5);
  }

  for (j = 0; j < 5; j++) {
    c = j * 7;
    jBcol = 0;
    ix = c;
    smax = fabs(A[c]);
    for (k = 2; k <= 6 - j; k++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }

    if (A[c + jBcol] != 0.0) {
      if (jBcol != 0) {
        ipiv[j] = (signed char)((j + jBcol) + 1);
        ix = j;
        jBcol += j;
        for (k = 0; k < 6; k++) {
          smax = A[ix];
          A[ix] = A[jBcol];
          A[jBcol] = smax;
          ix += 6;
          jBcol += 6;
        }
      }

      i5 = (c - j) + 6;
      for (i = c + 1; i + 1 <= i5; i++) {
        A[i] /= A[c];
      }
    }

    jBcol = c;
    kAcol = c + 6;
    for (i = 1; i <= 5 - j; i++) {
      smax = A[kAcol];
      if (A[kAcol] != 0.0) {
        ix = c + 1;
        i5 = (jBcol - j) + 12;
        for (k = 7 + jBcol; k + 1 <= i5; k++) {
          A[k] += A[ix] * -smax;
          ix++;
        }
      }

      kAcol += 6;
      jBcol += 6;
    }
  }

  for (i5 = 0; i5 < 6; i5++) {
    p[i5] = (signed char)(1 + i5);
  }

  for (k = 0; k < 5; k++) {
    if (ipiv[k] > 1 + k) {
      jBcol = p[ipiv[k] - 1];
      p[ipiv[k] - 1] = p[k];
      p[k] = (signed char)jBcol;
    }
  }

  for (k = 0; k < 6; k++) {
    c = p[k] - 1;
    y[k + 6 * (p[k] - 1)] = 1.0;
    for (j = k; j + 1 < 7; j++) {
      if (y[j + 6 * c] != 0.0) {
        for (i = j + 1; i + 1 < 7; i++) {
          y[i + 6 * c] -= y[j + 6 * c] * A[i + 6 * j];
        }
      }
    }
  }

  for (j = 0; j < 6; j++) {
    jBcol = 6 * j;
    for (k = 5; k >= 0; k += -1) {
      kAcol = 6 * k;
      if (y[k + jBcol] != 0.0) {
        y[k + jBcol] /= A[k + kAcol];
        for (i = 0; i + 1 <= k; i++) {
          y[i + jBcol] -= y[k + jBcol] * A[i + kAcol];
        }
      }
    }
  }
}

/*
 * Arguments    : const double x[9]
 *                double y[9]
 * Return Type  : void
 */
void b_inv(const double x[9], double y[9])
{
  double b_x[9];
  int p1;
  int p2;
  int p3;
  double absx11;
  double absx21;
  double absx31;
  int itmp;
  memcpy(&b_x[0], &x[0], 9U * sizeof(double));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabs(x[0]);
  absx21 = fabs(x[1]);
  absx31 = fabs(x[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    b_x[0] = x[1];
    b_x[1] = x[0];
    b_x[3] = x[4];
    b_x[4] = x[3];
    b_x[6] = x[7];
    b_x[7] = x[6];
  } else {
    if (absx31 > absx11) {
      p1 = 6;
      p3 = 0;
      b_x[0] = x[2];
      b_x[2] = x[0];
      b_x[3] = x[5];
      b_x[5] = x[3];
      b_x[6] = x[8];
      b_x[8] = x[6];
    }
  }

  absx11 = b_x[1] / b_x[0];
  b_x[1] /= b_x[0];
  absx21 = b_x[2] / b_x[0];
  b_x[2] /= b_x[0];
  b_x[4] -= absx11 * b_x[3];
  b_x[5] -= absx21 * b_x[3];
  b_x[7] -= absx11 * b_x[6];
  b_x[8] -= absx21 * b_x[6];
  if (fabs(b_x[5]) > fabs(b_x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    b_x[1] = absx21;
    b_x[2] = absx11;
    absx11 = b_x[4];
    b_x[4] = b_x[5];
    b_x[5] = absx11;
    absx11 = b_x[7];
    b_x[7] = b_x[8];
    b_x[8] = absx11;
  }

  absx11 = b_x[5] / b_x[4];
  b_x[5] /= b_x[4];
  b_x[8] -= absx11 * b_x[7];
  absx11 = (b_x[5] * b_x[1] - b_x[2]) / b_x[8];
  absx21 = -(b_x[1] + b_x[7] * absx11) / b_x[4];
  y[p1] = ((1.0 - b_x[3] * absx21) - b_x[6] * absx11) / b_x[0];
  y[p1 + 1] = absx21;
  y[p1 + 2] = absx11;
  absx11 = -b_x[5] / b_x[8];
  absx21 = (1.0 - b_x[7] * absx11) / b_x[4];
  y[p2] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / b_x[8];
  absx21 = -b_x[7] * absx11 / b_x[4];
  y[p3] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
}

/*
 * Arguments    : const double x[36]
 *                double y[36]
 * Return Type  : void
 */
void inv(const double x[36], double y[36])
{
  invNxN(x, y);
}

/*
 * File trailer for inv.c
 *
 * [EOF]
 */