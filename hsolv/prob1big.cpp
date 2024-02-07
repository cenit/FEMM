#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "hsolv.h"
#include "hsolvDlg.h"
#include "mesh.h"
#include "spars.h"
#include "hsolvdoc.h"

#define Ksb 5.67032e-8 // Stefan-Boltzmann Constant

// conversion to internal working units of m
double units[] = { 0.0254, 0.001, 0.01, 1, 2.54e-5, 1.e-6 };
double sq(double x) { return x * x; }

BOOL Chsolvdoc::AnalyzeProblem(CBigLinProb& L)
{
  int i, j, k, bf, pctr = 0;
  double Me[3][3], be[3]; // element matrices;
  double l[3], p[3], q[3]; // element shape parameters;
  int n[3], ne[3]; // numbers of nodes for a particular element;

  double a, K, r, z, kludge;
  double h, bta, Tinf, TinfRad, Tlast, *Vo;

  BOOL IsNonlinear = FALSE;
  CElement* El;
  CComplex kn;
  int iter = 0;

  Depth *= units[LengthUnits];
  extRo *= units[LengthUnits];
  extRi *= units[LengthUnits];
  extZo *= units[LengthUnits];
  kludge = 1;

  TheView->SetDlgItemText(IDC_FRAME1, "Matrix Construction");

  Vo = (double*)calloc(NumNodes, sizeof(double));

  // scan through the problem to see if there are any elements
  // with a nonlinear conductivity
  for (i = 0; i < NumNodes; i++) {
    if (blockproplist[meshele[i].blk].npts > 0) {
      IsNonlinear = TRUE;
      i = NumNodes;
    }
  }

  do {
    // copy old solution
    for (i = 0; i < NumNodes; i++)
      Vo[i] = L.V[i];
    L.Wipe();

    // do some book-keeping related to fixed boundary conditions;
    // The P vector denotes which nodes have an assigned value
    // The V vector denotes the assigned value
    for (i = 0; i < NumNodes; i++) {
      L.Q[i] = -2;
      if (meshnode[i].bc >= 0)
        if (nodeproplist[meshnode[i].bc].qp == 0) {
          L.V[i] = nodeproplist[meshnode[i].bc].V;
          L.Q[i] = -1;
        }

      if (meshnode[i].InConductor >= 0)
        if (circproplist[meshnode[i].InConductor].CircType == 1) {
          L.V[i] = circproplist[meshnode[i].InConductor].V;
          L.Q[i] = meshnode[i].InConductor;
        }
    }

    // account for fixed boundary conditions along segments;
    for (i = 0; i < NumEls; i++) {
      for (j = 0; j < 3; j++) {
        k = j + 1;
        if (k == 3)
          k = 0;
        if (meshele[i].e[j] >= 0) {
          if (lineproplist[meshele[i].e[j]].BdryFormat == 0) {
            L.V[meshele[i].p[j]] = lineproplist[meshele[i].e[j]].Tset;
            L.V[meshele[i].p[k]] = lineproplist[meshele[i].e[j]].Tset;
            L.Q[meshele[i].p[j]] = -1;
            L.Q[meshele[i].p[k]] = -1;
          }
        }
      }
    }

    // build element matrices using the matrices derived in Allaire's book.
    for (i = 0; i < NumEls; i++) {
      // update progress bar
      j = 5 * ((i * 20) / NumEls);
      if (j != pctr) {
        pctr = j;
        TheView->m_prog1.SetPos(pctr);
      }

      // zero out Me, be;
      for (j = 0; j < 3; j++) {
        for (k = 0; k < 3; k++)
          Me[j][k] = 0;
        be[j] = 0;
      }

      // Determine shape parameters.
      // l's are element side lengths;
      // p's corresponds to the `b' parameter in Allaire
      // q's corresponds to the `c' parameter in Allaire
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
        l[j] = sqrt(sq(meshnode[n[k]].x - meshnode[n[j]].x) + sq(meshnode[n[k]].y - meshnode[n[j]].y));
      }
      a = (p[0] * q[1] - p[1] * q[0]) / 2.;
      r = (meshnode[n[0]].x + meshnode[n[1]].x + meshnode[n[2]].x) / 3.;

      // get the thermal conductivites to use for this element;
      kn = (blockproplist[El->blk].GetK(Vo[n[0]]) + blockproplist[El->blk].GetK(Vo[n[1]]) + blockproplist[El->blk].GetK(Vo[n[2]])) / 3.;

      if (ProblemType == AXISYMMETRIC) {
        Depth = 2. * PI * r;

        // "Warp" the permeability of this element is part of
        // the conformally mapped external region
        if (labellist[meshele[i].lbl].IsExternal) {
          z = (meshnode[n[0]].y + meshnode[n[1]].y + meshnode[n[2]].y) / 3. - extZo;
          kludge = (r * r + z * z) / (extRi * extRo);
        } else
          kludge = 1;
      }

      // x-contribution;
      K = -Depth * Re(kn) / (4. * a) / kludge;
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++) {
          Me[j][k] += K * p[j] * p[k];
          if (j != k)
            Me[k][j] += K * p[j] * p[k];
        }

      // y-contribution;
      K = -Depth * Im(kn) / (4. * a) / kludge;
      for (j = 0; j < 3; j++)
        for (k = j; k < 3; k++) {
          Me[j][k] += K * q[j] * q[k];
          if (j != k)
            Me[k][j] += K * q[j] * q[k];
        }

      // contribution to Me and be from time-transient term
      /*			if (dT!=0)
            {
              K = -Depth*blockproplist[El->blk].Kt*a/(12.*dT);

              Me[0][0]+=2.*K;
              Me[1][1]+=2.*K;
              Me[2][2]+=2.*K;
              Me[0][1]+=K; Me[1][0]+=K;
              Me[0][2]+=K; Me[2][0]+=K;
              Me[1][2]+=K; Me[2][1]+=K;

              be[0]+=K*(2.*Tprev[n[0]] +    Tprev[n[1]] +    Tprev[n[2]]);
              be[1]+=K*(   Tprev[n[0]] + 2.*Tprev[n[1]] +    Tprev[n[2]]);
              be[2]+=K*(   Tprev[n[0]] +    Tprev[n[1]] + 2.*Tprev[n[2]]);
            } */

      if (dT != 0) {
        K = -Depth * blockproplist[El->blk].Kt * a / (3. * dT);

        Me[0][0] += K;
        Me[1][1] += K;
        Me[2][2] += K;

        be[0] += K * Tprev[n[0]];
        be[1] += K * Tprev[n[1]];
        be[2] += K * Tprev[n[2]];
      }

      // contribution to be[] from volume charge density
      for (j = 0; j < 3; j++) {
        K = -Depth * (blockproplist[El->blk].qv) * a / 3.;
        be[j] += K;
      }

      for (j = 0; j < 3; j++) {
        if (El->e[j] >= 0) {
          k = j + 1;
          if (k == 3)
            k = 0;

          if (ProblemType == AXISYMMETRIC)
            Depth = PI * (meshnode[n[j]].x + meshnode[n[k]].x);

          // contributions to Me, be from derivative boundary conditions;
          bf = lineproplist[El->e[j]].BdryFormat;
          if ((bf == 1) || (bf == 2) || (bf == 3)) {
            double c0, c1;

            switch (bf) {
            case 1: // prescribed heat flux
              c0 = 0;
              c1 = lineproplist[El->e[j]].qs;
              break;
            case 2: // convection + flux
              c0 = lineproplist[El->e[j]].h;
              c1 = -c0 * lineproplist[El->e[j]].Tinf + lineproplist[El->e[j]].qs;
              break;
            case 3: // radiation + convection + fjux
              IsNonlinear = TRUE;
              bta = lineproplist[El->e[j]].beta;
              Tinf = lineproplist[El->e[j]].Tinf;
              TinfRad = lineproplist[El->e[j]].TinfRad;
              h = lineproplist[El->e[j]].h;
              if (iter == 0) {
                if (strlen(PrevSoln) == 0)
                  // not transient and no explicit initial guess
                  Tlast = Tinf + 1.;
                else
                  // previous soln specified, use that as the initial guess
                  Tlast = (Tprev[n[j]] + Tprev[n[k]]) / 2.;
              } else
                // use soluntion from last iteration
                Tlast = (Vo[n[j]] + Vo[n[k]]) / 2.;

              c0 = h + 4. * bta * Ksb * pow(Tlast, 3.);
              c1 = -(h * Tinf + bta * Ksb * (pow(TinfRad, 4.) + 3. * pow(Tlast, 4.))) + lineproplist[El->e[j]].qs;

              break;
            default:
              break;
            }

            if (ProblemType == AXISYMMETRIC) {
              K = -2. * PI * c0 * l[j] / 6.;
              Me[j][j] += K * 2. * (3. * meshnode[n[j]].x + meshnode[n[k]].x) / 4.;
              Me[k][k] += K * 2. * (meshnode[n[j]].x + 3. * meshnode[n[k]].x) / 4.;
              Me[j][k] += K * (meshnode[n[j]].x + meshnode[n[k]].x) / 2.;
              Me[k][j] += K * (meshnode[n[j]].x + meshnode[n[k]].x) / 2.;

              K = 2. * PI * c1 * l[j] / 2.;
              be[j] += K * (2. * meshnode[n[j]].x + meshnode[n[k]].x) / 3.;
              be[k] += K * (meshnode[n[j]].x + 2. * meshnode[n[k]].x) / 3.;
            } else {
              K = -Depth * c0 * l[j] / 6.;
              Me[j][j] += K * 2.;
              Me[k][k] += K * 2.;
              Me[j][k] += K;
              Me[k][j] += K;

              K = Depth * c1 * l[j] / 2.;
              be[j] += K;
              be[k] += K;
            }
          }
          /*
            // contribution to be[] from surface heating
            if (lineproplist[El->e[j]].BdryFormat==2)
            {
              K =-Depth*lineproplist[El->e[j]].qs*l[j]/2.;
              be[j]+=K;
              be[k]+=K;
            }
          */
        }
      }

      // process any prescribed nodal values;
      for (j = 0; j < 3; j++) {
        if (L.Q[n[j]] != -2) {
          for (k = 0; k < 3; k++) {
            if (j != k) {
              be[k] -= Me[k][j] * L.V[n[j]];
              Me[k][j] = 0;
              Me[j][k] = 0;
            }
          }
          be[j] = L.V[n[j]] * Me[j][j];
        }
      }

      // combine block matrices into global matrices;
      for (j = 0; j < 3; j++) {
        ne[j] = n[j];
        if (meshnode[n[j]].InConductor >= 0)
          if (circproplist[meshnode[n[j]].InConductor].CircType == 0)
            ne[j] = meshnode[n[j]].InConductor + NumNodes;
      }
      for (j = 0; j < 3; j++) {
        for (k = j; k < 3; k++)
          L.Put(L.Get(ne[j], ne[k]) - Me[j][k], ne[j], ne[k]);
        L.b[ne[j]] -= be[j];

        if (ne[j] != n[j]) {
          L.Put(L.Get(n[j], n[j]) - Me[j][j], n[j], n[j]);
          L.Put(L.Get(n[j], ne[j]) + Me[j][j], n[j], ne[j]);
        }
      }

    } // end of loop that builds element matrices

    // add in contribution from point charge density;
    for (i = 0; i < NumNodes; i++) {
      if ((meshnode[i].bc >= 0) && (L.Q[i] == -2)) {
        if (ProblemType == AXISYMMETRIC)
          Depth = 2. * PI * meshnode[i].x;
        L.b[i] += (Depth * nodeproplist[meshnode[i].bc].qp);
        L.Q[i] = -1;
      }

      // some bookkeeping to denote which nodes we can smooth over
      if (meshnode[i].InConductor >= 0)
        L.Q[i] = meshnode[i].InConductor;
    }

    // Apply any periodicity/antiperiodicity boundary conditions that we have
    for (k = 0, pctr = 0; k < NumPBCs; k++) {
      if (pbclist[k].t == 0)
        L.Periodicity(pbclist[k].x, pbclist[k].y);
      if (pbclist[k].t == 1)
        L.AntiPeriodicity(pbclist[k].x, pbclist[k].y);
    }

    // Finish building the equations that assign conductor voltage;
    for (i = 0; i < NumCircProps; i++) {
      // put a placeholder on the main diagonal;
      k = NumNodes + i;

      if (circproplist[i].CircType == 1) {
        K = L.Get(0, 0);
        L.Put(K, k, k);
        L.b[k] = K * circproplist[i].V;
      }

      if (circproplist[i].CircType == 0) {
        for (j = 0, K = 0; j < L.n; j++)
          if (j != k)
            K += L.Get(k, j);
        if (K != 0) {
          L.Put(-K, k, k);
          L.b[k] = circproplist[i].q;
        } else
          L.Put(L.Get(0, 0), k, k);
      }
    }

    // solve the problem;
    if (L.PCGSolve(iter++) == FALSE) {
      free(Vo);
      return FALSE;
    }

    if (IsNonlinear) {
      double e1 = 0;
      double e2 = 0;
      int prog;
      CString fmsg;

      fmsg.Format("Iteration(%i)", iter);
      TheView->SetDlgItemText(IDC_FRAME2, fmsg);

      for (i = 0; i < NumNodes; i++) {
        e1 += (L.V[i] - Vo[i]) * (L.V[i] - Vo[i]);
        e2 += (Vo[i] * Vo[i]);
      }
      if (e2 != 0) {
        // test to see if we have converged.
        if (sqrt(e1 / e2) < Precision * 100.)
          IsNonlinear = FALSE;
        prog = (int)(100. * log10(e1 / e2) / (log10(Precision) + 2.));
        if (prog > 100)
          prog = 100;
        TheView->m_prog2.SetPos(prog);
      }
    }

  } while (IsNonlinear);

  // compute total charge on conductors
  // with a specified voltage
  for (i = 0; i < NumCircProps; i++)
    if (circproplist[i].CircType == 1)
      circproplist[i].q = ChargeOnConductor(i, L);

  free(Vo);
  return TRUE;
}

