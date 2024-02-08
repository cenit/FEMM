#include <stdafx.h>
#include <afxtempl.h>
#include <stdio.h>
#include <math.h>
#include "fkn.h"
#include "fknDlg.h"
#include "complex.h"
#include "mesh.h"
#include "spars.h"
#include "fullmatrix.h"
#include "FemmeDocCore.h"

#define ElementsPerSkinDepth 10

CMaterialProp::~CMaterialProp()
{
  if (Bdata != NULL)
    free(Bdata);
  if (Hdata != NULL)
    free(Hdata);
  if (slope != NULL)
    free(slope);
}

CMaterialProp::CMaterialProp()
{
  slope = NULL;
  Bdata = NULL;
  Hdata = NULL;
}

void CMaterialProp::GetSlopes()
{
  GetSlopes(0);
}

void CMaterialProp::GetSlopes(double omega)
{
  if (BHpoints == 0)
    return; // catch trivial case;
  if (slope != NULL)
    return; // already have computed the slopes;

  int i, k;
  BOOL CurveOK = FALSE;
  BOOL ProcessedLams = FALSE;
  CFullMatrix L;
  double l1, l2;
  CComplex* hn;
  double* bn;
  CComplex mu;

  L.Create(BHpoints);
  bn = (double*)calloc(BHpoints, sizeof(double));
  hn = (CComplex*)calloc(BHpoints, sizeof(CComplex));
  slope = (CComplex*)calloc(BHpoints, sizeof(CComplex));

  // strip off some info that we can use during the first
  // nonlinear iteration;
  mu_x = Bdata[1] / (muo * abs(Hdata[1]));
  mu_y = mu_x;
  Theta_hx = Theta_hn;
  Theta_hy = Theta_hn;

  // first, we need to doctor the curve if the problem is
  // being evaluated at a nonzero frequency.
  if (omega > 0) {
    // Make an effective B-H curve for harmonic problems.
    // this one convolves B(H) where H is sinusoidal with
    // a sine at the same frequency to get the effective
    // amplitude of B
    double munow, mumax = 0;
    for (i = 1; i < BHpoints; i++) {
      hn[i] = Hdata[i];
      for (k = 1, bn[i] = 0; k <= i; k++) {
        bn[i] += Re((4. * (Hdata[k] * Bdata[k - 1] - Hdata[k - 1] * Bdata[k]) * (-cos((Hdata[k - 1] * PI) / (2. * Hdata[i])) + cos((Hdata[k] * PI) / (2. * Hdata[i]))) + (-Bdata[k - 1] + Bdata[k]) * ((Hdata[k - 1] - Hdata[k]) * PI + Hdata[i] * (-sin((Hdata[k - 1] * PI) / Hdata[i]) + sin((Hdata[k] * PI) / Hdata[i])))) / ((Hdata[k - 1] - Hdata[k]) * PI));
      }
    }

    for (i = 1; i < BHpoints; i++) {
      Bdata[i] = bn[i];
      Hdata[i] = hn[i];
      munow = Re(Bdata[i] / Hdata[i]);
      if (munow > mumax)
        mumax = munow;
    }

    // apply complex permeability to approximate the
    // effects of hysteresis.  We will follow a kludge
    // suggested by O'Kelly where hysteresis angle
    // is proportional to permeability.  This implies
    // that loss goes with B^2
    for (i = 1; i < BHpoints; i++) {
      Hdata[i] *= exp(I * Bdata[i] * Theta_hn * DEG / (Hdata[i] * mumax));
    }

    MuMax = mumax / muo;
  }

  while (CurveOK != TRUE) {
    // make sure that the space for computing slopes is cleared out
    L.Wipe();

    // impose natural BC on the `left'
    l1 = Bdata[1] - Bdata[0];
    L.M[0][0] = 4. / l1;
    L.M[0][1] = 2. / l1;
    L.b[0] = 6. * (Hdata[1] - Hdata[0]) / (l1 * l1);

    // impose natural BC on the `right'
    int n = BHpoints;
    l1 = Bdata[n - 1] - Bdata[n - 2];
    L.M[n - 1][n - 1] = 4. / l1;
    L.M[n - 1][n - 2] = 2. / l1;
    L.b[n - 1] = 6. * (Hdata[n - 1] - Hdata[n - 2]) / (l1 * l1);

    for (i = 1; i < BHpoints - 1; i++) {
      l1 = Bdata[i] - Bdata[i - 1];
      l2 = Bdata[i + 1] - Bdata[i];

      L.M[i][i - 1] = 2. / l1;
      L.M[i][i] = 4. * (l1 + l2) / (l1 * l2);
      L.M[i][i + 1] = 2. / l2;

      L.b[i] = 6. * (Hdata[i] - Hdata[i - 1]) / (l1 * l1) + 6. * (Hdata[i + 1] - Hdata[i]) / (l2 * l2);
    }

    L.GaussSolve();
    for (i = 0; i < BHpoints; i++)
      slope[i] = L.b[i];

    // now, test to see if there are any "bad" segments in there.
    // it is probably sufficient to do this test just on the
    // real part of the BH curve...
    for (i = 1, CurveOK = TRUE; i < BHpoints; i++) {
      double L, c0, c1, c2, d0, d1, u0, u1, X0, X1;

      // it is probably sufficient to do this test just on the
      // real part of the BH curve.  We do the test on just the
      // real part of the curve by taking the real parts of
      // slope and Hdata
      d0 = slope[i - 1].re;
      d1 = slope[i].re;
      u0 = Hdata[i - 1].re;
      u1 = Hdata[i].re;
      L = Bdata[i] - Bdata[i - 1];

      c0 = d0;
      c1 = -(2. * (2. * d0 * L + d1 * L + 3. * u0 - 3. * u1)) / (L * L);
      c2 = (3. * (d0 * L + d1 * L + 2. * u0 - 2. * u1)) / (L * L * L);
      X0 = -1.;
      X1 = -1.;

      u0 = c1 * c1 - 4. * c0 * c2;
      // check out degenerate cases
      if (c2 == 0) {
        if (c1 != 0)
          X0 = -c0 / c1;
      } else if (u0 > 0) {
        u0 = sqrt(u0);
        X0 = -(c1 + u0) / (2. * c2);
        X1 = (-c1 + u0) / (2. * c2);
      }

      // now, see if we've struck gold!
      if (((X0 >= 0.) && (X0 <= L)) || ((X1 >= 0.) && (X1 <= L)))
        CurveOK = FALSE;
    }

    if (CurveOK != TRUE) // remedial action
    {
      // Smooth out input points
      // to get rid of rapid transitions;
      // Uses a 3-point moving average
      for (i = 1; i < BHpoints - 1; i++) {
        bn[i] = (Bdata[i - 1] + Bdata[i] + Bdata[i + 1]) / 3.;
        hn[i] = (Hdata[i - 1] + Hdata[i] + Hdata[i + 1]) / 3.;
      }

      for (i = 1; i < BHpoints - 1; i++) {
        Hdata[i] = hn[i];
        Bdata[i] = bn[i];
      }
    }

    if ((CurveOK == TRUE) && (ProcessedLams == FALSE)) {
      // if the material is laminated and has a non-zero conductivity,
      // we have to do some work to find the "right" apparent BH
      // curve for the material for AC problems.  Essentially, we have
      // to solve a 1-D finite element problem at each B-H point to
      // figure out how much flux the lamination is really carrying.
      if ((omega > 0) && (Lam_d != 0) && (Cduct != 0)) {
        // Calculate a new apparent b and h for each point on the
        // curve to account for the laminations.
        for (i = 1; i < BHpoints; i++) {
          mu = LaminatedBH(omega, i);
          bn[i] = abs(mu * Hdata[i]);
          hn[i] = bn[i] / mu;
        }

        // Replace the original BH points with the apparent ones
        for (i = 1; i < BHpoints; i++) {
          Bdata[i] = bn[i];
          Hdata[i] = hn[i];
        }

        // Make the program check the consistency and recompute the
        // proper slopes of the new BH curve one more time;
        CurveOK = FALSE;
      }

      // take care of LamType=0 situation by changing the apparent B-H curve.
      if ((LamType == 0) && (LamFill != 1)) {
        // this is changed from the previous version so that
        // a complex-valued H can be properly accommodated
        // in the algorithm.
        for (i = 1; i < BHpoints; i++) {
          mu = LamFill * Bdata[i] / Hdata[i] + (1. - LamFill) * muo;
          Bdata[i] = abs(mu * Hdata[i]);
          Hdata[i] = Bdata[i] / mu;
        }
        // Make the program check the consistency and recompute the
        // proper slopes of the new BH curve one more time;
        CurveOK = FALSE;
      }

      ProcessedLams = TRUE;
    }
  }

  //	FILE *fp;
  //	fp=fopen("c:\\temp\\bhjunk.txt","wt");
  //	for(i=1;i<BHpoints;i++)
  //		fprintf(fp,"%g	%g	%g\n",abs(Hdata[i]),Re(Bdata[i]*abs(Hdata[i])/Hdata[i]),Im(Bdata[i]*abs(Hdata[i])/Hdata[i]));
  //	fclose(fp);

  free(bn);
  free(hn);
  return;
}

