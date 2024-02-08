#include <stdafx.h>
#include <stdio.h>
#include <math.h>
#include "fkn.h"
#include "fknDlg.h"
#include "complex.h"
#include "mesh.h"
#include "spars.h"
#include "FemmeDocCore.h"
#define Log log

// #define NEWTON

// since all node positions were converted to units of cm
// the proper LengthConv is converts centimeters to meters
#define LengthConv 0.01

void CFemmeDocCore::GetPrevAxiB(int k, double& B1p, double& B2p)
{
  // for axisymmetric incremental problem,
  // get flux density from the previous solution.
  // Code cribbed from CFemmviewDoc::GetElementB(CElement &elm) in femm source
  int i, n[3];
  double b[3], c[3], da;
  double v[6], dp, dq;
  double R[3], Z[3], r;

  for (i = 0; i < 3; i++)
    n[i] = meshele[k].p[i];

  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;

  for (i = 0, r = 0; i < 3; i++) {
    R[i] = meshnode[n[i]].x;
    Z[i] = meshnode[n[i]].y;
    r += R[i] / 3.;
  }

  // corner nodes
  v[0] = Aprev[n[0]];
  v[2] = Aprev[n[1]];
  v[4] = Aprev[n[2]];

  // construct values for mid-side nodes;
  if ((R[0] < 1.e-06) && (R[1] < 1.e-06))
    v[1] = (v[0] + v[2]) / 2.;
  else
    v[1] = (R[1] * (3. * v[0] + v[2]) + R[0] * (v[0] + 3. * v[2])) / (4. * (R[0] + R[1]));

  if ((R[1] < 1.e-06) && (R[2] < 1.e-06))
    v[3] = (v[2] + v[4]) / 2.;
  else
    v[3] = (R[2] * (3. * v[2] + v[4]) + R[1] * (v[2] + 3. * v[4])) / (4. * (R[1] + R[2]));

  if ((R[2] < 1.e-06) && (R[0] < 1.e-06))
    v[5] = (v[4] + v[0]) / 2.;
  else
    v[5] = (R[0] * (3. * v[4] + v[0]) + R[2] * (v[4] + 3. * v[0])) / (4. * (R[2] + R[0]));

  // derivatives w.r.t. p and q:
  dp = (-v[0] + v[2] + 4. * v[3] - 4. * v[5]) / 3.;
  dq = (-v[0] - 4. * v[1] + 4. * v[3] + v[4]) / 3.;

  // now, compute flux.
  da = (b[0] * c[1] - b[1] * c[0]);
  da *= 2. * PI * r * LengthConv * LengthConv;
  B1p = Re(-(c[1] * dp + c[2] * dq) / da);
  B2p = Re((b[1] * dp + b[2] * dq) / da);
}

