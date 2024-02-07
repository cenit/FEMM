#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "fkn.h"
#include "fknDlg.h"
#include "complex.h"
#include "mesh.h"
#include "spars.h"
#include "FemmeDocCore.h"

// #define NEWTON

// since all node positions were converted to units of cm
// the proper LengthConv is converts centimeters to meters
#define LengthConv 0.01

double Power(double x, int y);

void CFemmeDocCore::GetPrev2DB(int k, double& B1p, double& B2p)
{
  int i, n[3];
  double b[3], c[3], da;

  for (i = 0; i < 3; i++)
    n[i] = meshele[k].p[i];

  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
  da = (b[0] * c[1] - b[1] * c[0]);

  B1p = 0;
  B2p = 0;

  for (i = 0; i < 3; i++) {
    B1p += Aprev[n[i]] * c[i] / (da * LengthConv);
    B2p -= Aprev[n[i]] * b[i] / (da * LengthConv);
  }
}

BOOL CFemmeDocCore::Harmonic2D(CBigComplexLinProb& L)
{
  int i, j, k, ww, s, pctr;
  CComplex Mx[3][3], My[3][3], Mxy[3][3];
  CComplex Me[3][3], be[3]; // element matrices;
  double l[3], p[3], q[3]; // element shape parameters;
  int n[3]; // numbers of nodes for a particular element;
  double a, r, t, x, y, B, w, res, lastres, ds, Cduct;
  CComplex K, mu, dv, B1, B2, v[3], u[3], mu1, mu2, lag, halflag, deg45, Jv;
  CComplex **Mu, *V_old;
  double c = PI * 4.e-05;
  double units[] = { 2.54, 0.1, 1., 100., 0.00254, 1.e-04 };
  CElement* El;
  int Iter = 0;
  BOOL LinearFlag = TRUE;
  BOOL bIncremental = FALSE;

  if (PrevSoln.GetLength() > 0)
    bIncremental = TRUE;
  res = 0;

  // #ifndef NEWTON
  CComplex murel, muinc;
  // #else;
  CComplex Mnh[3][3];
  CComplex Mna[3][3];
  CComplex Mns[3][3];
  // #endif

  CComplex Mn[3][3];

  deg45 = 1 + I;
  w = Frequency * 2. * PI;

  CComplex *CircInt1, *CircInt2, *CircInt3;

  // Can't handle LamType==1 or LamType==2 in AC problems.
  // Detect if this is being attempted.
  for (i = 0; i < NumEls; i++) {
    if ((blockproplist[meshele[i].blk].LamType == 1) || (blockproplist[meshele[i].blk].LamType == 2)) {
      MsgBox("On-edge lamination not supported in AC analyses");
      return FALSE;
    }
  }

  // Go through and evaluate permeability for regions subject to prox effects
  for (i = 0; i < NumBlockLabels; i++)
    GetFillFactor(i);

  V_old = (CComplex*)calloc(NumNodes + NumCircProps, sizeof(CComplex));

  // check to see if any circuits have been defined and process them;
  if (NumCircProps > 0) {
    CircInt1 = (CComplex*)calloc(NumCircProps, sizeof(CComplex));
    CircInt2 = (CComplex*)calloc(NumCircProps, sizeof(CComplex));
    CircInt3 = (CComplex*)calloc(NumCircProps, sizeof(CComplex));
    for (i = 0; i < NumEls; i++) {
      if (meshele[i].lbl >= 0)
        if (labellist[meshele[i].lbl].InCircuit != -1) {
          El = &meshele[i];

          // get element area;
          for (k = 0; k < 3; k++)
            n[k] = El->p[k];
          p[0] = meshnode[n[1]].y - meshnode[n[2]].y;
          p[1] = meshnode[n[2]].y - meshnode[n[0]].y;
          p[2] = meshnode[n[0]].y - meshnode[n[1]].y;
          q[0] = meshnode[n[2]].x - meshnode[n[1]].x;
          q[1] = meshnode[n[0]].x - meshnode[n[2]].x;
          q[2] = meshnode[n[1]].x - meshnode[n[0]].x;
          a = (p[0] * q[1] - p[1] * q[0]) / 2.;
          //	r=(meshnode[n[0]].x+meshnode[n[1]].x+meshnode[n[2]].x)/3.;

          // if coils are wound, they act like they have
          // a zero "bulk" conductivity...
          Cduct = blockproplist[El->blk].Cduct;
          if (labellist[El->lbl].bIsWound)
            Cduct = 0;

          // evaluate integrals;

          // total cross-section of circuit;
          CircInt1[labellist[El->lbl].InCircuit] += a;

          // integral of conductivity over the circuit;
          CircInt2[labellist[El->lbl].InCircuit] += a * Cduct;

          // integral of applied J over current;
          CircInt3[labellist[El->lbl].InCircuit] += (blockproplist[El->blk].Jr + I * blockproplist[El->blk].Ji) * a * 100.;
        }
    }

    for (i = 0; i < NumCircProps; i++) {
      // Case 0 :: a priori known voltage gradient is applied to the region;
      // Case 1 :: flat current density is applied to the region;
      // Case 2 :: voltage gradient applied to the region, but we gotta solve for it...

      if (circproplist[i].CircType == 0) // specified current
      {
        if (CircInt2[i] == 0) { // circuit composed of zero cond. materials
          circproplist[i].Case = 1;
          if (CircInt1[i] == 0.)
            circproplist[i].J = 0.;
          else
            circproplist[i].J = 0.01 * ((circproplist[i].Amps_re + I * circproplist[i].Amps_im) - CircInt3[i]) / CircInt1[i];
        } else {
          circproplist[i].Case = 2; // need to include an extra
                                    // entry in matrix to solve for
                                    // voltage grad in the circuit
        }
      } else {
        // case where voltage gradient is specified a priori...
        circproplist[i].Case = 0;
        circproplist[i].dV = circproplist[i].dVolts_re + I * circproplist[i].dVolts_im;
      }
    }
  }

  // compute effective permeability for each block type;
  Mu = (CComplex**)calloc(NumBlockProps, sizeof(CComplex*));
  for (i = 0; i < NumBlockProps; i++)
    Mu[i] = (CComplex*)calloc(2, sizeof(CComplex));

  for (k = 0; k < NumBlockProps; k++) {

    if (blockproplist[k].LamType == 0) {
      Mu[k][0] = blockproplist[k].mu_x * exp(-I * blockproplist[k].Theta_hx * DEG);
      Mu[k][1] = blockproplist[k].mu_y * exp(-I * blockproplist[k].Theta_hy * DEG);

      if (blockproplist[k].Lam_d != 0) {
        if (blockproplist[k].Cduct != 0) {
          halflag = exp(-I * blockproplist[k].Theta_hx * DEG / 2.);
          ds = sqrt(2. / (0.4 * PI * w * blockproplist[k].Cduct * blockproplist[k].mu_x));
          K = halflag * deg45 * blockproplist[k].Lam_d * 0.001 / (2. * ds);
          Mu[k][0] = ((Mu[k][0] * tanh(K)) / K) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);

          halflag = exp(-I * blockproplist[k].Theta_hy * DEG / 2.);
          ds = sqrt(2. / (0.4 * PI * w * blockproplist[k].Cduct * blockproplist[k].mu_y));
          K = halflag * deg45 * blockproplist[k].Lam_d * 0.001 / (2. * ds);
          Mu[k][1] = ((Mu[k][1] * tanh(K)) / K) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
        } else {
          Mu[k][0] = Mu[k][0] * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
          Mu[k][1] = Mu[k][1] * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
        }
      }
    } else {
      Mu[k][0] = 1;
      Mu[k][1] = 1;
    }
  }

  do {

    TheView->SetDlgItemText(IDC_FRAME1, "Matrix Construction");
    TheView->m_prog1.SetPos(0);
    pctr = 0;

    if (Iter > 0)
      L.Wipe();

    // first, tack in air gap element contributions
    for (i = 0; i < NumAGEs; i++) {
      double K, Ki;
      double MG[10][10];
      double ci, co;
      int nn[10];
      double ww[10];

      // K = dr/(R*dtta)
      K = 2. * (agelist[i].ro - agelist[i].ri) / ((PI / 180.) * (agelist[i].totalArcLength / agelist[i].totalArcElements) * (agelist[i].ro + agelist[i].ri));
      Ki = 1. / K;
      ci = agelist[i].InnerShift;
      co = agelist[i].OuterShift;

      if (ci > co) {
        ci = ci - co;
        co = 0;
      } else {
        ci = 1 - co + ci;
        co = 1;
      }

      // build the element matrix for each quad element in the annulus (same for each element)
      // matrix for quad element derived from serendipity element
      MG[0][0] = (5 * Power(-1 + ci, 2) * Power(ci, 4) * (K + Ki)) / 48.;
      MG[0][1] = -((-1 + ci) * Power(ci, 3) * (5 * (-1 + ci * (-5 + 4 * ci)) * K + (-5 + ci * (-19 + 14 * ci)) * Ki)) / 48.;
      MG[0][2] = ((-1 + ci) * Power(ci, 2) * (5 * (2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * K + (10 + ci * (1 + 3 * ci * (-7 + 4 * ci))) * Ki)) / 48.;
      MG[0][3] = -(Power(-1 + ci, 2) * Power(ci, 2) * (5 * (-2 + ci * (-3 + 4 * ci)) * K + (2 + ci * (-3 + 2 * ci)) * Ki)) / 48.;
      MG[0][4] = (Power(-1 + ci, 3) * Power(ci, 3) * (5 * K - Ki)) / 48.;
      MG[0][5] = ((-1 + ci) * Power(ci, 2) * (-1 + co) * Power(co, 2) * (K - 5 * Ki)) / 48.;
      MG[0][6] = -((-1 + ci) * Power(ci, 2) * co * ((-1 + co * (-5 + 4 * co)) * K + (5 + (19 - 14 * co) * co) * Ki)) / 48.;
      MG[0][7] = ((-1 + ci) * Power(ci, 2) * ((2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K - (10 + co * (1 + 3 * co * (-7 + 4 * co))) * Ki)) / 48.;
      MG[0][8] = -((-1 + ci) * Power(ci, 2) * (-1 + co) * ((-2 + co * (-3 + 4 * co)) * K + (-2 + (3 - 2 * co) * co) * Ki)) / 48.;
      MG[0][9] = ((-1 + ci) * Power(ci, 2) * Power(-1 + co, 2) * co * (K + Ki)) / 48.;
      MG[1][1] = (Power(ci, 2) * (5 * Power(1 + (5 - 4 * ci) * ci, 2) * K + (5 + ci * (38 + ci * (49 + 4 * ci * (-29 + 11 * ci)))) * Ki)) / 48.;
      MG[1][2] = (-5 * ci * (-1 + 2 * ci) * (-2 + 3 * (-1 + ci) * ci) * (-1 + ci * (-5 + 4 * ci)) * K + ci * (10 + ci * (39 - ci * (50 + ci * (85 + 6 * ci * (-23 + 8 * ci))))) * Ki) / 48.;
      MG[1][3] = ((-1 + ci) * ci * (5 * (2 + ci * (13 + ci * (3 + 16 * (-2 + ci) * ci))) * K + (-2 + 5 * ci * (1 + ci * (3 + 4 * (-2 + ci) * ci))) * Ki)) / 48.;
      MG[1][4] = -(Power(-1 + ci, 2) * Power(ci, 2) * (5 * (-1 + ci * (-5 + 4 * ci)) * K + Ki + ci * (-1 + 2 * ci) * Ki)) / 48.;
      MG[1][5] = -(ci * (-1 + co) * Power(co, 2) * ((-1 + ci * (-5 + 4 * ci)) * K + (5 + (19 - 14 * ci) * ci) * Ki)) / 48.;
      MG[1][6] = (ci * co * ((-1 + ci * (-5 + 4 * ci)) * (-1 + co * (-5 + 4 * co)) * K + (-5 + ci * (-19 + 14 * ci) - 19 * co + ci * (-77 + 58 * ci) * co + 2 * (7 + (29 - 22 * ci) * ci) * Power(co, 2)) * Ki)) / 48.;
      MG[1][7] = (-(ci * (-1 + ci * (-5 + 4 * ci)) * (2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K) + ci * (-10 + co * (-1 + 3 * (7 - 4 * co) * co) + ci * (-38 + co + 99 * Power(co, 2) - 60 * Power(co, 3)) + Power(ci, 2) * (28 + 2 * co * (-1 + 3 * co * (-13 + 8 * co)))) * Ki) / 48.;
      MG[1][8] = (ci * (-1 + co) * ((-1 + ci * (-5 + 4 * ci)) * (-2 + co * (-3 + 4 * co)) * K + (2 + co * (-3 + 2 * co) + Power(ci, 2) * (4 + 2 * (9 - 10 * co) * co) + ci * (-2 + co * (-21 + 22 * co))) * Ki)) / 48.;
      MG[1][9] = -(ci * Power(-1 + co, 2) * co * ((-1 + ci * (-5 + 4 * ci)) * K + (-1 + ci - 2 * Power(ci, 2)) * Ki)) / 48.;
      MG[2][2] = (5 * Power(-2 + ci + 9 * Power(ci, 2) - 6 * Power(ci, 3), 2) * K + (20 + (-1 + ci) * ci * (-4 + 3 * (-1 + ci) * ci * (-25 + 24 * (-1 + ci) * ci))) * Ki) / 48.;
      MG[2][3] = (-5 * (4 + Power(ci, 2) * (-33 + ci * (18 + ci * (65 + 6 * ci * (-13 + 4 * ci))))) * K + (4 + Power(ci, 2) * (39 - ci * (30 + ci * (115 + 6 * ci * (-25 + 8 * ci))))) * Ki) / 48.;
      MG[2][4] = (Power(-1 + ci, 2) * ci * (5 * (2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * K + (-2 + ci * (-5 + 3 * ci * (-5 + 4 * ci))) * Ki)) / 48.;
      MG[2][5] = ((-1 + co) * Power(co, 2) * ((2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * K - (10 + ci * (1 + 3 * ci * (-7 + 4 * ci))) * Ki)) / 48.;
      MG[2][6] = (-((2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * co * (-1 + co * (-5 + 4 * co)) * K) + co * (-10 - 38 * co + 28 * Power(co, 2) + Power(ci, 2) * (21 + 99 * co - 78 * Power(co, 2)) + ci * (-1 + co - 2 * Power(co, 2)) + 12 * Power(ci, 3) * (-1 + co * (-5 + 4 * co))) * Ki) / 48.;
      MG[2][7] = ((2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * (2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K - (2 * (10 + co) + 6 * Power(co, 2) * (-7 + 4 * co) + 3 * Power(ci, 2) * (-14 + co * (5 + (55 - 36 * co) * co)) + ci * (2 + co * (5 + 3 * (5 - 4 * co) * co)) + 12 * Power(ci, 3) * (2 + co * (-1 - 9 * co + 6 * Power(co, 2)))) * Ki) / 48.;
      MG[2][8] = (-((2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * (2 + co - 7 * Power(co, 2) + 4 * Power(co, 3)) * K) + (-1 + co) * (4 + 2 * ci * (5 + 3 * (5 - 4 * ci) * ci) + 3 * (-2 + ci * (3 + (17 - 12 * ci) * ci)) * co + 2 * (2 + ci * (-7 + 3 * ci * (-11 + 8 * ci))) * Power(co, 2)) * Ki) / 48.;
      MG[2][9] = (Power(-1 + co, 2) * co * ((2 + ci * (-1 - 9 * ci + 6 * Power(ci, 2))) * K + (2 + ci * (5 + 3 * (5 - 4 * ci) * ci)) * Ki)) / 48.;
      MG[3][3] = (Power(-1 + ci, 2) * (5 * Power(2 + (3 - 4 * ci) * ci, 2) * K + (20 + ci * (36 + ci * (-35 - 60 * ci + 44 * Power(ci, 2)))) * Ki)) / 48.;
      MG[3][4] = -(Power(-1 + ci, 3) * ci * (5 * (-2 + ci * (-3 + 4 * ci)) * K + (-10 + ci * (-9 + 14 * ci)) * Ki)) / 48.;
      MG[3][5] = -((-1 + ci) * (-1 + co) * Power(co, 2) * ((-2 + ci * (-3 + 4 * ci)) * K + (-2 + (3 - 2 * ci) * ci) * Ki)) / 48.;
      MG[3][6] = ((-1 + ci) * co * ((-2 + ci * (-3 + 4 * ci)) * (-1 + co * (-5 + 4 * co)) * K + (2 + ci * (-3 + 2 * ci) - 2 * co + ci * (-21 + 22 * ci) * co + 2 * (2 + (9 - 10 * ci) * ci) * Power(co, 2)) * Ki)) / 48.;
      MG[3][7] = (-((2 + ci - 7 * Power(ci, 2) + 4 * Power(ci, 3)) * (2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K) + (-1 + ci) * (4 + 2 * co * (5 + 3 * (5 - 4 * co) * co) + ci * (-6 + 3 * co * (3 + (17 - 12 * co) * co)) + 2 * Power(ci, 2) * (2 + co * (-7 + 3 * co * (-11 + 8 * co)))) * Ki) / 48.;
      MG[3][8] = ((-1 + ci) * (-1 + co) * ((-2 + ci * (-3 + 4 * ci)) * (-2 + co * (-3 + 4 * co)) * K + (-20 + 3 * ci * (1 + 2 * co) * (-6 + 5 * co) + 2 * co * (-9 + 14 * co) + Power(ci, 2) * (28 + 30 * co - 44 * Power(co, 2))) * Ki)) / 48.;
      MG[3][9] = -((-1 + ci) * Power(-1 + co, 2) * co * ((-2 + ci * (-3 + 4 * ci)) * K + (10 + (9 - 14 * ci) * ci) * Ki)) / 48.;
      MG[4][4] = (5 * Power(-1 + ci, 4) * Power(ci, 2) * (K + Ki)) / 48.;
      MG[4][5] = (Power(-1 + ci, 2) * ci * (-1 + co) * Power(co, 2) * (K + Ki)) / 48.;
      MG[4][6] = -(Power(-1 + ci, 2) * ci * co * ((-1 + co * (-5 + 4 * co)) * K + (-1 + co - 2 * Power(co, 2)) * Ki)) / 48.;
      MG[4][7] = (Power(-1 + ci, 2) * ci * ((2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K + (2 + co * (5 + 3 * (5 - 4 * co) * co)) * Ki)) / 48.;
      MG[4][8] = -(Power(-1 + ci, 2) * ci * (-1 + co) * ((-2 + co * (-3 + 4 * co)) * K + (10 + (9 - 14 * co) * co) * Ki)) / 48.;
      MG[4][9] = (Power(-1 + ci, 2) * ci * Power(-1 + co, 2) * co * (K - 5 * Ki)) / 48.;
      MG[5][5] = (5 * Power(-1 + co, 2) * Power(co, 4) * (K + Ki)) / 48.;
      MG[5][6] = -((-1 + co) * Power(co, 3) * (5 * (-1 + co * (-5 + 4 * co)) * K + (-5 + co * (-19 + 14 * co)) * Ki)) / 48.;
      MG[5][7] = ((-1 + co) * Power(co, 2) * (5 * (2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K + (10 + co * (1 + 3 * co * (-7 + 4 * co))) * Ki)) / 48.;
      MG[5][8] = -(Power(-1 + co, 2) * Power(co, 2) * (5 * (-2 + co * (-3 + 4 * co)) * K + (2 + co * (-3 + 2 * co)) * Ki)) / 48.;
      MG[5][9] = (Power(-1 + co, 3) * Power(co, 3) * (5 * K - Ki)) / 48.;
      MG[6][6] = (Power(co, 2) * (5 * Power(1 + (5 - 4 * co) * co, 2) * K + (5 + co * (38 + co * (49 + 4 * co * (-29 + 11 * co)))) * Ki)) / 48.;
      MG[6][7] = (-5 * co * (-1 + 2 * co) * (-2 + 3 * (-1 + co) * co) * (-1 + co * (-5 + 4 * co)) * K + co * (10 + co * (39 - co * (50 + co * (85 + 6 * co * (-23 + 8 * co))))) * Ki) / 48.;
      MG[6][8] = ((-1 + co) * co * (5 * (2 + co * (13 + co * (3 + 16 * (-2 + co) * co))) * K + (-2 + 5 * co * (1 + co * (3 + 4 * (-2 + co) * co))) * Ki)) / 48.;
      MG[6][9] = -(Power(-1 + co, 2) * Power(co, 2) * (5 * (-1 + co * (-5 + 4 * co)) * K + Ki + co * (-1 + 2 * co) * Ki)) / 48.;
      MG[7][7] = (5 * Power(-2 + co + 9 * Power(co, 2) - 6 * Power(co, 3), 2) * K + (20 + (-1 + co) * co * (-4 + 3 * (-1 + co) * co * (-25 + 24 * (-1 + co) * co))) * Ki) / 48.;
      MG[7][8] = (-5 * (4 + Power(co, 2) * (-33 + co * (18 + co * (65 + 6 * co * (-13 + 4 * co))))) * K + (4 + Power(co, 2) * (39 - co * (30 + co * (115 + 6 * co * (-25 + 8 * co))))) * Ki) / 48.;
      MG[7][9] = (Power(-1 + co, 2) * co * (5 * (2 + co * (-1 - 9 * co + 6 * Power(co, 2))) * K + (-2 + co * (-5 + 3 * co * (-5 + 4 * co))) * Ki)) / 48.;
      MG[8][8] = (Power(-1 + co, 2) * (5 * Power(2 + (3 - 4 * co) * co, 2) * K + (20 + co * (36 + co * (-35 - 60 * co + 44 * Power(co, 2)))) * Ki)) / 48.;
      MG[8][9] = -(Power(-1 + co, 3) * co * (5 * (-2 + co * (-3 + 4 * co)) * K + (-10 + co * (-9 + 14 * co)) * Ki)) / 48.;
      MG[9][9] = (5 * Power(-1 + co, 4) * Power(co, 2) * (K + Ki)) / 48.;

      // Add each annulus element to the global stiffness matrix
      for (k = 0; k < agelist[i].totalArcElements; k++) {
        // inner nodes
        if ((k - 1) < 0) {
          nn[0] = agelist[i].node[agelist[i].totalArcElements - 1].n0;
          ww[0] = agelist[i].node[agelist[i].totalArcElements - 1].w0;
        } else {
          nn[0] = agelist[i].node[k - 1].n0;
          ww[0] = agelist[i].node[k - 1].w0;
        }

        nn[1] = agelist[i].node[k].n0;
        nn[2] = agelist[i].node[k].n1;
        nn[3] = agelist[i].node[k + 1].n1;
        ww[1] = agelist[i].node[k].w0;
        ww[2] = agelist[i].node[k].w1;
        ww[3] = agelist[i].node[k + 1].w1;

        if ((k + 2) > agelist[i].totalArcElements) {
          nn[4] = agelist[i].node[1].n1;
          ww[4] = agelist[i].node[1].w1;
        } else {
          nn[4] = agelist[i].node[k + 2].n1;
          ww[4] = agelist[i].node[k + 2].w1;
        }

        // outer nodes
        if ((k - 1) < 0) {
          nn[5] = agelist[i].node[agelist[i].totalArcElements - 1].n2;
          ww[5] = agelist[i].node[agelist[i].totalArcElements - 1].w2;
        } else {
          nn[5] = agelist[i].node[k - 1].n2;
          ww[5] = agelist[i].node[k - 1].w2;
        }

        nn[6] = agelist[i].node[k].n2;
        nn[7] = agelist[i].node[k].n3;
        nn[8] = agelist[i].node[k + 1].n3;
        ww[6] = agelist[i].node[k].w2;
        ww[7] = agelist[i].node[k].w3;
        ww[8] = agelist[i].node[k + 1].w3;

        if ((k + 2) > agelist[i].totalArcElements) {
          nn[9] = agelist[i].node[1].n3;
          ww[9] = agelist[i].node[1].w3;
        } else {
          nn[9] = agelist[i].node[k + 2].n3;
          ww[9] = agelist[i].node[k + 2].w3;
        }

        // fix antiperiodic weights...
        if ((k == 0) && (agelist[i].BdryFormat == 1)) {
          ww[0] = -ww[0];
          ww[5] = -ww[5];
        }
        if ((k == agelist[i].totalArcElements) && (agelist[i].BdryFormat == 1)) {
          ww[4] = -ww[4];
          ww[9] = -ww[9];
        }

        // scale by weight to get periodic/antiperiodic right
        for (int ii = 0; ii < 10; ii++)
          for (int jj = ii; jj < 10; jj++)
            L.AddTo(-MG[ii][jj] * ww[ii] * ww[jj], nn[ii], nn[jj]); // needs different sign than prob1big version
      }
    }

    // build element matrices using the matrices derived in Allaire's book.
    for (i = 0; i < NumEls; i++) {

      // update ``building matrix'' progress bar...
      j = (i * 20) / NumEls + 1;
      if (j > pctr) {
        j = pctr * 5;
        if (j > 100)
          j = 100;
        TheView->m_prog1.SetPos(j);
        pctr++;
      }

      // zero out Me, be;
      for (j = 0; j < 3; j++) {
        for (k = 0; k < 3; k++) {
          Me[j][k] = 0;
          Mx[j][k] = 0;
          My[j][k] = 0;
          Mxy[j][k] = 0;
          // #ifdef NEWTON
          if (ACSolver == 1) {
            Mnh[j][k] = 0;
            Mna[j][k] = 0;
            Mns[j][k] = 0;
          }
          // #endif
          Mn[j][k] = 0;
        }
        be[j] = 0;
      }

      // Determine shape parameters.
      // l == element side lengths;
      // p corresponds to the `b' parameter in Allaire
      // q corresponds to the `c' parameter in Allaire
      El = &meshele[i];

      for (k = 0; k < 3; k++)
        n[k] = El->p[k];
      p[0] = meshnode[n[1]].y - meshnode[n[2]].y;
      p[1] = meshnode[n[2]].y - meshnode[n[0]].y;
      p[2] = meshnode[n[0]].y - meshnode[n[1]].y;
      q[0] = meshnode[n[2]].x - meshnode[n[1]].x;
      q[1] = meshnode[n[0]].x - meshnode[n[2]].x;
      q[2] = meshnode[n[1]].x - meshnode[n[0]].x;
      for (j = 0, k = 1; j < 3; k++, j++) {
        if (k == 3)
          k = 0;
        l[j] = sqrt(pow(meshnode[n[k]].x - meshnode[n[j]].x, 2.) + pow(meshnode[n[k]].y - meshnode[n[j]].y, 2.));
      }
      a = (p[0] * q[1] - p[1] * q[0]) / 2.;

      // x-contribution;
      K = (-1. / (4. * a));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++) {
          Mx[j][k] += K * p[j] * p[k];
          if (j != k)
            Mx[k][j] += K * p[j] * p[k];
        }

      // y-contribution;
      K = (-1. / (4. * a));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++) {
          My[j][k] += K * q[j] * q[k];
          if (j != k)
            My[k][j] += K * q[j] * q[k];
        }

      // xy-contribution;
      K = (-1. / (4. * a));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++) {
          Mxy[j][k] += K * (p[j] * q[k] + p[k] * q[j]);
          if (j != k)
            Mxy[k][j] += K * (p[j] * q[k] + p[k] * q[j]);
        }

      // contribution from eddy currents;
      K = -I * a * w * blockproplist[meshele[i].blk].Cduct * c / 12.;

      // in-plane laminated blocks appear to have no conductivity;
      // eddy currents are accounted for in these elements by their
      // frequency-dependent permeability.
      if ((blockproplist[El->blk].LamType == 0) && (blockproplist[El->blk].Lam_d > 0))
        K = 0;

      // if this element is part of a wound coil,
      // it should have a zero "bulk" conductivity...
      if (labellist[El->lbl].bIsWound)
        K = 0;

      for (j = 0; j < 3; j++) {
        for (k = j; k < 3; k++) {
          Me[j][k] += K;
          Me[k][j] += K;
        }
      }

      // contributions to Me, be from derivative boundary conditions;
      for (j = 0; j < 3; j++) {
        if (El->e[j] >= 0) {
          if (lineproplist[El->e[j]].BdryFormat == 2) {
            // conversion factor is 10^(-4) (I think...)
            K = (-0.0001 * c * lineproplist[El->e[j]].c0 * l[j] / 6.);
            k = j + 1;
            if (k == 3)
              k = 0;
            Me[j][j] += 2 * K;
            Me[k][k] += 2 * K;
            Me[j][k] += K;
            Me[k][j] += K;

            K = (lineproplist[El->e[j]].c1 * l[j] / 2.) * 0.0001;
            be[j] += K;
            be[k] += K;
          }

          if (lineproplist[El->e[j]].BdryFormat == 1) {
            ds = sqrt(2. / (0.4 * PI * w * lineproplist[El->e[j]].Sig * lineproplist[El->e[j]].Mu));
            K = deg45 / (-ds * lineproplist[El->e[j]].Mu * 100.);
            K *= (l[j] / 6.);
            k = j + 1;
            if (k == 3)
              k = 0;
            Me[j][j] += 2 * K;
            Me[k][k] += 2 * K;
            Me[j][k] += K;
            Me[k][j] += K;
          }
        }
      }

      // contribution to be from current density in the block
      for (j = 0; j < 3; j++) {
        Jv = 0;
        if (labellist[El->lbl].InCircuit >= 0) {
          k = labellist[El->lbl].InCircuit;
          if (circproplist[k].Case == 1)
            Jv = circproplist[k].J;
          if (circproplist[k].Case == 0)
            Jv = -circproplist[k].dV * blockproplist[El->blk].Cduct;
        }
        K = -(blockproplist[El->blk].Jr + I * blockproplist[El->blk].Ji + Jv) * a / 3.;
        be[j] += K;

        if (labellist[El->lbl].InCircuit >= 0) {
          k = labellist[El->lbl].InCircuit;
          if (circproplist[k].Case == 2)
            L.b[NumNodes + k] += K;
        }
      }

      // do Case 2 circuit stuff for element
      if (labellist[El->lbl].InCircuit >= 0) {
        k = labellist[El->lbl].InCircuit;
        if (circproplist[k].Case == 2) {
          K = -I * a * w * blockproplist[meshele[i].blk].Cduct * c;
          for (j = 0; j < 3; j++)
            L.Put(L.Get(n[j], NumNodes + k) + K / 3., n[j], NumNodes + k);
          L.Put(L.Get(NumNodes + k, NumNodes + k) + K, NumNodes + k, NumNodes + k);
        }
      }

      ///////////////////////////////////////////////////////////////
      //
      //	New Nonlinear stuff
      //
      ///////////////////////////////////////////////////////////////

      // update permeability for the element;
      if (Iter == 0) {
        k = meshele[i].blk;
        meshele[i].mu1 = Mu[k][0];
        meshele[i].mu2 = Mu[k][1];
        meshele[i].v12 = 0;
        if (blockproplist[k].BHpoints > 0) {
          if (bIncremental == FALSE) {
            // There's no previous solution.  This is a standard nonlinear time harmonic problem
            LinearFlag = FALSE;
          } else {
            double B1p, B2p;

            //	Get B from previous solution
            GetPrev2DB(i, B1p, B2p);
            B = sqrt(B1p * B1p + B2p * B2p);

            // look up incremental permeability and assign it to the element;
            blockproplist[k].IncrementalPermeability(B, w, muinc, murel);
            if (B == 0) {
              meshele[i].mu1 = muinc;
              meshele[i].mu2 = muinc;
              meshele[i].v12 = 0;
            } else {
              // need to actually compute B1 and B2 to build incremental permeability tensor
              meshele[i].mu1 = B * B * muinc * murel / (B1p * B1p * murel + B2p * B2p * muinc);
              meshele[i].mu2 = B * B * muinc * murel / (B1p * B1p * muinc + B2p * B2p * murel);
              meshele[i].v12 = -B1p * B2p * (murel - muinc) / (B * B * murel * muinc);
            }
          }
        }
      } else {

        k = meshele[i].blk;

        if ((blockproplist[k].LamType == 0) && (meshele[i].mu1 == meshele[i].mu2)
            && (blockproplist[k].BHpoints > 0)) {
          for (j = 0, B1 = 0., B2 = 0.; j < 3; j++) {
            B1 += L.V[n[j]] * q[j];
            B2 += L.V[n[j]] * p[j];
          }
          B = c * sqrt(abs(B1 * conj(B1)) + abs(B2 * conj(B2))) / (0.02 * a);
          // correction for lengths in cm of 1/0.02

          // #ifdef NEWTON
          if (ACSolver == 1) {
            // find out new mu from saturation curve;
            blockproplist[k].GetBHProps(B, mu, dv);
            mu = 1. / (muo * mu);
            meshele[i].mu1 = mu;
            meshele[i].mu2 = mu;
            for (j = 0; j < 3; j++) {
              for (ww = 0, v[j] = 0; ww < 3; ww++)
                v[j] += (Mx[j][ww] + My[j][ww]) * L.V[n[ww]];
            }

            // Newton-like Iteration
            // Comment out for successive approx
            K = -200. * c * c * c * dv / a;
            for (j = 0; j < 3; j++)
              for (ww = 0; ww < 3; ww++) {
                // Still compute Mn, the approximate N-R matrix used in
                // the complex-symmetric approx.  This will be useful
                // w.r.t. preconditioning.  However, subtract it off of Mnh and Mna
                // so that there is no net addition.
                Mn[j][ww] = K * Re(v[j] * conj(v[ww]));
                Mnh[j][ww] = 0.5 * Re(K) * v[j] * conj(v[ww]) - Re(Mn[j][ww]);
                Mna[j][ww] = I * 0.5 * Im(K) * v[j] * conj(v[ww]) - I * Im(Mn[j][ww]);
                Mns[j][ww] = 0.5 * K * v[j] * v[ww];
              }
          }
          // #else
          else {
            // find out new mu from saturation curve;
            murel = 1. / (muo * blockproplist[k].Get_v(B));
            muinc = 1. / (muo * blockproplist[k].GetdHdB(B));

            // successive approximation;
            //		       K=muinc;                            // total incremental
            //			   K=murel;                            // total updated
            K = 2. * murel * muinc / (murel + muinc); // averaged
            meshele[i].mu1 = K;
            meshele[i].mu2 = K;
            K = -(1. / murel - 1 / K);
            for (j = 0; j < 3; j++)
              for (ww = 0; ww < 3; ww++)
                Mn[j][ww] = K * (Mx[j][ww] + My[j][ww]);
          }
          // #endif
        }
      }

      // Apply correction for elements subject to prox effects
      if ((blockproplist[meshele[i].blk].LamType > 2) && (Iter == 0)) {
        meshele[i].mu1 = labellist[meshele[i].lbl].ProximityMu;
        meshele[i].mu2 = labellist[meshele[i].lbl].ProximityMu;
      }

      // combine block matrices into global matrices;
      for (j = 0; j < 3; j++)
        for (k = 0; k < 3; k++) {

          // #ifdef NEWTON
          if (ACSolver == 1) {
            Me[j][k] += (Mx[j][k] / (El->mu2) + My[j][k] / (El->mu1) + Mxy[j][k] * (El->v12) + Mn[j][k]);
            be[j] += (Mnh[j][k] + Mna[j][k] + Mn[j][k]) * L.V[n[k]];
            be[j] += Mns[j][k] * L.V[n[k]].Conj();
          }
          // #else
          else {
            Me[j][k] += (Mx[j][k] / (El->mu2) + My[j][k] / (El->mu1) + Mxy[j][k] * (El->v12));
            be[j] += Mn[j][k] * L.V[n[k]];
          }
          // #endif
        }

      for (j = 0; j < 3; j++) {
        for (k = j; k < 3; k++) {
          //			L.Put(L.Get(n[j],n[k]) + Me[j][k],n[j],n[k]);
          L.AddTo(Me[j][k], n[j], n[k]);
          // #ifdef NEWTON
          if (ACSolver == 1) {
            if (Mnh[j][k] != 0)
              L.Put(L.Get(n[j], n[k], 1) + Mnh[j][k], n[j], n[k], 1);
            if (Mns[j][k] != 0)
              L.Put(L.Get(n[j], n[k], 2) + Mns[j][k], n[j], n[k], 2);
            if (Mna[j][k] != 0)
              L.Put(L.Get(n[j], n[k], 3) + Mna[j][k], n[j], n[k], 3);
          }
          // #endif
        }
        L.b[n[j]] += be[j];
      }
    }

    // add in contribution from point currents;
    for (i = 0; i < NumNodes; i++)
      if (meshnode[i].bc >= 0) {
        K = 0.01 * (nodeproplist[meshnode[i].bc].Jr + I * nodeproplist[meshnode[i].bc].Ji);
        L.b[i] += (-K);
      }

    // add in total current constraints for circuits;
    for (i = 0; i < NumCircProps; i++)
      if (circproplist[i].Case == 2) {
        L.b[NumNodes + i] += 0.01 * (circproplist[i].Amps_re + I * circproplist[i].Amps_im);
      }

    // apply fixed boundary conditions at points;
    for (i = 0; i < NumNodes; i++)
      if (meshnode[i].bc >= 0)
        if ((nodeproplist[meshnode[i].bc].Jr == 0) && (nodeproplist[meshnode[i].bc].Ji == 0)) {
          K = (nodeproplist[meshnode[i].bc].Ar + I * nodeproplist[meshnode[i].bc].Ai) / c;
          L.SetValue(i, K);
        }

    // apply fixed boundary conditions along segments;
    for (i = 0; i < NumEls; i++)
      for (j = 0; j < 3; j++) {
        k = j + 1;
        if (k == 3)
          k = 0;
        if (meshele[i].e[j] >= 0)
          if (lineproplist[meshele[i].e[j]].BdryFormat == 0) {
            if (Coords == 0) {
              // first point on the side;
              x = meshnode[meshele[i].p[j]].x;
              y = meshnode[meshele[i].p[j]].y;
              x /= units[LengthUnits];
              y /= units[LengthUnits];
              s = meshele[i].e[j];
              a = lineproplist[s].A0 + x * lineproplist[s].A1 + y * lineproplist[s].A2;
              K = (a / c) * exp(I * lineproplist[s].phi * DEG);
              L.SetValue(meshele[i].p[j], K);

              // second point on the side;
              x = meshnode[meshele[i].p[k]].x;
              y = meshnode[meshele[i].p[k]].y;
              x /= units[LengthUnits];
              y /= units[LengthUnits];
              s = meshele[i].e[j];
              a = lineproplist[s].A0 + x * lineproplist[s].A1 + y * lineproplist[s].A2;
              K = (a / c) * exp(I * lineproplist[s].phi * DEG);
              L.SetValue(meshele[i].p[k], K);
            } else {
              // first point on the side;
              x = meshnode[meshele[i].p[j]].x;
              y = meshnode[meshele[i].p[j]].y;
              r = sqrt(x * x + y * y);
              if ((x == 0) && (y == 0))
                t = 0;
              else
                t = atan2(y, x) / DEG;
              r /= units[LengthUnits];
              s = meshele[i].e[j];
              a = lineproplist[s].A0 + r * lineproplist[s].A1 + t * lineproplist[s].A2;
              K = (a / c) * exp(I * lineproplist[s].phi * DEG);
              L.SetValue(meshele[i].p[j], K);

              // second point on the side;
              x = meshnode[meshele[i].p[k]].x;
              y = meshnode[meshele[i].p[k]].y;
              r = sqrt(x * x + y * y);
              if ((x == 0) && (y == 0))
                t = 0;
              else
                t = atan2(y, x) / DEG;
              r /= units[LengthUnits];
              s = meshele[i].e[j];
              a = lineproplist[s].A0 + r * lineproplist[s].A1 + t * lineproplist[s].A2;
              K = (a / c) * exp(I * lineproplist[s].phi * DEG);
              L.SetValue(meshele[i].p[k], K);
            }
          }
      }

    // "fix" diagonal entries associated with circuits that have
    // applied current or voltage that is known a priori
    // so that solver doesn't throw a "singular" flag
    for (j = 0; j < NumCircProps; j++)
      if (circproplist[j].Case < 2)
        L.Put(L.Get(0, 0), NumNodes + j, NumNodes + j);

    for (k = 0; k < NumPBCs; k++) {
      if (pbclist[k].t == 0)
        L.Periodicity(pbclist[k].x, pbclist[k].y);
      if (pbclist[k].t == 1)
        L.AntiPeriodicity(pbclist[k].x, pbclist[k].y);
    }

    // solve the problem;
    for (j = 0; j < NumNodes + NumCircProps; j++)
      V_old[j] = L.V[j];

    if (L.bNewton) {
      L.Precision = __min(1.e-4, 0.001 * res);
      if (L.Precision < Precision)
        L.Precision = Precision;
    }
    if (L.PBCGSolveMod(Iter) == FALSE)
      return FALSE;

    if (LinearFlag == FALSE) {

      for (j = 0, x = 0, y = 0; j < NumNodes; j++) {
        x += Re((L.V[j] - V_old[j]) * conj(L.V[j] - V_old[j]));
        y += Re(L.V[j] * conj(L.V[j]));
      }

      if (y == 0)
        LinearFlag = TRUE;
      else {
        lastres = res;
        res = sqrt(x / y);
      }

      // relaxation if we need it
      if (Iter > 5) {
        if ((res > lastres) && (Relax > 0.1))
          Relax /= 2.;
        else
          Relax += 0.1 * (1. - Relax);

        for (j = 0; j < NumNodes + NumCircProps; j++)
          L.V[j] = Relax * L.V[j] + (1.0 - Relax) * V_old[j];
      }

      // report some results
      char outstr[256];
      // #ifdef NEWTON
      if (ACSolver == 1)
        sprintf(outstr, "Newton Iteration(%i) Relax=%.4g", Iter, Relax);
      // #else
      else
        sprintf(outstr, "Successive Approx(%i) Relax=%.4g", Iter, Relax);
      // #endif
      TheView->SetDlgItemText(IDC_FRAME2, outstr);
      j = (int)(100. * log10(res) / (log10(Precision) + 2.));
      if (j > 100)
        j = 100;
      TheView->m_prog2.SetPos(j);
    }

    // nonlinear iteration has to have a looser tolerance
    // than the linear solver--otherwise, things can't ever
    // converge.  Arbitrarily choose 100*tolerance.
    if ((res < 100. * Precision) && Iter > 0)
      LinearFlag = TRUE;

    Iter++;

  } while (LinearFlag == FALSE);

  for (i = 0; i < NumNodes; i++)
    L.b[i] = (L.V[i] * c); // convert answer back to AMPS
  for (i = 0; i < NumCircProps; i++)
    L.b[NumNodes + i] = (I * c * w * L.V[NumNodes + i]);
  // free up space allocated in this routine
  for (k = 0; k < NumBlockProps; k++)
    free(Mu[k]);
  free(Mu);
  free(V_old);
  if (NumCircProps > 0) {
    free(CircInt1);
    free(CircInt2);
    free(CircInt3);
  }

  return TRUE;
}

BOOL CFemmeDocCore::WriteHarmonic2D(CBigComplexLinProb& L)
{
  // write solution to disk;

  char c[1024];
  FILE *fp, *fz;
  int i, k;
  double cf;
  double unitconv[] = { 2.54, 0.1, 1., 100., 0.00254, 1.e-04 };

  // first, echo input .fem file to the .ans file;
  sprintf(c, "%s.fem", PathName);
  if ((fz = fopen(c, "rt")) == NULL) {
    Sleep(500);
    if ((fz = fopen(c, "rt")) == NULL) {
      MsgBox("Couldn't open %s.fem\n", PathName);
      return FALSE;
    }
  }

  sprintf(c, "%s.ans", PathName);
  if ((fp = fopen(c, "wt")) == NULL) {
    Sleep(500);
    if ((fp = fopen(c, "wt")) == NULL) {
      MsgBox("Couldn't write to %s.ans\n", PathName);
      return FALSE;
    }
  }

  while (fgets(c, 1024, fz) != NULL)
    fputs(c, fp);
  fclose(fz);

  // then print out node, line, and element information
  fprintf(fp, "[Solution]\n");
  cf = unitconv[LengthUnits];
  fprintf(fp, "%i\n", NumNodes);
  for (i = 0; i < NumNodes; i++) {
    fprintf(fp, "%.17g	%.17g	%.17g	%.17g	%i	", meshnode[i].x / cf,
        meshnode[i].y / cf, L.b[i].re, L.b[i].im, meshnode[i].bc);
    // include A from previous solution if this is an incremental permeability problem
    if (Aprev != NULL)
      fprintf(fp, "%.17g\n", Aprev[i]);
    else
      fprintf(fp, "\n");
  }
  fprintf(fp, "%i\n", NumEls);
  for (i = 0; i < NumEls; i++) {
    fprintf(fp, "%i	%i	%i	%i	%i	%i	%i",
        meshele[i].p[0], meshele[i].p[1], meshele[i].p[2], meshele[i].lbl,
        meshele[i].e[0], meshele[i].e[1], meshele[i].e[2]);
    // include J from previous problem if this is an incremental permeability problem
    if (Aprev != NULL)
      fprintf(fp, "	%.17g\n", meshele[i].Jprev);
    else
      fprintf(fp, "\n");
  }
  /*
    // print out circuit info
    fprintf(fp,"%i\n",NumCircPropsOrig);
    for(i=0;i<NumCircPropsOrig;i++){
      if (circproplist[i].Case==0)
        fprintf(fp,"0	%.17g	%.17g\n",circproplist[i].dV.Re(),
                        circproplist[i].dV.Im());
      if (circproplist[i].Case==1)
        fprintf(fp,"1	%.17g	%.17g\n",circproplist[i].J.Re(),
                      circproplist[i].J.Im());

      if (circproplist[i].Case==2)
        fprintf(fp,"0	%.17g	%.17g\n",L.b[NumNodes+i].Re(),
                      L.b[NumNodes+i].Im());
    }
  */
  // print out circuit info on a blocklabel by blocklabel basis;
  fprintf(fp, "%i\n", NumBlockLabels);
  for (k = 0; k < NumBlockLabels; k++) {
    i = labellist[k].InCircuit;
    if (i < 0) // if block not associated with any particular circuit
    {
      // print out some "dummy" properties that say that
      // there is a fixed additional current density,
      // but that that additional current density is zero.
      fprintf(fp, "1	0	0\n");
    } else {
      if (circproplist[i].Case == 0)
        fprintf(fp, "0	%.17g	%.17g\n", circproplist[i].dV.Re(),
            circproplist[i].dV.Im());
      if (circproplist[i].Case == 1)
        fprintf(fp, "1	%.17g	%.17g\n", circproplist[i].J.Re(),
            circproplist[i].J.Im());

      if (circproplist[i].Case == 2)
        fprintf(fp, "0	%.17g	%.17g\n", L.b[NumNodes + i].Re(),
            L.b[NumNodes + i].Im());
    }
  }

  // print out information on periodic boundary conditions
  fprintf(fp, "%i\n", NumPBCs);
  for (k = 0; k < NumPBCs; k++)
    fprintf(fp, "%i	%i	%i\n", pbclist[k].x, pbclist[k].y, pbclist[k].t);

  // print out air gap element info
  fprintf(fp, "%i\n", NumAGEs);
  for (i = 0; i < NumAGEs; i++) {
    fprintf(fp, "%s", agelist[i].BdryName);
    fprintf(fp, "%i %.17g %.17g %.17g %.17g %.17g %.17g %.17g %i %.17g %.17g\n",
        agelist[i].BdryFormat, agelist[i].InnerAngle, agelist[i].OuterAngle,
        agelist[i].ri, agelist[i].ro, agelist[i].totalArcLength,
        agelist[i].agc.re, agelist[i].agc.im, agelist[i].totalArcElements,
        agelist[i].InnerShift, agelist[i].OuterShift);
    for (k = 0; k <= agelist[i].totalArcElements; k++) {
      fprintf(fp, "%i %.17g %i %.17g %i %.17g %i %.17g\n",
          agelist[i].node[k].n0, agelist[i].node[k].w0,
          agelist[i].node[k].n1, agelist[i].node[k].w1,
          agelist[i].node[k].n2, agelist[i].node[k].w2,
          agelist[i].node[k].n3, agelist[i].node[k].w3);
    }
  }

  fclose(fp);
  return TRUE;
}