CComplex CMaterialProp::GetH(double B)
{
  double b, z, z2, l;
  CComplex h;
  int i;

  b = fabs(B);

  if (BHpoints == 0)
    return CComplex(b / (mu_x * muo));

  if (b > Bdata[BHpoints - 1])
    return (Hdata[BHpoints - 1] + slope[BHpoints - 1] * (b - Bdata[BHpoints - 1]));

  for (i = 0; i < BHpoints - 1; i++)
    if ((b >= Bdata[i]) && (b <= Bdata[i + 1])) {
      l = (Bdata[i + 1] - Bdata[i]);
      z = (b - Bdata[i]) / l;
      z2 = z * z;
      h = (1. - 3. * z2 + 2. * z2 * z) * Hdata[i] + z * (1. - 2. * z + z2) * l * slope[i] + z2 * (3. - 2. * z) * Hdata[i + 1] + z2 * (z - 1.) * l * slope[i + 1];
      return h;
    }

  return CComplex(0);
}

CComplex CMaterialProp::GetdHdB(double B)
{
  double b, z, l;
  CComplex h;
  int i;

  b = fabs(B);

  if (BHpoints == 0)
    return CComplex(b / (mu_x * muo));

  if (b > Bdata[BHpoints - 1])
    return slope[BHpoints - 1];

  for (i = 0; i < BHpoints - 1; i++)
    if ((b >= Bdata[i]) && (b <= Bdata[i + 1])) {
      l = (Bdata[i + 1] - Bdata[i]);
      z = (b - Bdata[i]) / l;
      h = 6. * z * (z - 1.) * Hdata[i] / l + (1. - 4. * z + 3. * z * z) * slope[i] + 6. * z * (1. - z) * Hdata[i + 1] / l + z * (3. * z - 2.) * slope[i + 1];
      return h;
    }

  return CComplex(0);
}