//=========================================================================
//=========================================================================

BOOL Chsolvdoc::WriteResults(CBigLinProb& L)
{
  // write solution to disk;

  char c[1024];
  FILE *fp, *fz;
  int i;
  double cf;

  // first, echo input .feh file to the .anh file;
  sprintf(c, "%s.feh", PathName);
  if ((fz = fopen(c, "rt")) == NULL) {
    Sleep(500);
    if ((fz = fopen(c, "rt")) == NULL) {
      fprintf(stderr, "Couldn't open %s.feh\n", PathName);
      return FALSE;
    }
  }

  sprintf(c, "%s.anh", PathName);
  if ((fp = fopen(c, "wt")) == NULL) {
    Sleep(500);
    if ((fp = fopen(c, "wt")) == NULL) {
      fprintf(stderr, "Couldn't write to %s.anh\n", PathName);
      return FALSE;
    }
  }

  while (fgets(c, 1024, fz) != NULL)
    fputs(c, fp);
  fclose(fz);

  // then print out node, line, and element information
  fprintf(fp, "[Solution]\n");
  cf = units[LengthUnits];
  fprintf(fp, "%i\n", NumNodes);
  for (i = 0; i < NumNodes; i++)
    fprintf(fp, "%.17g	%.17g	%.17g	%i\n", meshnode[i].x / cf, meshnode[i].y / cf, L.V[i], L.Q[i]);
  fprintf(fp, "%i\n", NumEls);
  for (i = 0; i < NumEls; i++)
    fprintf(fp, "%i	%i	%i	%i\n",
        meshele[i].p[0], meshele[i].p[1], meshele[i].p[2], meshele[i].lbl);

  // print out circuit info
  fprintf(fp, "%i\n", NumCircProps);
  for (i = 0; i < NumCircProps; i++)
    fprintf(fp, "%.17g	%.17g\n", L.V[NumNodes + i], circproplist[i].q);

  fclose(fp);
  return TRUE;
}

