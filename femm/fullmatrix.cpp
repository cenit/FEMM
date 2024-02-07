#include "stdafx.h"
#include <math.h>
#include "fullmatrix.h"

CFullMatrix::CFullMatrix()
{
  n = 0;
  M = NULL;
  b = NULL;
}

CFullMatrix::CFullMatrix(int d)
{
  n = 0;
  M = NULL;
  b = NULL;
  Create(d);
}

CFullMatrix::~CFullMatrix()
{
  if (n == 0)
    return;

  int i;
  for (i = 0; i < n; i++)
    free(M[i]);
  free(M);
  free(b);
  n = 0;
}

void CFullMatrix::Wipe()
{
  /* fills up a square matrix with zeros in every entry */
  int i, j;

  for (i = 0; i < n; i++) {
    b[i] = 0;
    for (j = 0; j < n; j++)
      M[i][j] = 0;
  }
}

BOOL CFullMatrix::Create(int d)
{
  int i;

  M = (double**)calloc(d, sizeof(double*));
  b = (double*)calloc(d, sizeof(double));
  if ((M == NULL) || (b == NULL))
    return FALSE;
  for (i = 0; i < d; i++) {
    M[i] = (double*)calloc(d, sizeof(double));
    if (M[i] == NULL)
      return FALSE;
  }
  n = d;
  return TRUE;
}

BOOL CFullMatrix::GaussSolve()
{
  /* solves the linear system m x = b for x.  The result is returned
     in b, m is destroyed in the process */

  int i, j, k, q;
  double* z;
  double max, f;

  for (i = 0; i < n; i++) {
    for (j = i, max = 0; j < n; j++)
      if (fabs(M[j][i]) > fabs(max)) {
        max = M[j][i];
        q = j;
      }
    if (max == 0)
      return FALSE;
    z = M[i];
    M[i] = M[q];
    M[q] = z;
    f = b[i];
    b[i] = b[q];
    b[q] = f;
    for (j = i + 1; j < n; j++) {
      f = M[j][i] / M[i][i];
      b[j] = b[j] - f * b[i];
      for (k = i; k < n; k++)
        M[j][k] -= (f * M[i][k]);
    }
  }

  for (i = n - 1; i >= 0; i--) {
    for (j = n - 1, f = 0; j > i; j--)
      f += M[i][j] * b[j];
    b[i] = (b[i] - f) / M[i][i];
  }
  return TRUE;
}

CComplexFullMatrix::CComplexFullMatrix()
{
  n = 0;
  M = NULL;
  b = NULL;
}

CComplexFullMatrix::CComplexFullMatrix(int d)
{
  n = 0;
  M = NULL;
  b = NULL;
  Create(d);
}

CComplexFullMatrix::~CComplexFullMatrix()
{
  if (n == 0)
    return;

  int i;
  for (i = 0; i < n; i++)
    free(M[i]);
  free(M);
  free(b);
  n = 0;
}

void CComplexFullMatrix::Wipe()
{
  /* fills up a square matrix with zeros in every entry */
  int i, j;

  for (i = 0; i < n; i++) {
    b[i] = 0;
    for (j = 0; j < n; j++)
      M[i][j] = 0;
  }
}

BOOL CComplexFullMatrix::Create(int d)
{
  int i;

  M = (CComplex**)calloc(d, sizeof(CComplex*));
  b = (CComplex*)calloc(d, sizeof(CComplex));
  if ((M == NULL) || (b == NULL))
    return FALSE;
  for (i = 0; i < d; i++) {
    M[i] = (CComplex*)calloc(d, sizeof(CComplex));
    if (M[i] == NULL)
      return FALSE;
  }
  n = d;
  return TRUE;
}

BOOL CComplexFullMatrix::GaussSolve()
{
  /* solves the linear system m x = b for x.  The result is returned
     in b, m is destroyed in the process */

  int i, j, k, q;
  CComplex* z;
  CComplex max, f;

  for (i = 0; i < n; i++) {
    for (j = i, max = 0; j < n; j++)
      if (abs(M[j][i]) > abs(max)) {
        max = M[j][i];
        q = j;
      }
    if (max == 0)
      return FALSE;
    z = M[i];
    M[i] = M[q];
    M[q] = z;
    f = b[i];
    b[i] = b[q];
    b[q] = f;
    for (j = i + 1; j < n; j++) {
      f = M[j][i] / M[i][i];
      b[j] = b[j] - f * b[i];
      for (k = i; k < n; k++)
        M[j][k] -= (f * M[i][k]);
    }
  }

  for (i = n - 1; i >= 0; i--) {
    for (j = n - 1, f = 0; j > i; j--)
      f += M[i][j] * b[j];
    b[i] = (b[i] - f) / M[i][i];
  }
  return TRUE;
}
