// implementation of various incarnations of calls
// to triangle from the CcdrawDoc class

#include "stdafx.h"
#include "femm.h"
#include "cdrawDoc.h"
#include "cd_probdlg.h"
#include "cd_PtProp.h"
#include "cd_OpBlkDlg.h"
#include "cd_OpNodeDlg.h"
#include "cd_OpSegDlg.h"
#include "cd_OpArcSegDlg.h"
#include "OpGrp.h"
#include "ArcDlg.h"

extern BOOL bLinehook;
extern lua_State* lua;
extern HANDLE hProc;

double CcdrawDoc::LineLength(int i)
{
  return abs(nodelist[linelist[i].n0].CC() - nodelist[linelist[i].n1].CC());
}

BOOL CcdrawDoc::HasPeriodicBC()
{
  BOOL flag = FALSE;
  int i, j, k;

  for (i = 0; i < lineproplist.GetSize(); i++) {
    if ((lineproplist[i].BdryFormat == 3) || (lineproplist[i].BdryFormat == 4))
      flag = TRUE;
  }
  // if flag is false, there can't be any lines
  // with periodic BC's, because no periodic boundary
  // conditions have been defined.
  if (flag == FALSE)
    return FALSE;

  // now, if there are some periodic boundary conditions,
  // we have to check to see if any have actually been
  // applied to the model
  flag = FALSE; // reset flag

  // first, test the segments
  for (i = 0; i < linelist.GetSize(); i++) {
    for (j = 0, k = -1; j < lineproplist.GetSize(); j++) {
      if (lineproplist[j].BdryName == linelist[i].BoundaryMarker) {
        k = j;
        break;
      }
    }
    if (k >= 0) {
      if ((lineproplist[k].BdryFormat == 3) || (lineproplist[k].BdryFormat == 4)) {
        flag = TRUE;
        break;
      }
    }
  }

  if (flag == TRUE)
    return TRUE;

  // If we've gotten this far, we still need to check the
  // arc segments.
  for (i = 0; i < arclist.GetSize(); i++) {
    for (j = 0, k = -1; j < lineproplist.GetSize(); j++) {
      if (lineproplist[j].BdryName == arclist[i].BoundaryMarker) {
        k = j;
        break;
      }
    }
    if (k >= 0) {
      if ((lineproplist[k].BdryFormat == 3) || (lineproplist[k].BdryFormat == 4)) {
        flag = TRUE;
        break;
      }
    }
  }

  // Finally, we're done. The value of flag now reflects
  // the judgement on whether or not we have periodic
  // and/or antiperiodic boundaries.
  return flag;
}

