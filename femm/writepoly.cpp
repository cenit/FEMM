// implementation of various incarnations of calls
// to triangle from the CFemmeDoc class

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "probdlg.h"
#include "PtProp.h"
#include "OpBlkDlg.h"
#include "OpNodeDlg.h"
#include "OpSegDlg.h"
#include "OpArcSegDlg.h"
#include "OpGrp.h"
#include "ArcDlg.h"
#include "process.h"

extern BOOL bLinehook;
extern lua_State* lua;
extern HANDLE hProc;

#define toDegrees(x) ((Im(x) >= 0) ? arg(x) : (arg(x) + 2. * PI)) * (180. / PI)

double CFemmeDoc::LineLength(int i)
{
  return abs(nodelist[linelist[i].n0].CC() - nodelist[linelist[i].n1].CC());
}

BOOL CFemmeDoc::HasPeriodicBC()
{
  BOOL flag = FALSE;
  int i, j, k;

  for (i = 0; i < lineproplist.GetSize(); i++) {
    if ((lineproplist[i].BdryFormat == 4) || (lineproplist[i].BdryFormat == 5) || (lineproplist[i].BdryFormat == 6) || (lineproplist[i].BdryFormat == 7))
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
      if ((lineproplist[k].BdryFormat == 4) || (lineproplist[k].BdryFormat == 5)) {
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
      if ((lineproplist[k].BdryFormat == 4) || (lineproplist[k].BdryFormat == 5) || (lineproplist[k].BdryFormat == 6) || (lineproplist[k].BdryFormat == 7)) {
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
BOOL CFemmeDoc::OnWritePoly()
{
  // if incremental permeability solution, we crib mesh from the previous problem.
  // we can just bail out in that case.
  if ((PrevSoln.GetLength() > 0) && (PrevType > 0))
    return TRUE;

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
    arclist[i].mySideLength = arclist[i].MaxSideLength; // in regular case, mySideLength = MaxSideLength
    a2.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
    k = (int)ceil(arclist[i].ArcLength / arclist[i].MaxSideLength);
    segm.BoundaryMarker = arclist[i].BoundaryMarker;
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
  fprintf(fp, "%i	2	0	1\n", (int)nodelst.GetSize());
  for (i = 0; i < nodelst.GetSize(); i++) {
    for (j = 0, t = 0; j < nodeproplist.GetSize(); j++)
      if (nodeproplist[j].PointName == nodelst[i].BoundaryMarker)
        t = j + 2;
    fprintf(fp, "%i	%.17g	%.17g	%i\n", i, nodelst[i].x, nodelst[i].y, t);
  }

  // write out segment list
  fprintf(fp, "%i	1\n", (int)linelst.GetSize());
  for (i = 0; i < linelst.GetSize(); i++) {
    for (j = 0, t = 0; j < lineproplist.GetSize(); j++)
      if (lineproplist[j].BdryName == linelst[i].BoundaryMarker)
        t = -(j + 2);
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
  fprintf(fp, "%i\n", (int)blocklist.GetSize() - j);
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
  fprintf(fp, "0\n0\n");
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
BOOL CFemmeDoc::FunnyOnWritePoly()
{
  // if incremental permeability solution, we crib mesh from the previous problem.
  // we can just bail out in that case.
  if ((PrevSoln.GetLength() > 0) && (Frequency > 0))
    return TRUE;

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
  CArray<CAirGapElement, CAirGapElement&> agelst;
  CArray<CCommonPoint, CCommonPoint&> ptlst;
  CNode node;
  CSegment segm;
  CPeriodicBoundary pbc;
  CAirGapElement age;
  CCommonPoint pt;
  int bSmartMesh = theApp.session_SmartMesh;
  if (bSmartMesh < 0)
    bSmartMesh = SmartMesh;

  nodelst.RemoveAll();
  linelst.RemoveAll();
  pbclst.RemoveAll();
  agelst.RemoveAll();
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
  fprintf(fp, "%i	2	0	1\n", (int)nodelst.GetSize());
  for (i = 0; i < nodelst.GetSize(); i++) {
    fprintf(fp, "%i	%.17g	%.17g	%i\n",
        i, nodelst[i].x, nodelst[i].y, 0);
  }

  // write out segment list
  fprintf(fp, "%i	1\n", (int)linelst.GetSize());
  for (i = 0; i < linelst.GetSize(); i++) {
    t = -(linelst[i].IsSelected + 2);
    fprintf(fp, "%i	%i	%i	%i\n", i, linelst[i].n0, linelst[i].n1, t);
  }

  // write out list of holes;
  for (i = 0, j = 0; i < blocklist.GetSize(); i++)
    if ((blocklist[i].BlockType == "<No Mesh>") || (blocklist[i].BlockType == "<Inf>"))
      j++;
  fprintf(fp, "%i\n", j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if ((blocklist[i].BlockType == "<No Mesh>") || (blocklist[i].BlockType == "<Inf>")) {
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
  fprintf(fp, "%i\n", (int)blocklist.GetSize() - j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if ((blocklist[i].BlockType != "<No Mesh>") && (blocklist[i].BlockType != "<Inf>")) {
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
      // its properties.  //!!!
      char kludge[32];
      double newMaxSideLength;
      newMaxSideLength = arclist[i].ArcLength / ((double)arclist[i].IsSelected);
      sprintf(kludge, "%.1e", newMaxSideLength);
      sscanf(kludge, "%lf", &newMaxSideLength);

      arclist[i].MaxSideLength = newMaxSideLength;
    }
  }

  ptlst.RemoveAll();

  // Search through defined bc's for pbcs and ages;
  // Allocate space to store their properties if they are detected
  for (i = 0; i < lineproplist.GetSize(); i++) {
    // pbc
    if ((lineproplist[i].BdryFormat == 4) || (lineproplist[i].BdryFormat == 5)) {
      pbc.BdryName = lineproplist[i].BdryName;
      pbc.BdryFormat = lineproplist[i].BdryFormat - 4; // 0 for pbc, 1 for apbc
      pbclst.Add(pbc);
    }

    // age
    if ((lineproplist[i].BdryFormat == 6) || (lineproplist[i].BdryFormat == 7)) {
      // only add an AGE to the list if it's actually being used
      for (j = 0, k = 0; j < arclist.GetSize(); j++)
        if (arclist[j].BoundaryMarker == lineproplist[i].BdryName)
          k++;
      if (k > 1) {
        age.BdryName = lineproplist[i].BdryName;
        age.BdryFormat = lineproplist[i].BdryFormat - 6; // 0 for pbc, 1 for apbc
        age.InnerAngle = lineproplist[i].InnerAngle;
        age.OuterAngle = lineproplist[i].OuterAngle;
        agelst.Add(age);
      }
    }
  }

  // make sure all Air Gap Element arcs have the same discretization
  // for each arc associated with a particular Air Gap Element...

  // find out the total arc length and arc elements
  // corresponding to each lineproplist entry
  for (i = 0; i < arclist.GetSize(); i++) {
    if (arclist[i].BoundaryMarker != "<None>") {
      for (j = 0; j < agelst.GetSize(); j++) {

        if (arclist[i].BoundaryMarker == agelst[j].BdryName) {
          agelst[j].totalArcLength += arclist[i].ArcLength;
          agelst[j].totalArcElements += arclist[i].IsSelected;

          GetCircle(arclist[i], agelst[j].agc, R);
          if (agelst[j].ro == 0) {
            agelst[j].ri = R;
            agelst[j].ro = R;
          }
          if (R > agelst[j].ro)
            agelst[j].ro = R;
          if (R < agelst[j].ri)
            agelst[j].ri = R;

          break;
        }
      }
    }
  }

  // cycle through AGEs and fix constituent arcs so that all arcs have the same discretization
  for (i = 0; i < agelst.GetSize(); i++) {
    if (agelst[i].totalArcLength > 0) // if the AGE is actually in play
    {
      char kludge[32];
      double myMaxSideLength, altMaxSideLength;

      myMaxSideLength = agelst[i].totalArcLength / agelst[i].totalArcElements;
      agelst[i].totalArcLength /= 2; // this is now the angle spanned by the AGE

      // however, don't want long, skinny air gap elements.  Impose some limits
      // based on the inner and outer radii;
      altMaxSideLength = (360. / PI) * (agelst[i].ro - agelst[i].ri) / (agelst[i].ro + agelst[i].ri);
      if (altMaxSideLength < myMaxSideLength)
        myMaxSideLength = altMaxSideLength;
      sprintf(kludge, "%.1e", myMaxSideLength);
      sscanf(kludge, "%lf", &myMaxSideLength);

      // apply new side length to all arcs in this AGE
      for (j = 0; j < arclist.GetSize(); j++)
        if (arclist[j].BoundaryMarker == agelst[i].BdryName)
          arclist[j].MaxSideLength = myMaxSideLength;
    }
  }

  // and perform a quick error check; AGE BCs can't be applied to segments (at least yet)
  for (i = 0; i < linelist.GetSize(); i++) {
    if (linelist[i].BoundaryMarker != "<None>") {
      for (j = 0; j < agelst.GetSize(); j++) {

        if (linelist[i].BoundaryMarker == agelst[j].BdryName) {
          MsgBox("Can't apply Air Gap Element BCs to line segments");
          Undo();
          UnselectAll();
          return FALSE;
        }
      }
    }
  }

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

  // Now, discretize arcs that are part of an AGE
  for (n = 0; n < agelst.GetSize(); n++) {
    CArray<int, int> myVector;
    myVector.RemoveAll();

    z = (agelst[n].ro + agelst[n].ri) / 2.;

    for (i = 0; i < arclist.GetSize(); i++)
      if ((arclist[i].IsSelected == FALSE) && (arclist[i].BoundaryMarker == agelst[n].BdryName)) {
        arclist[i].IsSelected = TRUE;
        a2.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
        k = (int)ceil(arclist[i].ArcLength / arclist[i].MaxSideLength);
        segm.BoundaryMarker = arclist[i].BoundaryMarker;
        GetCircle(arclist[i], c, R);
        a1 = exp(I * arclist[i].ArcLength * PI / (((double)k) * 180.));

        // insert the starting node
        if (R > z) // on outer radius
          myVector.Add(arclist[i].n0);
        else // on inner radius
          myVector.InsertAt(0, arclist[i].n0);

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

              // insert newly created node
              if (R > z) // on outer radius
                myVector.Add(l);
              else // on inner radius
                myVector.InsertAt(0, l);
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

              // insert newly created node
              if (R > z) // on outer radius
                myVector.Add(l);
              else // on inner radius
                myVector.InsertAt(0, l);
            }
          }
      }

    agelst[n].node = (int*)calloc(myVector.GetSize() + 1, sizeof(int));
    agelst[n].node[0] = (int)myVector.GetSize();
    for (k = 0; k < myVector.GetSize(); k++)
      agelst[n].node[k + 1] = myVector[k];
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
  fprintf(fp, "%i	2	0	1\n", (int)nodelst.GetSize());
  for (i = 0; i < nodelst.GetSize(); i++) {
    for (j = 0, t = 0; j < nodeproplist.GetSize(); j++)
      if (nodeproplist[j].PointName == nodelst[i].BoundaryMarker)
        t = j + 2;
    fprintf(fp, "%i	%.17g	%.17g	%i\n", i, nodelst[i].x, nodelst[i].y, t);
  }

  // write out segment list
  fprintf(fp, "%i	1\n", (int)linelst.GetSize());
  for (i = 0; i < linelst.GetSize(); i++) {
    for (j = 0, t = 0; j < lineproplist.GetSize(); j++)
      if (lineproplist[j].BdryName == linelst[i].BoundaryMarker)
        t = -(j + 2);
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
  fprintf(fp, "%i\n", (int)blocklist.GetSize() - j);
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

  // write out a pbc file containing a list of linked nodes and air gap element info
  plyname = pn.Left(pn.ReverseFind('.')) + ".pbc";
  if ((fp = fopen(plyname, "wt")) == NULL) {
    MsgBox("Couldn't write to specified .pbc file");
    Undo();
    UnselectAll();
    return FALSE;
  }
  fprintf(fp, "%i\n", (int)ptlst.GetSize());
  for (k = 0; k < ptlst.GetSize(); k++)
    fprintf(fp, "%i	%i	%i	%i\n", k, ptlst[k].x, ptlst[k].y, ptlst[k].t);

  fprintf(fp, "%i\n", (int)agelst.GetSize());
  for (k = 0; k < agelst.GetSize(); k++) {
    double dtta;
    CArray<CQuadPoint, CQuadPoint&> InnerRing;
    CArray<CQuadPoint, CQuadPoint&> OuterRing;
    InnerRing.RemoveAll();
    OuterRing.RemoveAll();

    n = agelst[k].node[0] / 2;
    dtta = agelst[k].totalArcLength / n;
    n0 = (int)round(360. / dtta); // total elements in a 360deg annular ring;
    n1 = (int)round(360. / agelst[k].totalArcLength); // number of copied segments

    // Should do some consistency checking here;
    //   totalArcLength*n1 should equal 360
    //   no*dtta should equal 360
    //   if antiperiodic, n1 should be an even number
    //   otherwise, throw error message, clean up, and return

    InnerRing.SetSize(n0);
    OuterRing.SetSize(n0);

    // map each bdry point onto points on the ring;
    int kk;
    for (j = 0, kk = 0; j < n1; j++) // do each slice
    {
      if ((agelst[k].BdryFormat == 1) && (j % 2 != 0))
        dL = -1; // antiperiodic
      else
        dL = 1;

      a1 = exp(I * (j * agelst[k].totalArcLength + agelst[k].InnerAngle) * DEGREE);
      a2 = exp(I * (j * agelst[k].totalArcLength + agelst[k].OuterAngle) * DEGREE);
      for (i = 1; i <= n; i++) {
        a0 = a1 * (nodelst[agelst[k].node[i]].CC() - agelst[k].agc); // position of the shifted mesh node
        z = toDegrees(a0) / dtta;

        InnerRing[kk].n0 = agelst[k].node[i];
        InnerRing[kk].w0 = z;
        InnerRing[kk].w1 = dL;

        a0 = a2 * (nodelst[agelst[k].node[i + n]].CC() - agelst[k].agc); // position of the shifted mesh node
        z = toDegrees(a0) / dtta;

        OuterRing[kk].n0 = agelst[k].node[i + n];
        OuterRing[kk].w0 = z;
        OuterRing[kk].w1 = dL;

        kk++;
      }
    }

    // InnerRing and OuterRing contain a list of boundary nodes, but the aren't yet properly sorted.
    // Sort out the rings based on the angle of the points in the ring
    for (int ii = 0; ii < n0; ii++) {
      int bDone = 1;
      CQuadPoint qq;

      for (int jj = 0; jj < (n0 - 1); jj++) {
        if (InnerRing[jj].w0 > InnerRing[jj + 1].w0) {
          qq = InnerRing[jj];
          InnerRing[jj] = InnerRing[jj + 1];
          InnerRing[jj + 1] = qq;
          bDone = 0;
        }
      }
      if (bDone)
        break;
    }

    for (int ii = 0; ii < n0; ii++) {
      int bDone = 1;
      CQuadPoint qq;

      for (int jj = 0; jj < (n0 - 1); jj++) {
        if (OuterRing[jj].w0 > OuterRing[jj + 1].w0) {
          qq = OuterRing[jj];
          OuterRing[jj] = OuterRing[jj + 1];
          OuterRing[jj + 1] = qq;
          bDone = 0;
        }
      }
      if (bDone)
        break;
    }

    // print out AGE definition
    fprintf(fp, "\"%s\"\n", agelst[k].BdryName.GetString());
    fprintf(fp, "%i %.17g %.17g %.17g %.17g %.17g %.17g %.17g %i %.17g %.17g\n",
        agelst[k].BdryFormat, agelst[k].InnerAngle, agelst[k].OuterAngle,
        agelst[k].ri, agelst[k].ro, agelst[k].totalArcLength,
        Re(agelst[k].agc), Im(agelst[k].agc), n,
        InnerRing[0].w0, OuterRing[0].w0);

    for (i = 0; i <= n; i++) {
      int p0, p1;

      p1 = i;
      if (p1 == n0)
        p1 = 0;
      p0 = p1 - 1;
      if (p0 < 0)
        p0 = n0 + p0;

      // ring points that bracket points in the annulus mesh
      // and their sign, for the purposes of periodicity/antiperiodicity
      fprintf(fp, "%i %g %i %g %i %g %i %g\n",
          InnerRing[p0].n0, InnerRing[p0].w1,
          InnerRing[p1].n0, InnerRing[p1].w1,
          OuterRing[p0].n0, OuterRing[p0].w1,
          OuterRing[p1].n0, OuterRing[p1].w1);
    }

    /*
        fprintf(fp,"%s\n",agelst[k].BdryName);
        fprintf(fp,"%i %.17g %.17g %.17g %.17g %.17g %.17g %.17g %i\n",
          agelst[k].BdryFormat,agelst[k].InnerAngle,agelst[k].OuterAngle,
          agelst[k].ri,agelst[k].ro,agelst[k].totalArcLength,
          Re(agelst[k].agc),Im(agelst[k].agc),n);
        for(i=1;i<=n;i++)
          fprintf(fp,"%i %i\n",agelst[k].node[i],agelst[k].node[n+i]); */
  }

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
    MsgBox("Call to triangle was unsuccessful");
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