#include <stdafx.h>
#include <stdio.h>
#include <math.h>
#include "fkn.h"
#include "fknDlg.h"
#include "complex.h"
#include "mesh.h"
#include "spars.h"
#include "FemmeDocCore.h"
#include "lua.h"

extern lua_State* lua;

double Power(double x, int y)
{
  return pow(x, (double)y);
}

BOOL CFemmeDocCore::Static2D(CBigLinProb& L)
{
  int i, j, k, w, s;
  double Me[3][3], be[3]; // element matrices;
  double Mx[3][3], My[3][3], Mxy[3][3], Mn[3][3];
  double l[3], p[3], q[3]; // element shape parameters;
  int n[3]; // numbers of nodes for a particular element;
  double a, K, Ki, r, t, x, y, B, B1, B2, mu, v[3], u[3], dv, res, lastres, Cduct;
  double *V_old, *CircInt1, *CircInt2, *CircInt3;
  double c = PI * 4.e-05;
  double units[] = { 2.54, 0.1, 1., 100., 0.00254, 1.e-04 };
  int Iter = 0, pctr;
  BOOL LinearFlag = TRUE;
  BOOL bIncremental = FALSE;
  BOOL bRestart = FALSE;
  double murel, muinc;

  if (PrevSoln.GetLength() > 0)
    bIncremental = PrevType;
  res = 0;

  CElement* El;
  V_old = (double*)calloc(NumNodes, sizeof(double));

  for (i = 0; i < NumBlockLabels; i++)
    GetFillFactor(i);

  // check to see if any circuits have been defined and process them;
  if (NumCircProps > 0) {
    CircInt1 = (double*)calloc(NumCircProps, sizeof(double));
    CircInt2 = (double*)calloc(NumCircProps, sizeof(double));
    CircInt3 = (double*)calloc(NumCircProps, sizeof(double));
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
          CircInt1[labellist[El->lbl].InCircuit] += a;
          CircInt2[labellist[El->lbl].InCircuit] += a * Cduct;
          CircInt3[labellist[El->lbl].InCircuit] += blockproplist[El->blk].Jr * a * 100.;
        }
    }

    for (i = 0; i < NumCircProps; i++) {
      // Case 0 :: voltage gradient is applied to the region;
      // Case 1 :: flat current density is applied to the region;
      if (circproplist[i].CircType == 0) {
        if (CircInt2[i] == 0) {
          circproplist[i].Case = 1;
          if (CircInt1[i] == 0.)
            circproplist[i].J = 0.;
          else
            circproplist[i].J = 0.01 * (circproplist[i].Amps_re - CircInt3[i]) / CircInt1[i];
        } else {
          circproplist[i].Case = 0;
          circproplist[i].dV = -0.01 * (circproplist[i].Amps_re - CircInt3[i]) / CircInt2[i];
        }
      } else {
        circproplist[i].Case = 0;
        circproplist[i].dV = circproplist[i].dVolts_re;
      }
    }
  }

  // first, need to define permeability in each block.  In nonlinear
  // case, this is sort of a hassle.  Linear permeability is simply
  // copied from the associated block definition, but nonlinear
  // permeability must be updated from iteration to iteration...

  // build element matrices using the matrices derived in Allaire's book.

  do {

    TheView->SetDlgItemText(IDC_FRAME1, "Matrix Construction");
    TheView->m_prog1.SetPos(0);
    pctr = 0;

    if (Iter > 0)
      L.Wipe();
    else {

      // could be starting from a previous solution; load up the initial guess from Aprev
      if (bIncremental == 1) {
        // try to restart from a previous file of the same name
        // to cut down on the time to solve the incremental problem
        CString myFile;
        myFile.Format("%s.ans", PathName);
        if (LoadPrevA(myFile, L.V))
          bRestart = 2;
      } else if ((bIncremental == 0) && (PrevSoln.GetLength() > 0)) {
        // load initial guess from the explicitly specified previous solution
        bRestart = LoadPrevA(PrevSoln, L.V);
      }
    }

    // first, tack in air gap element contributions
    for (i = 0; i < NumAGEs; i++) {
      double MG[10][10];
      double ci, co;
      int nn[10];
      double ww[10];
      double dt;

      // K = dr/(R*dtta)
      dt = (PI / 180.) * (agelist[i].totalArcLength / agelist[i].totalArcElements);
      K = 2. * (agelist[i].ro - agelist[i].ri) / (dt * (agelist[i].ro + agelist[i].ri));
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
        if (((k + 1) == agelist[i].totalArcElements) && (agelist[i].BdryFormat == 1)) {
          ww[4] = -ww[4];
          ww[9] = -ww[9];
        }

        // scale by weight to get periodic/antiperiodic right and tack into mesh
        for (int ii = 0; ii < 10; ii++)
          for (int jj = ii; jj < 10; jj++)
            L.AddTo(MG[ii][jj] * ww[ii] * ww[jj], nn[ii], nn[jj]);
      }
    }

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
          Me[j][k] = 0.;
          Mx[j][k] = 0.;
          My[j][k] = 0.;
          Mn[j][k] = 0.;
          Mxy[j][k] = 0.;
        }
        be[j] = 0.;
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
      r = (meshnode[n[0]].x + meshnode[n[1]].x + meshnode[n[2]].x) / 3.;

      // x-contribution; only need to do main diagonal and above;
      K = (-1. / (4. * a));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++) {
          Mx[j][k] += K * p[j] * p[k];
          if (j != k)
            Mx[k][j] += K * p[j] * p[k];
        }

      // y-contribution; only need to do main diagonal and above;
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

      // contributions to Me, be from derivative boundary conditions;
      for (j = 0; j < 3; j++) {
        if (El->e[j] >= 0)
          if (lineproplist[El->e[j]].BdryFormat == 2) {
            // conversion factor is 10^(-4) (I think...)
            K = -0.0001 * c * lineproplist[El->e[j]].c0.re * l[j] / 6.;
            k = j + 1;
            if (k == 3)
              k = 0;
            Me[j][j] += K * 2.;
            Me[k][k] += K * 2.;
            Me[j][k] += K;
            Me[k][j] += K;

            K = (lineproplist[El->e[j]].c1.re * l[j] / 2.) * 0.0001;
            be[j] += K;
            be[k] += K;
          }
      }

      // contribution to be from current density in the block
      for (j = 0, El->Jprev = 0; j < 3; j++) {
        if (labellist[El->lbl].InCircuit >= 0) {
          k = labellist[El->lbl].InCircuit;
          if (circproplist[k].Case == 1)
            t = circproplist[k].J.Re();
          if (circproplist[k].Case == 0)
            t = -circproplist[k].dV.Re() * blockproplist[El->blk].Cduct;
        } else
          t = 0;
        K = -(blockproplist[El->blk].Jr + t) * a / 3.;
        be[j] += K;

        // record avg current density in the block for use in incremental solutions
        if (bIncremental == FALSE)
          El->Jprev += (blockproplist[El->blk].Jr + t) / 3.;
      }

      // contribution to be from magnetization in the block;
      t = labellist[El->lbl].MagDir;
      if (labellist[El->lbl].MagDirFctn != NULL) // functional magnetization direction
      {
        CString str;
        CComplex X;
        int top1, top2, lua_error_code;
        for (j = 0, X = 0; j < 3; j++)
          X += (meshnode[n[j]].x + I * meshnode[n[j]].y);
        X = X / units[LengthUnits] / 3.;
        str.Format("x=%.17g\ny=%.17g\nr=x\nz=y\ntheta=%.17g\nR=%.17g\nreturn %s",
            X.re, X.im, arg(X) * 180 / PI, abs(X), labellist[El->lbl].MagDirFctn);
        top1 = lua_gettop(lua);
        if ((lua_error_code = lua_dostring(lua, str)) != 0) {
          /*
              if (lua_error_code==LUA_ERRRUN)
                AfxMessageBox("Run Error");
              if (lua_error_code==LUA_ERRMEM)
                AfxMessageBox("Lua memory Error");
              if (lua_error_code==LUA_ERRERR)
                AfxMessageBox("User error error");
              if (lua_error_code==LUA_ERRFILE)
                AfxMessageBox("File Error");
          */

          MsgBox("Lua error evaluating \"%s\"", labellist[El->lbl].MagDirFctn);

          exit(7);
        }
        top2 = lua_gettop(lua);
        if (top2 != top1) {
          str = lua_tostring(lua, -1);
          if (str.GetLength() == 0) {
            MsgBox("\"%s\" does not evaluate to a numerical value",
                labellist[El->lbl].MagDirFctn);
            exit(7);
          } else
            t = Re(lua_tonumber(lua, -1));
          lua_pop(lua, 1);
        }
      }
      for (j = 0; j < 3; j++) {
        k = j + 1;
        if (k == 3)
          k = 0;
        // need to scale so that everything is in proper units...
        // conversion is 0.0001
        K = 0.0001 * blockproplist[El->blk].H_c * (cos(t * PI / 180.) * (meshnode[n[k]].x - meshnode[n[j]].x) + sin(t * PI / 180.) * (meshnode[n[k]].y - meshnode[n[j]].y)) / 2.;
        be[j] += K;
        be[k] += K;
      }

      //////// Nonlinear Part

      // update permeability for the element;
      if (Iter == 0) {
        k = meshele[i].blk;

        if (blockproplist[k].LamType == 0) {
          t = blockproplist[k].LamFill;
          meshele[i].mu1 = blockproplist[k].mu_x * t + (1. - t);
          meshele[i].mu2 = blockproplist[k].mu_y * t + (1. - t);
        }
        if (blockproplist[k].LamType == 1) {
          t = blockproplist[k].LamFill;
          mu = blockproplist[k].mu_x;
          meshele[i].mu1 = mu * t + (1. - t);
          meshele[i].mu2 = mu / (t + mu * (1. - t));
        }
        if (blockproplist[k].LamType == 2) {
          t = blockproplist[k].LamFill;
          mu = blockproplist[k].mu_y;
          meshele[i].mu2 = mu * t + (1. - t);
          meshele[i].mu1 = mu / (t + mu * (1. - t));
        }
        if (blockproplist[k].LamType > 2) {
          meshele[i].mu1 = 1;
          meshele[i].mu2 = 1;
        }

        if (blockproplist[k].BHpoints != 0) {
          if (bIncremental == FALSE) {
            // There's no previous solution.  This is a standard nonlinear problem
            LinearFlag = FALSE;
          } else {
            double B1p, B2p;

            // too lazy to consistently code incremental/frozen formulation for on-edge lams.
            // detect this condition, throw an error, and exit.
            if (blockproplist[k].LamType > 0) {
              MsgBox("On-edge Lam Types not yet supported in\nincremental/frozen permeability problems");
              exit(0);
            }

            //	Get B from previous solution
            GetPrev2DB(i, B1p, B2p);
            B = sqrt(B1p * B1p + B2p * B2p);

            // look up incremental permeability and assign it to the element;
            blockproplist[k].IncrementalPermeability(B, muinc, murel);

            if (B == 0) {
              meshele[i].mu1 = muinc;
              meshele[i].mu2 = muinc;
              meshele[i].v12 = 0;
            } else {
              if (bIncremental == 1) {
                // Need to actually compute B1 and B2 to build incremental permeability tensor
                meshele[i].mu1 = B * B * muinc * murel / (B1p * B1p * murel + B2p * B2p * muinc);
                meshele[i].mu2 = B * B * muinc * murel / (B1p * B1p * muinc + B2p * B2p * murel);
                meshele[i].v12 = -B1p * B2p * (murel - muinc) / (B * B * murel * muinc);
              } else {
                // Define "frozen permeability"
                meshele[i].mu1 = murel;
                meshele[i].mu2 = murel;
                meshele[i].v12 = 0;
              }
            }
          }
        }
      }

      if ((Iter > 0) || (bRestart == TRUE)) {
        k = meshele[i].blk;

        if ((blockproplist[k].LamType == 0) && (meshele[i].mu1 == meshele[i].mu2)
            && (blockproplist[k].BHpoints > 0)) {
          for (j = 0, B1 = 0., B2 = 0.; j < 3; j++) {
            B1 += L.V[n[j]] * q[j];
            B2 += L.V[n[j]] * p[j];
          }
          B = c * sqrt(B1 * B1 + B2 * B2) / (0.02 * a);
          // correction for lengths in cm of 1/0.02

          // find out new mu from saturation curve;
          blockproplist[k].GetBHProps(B, mu, dv);
          mu = 1. / (muo * mu);
          meshele[i].mu1 = mu;
          meshele[i].mu2 = mu;
          for (j = 0; j < 3; j++) {
            for (w = 0, v[j] = 0; w < 3; w++)
              v[j] += (Mx[j][w] + My[j][w]) * L.V[n[w]];
          }
          K = -200. * c * c * c * dv / a;
          for (j = 0; j < 3; j++)
            for (w = 0; w < 3; w++)
              Mn[j][w] = K * v[j] * v[w];
        }

        if ((blockproplist[k].LamType == 1) && (blockproplist[k].BHpoints > 0)) {
          t = blockproplist[k].LamFill;
          for (j = 0, B1 = 0., B2 = 0.; j < 3; j++) {
            B1 += L.V[n[j]] * q[j];
            B2 += L.V[n[j]] * p[j] / t;
          }
          B = c * sqrt(B1 * B1 + B2 * B2) / (0.02 * a);
          blockproplist[k].GetBHProps(B, mu, dv);
          mu = 1. / (muo * mu);
          meshele[i].mu1 = mu * t;
          meshele[i].mu2 = mu / (t + mu * (1. - t));
          for (j = 0; j < 3; j++) {
            for (w = 0, v[j] = 0, u[j] = 0; w < 3; w++) {
              v[j] += (My[j][w] / t + Mx[j][w]) * L.V[n[w]];
              u[j] += (My[j][w] / t + t * Mx[j][w]) * L.V[n[w]];
            }
          }
          K = -100. * c * c * c * dv / (a);
          for (j = 0; j < 3; j++)
            for (w = 0; w < 3; w++)
              Mn[j][w] = K * (v[j] * u[w] + v[w] * u[j]);
        }
        if ((blockproplist[k].LamType == 2) && (blockproplist[k].BHpoints > 0)) {
          t = blockproplist[k].LamFill;
          for (j = 0, B1 = 0., B2 = 0.; j < 3; j++) {
            B1 += (L.V[n[j]] * q[j]) / t;
            B2 += L.V[n[j]] * p[j];
          }
          B = c * sqrt(B1 * B1 + B2 * B2) / (0.02 * a);
          blockproplist[k].GetBHProps(B, mu, dv);
          mu = 1. / (muo * mu);
          meshele[i].mu2 = mu * t;
          meshele[i].mu1 = mu / (t + mu * (1. - t));
          for (j = 0; j < 3; j++) {
            for (w = 0, v[j] = 0, u[j] = 0; w < 3; w++) {
              v[j] += (Mx[j][w] / t + My[j][w]) * L.V[n[w]];
              u[j] += (Mx[j][w] / t + t * My[j][w]) * L.V[n[w]];
            }
          }
          K = -100. * c * c * c * dv / (a);
          for (j = 0; j < 3; j++)
            for (w = 0; w < 3; w++)
              Mn[j][w] = K * (v[j] * u[w] + v[w] * u[j]);
        }
      }

      // combine block matrices into global matrices;
      for (j = 0; j < 3; j++)
        for (k = 0; k < 3; k++) {
          Me[j][k] += (Mx[j][k] / Re(El->mu2) + My[j][k] / Re(El->mu1) + Mxy[j][k] * Re(El->v12) + Mn[j][k]);
          be[j] += Mn[j][k] * L.V[n[k]];
        }

      for (j = 0; j < 3; j++) {
        for (k = j; k < 3; k++)
          L.AddTo(-Me[j][k], n[j], n[k]);
        L.b[n[j]] -= be[j];
      }
    }

    // add in contribution from point currents;
    for (i = 0; i < NumNodes; i++)
      if (meshnode[i].bc >= 0)
        L.b[i] += (0.01 * nodeproplist[meshnode[i].bc].Jr);

    // apply fixed boundary conditions at points;
    for (i = 0; i < NumNodes; i++)
      if (meshnode[i].bc >= 0)
        if ((nodeproplist[meshnode[i].bc].Jr == 0) && (nodeproplist[meshnode[i].bc].Ji == 0))
          L.SetValue(i, nodeproplist[meshnode[i].bc].Ar / c);

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
              // just take ``real'' component.
              a *= cos(lineproplist[s].phi * DEG);
              L.SetValue(meshele[i].p[j], a / c);

              // second point on the side;
              x = meshnode[meshele[i].p[k]].x;
              y = meshnode[meshele[i].p[k]].y;
              x /= units[LengthUnits];
              y /= units[LengthUnits];
              s = meshele[i].e[j];
              a = lineproplist[s].A0 + x * lineproplist[s].A1 + y * lineproplist[s].A2;
              // just take``real'' component.
              a *= cos(lineproplist[s].phi * DEG);
              L.SetValue(meshele[i].p[k], a / c);
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
              a *= cos(lineproplist[s].phi * DEG); // just take ``real'' component.
              L.SetValue(meshele[i].p[j], a / c);

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
              a *= cos(lineproplist[s].phi * DEG); // just take ``real'' component.
              L.SetValue(meshele[i].p[k], a / c);
            }
          }
      }

    // Apply any periodicity/antiperiodicity boundary conditions that we have
    for (k = 0; k < NumPBCs; k++) {
      if (pbclist[k].t == 0)
        L.Periodicity(pbclist[k].x, pbclist[k].y);
      if (pbclist[k].t == 1)
        L.AntiPeriodicity(pbclist[k].x, pbclist[k].y);
    }

    // Dump problem info to Matlab, if that was commanded
    if ((Iter == 0) && (bDump == TRUE))
      MatlabDumpStatic2D(L);

    // solve the problem;
    for (j = 0; j < NumNodes; j++)
      V_old[j] = L.V[j];
    if (L.PCGSolve(Iter + bRestart) == FALSE)
      return FALSE;

    if (LinearFlag == FALSE) {

      for (j = 0, x = 0, y = 0; j < NumNodes; j++) {
        x += (L.V[j] - V_old[j]) * (L.V[j] - V_old[j]);
        y += (L.V[j] * L.V[j]);
      }

      if (y == 0)
        LinearFlag = TRUE;
      else {
        lastres = res;
        res = sqrt(x / y);
      }

      // relaxation if we need it
      if (Iter > 5) {
        if ((res > lastres) && (Relax > 0.125))
          Relax /= 2.;
        else
          Relax += 0.1 * (1. - Relax);

        for (j = 0; j < NumNodes; j++)
          L.V[j] = Relax * L.V[j] + (1.0 - Relax) * V_old[j];
      }

      // report some results
      char outstr[256];
      sprintf(outstr, "Newton Iteration(%i) Relax=%.4g", Iter, Relax);
      TheView->SetDlgItemText(IDC_FRAME2, outstr);
      j = (int)(100. * log10(res) / (log10(Precision) + 2.));
      if (j > 100)
        j = 100;
      TheView->m_prog2.SetPos(j);
    }

    // nonlinear iteration has to have a looser tolerance
    // than the linear solver--otherwise, things can't ever
    // converge.  Arbitrarily choose 100*tolerance.
    if ((res < 100. * Precision) && (Iter > 0))
      LinearFlag = TRUE;

    Iter++;

  } while (LinearFlag == FALSE);

  for (i = 0; i < NumNodes; i++)
    L.b[i] = L.V[i] * c; // convert answer to Amps
  free(V_old);

  if (NumCircProps > 0) {
    free(CircInt1);
    free(CircInt2);
    free(CircInt3);
  }
  return TRUE;
}