BOOL CFemmeDocCore::HarmonicAxisymmetric(CBigComplexLinProb& L)
{
  int i, j, k, s, flag, ww, Iter = 0;
  int pctr;
  CComplex Mx[3][3], My[3][3], Mxy[3][3], Mn[3][3], Me[3][3], be[3]; // element matrices;
  double l[3], p[3], q[3]; // element shape parameters;
  int n[3]; // numbers of nodes for a particular element;
  double a, r, t, x, y, B, w, res, lastres, ds, R, rn[3], g[3], a_hat, R_hat, vol, Cduct;
  CComplex K, mu, dv, B1, B2, v[3], u[3], mu1, mu2, lag, halflag, deg45, Jv;
  CComplex **Mu, *V_old;
  double c = PI * 4.e-05;
  double units[] = { 2.54, 0.1, 1., 100., 0.00254, 1.e-04 };
  CElement* El;
  BOOL LinearFlag = TRUE;
  BOOL bIncremental = FALSE;

  if (PrevSoln.GetLength() > 0)
    bIncremental = TRUE;
  res = 0;

  // #ifndef NEWTON
  CComplex murel, muinc;
  // #else
  CComplex Mnh[3][3];
  CComplex Mna[3][3];
  CComplex Mns[3][3];
  // #endif

  extRo *= units[LengthUnits];
  extRi *= units[LengthUnits];
  extZo *= units[LengthUnits];

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
          r = (meshnode[n[0]].x + meshnode[n[1]].x + meshnode[n[2]].x) / 3.;

          // if coils are wound, they act like they have
          // a zero "bulk" conductivity...
          Cduct = blockproplist[El->blk].Cduct;
          if (labellist[El->lbl].bIsWound)
            Cduct = 0;

          // evaluate integrals;

          // total cross-section of circuit;
          CircInt1[labellist[El->lbl].InCircuit] += a;

          // integral of conductivity / R over the circuit;
          CircInt2[labellist[El->lbl].InCircuit] += a * Cduct / (0.01 * r);

          // integral of applied J over current;
          CircInt3[labellist[El->lbl].InCircuit] += (blockproplist[El->blk].Jr + I * blockproplist[El->blk].Ji) * a * 100.;
        }
    }

    for (i = 0; i < NumCircProps; i++) {
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

      Mu[k][0] = blockproplist[k].mu_x * exp(-I * blockproplist[k].Theta_hx * PI / 180.);
      Mu[k][1] = blockproplist[k].mu_y * exp(-I * blockproplist[k].Theta_hy * PI / 180.);

      if (blockproplist[k].Lam_d != 0) {
        if (blockproplist[k].Cduct != 0) {
          halflag = exp(-I * blockproplist[k].Theta_hx * PI / 360.);
          ds = sqrt(2. / (0.4 * PI * w * blockproplist[k].Cduct * blockproplist[k].mu_x));
          K = halflag * deg45 * blockproplist[k].Lam_d * 0.001 / (2. * ds);
          Mu[k][0] = ((Mu[k][0] * tanh(K)) / K) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);

          halflag = exp(-I * blockproplist[k].Theta_hy * PI / 360.);
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
          Mn[j][k] = 0;
          // #ifdef NEWTON
          if (ACSolver == 1) {
            Mnh[j][k] = 0;
            Mna[j][k] = 0;
            Mns[j][k] = 0;
          }
          // #endif
        }
        be[j] = 0;
      }

      // Determine shape parameters.
      // l == element side lengths;
      // p corresponds to the `b' parameter in Allaire
      // q corresponds to the `c' parameter in Allaire
      El = &meshele[i];

      for (k = 0; k < 3; k++) {
        n[k] = El->p[k];
        rn[k] = meshnode[n[k]].x;
      }

      p[0] = meshnode[n[1]].y - meshnode[n[2]].y;
      p[1] = meshnode[n[2]].y - meshnode[n[0]].y;
      p[2] = meshnode[n[0]].y - meshnode[n[1]].y;
      q[0] = meshnode[n[2]].x - meshnode[n[1]].x;
      q[1] = meshnode[n[0]].x - meshnode[n[2]].x;
      q[2] = meshnode[n[1]].x - meshnode[n[0]].x;
      g[0] = (meshnode[n[2]].x + meshnode[n[1]].x) / 2.;
      g[1] = (meshnode[n[0]].x + meshnode[n[2]].x) / 2.;
      g[2] = (meshnode[n[1]].x + meshnode[n[0]].x) / 2.;

      for (j = 0, k = 1; j < 3; k++, j++) {
        if (k == 3)
          k = 0;
        l[j] = sqrt(pow(meshnode[n[k]].x - meshnode[n[j]].x, 2.) + pow(meshnode[n[k]].y - meshnode[n[j]].y, 2.));
      }
      a = (p[0] * q[1] - p[1] * q[0]) / 2.;
      R = (meshnode[n[0]].x + meshnode[n[1]].x + meshnode[n[2]].x) / 3.;

      for (j = 0, a_hat = 0; j < 3; j++)
        a_hat += (rn[j] * rn[j] * p[j] / (4. * R));
      a_hat = fabs(a_hat);
      vol = 2. * R * a_hat;

      for (j = 0, flag = 0; j < 3; j++)
        if (rn[j] < 1.e-06)
          flag++;
      switch (flag) {
      case 2:
        R_hat = R;

        break;

      case 1:

        if (rn[0] < 1.e-06) {
          if (fabs(rn[1] - rn[2]) < 1.e-06)
            R_hat = rn[2] / 2.;
          else
            R_hat = (rn[1] - rn[2]) / (2. * log(rn[1]) - 2. * log(rn[2]));
        }
        if (rn[1] < 1.e-06) {
          if (fabs(rn[2] - rn[0]) < 1.e-06)
            R_hat = rn[0] / 2.;
          else
            R_hat = (rn[2] - rn[0]) / (2. * log(rn[2]) - 2. * log(rn[0]));
        }
        if (rn[2] < 1.e-06) {
          if (fabs(rn[0] - rn[1]) < 1.e-06)
            R_hat = rn[1] / 2.;
          else
            R_hat = (rn[0] - rn[1]) / (2. * log(rn[0]) - 2. * log(rn[1]));
        }

        break;

      default:

        if (fabs(q[0]) < 1.e-06)
          R_hat = (q[1] * q[1]) / (2. * (-q[1] + rn[0] * log(rn[0] / rn[2])));
        else if (fabs(q[1]) < 1.e-06)
          R_hat = (q[2] * q[2]) / (2. * (-q[2] + rn[1] * log(rn[1] / rn[0])));
        else if (fabs(q[2]) < 1.e-06)
          R_hat = (q[0] * q[0]) / (2. * (-q[0] + rn[2] * log(rn[2] / rn[1])));
        else
          R_hat = -(q[0] * q[1] * q[2]) / (2. * (q[0] * rn[0] * log(rn[0]) + q[1] * rn[1] * log(rn[1]) + q[2] * rn[2] * log(rn[2])));

        break;
      }

      // Mr Contribution
      // Derived from flux formulation with c0 + c1 r^2 + c2 z
      // interpolation in the element.
      K = (-1. / (2. * a_hat * R));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++)
          Mx[j][k] += K * p[j] * rn[j] * p[k] * rn[k];

      // need this loop to avoid singularities.  This just puts something
      // on the main diagonal of nodes that are on the r=0 line.
      // The program later sets these nodes to zero, but it's good to
      // for scaling reasons to grab entries from the neighboring diagonals
      // rather than just setting these entries to 1 or something....
      for (j = 0; j < 3; j++)
        if (rn[j] < 1.e-06)
          Mx[j][j] += Mx[0][0] + Mx[1][1] + Mx[2][2];

      // Mz Contribution;
      // Derived from flux formulation with c0 + c1 r^2 + c2 z
      // interpolation in the element.
      K = (-1. / (2. * a_hat * R_hat));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++)
          My[j][k] += K * (q[j] * rn[j]) * (q[k] * rn[k]) * (g[j] / R) * (g[k] / R);

      // Mrz Contribution;
      // Derived from flux formulation with c0 + c1 r^2 + c2 z
      // interpolation in the element.
      K = (-1. / (2. * a_hat * R_hat));
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++)
          //		My[j][k]  += K * ((q[j]*rn[j])*(g[j]/R)) * ((q[k]*rn[k])*(g[k]/R));
          //		Mx[j][j]  += K * (p[j]*rn[j]) * (p[k]*rn[k]);
          Mxy[j][k] += K * ((q[j] * rn[j]) * (g[j] / R)) * (p[k] * rn[k]) + K * ((q[k] * rn[k]) * (g[k] / R)) * (p[j] * rn[j]);

      // Fill out rest of entries of Mx, My and Mxy;
      Mx[1][0] = Mx[0][1];
      Mx[2][0] = Mx[0][2];
      Mx[2][1] = Mx[1][2];
      My[1][0] = My[0][1];
      My[2][0] = My[0][2];
      My[2][1] = My[1][2];
      Mxy[1][0] = Mxy[0][1];
      Mxy[2][0] = Mxy[0][2];
      Mxy[2][1] = Mxy[1][2];

      // contribution from eddy currents;
      // induced current interpolated as constant (avg. of nodal values)
      // over the entire element;
      K = -I * R * a * w * blockproplist[meshele[i].blk].Cduct * c / 6.;

      // radially laminated blocks appear to have no conductivity;
      // eddy currents are accounted for in these elements by their
      // frequency-dependent permeability.
      if ((blockproplist[El->blk].LamType == 0) && (blockproplist[El->blk].Lam_d > 0))
        K = 0;

      // if this element is part of a wound coil,
      // it should have a zero "bulk" conductivity...
      if (labellist[El->lbl].bIsWound)
        K = 0;

      for (j = 0; j < 3; j++)
        for (k = 0; k < 3; k++)
          Me[j][k] += K * 4. / 3.;

      // contributions to Me, be from derivative boundary conditions;
      for (j = 0; j < 3; j++) {
        k = j + 1;
        if (k == 3)
          k = 0;
        r = (meshnode[n[j]].x + meshnode[n[k]].x) / 2.;
        if (El->e[j] >= 0) {

          if (lineproplist[El->e[j]].BdryFormat == 2) {
            // conversion factor is 10^(-4) (I think...)

            K = -0.0001 * c * 2. * r * lineproplist[El->e[j]].c0 * l[j] / 6.;
            Me[j][j] += 2 * K;
            Me[k][k] += 2 * K;
            Me[j][k] += K;
            Me[k][j] += K;

            K = (lineproplist[El->e[j]].c1 * l[j] / 2.) * 2. * r * 0.0001;
            be[j] += K;
            be[k] += K;
          }

          if (lineproplist[El->e[j]].BdryFormat == 1) {
            ds = sqrt(2. / (0.4 * PI * w * lineproplist[El->e[j]].Sig * lineproplist[El->e[j]].Mu));
            K = deg45 / (-ds * lineproplist[El->e[j]].Mu * 100.);
            K *= (2. * r * l[j] / 6.);
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
            Jv = -100. * circproplist[k].dV * blockproplist[El->blk].Cduct / R;
        }

        K = -2. * R * (blockproplist[El->blk].Jr + I * blockproplist[El->blk].Ji + Jv) * a / 3.;
        be[j] += K;

        if (labellist[El->lbl].InCircuit >= 0) {
          k = labellist[El->lbl].InCircuit;
          if (circproplist[k].Case == 2)
            L.b[NumNodes + k] += K / R;
        }
      }

      // do Case 2 circuit stuff for element
      if (labellist[El->lbl].InCircuit >= 0) {
        k = labellist[El->lbl].InCircuit;
        if (circproplist[k].Case == 2) {
          K = -2. * I * a * w * blockproplist[meshele[i].blk].Cduct * c;
          for (j = 0; j < 3; j++)
            L.Put(L.Get(n[j], NumNodes + k) + K / 3., n[j], NumNodes + k);
          L.Put(L.Get(NumNodes + k, NumNodes + k) + K / R, NumNodes + k, NumNodes + k);
        }
      }

      /////////////////////////
      //
      //  Nonlinear Stuff
      //
      /////////////////////////

      // update permeability for the element;
      if (Iter == 0) {
        k = meshele[i].blk;
        meshele[i].mu1 = Mu[k][0];
        meshele[i].mu2 = Mu[k][1];
        meshele[i].v12 = 0;
        if (blockproplist[k].BHpoints > 0) {
          if (bIncremental == FALSE)
            LinearFlag = FALSE;
          else {
            double B1p, B2p;

            //	Get B from previous solution
            GetPrevAxiB(i, B1p, B2p);
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
          //	Derive B directly from energy;
          v[0] = 0;
          v[1] = 0;
          v[2] = 0;
          for (j = 0; j < 3; j++)
            for (ww = 0; ww < 3; ww++)
              v[j] += (Mx[j][ww] + My[j][ww]) * L.V[n[ww]];
          for (j = 0, dv = 0; j < 3; j++)
            dv += conj(L.V[n[j]]) * v[j];
          dv *= (10000. * c * c / vol);
          B = sqrt(abs(dv));

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

            // Newton iteration
            K = -200. * c * c * c * dv / vol;
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
            //      K=muinc;                            // total incremental
            //      K=murel;                            // total updated
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

      // "Warp" the permeability of this element if part of
      // the conformally mapped external region
      if ((labellist[meshele[i].lbl].IsExternal) && (Iter == 0)) {
        double Z = (meshnode[n[0]].y + meshnode[n[1]].y + meshnode[n[2]].y) / 3. - extZo;
        double kludge = (R * R + Z * Z) * extRi / (extRo * extRo * extRo);
        meshele[i].mu1 /= kludge;
        meshele[i].mu2 /= kludge;
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
          L.Put(L.Get(n[j], n[k]) + Me[j][k], n[j], n[k]);
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

      ///////////////////////////////////////////////////
    }

    // add in contribution from point currents;
    for (i = 0; i < NumNodes; i++)
      if (meshnode[i].bc >= 0) {
        r = meshnode[i].x;
        K = (2. * r * 0.01) * (nodeproplist[meshnode[i].bc].Jr + I * nodeproplist[meshnode[i].bc].Ji);
        L.b[i] -= K;
      }

    // add in total current constraints for circuits;
    for (i = 0; i < NumCircProps; i++)
      if (circproplist[i].Case == 2) {
        L.b[NumNodes + i] += 2. * 0.01 * (circproplist[i].Amps_re + I * circproplist[i].Amps_im);
      }

    // apply fixed boundary conditions at points;
    for (i = 0; i < NumNodes; i++)
      if (meshnode[i].x < (units[LengthUnits] * 1.e-06)) {
        K = 0;
        L.SetValue(i, K);
      } else if (meshnode[i].bc >= 0)
        if ((nodeproplist[meshnode[i].bc].Jr == 0) && (nodeproplist[meshnode[i].bc].Ji == 0)) {
          K = (nodeproplist[meshnode[i].bc].Ar
                  + I * nodeproplist[meshnode[i].bc].Ai)
              / c;
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

  // convert answer back to webers
  for (i = 0; i < NumNodes; i++)
    L.b[i] = L.V[i] * c * 2. * PI * meshnode[i].x * 0.01;
  for (i = 0; i < NumCircProps; i++)
    L.b[NumNodes + i] = (I * w * c * 0.01 * L.V[NumNodes + i]);

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