CComplex CMaterialProp::Get_v(double B)
{
  if (B == 0)
    return slope[0];

  return (GetH(B) / B);
}

CComplex CMaterialProp::Get_dvB2(double B)
{
  if (B == 0)
    return 0;

  return 0.5 * (GetdHdB(B) / (B * B) - GetH(B) / (B * B * B));
}

void CMaterialProp::GetBHProps(double B, double& v, double& dv)
{
  // version to use in the magnetostatic case in
  // which we know that v and dv ought to be real-valued.
  CComplex vc, dvc;

  GetBHProps(B, vc, dvc);
  v = Re(vc);
  dv = Re(dvc);
}

void CMaterialProp::GetBHProps(double B, CComplex& v, CComplex& dv)
{
  double b, z, z2, l;
  CComplex h, dh;
  int i;

  b = fabs(B);

  if (BHpoints == 0) {
    v = mu_x;
    dv = 0;
    return;
  }

  if (b == 0) {
    v = slope[0];
    dv = 0;
    return;
  }

  if (b > Bdata[BHpoints - 1]) {
    h = (Hdata[BHpoints - 1] + slope[BHpoints - 1] * (b - Bdata[BHpoints - 1]));
    dh = slope[BHpoints - 1];
    v = h / b;
    dv = 0.5 * (dh / (b * b) - h / (b * b * b));
    return;
  }

  for (i = 0; i < BHpoints - 1; i++)
    if ((b >= Bdata[i]) && (b <= Bdata[i + 1])) {
      l = (Bdata[i + 1] - Bdata[i]);
      z = (b - Bdata[i]) / l;
      z2 = z * z;
      h = (1. - 3. * z2 + 2. * z2 * z) * Hdata[i] + z * (1. - 2. * z + z2) * l * slope[i] + z2 * (3. - 2. * z) * Hdata[i + 1] + z2 * (z - 1.) * l * slope[i + 1];
      dh = 6. * z * (z - 1.) * Hdata[i] / l + (1. - 4. * z + 3. * z * z) * slope[i] + 6. * z * (1. - z) * Hdata[i + 1] / l + z * (3. * z - 2.) * slope[i + 1];
      v = h / b;
      dv = 0.5 * (dh / (b * b) - h / (b * b * b));
      return;
    }
}

