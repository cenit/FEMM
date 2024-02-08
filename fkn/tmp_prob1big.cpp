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

BOOL CFemmeDocCore::Static2D(CBigLinProb& L)
{
  int i, j, k, w, s, sdi_iter, sdin;
  double Me[3][3], be[3]; // element matrices;
  double Mx[3][3], My[3][3], Mn[3][3];
  double l[3], p[3], q[3]; // element shape parameters;
  int n[3]; // numbers of nodes for a particular element;
  double a, K, r, t, x, y, B, B1, B2, mu, v[3], u[3], dv, res, lastres, Cduct;
  double *V_old, *V_sdi, *CircInt1, *CircInt2, *CircInt3;
  double c = PI * 4.e-05;
  double units[] = { 2.54, 0.1, 1., 100., 0.00254, 1.e-04 };
  int Iter = 0, pctr;
  BOOL LinearFlag = TRUE;
  BOOL bIncremental = FALSE;
  double murel, muinc;
  BOOL SDIflag = FALSE;

  if (PrevSoln.GetLength() > 0)
    bIncremental = TRUE;
  res = 0;

  CElement* El;
  V_old = (double*)calloc(NumNodes, sizeof(double));

  for (i = 0; i < NumBlockLabels; i++)
    GetFillFactor(i);

  // check to see if there are any SDI boundaries...
  // lineproplist[ meshele[i].e[j] ].BdryFormat==0
  for (i = 0; i < NumLineProps; i++)
    if (lineproplist[i].BdryFormat == 3)
      SDIflag = TRUE;

  if (SDIflag == TRUE) {
    // there is an SDI boundary defined; check to see if it is in use
    SDIflag = FALSE;
    for (i = 0; i < NumEls; i++)
      for (j = 0; j < 3; j++)
        if (lineproplist[meshele[i].e[j]].BdryFormat == 3) {
          SDIflag = TRUE;
          printf("Problem has SDI boundaries\n");
          i = NumEls;
          j = 3;
        }
  }

  if (SDIflag == TRUE) {
    V_sdi = (double*)calloc(NumNodes, sizeof(double));
    sdin = 2;
  } else
    sdin = 1;

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
    for (sdi_iter = 0; sdi_iter < sdin; sdi_iter++) {

      TheView->SetDlgItemText(IDC_FRAME1, "Matrix Construction");
      TheView->m_prog1.SetPos(0);
      pctr = 0;

      if (Iter > 0)
        L.Wipe();

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

          if (blockproplist[k].BHpoints != 0) {
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
                if (bIncremental == 1) {
                  // Need to actually compute B1 and B2 to build incremental permeability tensor
                  meshele[i].mu1 = B * B * muinc * murel / (B1p * B1p * murel + B2p * B2p * muinc);
                  meshele[i].mu2 = B * B * muinc * murel / (B1p * B1p * muinc + B2p * B2p * murel);
                  meshele[i].v12 = B1p * B2p * (murel - muinc) / (B * B * murel * muinc);
                } else {
                  // Define "frozen permeability"
                  meshele[i].mu1 = murel;
                  meshele[i].mu2 = murel;
                  meshele[i].v12 = 0;
                }
              }
            }
          }

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
        } else {
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
            Me[j][k] += (Mx[j][k] / Re(El->mu2) + My[j][k] / Re(El->mu1) + Mn[j][k]);
            be[j] += Mn[j][k] * L.V[n[k]];
          }

        for (j = 0; j < 3; j++) {
          for (k = j; k < 3; k++)
            L.Put(L.Get(n[j], n[k]) - Me[j][k], n[j], n[k]);
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
          if ((nodeproplist[meshnode[i].bc].Jr == 0) && (nodeproplist[meshnode[i].bc].Ji == 0) && (sdi_iter == 0))
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

      // Apply SDI boundary condition;
      if ((SDIflag == TRUE) && (sdi_iter == 1))
        for (i = 0; i < NumEls; i++)
          for (j = 0; j < 3; j++) {
            k = j + 1;
            if (k == 3)
              k = 0;
            if (meshele[i].e[j] >= 0)
              if (lineproplist[meshele[i].e[j]].BdryFormat == 3) {
                L.SetValue(meshele[i].p[j], 0.);
                L.SetValue(meshele[i].p[k], 0.);
              }
          }

      // Apply any periodicity/antiperiodicity boundary conditions that we have
      for (k = 0; k < NumPBCs; k++) {
        if (pbclist[k].t == 0)
          L.Periodicity(pbclist[k].x, pbclist[k].y);
        if (pbclist[k].t == 1)
          L.AntiPeriodicity(pbclist[k].x, pbclist[k].y);
      }

      // solve the problem;
      if (SDIflag == FALSE)
        for (j = 0; j < NumNodes; j++)
          V_old[j] = L.V[j];
      else {
        if (sdi_iter == 0)
          for (j = 0; j < NumNodes; j++)
            V_sdi[j] = L.V[j];
        else
          for (j = 0; j < NumNodes; j++) {
            V_old[j] = V_sdi[j];
            V_sdi[j] = L.V[j];
          }
      }

      if (L.PCGSolve(Iter + sdi_iter) == FALSE)
        return FALSE;

      if (sdi_iter == 1)
        for (j = 0; j < NumNodes; j++)
          L.V[j] = (V_sdi[j] + L.V[j]) / 2.;

    } // end of SDI iteration loop;

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
  if (SDIflag == TRUE)
    free(V_sdi);
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
    MsgBox("Couldn't open %s.fem\n", PathName);
    return FALSE;
  }

  sprintf(c, "%s.ans", PathName);
  if ((fp = fopen(c, "wt")) == NULL) {
    MsgBox("Couldn't write to %s.ans\n", PathName);
    return FALSE;
  }

  while (fgets(c, 1024, fz) != NULL)
    fputs(c, fp);
  fclose(fz);

  // then print out node, line, and element information
  fprintf(fp, "[Solution]\n");
  cf = unitconv[LengthUnits];
  fprintf(fp, "%i\n", NumNodes);
  for (i = 0; i < NumNodes; i++)
    fprintf(fp, "%.17g	%.17g	%.17g	%i\n", meshnode[i].x / cf,
        meshnode[i].y / cf, L.b[i], meshnode[i].bc);
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

  fclose(fp);
  return TRUE;
}

//=========================================================================
//=========================================================================
