#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "cv_problem.h"
#include "femm.h"
#include "xyplot.h"
#include "cviewDoc.h"
#include "cviewView.h"
#include "MainFrm.h"

#include "maskprogress.h"
#include "lua.h"

#include "spars.h"

extern BOOL bLinehook;
extern CLuaConsoleDlg* LuaConsole;

// #define SIMPLE

#ifdef SIMPLE

BOOL CcviewDoc::MakeMask()
{
  // Good placeholder mask generator
  // This gives a valid mask that butts right up against the
  // selected region. This mask yields the same results as the
  // Coulomb virtual work method.

  int i, j;

  for (i = 0; i < meshnode.GetSize(); i++)
    meshnode[i].msk = 0;

  for (i = 0; i < meshelem.GetSize(); i++) {
    if (blocklist[meshelem[i].lbl].IsSelected) {
      for (j = 0; j < 3; j++)
        meshnode[meshelem[i].p[j]].msk = 1;
    }
  }

  return TRUE;
}

#else

BOOL CcviewDoc::MakeMask()
{
  if (bHasMask)
    return TRUE;

  int i, j, k, d;
  CBigLinProb L;
  CMaskProgress dlg;
  double v;
  double Me[3][3], be[3]; // element matrix;
  double p[3], q[3]; // element shape parameters;
  double a; // element area;
  int n[3]; // numbers of nodes for a particular element;
  int* matflag;
  int* lblflag;

  int NumNodes = (int)meshnode.GetSize();
  int NumEls = (int)meshelem.GetSize();
  BOOL bOnAxis = FALSE;

  static int plus1mod3[3] = { 1, 2, 0 };
  static int minus1mod3[3] = { 2, 0, 1 };

  // Display progress dialog
  if (bLinehook == FALSE) {
    dlg.Create(IDD_MASKPROGRESS);
    dlg.ShowWindow(SW_SHOW);
    L.bar = &dlg.m_mask_progress_status;
    dlg.m_mask_progress_status.SetPos(0);
    L.Pump();
  }

  // figure out bandwidth--helps speed somethings up;
  int bw = 0;
  for (i = 0; i < NumEls; i++) {
    for (j = 0; j < 3; j++) {
      k = j + 1;
      if (k == 3)
        k = 0;
      d = abs(meshelem[i].p[j] - meshelem[i].p[k]);
      if (d > bw)
        bw = d;
    }
  }
  bw++;

  L.Create(NumNodes, bw);

  // if the problem is axisymmetric, does the selection lie along r=0?
  if (ProblemType == 1) {
    for (i = 0; i < NumEls; i++)
      if (blocklist[meshelem[i].lbl].IsSelected) {
        for (j = 0; j < 3; j++)
          if (meshnode[meshelem[i].p[j]].x < 1.e-6) {
            bOnAxis = TRUE;
            break;
          }
        if (bOnAxis)
          break;
      }

    if (!bOnAxis)
      for (i = 0; i < NumNodes; i++)
        if ((meshnode[i].IsSelected == TRUE) && (meshnode[i].x < 1.e-6)) {
          bOnAxis = TRUE;
          break;
        }
  }

  // Sort through materials to see if they denote air;
  matflag = (int*)calloc(blockproplist.GetSize(), sizeof(int));
  lblflag = (int*)calloc(blocklist.GetSize(), sizeof(int));
  for (i = 0; i < blockproplist.GetSize(); i++) {
    // k==0 for air, k==1 for other than air
    k = 0;
    if ((blockproplist[i].ex != 1) || (blockproplist[i].ey != 1))
      k = 1;
    matflag[i] = k;
  }

  // Now, sort through the labels to see which ones correspond to air blocks.
  for (i = 0; i < blocklist.GetSize(); i++) {
    lblflag[i] = matflag[blocklist[i].BlockType];
  }

  // Determine which nodal values should be fixed
  // and what values they should be fixed at;
  for (i = 0; i < NumNodes; i++) {
    if (meshnode[i].Q != -2)
      L.V[i] = 0;
    else
      L.V[i] = -1;
  }

  // Figure out which nodes are exterior edges and set them to zero;
  for (i = 0; i < NumEls; i++) {
    for (j = 0; j < 3; j++) {
      if (meshelem[i].n[j] == 1) {
        k = meshelem[i].p[plus1mod3[j]];
        if ((!bOnAxis) || (IsKosher(k)))
          L.V[k] = 0;
        k = meshelem[i].p[minus1mod3[j]];
        if ((!bOnAxis) || (IsKosher(k)))
          L.V[k] = 0;
      }
    }
  }

  // Set all nodes in a selected block equal to 1;
  for (i = 0; i < NumEls; i++) {
    if (blocklist[meshelem[i].lbl].IsSelected) {
      for (j = 0; j < 3; j++) {
        L.V[meshelem[i].p[j]] = 1;
      }
    } else if (lblflag[meshelem[i].lbl] != 0) {
      for (j = 0; j < 3; j++)
        L.V[meshelem[i].p[j]] = 0;
    }
  }

  // Any nodes that have point currents applied to them but are not in the
  // selected region should also be set to zero so that they don't mess up
  // the force calculation
  if (nodeproplist.GetSize() > 0) {
    CComplex* p;
    int npts;

    p = (CComplex*)calloc(nodelist.GetSize(), sizeof(CComplex));
    for (i = 0, npts = 0; i < nodelist.GetSize(); i++)
      if (nodelist[i].BoundaryMarker >= 0) {
        p[npts] = nodelist[i].CC();
        npts++;
      }

    if (npts > 0)
      for (i = 0; i < NumNodes; i++)
        for (j = 0; j < npts; j++)
          if (abs(p[j] - meshnode[i].CC()) < 1.e-8) {
            if (L.V[i] < 0)
              L.V[i] = 0.;
            npts--;
            if (npts > 0) {
              p[j] = p[npts];
              j = npts;
            } else {
              j = npts;
              i = NumNodes;
            }
          }
    free(p);
  }

  for (i = 0; i < NumNodes; i++)
    if (meshnode[i].IsSelected == TRUE)
      L.V[i] = 1;

  // build up element matrices;
  for (i = 0; i < NumEls; i++) {

    // zero out Me;
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 3; k++)
        Me[j][k] = 0;
      be[j] = 0;
    }

    // Determine shape parameters.
    // l == element side lengths;
    // p corresponds to the `b' parameter in Allaire
    // q corresponds to the `c' parameter in Allaire

    for (k = 0; k < 3; k++)
      n[k] = meshelem[i].p[k];
    p[0] = meshnode[n[1]].y - meshnode[n[2]].y;
    p[1] = meshnode[n[2]].y - meshnode[n[0]].y;
    p[2] = meshnode[n[0]].y - meshnode[n[1]].y;
    q[0] = meshnode[n[2]].x - meshnode[n[1]].x;
    q[1] = meshnode[n[0]].x - meshnode[n[2]].x;
    q[2] = meshnode[n[1]].x - meshnode[n[0]].x;

    a = (p[0] * q[1] - p[1] * q[0]) / 2.;

    // quick check for consistency--
    // if the block is not air and is not selected,
    // all of the nodes in the block better be defined
    // to be zero;  Otherwise, the region for force
    // integration has been selected in an invalid way;
    if ((!blocklist[meshelem[i].lbl].IsSelected) && (lblflag[meshelem[i].lbl])) {
      for (j = 0, k = 0; j < 3; j++)
        if (L.V[n[j]] == 0)
          k++;
      if (k < 3) {
        CString outmsg;

        if (bLinehook != FALSE) {
          outmsg += "*** The selected region is invalid. A valid selection\r\n";
          outmsg += "*** cannot abut a region which is not free space.\r\n";
          MessageBeep(MB_ICONEXCLAMATION);
          LuaConsole->ToOutput(outmsg);
        } else {
          outmsg = "The selected region is invalid. A valid selection\n";
          outmsg += "cannot abut a region which is not free space.";
          MsgBox(outmsg);
        }
        free(matflag);
        free(lblflag);
        return FALSE;
      }
    }

    // Each element weighted by its region's
    // mesh size specification;
    v = blocklist[meshelem[i].lbl].MaxArea;
    if (v <= 0)
      v = sqrt(a);
    else
      v = sqrt(v);

    // build element matrix;
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
        Me[j][k] += v * (p[j] * p[k] + q[j] * q[k]) / a;

    // process any prescribed nodal values;
    // doing it here saves a lot of time.
    for (j = 0; j < 3; j++) {
      if (L.V[n[j]] >= 0) {
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
      for (k = j; k < 3; k++)
        if (Me[j][k] != 0)
          L.Put(L.Get(n[j], n[k]) - Me[j][k], n[j], n[k]);
      L.b[n[j]] -= be[j];
    }
  }

  // solve the problem;
  bLinehook = BuildMask;
  if (L.PCGSolve(FALSE) == FALSE)
    return FALSE;
  bLinehook = FALSE;

  // Process the results to get one row of elements
  // that runs down the center of the gap away from boundaries.
  for (i = 0; i < NumNodes; i++)
    if (L.V[i] > 0.5)
      meshnode[i].msk = 1;
    else
      meshnode[i].msk = 0;

  free(matflag);
  free(lblflag);
  bHasMask = TRUE;

  return TRUE;
}

#endif

BOOL CcviewDoc::IsKosher(int k)
{
  // If:
  //    1) this is an axisymmetric problem;
  //    2) the selected geometry lies along the r=0 axis, and
  //    3) we have a node on the r=0 axis that we are trying to determine
  //     if we should set to zero.
  // This routine determines whether the node is at the extents of
  // the r=0 domain (or lies at a break in some sub-interval).
  //
  // Returns TRUE if it is OK to define the node as zero;

  if ((ProblemType == 0) || (meshnode[k].x > 1.e-6))
    return TRUE;

  int i, j, n;
  int score = 0;

  for (i = 0; i < NumList[k]; i++) {
    for (j = 0; j < 3; j++) {
      n = meshelem[ConList[k][i]].p[j];
      if ((n != k) && (meshnode[n].x < 1.e-6)) {
        score++;
        if (score > 1)
          return FALSE;
      }
    }
  }

  return TRUE;
}