CComplex CMaterialProp::LaminatedBH(double w, int i)
{
  int k, n, iter = 0;
  CComplex *m0, *m1, *b, *x;
  double L, o, d, ds, B, lastres, res;
  double Relax = 1;
  CComplex mu, vo, vi, c, H;
  CComplex Md, Mo;
  BOOL Converged = FALSE;
  res = 0;

  // Base the required element spacing on the skin depth
  // at the surface of the material
  mu = Bdata[i] / Hdata[i];
  o = Cduct * 1.e6;
  d = (Lam_d * 0.001) / 2.;
  ds = sqrt(2 / (w * o * abs(mu)));
  n = ElementsPerSkinDepth * ((int)ceil(d / ds));
  L = d / ((double)n);

  x = (CComplex*)calloc(n + 1, sizeof(CComplex));
  b = (CComplex*)calloc(n + 1, sizeof(CComplex));
  m0 = (CComplex*)calloc(n + 1, sizeof(CComplex));
  m1 = (CComplex*)calloc(n + 1, sizeof(CComplex));

  do {
    // make sure that the old stuff is wiped out;
    for (k = 0; k <= n; k++) {
      m0[k] = 0;
      m1[k] = 0;
      b[k] = 0;
    }

    // build matrix
    for (k = 0; k < n; k++) {
      if (iter != 0) {
        B = abs(x[k + 1] - x[k]) / L;
        vi = GetdHdB(B);
        vo = GetH(B) / B;
      } else {
        vo = 1. / mu;
        vi = 1. / mu;
      }

      Md = ((vi + vo) / (2. * L) + I * w * o * L / 4.);
      Mo = (-(vi + vo) / (2. * L) + I * w * o * L / 4.);
      m0[k] += Md;
      m1[k] += Mo;
      m0[k + 1] += Md;

      Md = ((vi - vo) / (2. * L));
      Mo = (-(vi - vo) / (2. * L));
      b[k] += (Md * x[k] + Mo * x[k + 1]);
      b[k + 1] += (Mo * x[k] + Md * x[k + 1]);
    }

    // set boundary conditions
    m1[0] = 0;
    b[0] = 0;
    b[n] += Hdata[i];

    // solve tridiagonal equation
    // solution ends up in b;
    for (k = 0; k < n; k++) {
      c = m1[k] / m0[k];
      m0[k + 1] -= m1[k] * c;
      b[k + 1] -= b[k] * c;
    }
    b[n] = b[n] / m0[n];
    for (k = n - 1; k >= 0; k--)
      b[k] = (b[k] - m1[k] * b[k + 1]) / m0[k];

    iter++;

    lastres = res;
    res = abs(b[n] - x[n]) / d;

    if (res < 1.e-8)
      Converged = TRUE;

    // Do the same relaxation scheme as is implemented
    // in the solver to make sure that this effective
    // lamination permeability calculation converges
    if (iter > 5) {
      if ((res > lastres) && (Relax > 0.1))
        Relax /= 2.;
      else
        Relax += 0.1 * (1. - Relax);
    }

    for (k = 0; k <= n; k++)
      x[k] = Relax * b[k] + (1.0 - Relax) * x[k];

  } while (Converged != TRUE);

  mu = x[n] / (Hdata[i] * d);

  free(x);
  free(b);
  free(m0);
  free(m1);

  return mu;
}