//=========================================================================
//=========================================================================

double Chsolvdoc::ChargeOnConductor(int u, CBigLinProb& L)
{
  int i, k;
  double b[3], c[3]; // element shape parameters;
  int n[3]; // numbers of nodes for a particular element;
  double a, da, Dx, Dy, vx, vy, Z;
  CComplex kn;

  for (i = 0; i < NumNodes; i++)
    if (meshnode[i].InConductor == u)
      L.P[i] = 1;
    else
      L.P[i] = 0;

  // build element matrices using the matrices derived in Allaire's book.
  for (i = 0, Z = 0; i < NumEls; i++) {
    for (k = 0; k < 3; k++)
      n[k] = meshele[i].p[k];

    if ((L.P[n[0]] != 0) || (L.P[n[1]] != 0) || (L.P[n[2]] != 0)) {
      // Determine shape parameters.
      b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
      b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
      b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
      c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
      c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
      c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
      da = (b[0] * c[1] - b[1] * c[0]);
      a = da / 2.;
      if (ProblemType == AXISYMMETRIC)
        a *= (2. * PI * (meshnode[n[0]].x + meshnode[n[1]].x + meshnode[n[2]].x) / 3.);
      else
        a *= Depth;
      // get normal vector and element flux density;
      for (k = 0, kn = 0, vx = 0, vy = 0, Dx = 0, Dy = 0; k < 3; k++) {
        vx -= (L.P[n[k]] * b[k]) / da;
        vy -= (L.P[n[k]] * c[k]) / da;
        Dx -= (L.V[n[k]] * b[k]) / da;
        Dy -= (L.V[n[k]] * c[k]) / da;
        kn += blockproplist[meshele[i].blk].GetK(L.V[n[k]]) / 3.;
      }
      Dx *= Re(kn);
      Dy *= Im(kn);

      Z += a * (Dx * vx + Dy * vy);
    }
  }

  return Z;
}