//=========================================================================
//=========================================================================

BOOL CFemmeDocCore::WriteStatic2D(CBigLinProb& L)
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
    fprintf(fp, "%.17g	%.17g	%.17g	%i	", meshnode[i].x / cf,
        meshnode[i].y / cf, L.b[i], meshnode[i].bc);
    // include A from previous solution if this is an incremental permeability problem
    if (Aprev != NULL)
      fprintf(fp, "%.17g\n", Aprev[i]);
    else
      fprintf(fp, "\n");
  }

  fprintf(fp, "%i\n", NumEls);
  for (i = 0; i < NumEls; i++)
    fprintf(fp, "%i	%i	%i	%i	%i	%i	%i	%.17g\n",
        meshele[i].p[0], meshele[i].p[1], meshele[i].p[2], meshele[i].lbl,
        meshele[i].e[0], meshele[i].e[1], meshele[i].e[2], meshele[i].Jprev);
  /*
    // print out circuit info
    fprintf(fp,"%i\n",NumCircPropsOrig);
    for(i=0;i<NumCircPropsOrig;i++){
      if (circproplist[i].Case==0)
        fprintf(fp,"0	%.17g\n",circproplist[i].dV.Re());
      if (circproplist[i].Case==1)
        fprintf(fp,"1	%.17g\n",circproplist[i].J.Re());
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
      fprintf(fp, "1	0\n");
    } else {
      if (circproplist[i].Case == 0)
        fprintf(fp, "0	%.17g\n", circproplist[i].dV.Re());
      if (circproplist[i].Case == 1)
        fprintf(fp, "1	%.17g\n", circproplist[i].J.Re());
    }
  }

  // print out information on periodic boundary conditions for
  // possible re-use in AC incremental permeability solutions
  fprintf(fp, "%i\n", NumPBCs);
  for (k = 0; k < NumPBCs; k++)
    fprintf(fp, "%i	%i	%i\n", pbclist[k].x, pbclist[k].y, pbclist[k].t);

  // print out information on periodic boundary conditions for
  // possible re-use in AC incremental permeability solutions
  // and in post-processing of forces and torques
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

BOOL CFemmeDocCore::MatlabDumpStatic2D(CBigLinProb& L)
{
  FILE* fp;
  int k;

  CString path = PathName;
  if ((fp = fopen(path + ".m", "wt")) == NULL) {
    // couldn't open file
    return 0;
  }

  // rhs of problem
  fprintf(fp, "b = [");
  for (k = 0; k < L.n; k++) {
    fprintf(fp, "%.15g", L.b[k]);
    if ((k + 1) == L.n)
      fprintf(fp, "]");
    fprintf(fp, ";\n");
  }
  CString MatrixFileName = path.Mid(path.ReverseFind('\\') + 1);
  fprintf(fp, "load %s.dat;\n", MatrixFileName);
  fprintf(fp, "M = spconvert(%s);\n", MatrixFileName);

  // "stiffness" matrix
  L.SaveMe(path + ".dat");

  return TRUE;
}