// What we do in the normal case is OnWritePoly
BOOL CcdrawDoc::OnWritePoly()
{
  FILE* fp;
  int i, j, k, l, t;
  double z, R, dL;
  CComplex a0, a1, a2, c;
  CString s;
  CArray<CNode, CNode&> nodelst;
  CArray<CSegment, CSegment&> linelst;
  CArray<CArcSegment, CArcSegment&> arclst;
  CArray<CBlockLabel, CBlockLabel&> blocklst;
  CNode node;
  CSegment segm;
  int bSmartMesh = theApp.session_SmartMesh;
  if (bSmartMesh < 0)
    bSmartMesh = SmartMesh;

  nodelst.RemoveAll();
  linelst.RemoveAll();

  // calculate length used to kludge fine meshing near input node points
  for (i = 0, z = 0; i < linelist.GetSize(); i++) {
    a0.Set(nodelist[linelist[i].n0].x, nodelist[linelist[i].n0].y);
    a1.Set(nodelist[linelist[i].n1].x, nodelist[linelist[i].n1].y);
    z += (abs(a1 - a0) / ((double)linelist.GetSize()));
  }
  dL = z / LineFraction;

  // copy node list as it is;
  for (i = 0; i < nodelist.GetSize(); i++)
    nodelst.Add(nodelist[i]);

  // discretize input segments
  for (i = 0; i < linelist.GetSize(); i++) {
    a0.Set(nodelist[linelist[i].n0].x, nodelist[linelist[i].n0].y);
    a1.Set(nodelist[linelist[i].n1].x, nodelist[linelist[i].n1].y);
    if (linelist[i].MaxSideLength == -1)
      k = 1;
    else {
      z = abs(a1 - a0);
      k = (int)ceil(z / linelist[i].MaxSideLength);
    }

    if (k == 1) // default condition where discretization on line is not specified
    {
      if ((abs(a1 - a0) < (3. * dL)) || (!bSmartMesh))
        linelst.Add(linelist[i]); // line is too short to add extra points
      else {
        // add extra points at a distance of dL from the ends of the line.
        // this forces Triangle to finely mesh near corners
        segm = linelist[i];
        for (j = 0; j < 3; j++) {
          if (j == 0) {
            a2 = a0 + dL * (a1 - a0) / abs(a1 - a0);
            node.x = a2.re;
            node.y = a2.im;
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = linelist[i].n0;
            segm.n1 = l;
            linelst.Add(segm);
          }

          if (j == 1) {
            a2 = a1 + dL * (a0 - a1) / abs(a1 - a0);
            node.x = a2.re;
            node.y = a2.im;
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = l - 1;
            segm.n1 = l;
            linelst.Add(segm);
          }

          if (j == 2) {
            l = (int)nodelst.GetSize() - 1;
            segm.n0 = l;
            segm.n1 = linelist[i].n1;
            linelst.Add(segm);
          }
        }
      }
    } else {
      segm = linelist[i];
      for (j = 0; j < k; j++) {
        a2 = a0 + (a1 - a0) * ((double)(j + 1)) / ((double)k);
        node.x = a2.re;
        node.y = a2.im;
        if (j == 0) {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = linelist[i].n0;
          segm.n1 = l;
          linelst.Add(segm);
        } else if (j == (k - 1)) {
          l = (int)nodelst.GetSize() - 1;
          segm.n0 = l;
          segm.n1 = linelist[i].n1;
          linelst.Add(segm);
        } else {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = l - 1;
          segm.n1 = l;
          linelst.Add(segm);
        }
      }
    }
  }

  // discretize input arc segments
  for (i = 0; i < arclist.GetSize(); i++) {
    arclist[i].mySideLength = arclist[i].MaxSideLength;
    a2.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
    k = (int)ceil(arclist[i].ArcLength / arclist[i].MaxSideLength);
    segm.BoundaryMarker = arclist[i].BoundaryMarker;
    segm.InConductor = arclist[i].InConductor;

    GetCircle(arclist[i], c, R);
    a1 = exp(I * arclist[i].ArcLength * PI / (((double)k) * 180.));

    if (k == 1) {
      segm.n0 = arclist[i].n0;
      segm.n1 = arclist[i].n1;
      linelst.Add(segm);
    } else
      for (j = 0; j < k; j++) {
        a2 = (a2 - c) * a1 + c;
        node.x = a2.re;
        node.y = a2.im;
        if (j == 0) {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = arclist[i].n0;
          segm.n1 = l;
          linelst.Add(segm);
        } else if (j == (k - 1)) {
          l = (int)nodelst.GetSize() - 1;
          segm.n0 = l;
          segm.n1 = arclist[i].n1;
          linelst.Add(segm);
        } else {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = l - 1;
          segm.n1 = l;
          linelst.Add(segm);
        }
      }
  }

  // create correct output filename;
  CString pn = GetPathName();
  CString plyname = pn.Left(pn.ReverseFind('.')) + ".poly";

  // check to see if we are ready to write a datafile;

  if ((fp = fopen(plyname, "wt")) == NULL) {
    MsgBox("Couldn't write to specified .poly file");
    return FALSE;
  }

  // write out node list
  fprintf(fp, "%i	2	0	1\n", nodelst.GetSize());
  for (i = 0; i < nodelst.GetSize(); i++) {
    // include boundary marker
    for (j = 0, t = 0; j < nodeproplist.GetSize(); j++)
      if (nodeproplist[j].PointName == nodelst[i].BoundaryMarker)
        t = j + 2;

    // include conductor number;
    for (j = 0; j < circproplist.GetSize(); j++) {
      if (circproplist[j].CircName == nodelst[i].InConductor) {
        t += ((j + 1) * 0x10000);
        break;
      }
    }

    fprintf(fp, "%i	%.17g	%.17g	%i\n", i, nodelst[i].x, nodelst[i].y, t);
  }

  // write out segment list
  fprintf(fp, "%i	1\n", linelst.GetSize());
  for (i = 0; i < linelst.GetSize(); i++) {
    // include boundary marker
    for (j = 0, t = 0; j < lineproplist.GetSize(); j++)
      if (lineproplist[j].BdryName == linelst[i].BoundaryMarker)
        t = -(j + 2);

    // include conductor number;
    for (j = 0; j < circproplist.GetSize(); j++) {
      if (circproplist[j].CircName == linelst[i].InConductor) {
        t -= ((j + 1) * 0x10000);
        break;
      }
    }

    fprintf(fp, "%i	%i	%i	%i\n", i, linelst[i].n0, linelst[i].n1, t);
  }

  // write out list of holes;
  for (i = 0, j = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>")
      j++;
  fprintf(fp, "%i\n", j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>") {
      fprintf(fp, "%i	%.17g	%.17g\n", k, blocklist[i].x, blocklist[i].y);
      k++;
    }

  // figure out a good default mesh size for block labels where
  // mesh size isn't explicitly specified
  CComplex xx, yy;
  double DefaultMeshSize;
  if (nodelst.GetSize() > 1) {
    xx = nodelst[0].CC();
    yy = xx;
    for (k = 0; k < nodelst.GetSize(); k++) {
      if (nodelst[k].x < Re(xx))
        xx.re = nodelst[k].x;
      if (nodelst[k].y < Im(xx))
        xx.im = nodelst[k].y;
      if (nodelst[k].x > Re(yy))
        yy.re = nodelst[k].x;
      if (nodelst[k].y > Im(yy))
        yy.im = nodelst[k].y;
    }
    DefaultMeshSize = pow(abs(yy - xx) / BoundingBoxFraction, 2.);
    if (!bSmartMesh)
      DefaultMeshSize = abs(yy - xx);
  } else
    DefaultMeshSize = -1;

  // write out regional attributes
  fprintf(fp, "%i\n", blocklist.GetSize() - j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType != "<No Mesh>") {
      fprintf(fp, "%i	%.17g	%.17g	", k, blocklist[i].x, blocklist[i].y);
      fprintf(fp, "%i	", k + 1);
      if ((blocklist[i].MaxArea > 0) && (blocklist[i].MaxArea < DefaultMeshSize))
        fprintf(fp, "%.17g\n", blocklist[i].MaxArea);
      else
        fprintf(fp, "%.17g\n", DefaultMeshSize);
      k++;
    }
  fclose(fp);

  // write out a trivial pbc file
  plyname = pn.Left(pn.ReverseFind('.')) + ".pbc";
  if ((fp = fopen(plyname, "wt")) == NULL) {
    MsgBox("Couldn't write to specified .pbc file");
    return FALSE;
  }
  fprintf(fp, "0\n");
  fclose(fp);

  // call triangle

  CString rootname = "\"" + pn.Left(pn.ReverseFind('.')) + "\"";
  char CommandLine[512];
  sprintf(CommandLine, "\"%striangle.exe\" -p -P -j -q%f -e -A -a -z -Q -I %s",
      (const char*)BinDir, __min(MinAngle + MINANGLE_BUMP, MINANGLE_MAX), (const char*)rootname);

  STARTUPINFO StartupInfo = { 0 };
  PROCESS_INFORMATION ProcessInfo;
  StartupInfo.cb = sizeof(STARTUPINFO);
  StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
  StartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;
  if (CreateProcess(NULL, CommandLine, NULL, NULL, FALSE,
          0, NULL, NULL, &StartupInfo, &ProcessInfo)) {

    if (bLinehook == FALSE)
      WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    else {
      DWORD ExitCode;
      hProc = ProcessInfo.hProcess;
      do {
        GetExitCodeProcess(ProcessInfo.hProcess, &ExitCode);
        ((CFemmApp*)AfxGetApp())->line_hook(lua, NULL);
        Sleep(1);
      } while (ExitCode == STILL_ACTIVE);
      hProc = NULL;
    }

  } else {
    MsgBox("Couldn't spawn triangle.exe");
    return FALSE;
  }

  DWORD ExitCode;
  GetExitCodeProcess(
      ProcessInfo.hProcess, // handle to the process
      &ExitCode // address to receive termination status
  );
  CloseHandle(ProcessInfo.hProcess);
  CloseHandle(ProcessInfo.hThread);
  if (ExitCode != 0) {
    MsgBox("Call to triangle was unsuccessful. Check for small angles.");
    return FALSE;
  }

  return TRUE;
}

// Call triangle to order segments on the boundary properly
BOOL CcdrawDoc::FunnyOnWritePoly()
{
  FILE* fp;
  int i, j, k, l, t, n, n0, n1, n2;
  double z, R, dL;
  CComplex a0, a1, a2, c;
  CComplex b0, b1, b2;
  char instring[1024];
  CString s;
  CArray<CNode, CNode&> nodelst;
  CArray<CSegment, CSegment&> linelst;
  CArray<CArcSegment, CArcSegment&> arclst;
  CArray<CBlockLabel, CBlockLabel&> blocklst;
  CArray<CPeriodicBoundary, CPeriodicBoundary&> pbclst;
  CArray<CCommonPoint, CCommonPoint&> ptlst;
  CNode node;
  CSegment segm;
  CPeriodicBoundary pbc;
  CCommonPoint pt;
  int bSmartMesh = theApp.session_SmartMesh;
  if (bSmartMesh < 0)
    bSmartMesh = SmartMesh;

  nodelst.RemoveAll();
  linelst.RemoveAll();
  pbclst.RemoveAll();
  ptlst.RemoveAll();

  UpdateUndo();

  // calculate length used to kludge fine meshing near input node points
  for (i = 0, z = 0; i < linelist.GetSize(); i++) {
    a0.Set(nodelist[linelist[i].n0].x, nodelist[linelist[i].n0].y);
    a1.Set(nodelist[linelist[i].n1].x, nodelist[linelist[i].n1].y);
    z += (abs(a1 - a0) / ((double)linelist.GetSize()));
  }
  dL = z / LineFraction;

  // copy node list as it is;
  for (i = 0; i < nodelist.GetSize(); i++)
    nodelst.Add(nodelist[i]);

  // discretize input segments
  for (i = 0; i < linelist.GetSize(); i++) {
    // abuse the IsSelected flag to carry a notation
    // of which line or arc in the input geometry a
    // particular segment is associated with
    segm = linelist[i];
    segm.IsSelected = i;
    a0.Set(nodelist[linelist[i].n0].x, nodelist[linelist[i].n0].y);
    a1.Set(nodelist[linelist[i].n1].x, nodelist[linelist[i].n1].y);

    if (linelist[i].MaxSideLength == -1)
      k = 1;
    else {
      z = abs(a1 - a0);
      k = (int)ceil(z / linelist[i].MaxSideLength);
    }

    if (k == 1) // default condition where discretization on line is not specified
    {
      if ((abs(a1 - a0) < (3. * dL)) || (!bSmartMesh))
        linelst.Add(segm); // line is too short to add extra points
      else {
        // add extra points at a distance of dL from the ends of the line.
        // this forces Triangle to finely mesh near corners
        for (j = 0; j < 3; j++) {
          if (j == 0) {
            a2 = a0 + dL * (a1 - a0) / abs(a1 - a0);
            node.x = a2.re;
            node.y = a2.im;
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = linelist[i].n0;
            segm.n1 = l;
            linelst.Add(segm);
          }

          if (j == 1) {
            a2 = a1 + dL * (a0 - a1) / abs(a1 - a0);
            node.x = a2.re;
            node.y = a2.im;
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = l - 1;
            segm.n1 = l;
            linelst.Add(segm);
          }

          if (j == 2) {
            l = (int)nodelst.GetSize() - 1;
            segm.n0 = l;
            segm.n1 = linelist[i].n1;
            linelst.Add(segm);
          }
        }
      }
    } else {
      for (j = 0; j < k; j++) {
        a2 = a0 + (a1 - a0) * ((double)(j + 1)) / ((double)k);
        node.x = a2.re;
        node.y = a2.im;
        if (j == 0) {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = linelist[i].n0;
          segm.n1 = l;
          linelst.Add(segm);
        } else if (j == (k - 1)) {
          l = (int)nodelst.GetSize() - 1;
          segm.n0 = l;
          segm.n1 = linelist[i].n1;
          linelst.Add(segm);
        } else {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = l - 1;
          segm.n1 = l;
          linelst.Add(segm);
        }
      }
    }
  }

  // discretize input arc segments
  for (i = 0; i < arclist.GetSize(); i++) {
    segm.IsSelected = i + (int)linelist.GetSize();
    a2.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
    k = (int)ceil(arclist[i].ArcLength / arclist[i].MaxSideLength);
    segm.BoundaryMarker = arclist[i].BoundaryMarker;
    segm.InConductor = arclist[i].InConductor;
    GetCircle(arclist[i], c, R);
    a1 = exp(I * arclist[i].ArcLength * PI / (((double)k) * 180.));

    if (k == 1) {
      segm.n0 = arclist[i].n0;
      segm.n1 = arclist[i].n1;
      linelst.Add(segm);
    } else
      for (j = 0; j < k; j++) {
        a2 = (a2 - c) * a1 + c;
        node.x = a2.re;
        node.y = a2.im;
        if (j == 0) {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = arclist[i].n0;
          segm.n1 = l;
          linelst.Add(segm);
        } else if (j == (k - 1)) {
          l = (int)nodelst.GetSize() - 1;
          segm.n0 = l;
          segm.n1 = arclist[i].n1;
          linelst.Add(segm);
        } else {
          l = (int)nodelst.GetSize();
          nodelst.Add(node);
          segm.n0 = l - 1;
          segm.n1 = l;
          linelst.Add(segm);
        }
      }
  }

  // create correct output filename;
  CString pn = GetPathName();
  CString plyname = pn.Left(pn.ReverseFind('.')) + ".poly";

  // check to see if we are ready to write a datafile;

  if ((fp = fopen(plyname, "wt")) == NULL) {
    MsgBox("Couldn't write to specified .poly file");
    Undo();
    UnselectAll();
    return FALSE;
  }

  // write out node list
  fprintf(fp, "%i	2	0	1\n", nodelst.GetSize());
  for (i = 0; i < nodelst.GetSize(); i++) {
    fprintf(fp, "%i	%.17g	%.17g	%i\n",
        i, nodelst[i].x, nodelst[i].y, 0);
  }

  // write out segment list
  fprintf(fp, "%i	1\n", linelst.GetSize());
  for (i = 0; i < linelst.GetSize(); i++) {
    t = -(linelst[i].IsSelected + 2);
    fprintf(fp, "%i	%i	%i	%i\n", i, linelst[i].n0, linelst[i].n1, t);
  }

  // write out list of holes;
  for (i = 0, j = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>")
      j++;
  fprintf(fp, "%i\n", j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>") {
      fprintf(fp, "%i	%.17g	%.17g\n", k, blocklist[i].x, blocklist[i].y);
      k++;
    }

  // figure out a good default mesh size for block labels where
  // mesh size isn't explicitly specified
  CComplex xx, yy;
  double DefaultMeshSize;
  if (nodelst.GetSize() > 1) {
    xx = nodelst[0].CC();
    yy = xx;
    for (k = 0; k < nodelst.GetSize(); k++) {
      if (nodelst[k].x < Re(xx))
        xx.re = nodelst[k].x;
      if (nodelst[k].y < Im(xx))
        xx.im = nodelst[k].y;
      if (nodelst[k].x > Re(yy))
        yy.re = nodelst[k].x;
      if (nodelst[k].y > Im(yy))
        yy.im = nodelst[k].y;
    }
    DefaultMeshSize = pow(abs(yy - xx) / BoundingBoxFraction, 2.);
    if (!bSmartMesh)
      DefaultMeshSize = abs(yy - xx);
  } else
    DefaultMeshSize = -1;

  // write out regional attributes
  fprintf(fp, "%i\n", blocklist.GetSize() - j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType != "<No Mesh>") {
      fprintf(fp, "%i	%.17g	%.17g	", k, blocklist[i].x, blocklist[i].y);
      fprintf(fp, "%i	", k + 1);
      if ((blocklist[i].MaxArea > 0) && (blocklist[i].MaxArea < DefaultMeshSize))
        fprintf(fp, "%.17g\n", blocklist[i].MaxArea);
      else
        fprintf(fp, "%.17g\n", DefaultMeshSize);
      k++;
    }

  fclose(fp);

  // call triangle
  CString rootname = "\"" + pn.Left(pn.ReverseFind('.')) + "\"";
  char CommandLine[512];
  sprintf(CommandLine, "\"%striangle.exe\" -p -P -j -q%f -e -A -a -z -Q -I %s",
      (const char*)BinDir, __min(MinAngle + MINANGLE_BUMP, MINANGLE_MAX), (const char*)rootname);

  STARTUPINFO StartupInfo = { 0 };
  PROCESS_INFORMATION ProcessInfo;
  StartupInfo.cb = sizeof(STARTUPINFO);
  StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
  StartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;
  if (CreateProcess(NULL, CommandLine, NULL, NULL, FALSE,
          0, NULL, NULL, &StartupInfo, &ProcessInfo)) {

    if (bLinehook == FALSE)
      WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    else {
      DWORD ExitCode;
      hProc = ProcessInfo.hProcess;
      do {
        GetExitCodeProcess(ProcessInfo.hProcess, &ExitCode);
        ((CFemmApp*)AfxGetApp())->line_hook(lua, NULL);
        Sleep(1);
      } while (ExitCode == STILL_ACTIVE);
      hProc = NULL;
    }

  } else {
    MsgBox("Couldn't spawn triangle.exe");
    Undo();
    UnselectAll();
    return FALSE;
  }

  DWORD ExitCode;
  GetExitCodeProcess(
      ProcessInfo.hProcess, // handle to the process
      &ExitCode // address to receive termination status
  );
  CloseHandle(ProcessInfo.hProcess);
  CloseHandle(ProcessInfo.hThread);
  if (ExitCode != 0) {
    MsgBox("Call to triangle was unsuccessful. Check for small angles.");
    Undo();
    UnselectAll();
    return FALSE;
  }
  // #endif

  // So far, so good.  Now, read back in the .edge file
  // to make sure the points in the segments and arc
  // segments are ordered in a consistent way so that
  // the (anti)periodic boundary conditions can be applied.

  // read meshlines;
  plyname = pn.Left(pn.ReverseFind('.')) + ".edge";
  if ((fp = fopen(plyname, "rt")) == NULL) {
    MsgBox("Call to triangle was unsuccessful");
    Undo();
    UnselectAll();
    return FALSE;
  }
  fgets(instring, 1024, fp);
  sscanf(instring, "%i", &k);
  UnselectAll(); // abuse IsSelected again to keep a
                 // tally of how many subsegments each
                 // entity is sliced into.

  ptlst.SetSize(linelist.GetSize() + arclist.GetSize());
  for (i = 0; i < ptlst.GetSize(); i++)
    ptlst[i].t = 0;

  for (i = 0; i < k; i++) {
    fgets(instring, 1024, fp);
    sscanf(instring, "%i	%i	%i	%i", &l, &n0, &n1, &j);
    if (j != 0) {
      j = -(j + 2); // convert back to the `right' numbering

      // store a reference line that we can use to
      // determine whether or not this is a
      // boundary segment w/out re-running triangle.
      if (ptlst[j].t == 0) {
        ptlst[j].t = 1;
        if (n0 < n1) {
          ptlst[j].x = n0;
          ptlst[j].y = n1;
        } else {
          ptlst[j].x = n1;
          ptlst[j].y = n0;
        }
      }

      if (j < linelist.GetSize()) {
        // deal with segments
        linelist[j].IsSelected++;

        if ((linelist[j].n0 == n1) || (linelist[j].n1 == n0)) {
          t = linelist[j].n0;
          linelist[j].n0 = linelist[j].n1;
          linelist[j].n1 = t;
        }
      } else {
        // deal with arc segments;
        // Can't just flip the point order with
        // impunity in the arc segments, so we flip
        // a marker which denotes which side the
        // normal is on.

        j = j - (int)linelist.GetSize();
        arclist[j].IsSelected++;
        if ((arclist[j].n0 == n1) || (arclist[j].n1 == n0))
          arclist[j].NormalDirection = FALSE;
        if ((arclist[j].n0 == n0) || (arclist[j].n1 == n1))
          arclist[j].NormalDirection = TRUE;
      }
    }
  }
  fclose(fp);

  // figure out which segments / arcsegments are on the
  // boundary and force an appropriate mesh density on
  // these based on how many divisions are in the first
  // trial meshing of the domain.

  // paw through the element list to find out how many
  // elements each reference segment appears in.  If a
  // segment is on the boundary, it ought to appear in just
  // one element.  Otherwise, it appears in two.
  plyname = pn.Left(pn.ReverseFind('.')) + ".ele";
  if ((fp = fopen(plyname, "rt")) == NULL) {
    MsgBox("Call to triangle was unsuccessful");
    Undo();
    UnselectAll();
    return FALSE;
  }
  fgets(instring, 1024, fp);
  sscanf(instring, "%i", &k);
  for (i = 0; i < k; i++) {
    fgets(instring, 1024, fp);
    sscanf(instring, "%i	%i	%i	%i", &j, &n0, &n1, &n2);
    // Sort out the three nodes...
    if (n0 > n1) {
      n = n0;
      n0 = n1;
      n1 = n;
    }
    if (n1 > n2) {
      n = n1;
      n1 = n2;
      n2 = n;
    }
    if (n0 > n1) {
      n = n0;
      n0 = n1;
      n1 = n;
    }

    // now, check to see if any of the test segments
    // are sides of this node...
    for (j = 0; j < ptlst.GetSize(); j++) {
      if ((n0 == ptlst[j].x) && (n1 == ptlst[j].y))
        ptlst[j].t--;
      if ((n0 == ptlst[j].x) && (n2 == ptlst[j].y))
        ptlst[j].t--;
      if ((n1 == ptlst[j].x) && (n2 == ptlst[j].y))
        ptlst[j].t--;
    }
  }
  fclose(fp);

  // impose "new" mesh constraints on bdry arcs and segments....
  for (i = 0; i < linelist.GetSize(); i++) {
    if (ptlst[i].t == 0)
      linelist[i].MaxSideLength = LineLength(i) / ((double)linelist[i].IsSelected);
  }
  for (i = 0; i < arclist.GetSize(); i++) {
    if (ptlst[i + linelist.GetSize()].t == 0) {
      // alter maxsidelength, but do it in such
      // a way that it carries only 4 significant
      // digits.  There's no use in carrying double
      // precision here, because it looks crappy
      // when you open up the arc segment to see
      // its properties.
      char kludge[32];
      arclist[i].MaxSideLength = arclist[i].ArcLength / ((double)arclist[i].IsSelected);
      sprintf(kludge, "%.1e", arclist[i].MaxSideLength);
      sscanf(kludge, "%lf", &arclist[i].MaxSideLength);
    }
  }
  ptlst.RemoveAll();

  // want to impose explicit discretization only on
  // the boundary arcs and segments.  After the meshing
  // is done, spacing on boundary segments should be
  // restored to the value that was there before meshing
  // was called, but the arc segments should keep the
  // "new" MaxSideLength--this is used in other places
  // and must always be consistent with the the mesh.

  // Now, do a shitload of checking to make sure that
  // the PBCs haven't been defined by the user
  // in a messed up way.

  // First, search through defined bc's for periodic ones;
  for (i = 0; i < lineproplist.GetSize(); i++) {
    if ((lineproplist[i].BdryFormat == 3) || (lineproplist[i].BdryFormat == 4)) {
      pbc.BdryName = lineproplist[i].BdryName;
      pbc.BdryFormat = lineproplist[i].BdryFormat - 3; // 0 for pbc, 1 for apbc
      pbclst.Add(pbc);
    }
  }

  for (i = 0; i < linelist.GetSize(); i++) {
    for (j = 0; j < pbclst.GetSize(); j++) {
      if (pbclst[j].BdryName == linelist[i].BoundaryMarker) {
        // A pbc or apbc can only be applied to 2 segs
        // at a time.  If it is applied to multiple segs
        // at the same time, flag it and kick it out.
        if (pbclst[j].nseg == 2) {
          MsgBox("An (anti)periodic BC is assigned to more than two segments");
          Undo();
          UnselectAll();
          return FALSE;
        }
        pbclst[j].seg[pbclst[j].nseg] = i;
        pbclst[j].nseg++;
      }
    }
  }

  for (i = 0; i < arclist.GetSize(); i++) {
    for (j = 0; j < pbclst.GetSize(); j++) {
      if (pbclst[j].BdryName == arclist[i].BoundaryMarker) {
        // A pbc or apbc can only be applied to 2 arcs
        // at a time.  If it is applied to multiple arcs
        // at the same time, flag it and kick it out.
        if (pbclst[j].narc == 2) {
          MsgBox("An (anti)periodic BC is assigned to more than two arcs");
          Undo();
          UnselectAll();
          return FALSE;
        }
        pbclst[j].seg[pbclst[j].narc] = i;
        pbclst[j].narc++;
      }
    }
  }

  j = 0;
  while (j < pbclst.GetSize()) {
    // check for a bc that is a mix of arcs and segments.
    // this is an error, and it should get flagged.
    if ((pbclst[j].nseg > 0) && (pbclst[j].narc > 0)) {
      MsgBox("Can't mix arcs and segments for (anti)periodic BCs");
      Undo();
      UnselectAll();
      return FALSE;
    }

    // remove any periodic BC's that aren't actually in play
    if ((pbclst[j].nseg < 2) && (pbclst[j].narc < 2))
      pbclst.RemoveAt(j);
    else
      j++;
  }

  for (j = 0; j < pbclst.GetSize(); j++) {
    // check to see if adjoining entries are applied
    // to objects of compatible size/shape, and
    // reconcile meshing on the objects.

    // for segments:
    if (pbclst[j].nseg > 0) {

      // make sure that lines are pretty much the same length
      if (fabs(LineLength(pbclst[j].seg[0])
              - LineLength(pbclst[j].seg[1]))
          > 1.e-06) {
        MsgBox("(anti)periodic BCs applied to dissimilar segments");
        Undo();
        UnselectAll();
        return FALSE;
      }

      // make sure that both lines have the same spacing
      double len1, len2, len;
      len1 = linelist[pbclst[j].seg[0]].MaxSideLength;
      len2 = linelist[pbclst[j].seg[1]].MaxSideLength;

      if (len1 <= 0)
        len1 = len2;
      if (len2 <= 0)
        len2 = len1;
      len = min(len1, len2);

      linelist[pbclst[j].seg[0]].MaxSideLength = len;
      linelist[pbclst[j].seg[1]].MaxSideLength = len;
    }

    // for arc segments:
    if (pbclst[j].narc > 0) {

      // make sure that arcs are pretty much the
      // same arc length
      if (fabs(arclist[pbclst[j].seg[0]].ArcLength
              - arclist[pbclst[j].seg[1]].ArcLength)
          > 1.e-06) {
        MsgBox("(anti)periodic BCs applied to dissimilar arc segments");
        Undo();
        UnselectAll();
        return FALSE;
      }

      // make sure that both lines have the same spacing
      double len1, len2, len;
      len1 = arclist[pbclst[j].seg[0]].MaxSideLength;
      len2 = arclist[pbclst[j].seg[1]].MaxSideLength;

      len = min(len1, len2);

      arclist[pbclst[j].seg[0]].MaxSideLength = len;
      arclist[pbclst[j].seg[1]].MaxSideLength = len;
    }
  }

  // write out new poly and write out adjacent
  // boundary nodes in a separate .pbc file.

  // kludge things a bit and use IsSelected to denote
  // whether or not a line or arc has already been processed.
  UnselectAll();
  nodelst.RemoveAll();
  linelst.RemoveAll();

  // first, add in existing nodes
  for (n = 0; n < nodelist.GetSize(); n++)
    nodelst.Add(nodelist[n]);

  for (n = 0; n < pbclst.GetSize(); n++) {
    if (pbclst[n].nseg != 0) // if this pbc is a line segment...
    {
      int s0, s1;
      CNode node0, node1;

      s0 = pbclst[n].seg[0];
      s1 = pbclst[n].seg[1];
      linelist[s0].IsSelected = TRUE;
      linelist[s1].IsSelected = TRUE;

      // make is so that first point on first line
      // maps to first point on second line...
      t = linelist[s1].n1;
      linelist[s1].n1 = linelist[s1].n0;
      linelist[s1].n0 = t;

      // store number of sub-segments in k
      if (linelist[s0].MaxSideLength == -1)
        k = 1;
      else {
        a0 = nodelist[linelist[s0].n0].CC();
        a1 = nodelist[linelist[s0].n1].CC();
        b0 = nodelist[linelist[s1].n0].CC();
        b1 = nodelist[linelist[s1].n1].CC();
        z = abs(a1 - a0);
        k = (int)ceil(z / linelist[s0].MaxSideLength);
      }

      // add segment end points to the list;
      pt.x = linelist[s0].n0;
      pt.y = linelist[s1].n0;
      pt.t = pbclst[n].BdryFormat;
      ptlst.Add(pt);
      pt.x = linelist[s0].n1;
      pt.y = linelist[s1].n1;
      pt.t = pbclst[n].BdryFormat;
      ptlst.Add(pt);

      if (k == 1) {
        // catch the case in which the line
        // doesn't get subdivided.
        linelst.Add(linelist[s0]);
        linelst.Add(linelist[s1]);
      } else {
        segm = linelist[s0];
        for (j = 0; j < k; j++) {
          a2 = a0 + (a1 - a0) * ((double)(j + 1)) / ((double)k);
          b2 = b0 + (b1 - b0) * ((double)(j + 1)) / ((double)k);
          node0.x = a2.re;
          node0.y = a2.im;
          node1.x = b2.re;
          node1.y = b2.im;
          if (j == 0) {
            l = (int)nodelst.GetSize();
            nodelst.Add(node0);
            segm.n0 = linelist[s0].n0;
            segm.n1 = l;
            linelst.Add(segm);
            pt.x = l;

            l = (int)nodelst.GetSize();
            nodelst.Add(node1);
            segm.n0 = linelist[s1].n0;
            segm.n1 = l;
            linelst.Add(segm);
            pt.y = l;

            pt.t = pbclst[n].BdryFormat;
            ptlst.Add(pt);
          } else if (j == (k - 1)) {
            // last subdivision--no ptlst
            // entry associated with this one.
            l = (int)nodelst.GetSize() - 2;
            segm.n0 = l;
            segm.n1 = linelist[s0].n1;
            linelst.Add(segm);

            l = (int)nodelst.GetSize() - 1;
            segm.n0 = l;
            segm.n1 = linelist[s1].n1;
            linelst.Add(segm);
          } else {
            l = (int)nodelst.GetSize();

            nodelst.Add(node0);
            nodelst.Add(node1);

            segm.n0 = l - 2;
            segm.n1 = l;
            linelst.Add(segm);

            segm.n0 = l - 1;
            segm.n1 = l + 1;
            linelst.Add(segm);

            pt.x = l;
            pt.y = l + 1;
            pt.t = pbclst[n].BdryFormat;
            ptlst.Add(pt);
          }
        }
      }
    } else { // if this pbc is an arc segment...

      int s0, s1;
      int p0[2], p1[2];
      CNode node0, node1;
      CComplex bgn0, bgn1, c0, c1, d0, d1;
      double r0, r1;

      s0 = pbclst[n].seg[0];
      s1 = pbclst[n].seg[1];
      arclist[s0].IsSelected = TRUE;
      arclist[s1].IsSelected = TRUE;

      k = (int)ceil(arclist[s0].ArcLength / arclist[s0].MaxSideLength);
      segm.BoundaryMarker = arclist[s0].BoundaryMarker;
      segm.InConductor = arclist[s0].InConductor;
      GetCircle(arclist[s0], c0, r0);
      GetCircle(arclist[s1], c1, r1);

      if (arclist[s0].NormalDirection == 0) {
        bgn0 = nodelist[arclist[s0].n0].CC();
        d0 = exp(I * arclist[s0].ArcLength * PI / (((double)k) * 180.));
        p0[0] = arclist[s0].n0;
        p0[1] = arclist[s0].n1;
      } else {
        bgn0 = nodelist[arclist[s0].n1].CC();
        d0 = exp(-I * arclist[s0].ArcLength * PI / (((double)k) * 180.));
        p0[0] = arclist[s0].n1;
        p0[1] = arclist[s0].n0;
      }

      if (arclist[s1].NormalDirection != 0) {
        bgn1 = nodelist[arclist[s1].n0].CC();
        d1 = exp(I * arclist[s1].ArcLength * PI / (((double)k) * 180.));
        p1[0] = arclist[s1].n0;
        p1[1] = arclist[s1].n1;
      } else {
        bgn1 = nodelist[arclist[s1].n1].CC();
        d1 = exp(-I * arclist[s1].ArcLength * PI / (((double)k) * 180.));
        p1[0] = arclist[s1].n1;
        p1[1] = arclist[s1].n0;
      }

      // add arc segment end points to the list;
      pt.x = p0[0];
      pt.y = p1[0];
      pt.t = pbclst[n].BdryFormat;
      ptlst.Add(pt);
      pt.x = p0[1];
      pt.y = p1[1];
      pt.t = pbclst[n].BdryFormat;
      ptlst.Add(pt);

      if (k == 1) {

        // catch the case in which the line
        // doesn't get subdivided.
        segm.n0 = p0[0];
        segm.n1 = p0[1];
        linelst.Add(segm);
        segm.n0 = p1[0];
        segm.n1 = p1[1];
        linelst.Add(segm);
      } else {
        for (j = 0; j < k; j++) {
          bgn0 = (bgn0 - c0) * d0 + c0;
          node0.x = bgn0.re;
          node0.y = bgn0.im;

          bgn1 = (bgn1 - c1) * d1 + c1;
          node1.x = bgn1.re;
          node1.y = bgn1.im;

          if (j == 0) {
            l = (int)nodelst.GetSize();
            nodelst.Add(node0);
            segm.n0 = p0[0];
            segm.n1 = l;
            linelst.Add(segm);
            pt.x = l;

            l = (int)nodelst.GetSize();
            nodelst.Add(node1);
            segm.n0 = p1[0];
            segm.n1 = l;
            linelst.Add(segm);
            pt.y = l;

            pt.t = pbclst[n].BdryFormat;
            ptlst.Add(pt);
          } else if (j == (k - 1)) {
            // last subdivision--no ptlst
            // entry associated with this one.
            l = (int)nodelst.GetSize() - 2;
            segm.n0 = l;
            segm.n1 = p0[1];
            linelst.Add(segm);

            l = (int)nodelst.GetSize() - 1;
            segm.n0 = l;
            segm.n1 = p1[1];
            linelst.Add(segm);
          } else {
            l = (int)nodelst.GetSize();

            nodelst.Add(node0);
            nodelst.Add(node1);

            segm.n0 = l - 2;
            segm.n1 = l;
            linelst.Add(segm);

            segm.n0 = l - 1;
            segm.n1 = l + 1;
            linelst.Add(segm);

            pt.x = l;
            pt.y = l + 1;
            pt.t = pbclst[n].BdryFormat;
            ptlst.Add(pt);
          }
        }
      }
    }
  }

  // Then, do the rest of the lines and arcs in the
  // "normal" way and write .poly file.

  // discretize input segments
  for (i = 0; i < linelist.GetSize(); i++)
    if (linelist[i].IsSelected == FALSE) {

      a0.Set(nodelist[linelist[i].n0].x, nodelist[linelist[i].n0].y);
      a1.Set(nodelist[linelist[i].n1].x, nodelist[linelist[i].n1].y);
      if (linelist[i].MaxSideLength == -1)
        k = 1;
      else {
        z = abs(a1 - a0);
        k = (int)ceil(z / linelist[i].MaxSideLength);
      }

      segm = linelist[i];
      if (k == 1) // default condition where discretization on line is not specified
      {
        if ((abs(a1 - a0) < (3. * dL)) || (!bSmartMesh))
          linelst.Add(segm); // line is too short to add extra points
        else {
          // add extra points at a distance of dL from the ends of the line.
          // this forces Triangle to finely mesh near corners
          for (j = 0; j < 3; j++) {
            if (j == 0) {
              a2 = a0 + dL * (a1 - a0) / abs(a1 - a0);
              node.x = a2.re;
              node.y = a2.im;
              l = (int)nodelst.GetSize();
              nodelst.Add(node);
              segm.n0 = linelist[i].n0;
              segm.n1 = l;
              linelst.Add(segm);
            }

            if (j == 1) {
              a2 = a1 + dL * (a0 - a1) / abs(a1 - a0);
              node.x = a2.re;
              node.y = a2.im;
              l = (int)nodelst.GetSize();
              nodelst.Add(node);
              segm.n0 = l - 1;
              segm.n1 = l;
              linelst.Add(segm);
            }

            if (j == 2) {
              l = (int)nodelst.GetSize() - 1;
              segm.n0 = l;
              segm.n1 = linelist[i].n1;
              linelst.Add(segm);
            }
          }
        }
      } else {
        for (j = 0; j < k; j++) {
          a2 = a0 + (a1 - a0) * ((double)(j + 1)) / ((double)k);
          node.x = a2.re;
          node.y = a2.im;
          if (j == 0) {
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = linelist[i].n0;
            segm.n1 = l;
            linelst.Add(segm);
          } else if (j == (k - 1)) {
            l = (int)nodelst.GetSize() - 1;
            segm.n0 = l;
            segm.n1 = linelist[i].n1;
            linelst.Add(segm);
          } else {
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = l - 1;
            segm.n1 = l;
            linelst.Add(segm);
          }
        }
      }
    }

  // discretize input arc segments
  for (i = 0; i < arclist.GetSize(); i++)
    if (arclist[i].IsSelected == FALSE) {
      a2.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
      k = (int)ceil(arclist[i].ArcLength / arclist[i].MaxSideLength);
      segm.BoundaryMarker = arclist[i].BoundaryMarker;
      segm.InConductor = arclist[i].InConductor;
      GetCircle(arclist[i], c, R);
      a1 = exp(I * arclist[i].ArcLength * PI / (((double)k) * 180.));

      if (k == 1) {
        segm.n0 = arclist[i].n0;
        segm.n1 = arclist[i].n1;
        linelst.Add(segm);
      } else
        for (j = 0; j < k; j++) {
          a2 = (a2 - c) * a1 + c;
          node.x = a2.re;
          node.y = a2.im;
          if (j == 0) {
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = arclist[i].n0;
            segm.n1 = l;
            linelst.Add(segm);
          } else if (j == (k - 1)) {
            l = (int)nodelst.GetSize() - 1;
            segm.n0 = l;
            segm.n1 = arclist[i].n1;
            linelst.Add(segm);
          } else {
            l = (int)nodelst.GetSize();
            nodelst.Add(node);
            segm.n0 = l - 1;
            segm.n1 = l;
            linelst.Add(segm);
          }
        }
    }

  // create correct output filename;
  pn = GetPathName();
  plyname = pn.Left(pn.ReverseFind('.')) + ".poly";

  // check to see if we are ready to write a datafile;

  if ((fp = fopen(plyname, "wt")) == NULL) {
    MsgBox("Couldn't write to specified .poly file");
    Undo();
    UnselectAll();
    return FALSE;
  }

  // write out node list
  fprintf(fp, "%i	2	0	1\n", nodelst.GetSize());
  for (i = 0; i < nodelst.GetSize(); i++) {
    // include boundary marker;
    for (j = 0, t = 0; j < nodeproplist.GetSize(); j++)
      if (nodeproplist[j].PointName == nodelst[i].BoundaryMarker)
        t = j + 2;

    // include conductor number;
    for (j = 0; j < circproplist.GetSize(); j++) {
      if (circproplist[j].CircName == nodelst[i].InConductor) {
        t += ((j + 1) * 0x10000);
        break;
      }
    }

    fprintf(fp, "%i	%.17g	%.17g	%i\n", i, nodelst[i].x, nodelst[i].y, t);
  }

  // write out segment list
  fprintf(fp, "%i	1\n", linelst.GetSize());
  for (i = 0; i < linelst.GetSize(); i++) {
    // include boundary marker;
    for (j = 0, t = 0; j < lineproplist.GetSize(); j++)
      if (lineproplist[j].BdryName == linelst[i].BoundaryMarker)
        t = -(j + 2);

    // include conductor number;
    for (j = 0; j < circproplist.GetSize(); j++) {
      if (circproplist[j].CircName == linelst[i].InConductor) {
        t -= ((j + 1) * 0x10000);
        break;
      }
    }

    fprintf(fp, "%i	%i	%i	%i\n", i, linelst[i].n0, linelst[i].n1, t);
  }

  // write out list of holes;
  for (i = 0, j = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>")
      j++;
  fprintf(fp, "%i\n", j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>") {
      fprintf(fp, "%i	%.17g	%.17g\n", k, blocklist[i].x, blocklist[i].y);
      k++;
    }

  // write out regional attributes
  fprintf(fp, "%i\n", blocklist.GetSize() - j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType != "<No Mesh>") {
      fprintf(fp, "%i	%.17g	%.17g	", k, blocklist[i].x, blocklist[i].y);
      fprintf(fp, "%i	", k + 1);
      if ((blocklist[i].MaxArea > 0) && (blocklist[i].MaxArea < DefaultMeshSize))
        fprintf(fp, "%.17g\n", blocklist[i].MaxArea);
      else
        fprintf(fp, "%.17g\n", DefaultMeshSize);
      k++;
    }
  fclose(fp);

  // Make sure to prune out any duplications in the ptlst
  for (k = 0; k < ptlst.GetSize(); k++)
    ptlst[k].Order();
  k = 0;
  while ((k + 1) < ptlst.GetSize()) {
    j = k + 1;
    while (j < ptlst.GetSize()) {
      if ((ptlst[k].x == ptlst[j].x) && (ptlst[k].y == ptlst[j].y))
        ptlst.RemoveAt(j);
      else
        j++;
    }
    k++;
  }

  // used to have a check to eliminate the case where a point
  // and its companion are the same point--actually, this shouldn't
  // be a problem just to let the algorithm deal with this
  // as usual.

  // One last error check--each point must have only one companion point.
  // however, it would be possible to screw up in the definition of the BCs
  // so that this isn't the case.  Look through the points to try and catch
  // this one.
  /*
    // let's let this check go away for a minute...

    for(k=0,n=FALSE;(k+1)<ptlst.GetSize();k++)
    {
      for(j=k+1;j<ptlst.GetSize();j++)
      {
        if(ptlst[k].x==ptlst[j].x) n=TRUE;
        if(ptlst[k].y==ptlst[j].y) n=TRUE;
        if(ptlst[k].x==ptlst[j].y) n=TRUE;
        if(ptlst[k].y==ptlst[j].x) n=TRUE;
      }
    }
    if (n==TRUE){
      MsgBox("Nonphysical (anti)periodic boundary assignments");
      Undo();  UnselectAll();
      return FALSE;
    }
  */
  // write out a pbc file containing a list of linked nodes
  plyname = pn.Left(pn.ReverseFind('.')) + ".pbc";
  if ((fp = fopen(plyname, "wt")) == NULL) {
    MsgBox("Couldn't write to specified .pbc file");
    Undo();
    UnselectAll();
    return FALSE;
  }
  fprintf(fp, "%i\n", ptlst.GetSize());
  for (k = 0; k < ptlst.GetSize(); k++)
    fprintf(fp, "%i	%i	%i	%i\n", k, ptlst[k].x, ptlst[k].y, ptlst[k].t);
  fclose(fp);

  // call triangle with -Y flag.

  rootname = "\"" + pn.Left(pn.ReverseFind('.')) + "\"";
  sprintf(CommandLine, "\"%striangle.exe\" -p -P -j -q%f -e -A -a -z -Q -I -Y %s",
      (const char*)BinDir, __min(MinAngle + MINANGLE_BUMP, MINANGLE_MAX), (const char*)rootname);

  StartupInfo.cb = sizeof(STARTUPINFO);
  StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
  StartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;
  if (CreateProcess(NULL, CommandLine, NULL, NULL, FALSE,
          0, NULL, NULL, &StartupInfo, &ProcessInfo)) {

    if (bLinehook == FALSE)
      WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    else {
      DWORD ExitCode;
      hProc = ProcessInfo.hProcess;
      do {
        GetExitCodeProcess(ProcessInfo.hProcess, &ExitCode);
        ((CFemmApp*)AfxGetApp())->line_hook(lua, NULL);
        Sleep(1);
      } while (ExitCode == STILL_ACTIVE);
      hProc = NULL;
    }

  } else {
    MsgBox("Couldn't spawn triangle.exe");
    Undo();
    UnselectAll();
    return FALSE;
  }

  GetExitCodeProcess(
      ProcessInfo.hProcess, // handle to the process
      &ExitCode // address to receive termination status
  );
  CloseHandle(ProcessInfo.hProcess);
  CloseHandle(ProcessInfo.hThread);
  if (ExitCode != 0) {
    MsgBox("Call to triangle was unsuccessful. Check for small angles.");
    Undo();
    UnselectAll();
    return FALSE;
  }

  UnselectAll();

  // Now restore boundary segment discretizations that have
  // been mucked up in the process...
  for (i = 0; i < linelist.GetSize(); i++)
    linelist[i] = undolinelist[i];
  for (i = 0; i < arclist.GetSize(); i++) {
    arclist[i].mySideLength = arclist[i].MaxSideLength;
    arclist[i].MaxSideLength = undoarclist[i].MaxSideLength;
  }

  // and save the latest version of the document to make sure
  // any changes to arc discretization get propagated into
  // the solution description....
  OnSaveDocument(pn);

  return TRUE;
}