// get incremental permeability of a nonlinear material for use in
// incremental permeability formulation about DC offset
void CMaterialProp::IncrementalPermeability(double B, double w, CComplex& mu1, CComplex& mu2)
{
  // B == flux density in Tesla
  // w == frequency in rad/s

  double muinc, murel;
  CComplex k;
  double mu;

  // get incremental permeability of the DC material
  // (i.e. incremental permeability at the offset)
  muinc = 1. / (muo * Re(GetdHdB(B)));
  murel = 1. / (muo * Re(Get_v(B)));

  // if material is not laminated, just apply hysteresis lag...
  if ((Lam_d == 0) || (LamFill == 0)) {
    mu1 = muinc * exp(-I * Theta_hn * DEG * muinc / MuMax);
    mu2 = murel * exp(-I * Theta_hn * DEG * murel / MuMax);
    return;
  }

  // crap.  Need to make an equivalent permeability that rolls in the effects of laminated
  // eddy currents, using the incremental permeability as the basis for creating the impedance.
  // this can get annoying because we need to back out the iron portion of the permeability
  // in the lamfill<1 case...

  if (Cduct != 0) {
    CComplex deg45;
    deg45 = 1 + I;
    CComplex K, halflag;
    double ds;

    // incremental permeability direction
    mu = (muinc - (1. - LamFill)) / LamFill;
    halflag = exp(-I * Theta_hn * DEG * mu / (2. * MuMax));
    ds = sqrt(2. / (0.4 * PI * w * Cduct * mu));
    K = halflag * deg45 * Lam_d * 0.001 / (2. * ds);
    mu1 = (LamFill * mu * tanh(K) / K + (1. - LamFill));

    // normal permeability direction
    mu = (murel - (1. - LamFill)) / LamFill;
    halflag = exp(-I * Theta_hn * DEG * mu / (2. * MuMax));
    ds = sqrt(2. / (0.4 * PI * w * Cduct * mu));
    K = halflag * deg45 * Lam_d * 0.001 / (2. * ds);
    mu2 = (LamFill * mu * tanh(K) / K + (1. - LamFill));
    return;
  } else {
    // incremental permeability direction
    mu = (muinc - (1. - LamFill)) / LamFill;
    mu1 = (mu * exp(-I * Theta_hn * DEG * mu / MuMax) * LamFill + (1. - LamFill));

    // normal permeability direction
    mu = (murel - (1. - LamFill)) / LamFill;
    mu2 = (mu * exp(-I * Theta_hn * DEG * mu / MuMax) * LamFill + (1. - LamFill));
    return;
  }
}

void CMaterialProp::IncrementalPermeability(double B, double& mu1, double& mu2)
{
  // B == flux density in Tesla

  double muinc, murel;

  // get incremental permeability of the DC material
  // (i.e. incremental permeability at the offset)
  muinc = 1. / (muo * Re(GetdHdB(B)));
  murel = 1. / (muo * Re(Get_v(B)));

  // if material is not laminated, just return
  if ((Lam_d == 0) || (LamFill == 0)) {
    mu1 = muinc;
    mu2 = murel;
    return;
  }

  // incremental permeability direction
  mu1 = (muinc * LamFill + (1. - LamFill));

  // normal permeability direction
  mu2 = (murel * LamFill + (1. - LamFill));

  return;
}
