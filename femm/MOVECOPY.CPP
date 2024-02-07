// femmeDoc.cpp : implementation of the CFemmeDoc class (continued...)
//

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "femmeView.h"
#include "probdlg.h"
#include "PtProp.h"
#include "OpBlkDlg.h"
#include "OpNodeDlg.h"
#include "OpSegDlg.h"
#include "OpArcSegDlg.h"
#include "ArcDlg.h"
#include "DXFImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL bLinehook;

//-----------------------------------------------------------------

BOOL CFemmeDoc::ReadDXF(CString fname, double DefTol)
{
  FILE* fp;
  int j, k;
  CArray<CString, CString&> layerlist;
  CString myLayer;

  if ((fp = fopen(fname, "rt")) == NULL)
    return FALSE;
  NoDraw = TRUE;

  // clear out any old drawing that we might have;
  //	nodelist.RemoveAll();
  //	linelist.RemoveAll();
  //	arclist.RemoveAll();
  //	blocklist.RemoveAll();
  //	undonodelist.RemoveAll();
  //	undolinelist.RemoveAll();
  //	undoarclist.RemoveAll();
  //	undoblocklist.RemoveAll();
  //	nodeproplist.RemoveAll();
  //	lineproplist.RemoveAll();
  //	blockproplist.RemoveAll();
  //	circproplist.RemoveAll();
  //	meshnode.RemoveAll();
  //	meshline.RemoveAll();
  //	greymeshline.RemoveAll();

  char s[256];
  char v[256];
  BOOL PolylineFlag = FALSE;
  CNode n[4];
  CSegment segm;
  CArcSegment asegm;
  CComplex c, p, q;
  double R, a0, a1;
  int xx, PolyLineClosed, firstpoint;
  double angle = 0;

  while (fgets(s, 256, fp) != NULL) {
    if (strncmp(s, "LAYER", 5) == 0) {
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 2) {
          sscanf(v, "%s", s);
          myLayer = s;
          layerlist.Add(myLayer);
        }
      } while (1 > 0);
    }

    if (strncmp(s, "POINT", 5) == 0) {
      xx = 0;
      n[0].InGroup = 0;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 10) {
          n[0].x = atof(v);
          xx = xx | 1;
        }
        if (k == 20) {
          n[0].y = atof(v);
          xx = xx | 2;
        }
        if (k == 8) {
          sscanf(v, "%s", s);
          myLayer = s;
          for (j = 0; j < layerlist.GetSize(); j++)
            if (myLayer == layerlist[j])
              n[0].InGroup = j;
        }
      } while (1 > 0);
      if (xx == 3)
        nodelist.Add(n[0]);
    }

    if (strncmp(s, "LWPOLYLINE", 10) == 0) {
      int segs = 0;
      PolyLineClosed = FALSE;
      firstpoint = -1;
      angle = 0;
      n[0].InGroup = 0;
      segm.InGroup = 0;
      asegm.InGroup = 0;
      int n0, n1;
      xx = 0;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 8) {
          sscanf(v, "%s", s);
          myLayer = s;
          for (j = 0; j < layerlist.GetSize(); j++)
            if (myLayer == layerlist[j]) {
              n[0].InGroup = j;
              segm.InGroup = j;
              asegm.InGroup = j;
            }
        }
        if (k == 10) {
          n[0].x = atof(v);
          xx = xx | 1;
        }
        if (k == 20) {
          n[0].y = atof(v);
          xx = xx | 2;
        }
        if (k == 42) {
          angle = 720. * atan(atof(v)) / PI;
          if (angle < -360.)
            angle = -360.;
          if (angle > 360.)
            angle = 360.;
        }
        if (k == 70)
          PolyLineClosed = TRUE;
        if (xx == 3) {
          j = (int)nodelist.GetSize();
          nodelist.Add(n[0]);
          xx = 0;
          if (segs == 0)
            firstpoint = j;
          else {
            if (angle == 0) {
              segm.n0 = j;
              segm.n1 = j - 1;
              linelist.Add(segm);
            } else {
              if (angle > 0) {
                n0 = j - 1;
                n1 = j;
              } else {
                n0 = j;
                n1 = j - 1;
                angle = fabs(angle);
              }
              if (angle > 180.) {
                CComplex p0, p1, p2;
                angle /= 2.;
                p0 = nodelist[n0].x + I * nodelist[n0].y;
                p1 = nodelist[n1].x + I * nodelist[n1].y;
                p2 = (p0 + p1) / 2. - I * (1. - cos(angle * PI / 180.)) * (p1 - p0) / (2. * sin(angle * PI / 180.));
                n[0].x = Re(p2);
                n[0].y = Im(p2);
                nodelist.InsertAt(j, n[0]);
                asegm.ArcLength = angle;
                asegm.MaxSideLength = 5.;
                if (n0 < n1) {
                  asegm.n0 = n0;
                  asegm.n1 = j;
                } else {
                  asegm.n0 = j;
                  asegm.n1 = n1;
                }
                arclist.Add(asegm);
                if (n1 == j) {
                  n1++;
                  n0 = j;
                } else {
                  n0++;
                  n1 = j;
                }
              }
              asegm.n0 = n0;
              asegm.n1 = n1;
              asegm.ArcLength = angle;
              asegm.MaxSideLength = 5.;
              arclist.Add(asegm);
              angle = 0;
            }
          }
          segs++;
        }
      } while (1 > 0);
      if ((PolyLineClosed == TRUE) && (firstpoint >= 0) && (segs > 0)) {
        // take care of closing the contour, if required.
        j = (int)nodelist.GetSize() - 1;
        if (angle == 0) {
          segm.n0 = j;
          segm.n1 = firstpoint;
          linelist.Add(segm);
        } else {
          if (angle > 0) {
            n0 = j;
            n1 = firstpoint;
          } else {
            n0 = firstpoint;
            n1 = j;
            angle = fabs(angle);
          }
          if (angle > 180.) {
            CComplex p0, p1, p2;
            angle /= 2.;
            p0 = nodelist[n0].x + I * nodelist[n0].y;
            p1 = nodelist[n1].x + I * nodelist[n1].y;
            p2 = (p0 + p1) / 2. - I * (1. - cos(angle * PI / 180.)) * (p1 - p0) / (2. * sin(angle * PI / 180.));
            n[0].x = Re(p2);
            n[0].y = Im(p2);
            nodelist.Add(n[0]);
            j++;
            asegm.ArcLength = angle;
            asegm.MaxSideLength = 5.;
            if (n0 > n1) {
              asegm.n0 = n0;
              asegm.n1 = j;
            } else {
              asegm.n0 = j;
              asegm.n1 = n1;
            }
            arclist.Add(asegm);

            if (n1 == j - 1)
              n1 = j;
            else
              n0 = j;
          }
          asegm.n0 = n0;
          asegm.n1 = n1;
          asegm.ArcLength = angle;
          asegm.MaxSideLength = 5.;
          arclist.Add(asegm);
          angle = 0;
        }
      }
    }

    if (strncmp(s, "POLYLINE", 8) == 0) {
      PolylineFlag = -1;
      firstpoint = -1;
      PolyLineClosed = FALSE;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 70) {
          j = atoi(v);
          if (j == 1)
            PolyLineClosed = TRUE;
        }
      } while (1 > 0);
    }

    if (strncmp(s, "SEQEND", 6) == 0) {
      PolylineFlag = FALSE;
      int j = ((int)nodelist.GetSize()) - 1;
      if ((PolyLineClosed == TRUE) && (firstpoint >= 0)) {
        if (angle == 0.0) {
          segm.n0 = j;
          segm.n1 = firstpoint;
          linelist.Add(segm);
        } else {
          int n0, n1;
          if (angle > 0) {
            n0 = j;
            n1 = firstpoint;
          } else {
            n0 = firstpoint;
            n1 = j;
            angle = fabs(angle);
          }
          if (angle > 180.) {
            CComplex p0, p1, p2;
            angle /= 2.;
            p0 = nodelist[n0].x + I * nodelist[n0].y;
            p1 = nodelist[n1].x + I * nodelist[n1].y;
            p2 = (p0 + p1) / 2. - I * (1. - cos(angle * PI / 180.)) * (p1 - p0) / (2. * sin(angle * PI / 180.));
            n[0].x = Re(p2);
            n[0].y = Im(p2);
            nodelist.Add(n[0]);
            j++;
            asegm.ArcLength = angle;
            asegm.MaxSideLength = 5.;
            if (n0 > n1) {
              asegm.n0 = n0;
              asegm.n1 = j;
            } else {
              asegm.n0 = j;
              asegm.n1 = n1;
            }
            arclist.Add(asegm);

            if (n1 == j - 1)
              n1 = j;
            else
              n0 = j;
          }
          asegm.n0 = n0;
          asegm.n1 = n1;
          asegm.ArcLength = angle;
          asegm.MaxSideLength = 5.;
          arclist.Add(asegm);
        }
      }
      PolyLineClosed = FALSE;
      angle = 0.0;
    }

    if (strncmp(s, "VERTEX", 6) == 0) {
      n[0].InGroup = 0;
      segm.InGroup = 0;
      asegm.InGroup = 0;
      double next_angle = 0.0;
      xx = 0;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 8) {
          sscanf(v, "%s", s);
          myLayer = s;
          for (j = 0; j < layerlist.GetSize(); j++)
            if (myLayer == layerlist[j]) {
              n[0].InGroup = j;
              segm.InGroup = j;
              asegm.InGroup = j;
            }
        }
        if (k == 10) {
          n[0].x = atof(v);
          xx |= 1 << 0;
        }
        if (k == 20) {
          n[0].y = atof(v);
          xx |= 1 << 1;
        }
        if (k == 42) {
          // Bulge argument for arc segments within a POLYLINE
          // The bulge applies to the line connecting this VERTEX and the *next* one.
          next_angle = 720.0 * atan(atof(v)) / PI;
          if (next_angle < -360.)
            next_angle = -360.;
          if (next_angle > 360.)
            next_angle = 360.;
        }
      } while (1 > 0);

      if (xx == 3) {
        nodelist.Add(n[0]);
        if (angle != 0.0) {
          j = (int)nodelist.GetSize();

          int n0, n1;
          if (angle > 0) {
            n0 = j - 2;
            n1 = j - 1;
          } else {
            n0 = j - 1;
            n1 = j - 2;
            angle = fabs(angle);
          }

          if (angle > 180.) {
            CComplex p0, p1, p2;
            angle /= 2.;
            p0 = nodelist[n0].x + I * nodelist[n0].y;
            p1 = nodelist[n1].x + I * nodelist[n1].y;
            p2 = (p0 + p1) / 2. - I * (1. - cos(angle * PI / 180.)) * (p1 - p0) / (2. * sin(angle * PI / 180.));
            n[0].x = Re(p2);
            n[0].y = Im(p2);
            nodelist.InsertAt(j, n[0]);
            asegm.ArcLength = angle;
            asegm.MaxSideLength = 5.;
            if (n0 < n1) {
              asegm.n0 = n0;
              asegm.n1 = j;
            } else {
              asegm.n0 = j;
              asegm.n1 = n1;
            }
            arclist.Add(asegm);
            if (n1 == j) {
              n1++;
              n0 = j;
            } else {
              n0++;
              n1 = j;
            }
          }
          asegm.n0 = n0;
          asegm.n1 = n1;
          asegm.ArcLength = angle;
          asegm.MaxSideLength = 5.;
          arclist.Add(asegm);
        } else {
          if (PolylineFlag == TRUE) {
            j = (int)nodelist.GetSize();
            segm.n0 = j - 2;
            segm.n1 = j - 1;
            linelist.Add(segm);
          }
          if (PolylineFlag == -1) {
            firstpoint = ((int)nodelist.GetSize()) - 1;
            PolylineFlag = TRUE;
          }
        }
      }
      angle = next_angle;
    }

    if (strncmp(s, "LINE", 4) == 0) {
      n[0].InGroup = 0;
      n[1].InGroup = 0;
      segm.InGroup = 0;
      xx = 0;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 8) {
          sscanf(v, "%s", s);
          myLayer = s;
          for (j = 0; j < layerlist.GetSize(); j++)
            if (myLayer == layerlist[j]) {
              n[0].InGroup = j;
              n[1].InGroup = j;
              segm.InGroup = j;
            }
        }
        if (k == 10) {
          n[0].x = atof(v);
          xx = xx | 1;
        }
        if (k == 20) {
          n[0].y = atof(v);
          xx = xx | 2;
        }
        if (k == 11) {
          n[1].x = atof(v);
          xx = xx | 4;
        }
        if (k == 21) {
          n[1].y = atof(v);
          xx = xx | 8;
        }
      } while (1 > 0);

      if (xx == 15) {
        j = (int)nodelist.GetSize();
        nodelist.Add(n[0]);
        nodelist.Add(n[1]);
        segm.n0 = j;
        segm.n1 = j + 1;
        linelist.Add(segm);
      }
    }

    // catch ARCALIGNEDTEXT, which derails the ARC code...
    if (strncmp(s, "ARCA", 4) == 0)
      s[0] = NULL;

    if (strncmp(s, "ARC", 3) == 0) {
      n[0].InGroup = 0;
      n[1].InGroup = 0;
      asegm.InGroup = 0;
      xx = 0;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 8) {
          sscanf(v, "%s", s);
          myLayer = s;
          for (j = 0; j < layerlist.GetSize(); j++)
            if (myLayer == layerlist[j]) {
              n[0].InGroup = j;
              n[1].InGroup = j;
              asegm.InGroup = j;
            }
        }
        if (k == 10) {
          c.re = atof(v);
          xx = xx | 1;
        }
        if (k == 20) {
          c.im = atof(v);
          xx = xx | 2;
        }
        if (k == 40) {
          R = atof(v);
          xx = xx | 4;
        }
        if (k == 50) {
          a0 = atof(v);
          xx = xx | 8;
        }
        if (k == 51) {
          a1 = atof(v);
          xx = xx | 16;
        }
      } while (1 > 0);

      if (xx == 31) {
        if (a1 < a0)
          a1 += 360.;

        if ((a1 - a0) <= 180.) {
          p = R * exp(I * PI * a0 / 180.) + c;
          q = R * exp(I * PI * a1 / 180.) + c;
          n[0].x = p.re;
          n[1].x = q.re;
          n[0].y = p.im;
          n[1].y = q.im;
          j = (int)nodelist.GetSize();
          nodelist.Add(n[0]);
          nodelist.Add(n[1]);
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.MaxSideLength = 5.;
          asegm.ArcLength = (a1 - a0);
          arclist.Add(asegm);
        } else {
          p = R * exp(I * PI * a0 / 180.) + c;
          q = R * exp(I * PI * (a1 + a0) / 360.) + c;
          n[0].x = p.re;
          n[1].x = q.re;
          n[0].y = p.im;
          n[1].y = q.im;
          j = (int)nodelist.GetSize();
          nodelist.Add(n[0]);
          nodelist.Add(n[1]);
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.MaxSideLength = 5.;
          asegm.ArcLength = (a1 - a0) / 2.;
          arclist.Add(asegm);

          p = q;
          q = R * exp(I * PI * a1 / 180.) + c;
          n[0].x = p.re;
          n[1].x = q.re;
          n[0].y = p.im;
          n[1].y = q.im;
          j = (int)nodelist.GetSize();
          nodelist.Add(n[0]);
          nodelist.Add(n[1]);
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.MaxSideLength = 5.;
          asegm.ArcLength = (a1 - a0) / 2.;
          arclist.Add(asegm);
        }
      }
    }

    if (strncmp(s, "CIRCLE", 6) == 0) {
      n[0].InGroup = 0;
      n[0].InGroup = 0;
      asegm.InGroup = 0;
      xx = 0;
      do {
        if (fgets(s, 256, fp) == NULL)
          break;
        k = atoi(s);
        if (k == 0)
          break;
        fgets(v, 256, fp);
        if (k == 8) {
          sscanf(v, "%s", s);
          myLayer = s;
          for (j = 0; j < layerlist.GetSize(); j++)
            if (myLayer == layerlist[j]) {
              n[0].InGroup = j;
              n[1].InGroup = j;
              asegm.InGroup = j;
            }
        }
        if (k == 10) {
          c.re = atof(v);
          xx = xx | 1;
        }
        if (k == 20) {
          c.im = atof(v);
          xx = xx | 2;
        }
        if (k == 40) {
          R = atof(v);
          xx = xx | 4;
        }
      } while (1 > 0);

      if (xx == 7) {
        n[0].x = c.re + R;
        n[1].x = c.re - R;
        n[0].y = c.im;
        n[1].y = c.im;
        j = (int)nodelist.GetSize();
        nodelist.Add(n[0]);
        nodelist.Add(n[1]);
        asegm.n0 = j;
        asegm.n1 = j + 1;
        asegm.MaxSideLength = 5.;
        asegm.ArcLength = 180.;
        arclist.Add(asegm);
        asegm.n1 = j;
        asegm.n0 = j + 1;
        arclist.Add(asegm);
      }
    }
  }

  fclose(fp);

  // could be that nothing actually got read.  We want to
  // catch this case and tell the user about it.
  if (nodelist.GetSize() == 0)
    return FALSE;

  // suggest the proper tolerance
  CComplex p0, p1;
  CDXFImport dlg;
  int i;

  p0 = nodelist[0].CC();
  p1 = p0;
  for (i = 1; i < nodelist.GetSize(); i++) {
    if (nodelist[i].x < p0.re)
      p0.re = nodelist[i].x;
    if (nodelist[i].x > p1.re)
      p1.re = nodelist[i].x;
    if (nodelist[i].y < p0.im)
      p0.im = nodelist[i].y;
    if (nodelist[i].y > p1.im)
      p1.im = nodelist[i].y;
  }

  R = abs(p1 - p0) * 1.e-04;
  dlg.m_dxftol = floor(R / pow(10., floor(log10(R)))) * pow(10., floor(log10(R)));

  if (DefTol < 0) {
    dlg.DoModal();
    FancyEnforcePSLG(dlg.m_dxftol);
  } else {
    if (DefTol == 0)
      DefTol = dlg.m_dxftol;
    FancyEnforcePSLG(DefTol);
  }

  NoDraw = FALSE;

  return TRUE;
}

void CFemmeDoc::RotateMove(CComplex c, double t, int EditAction)
{
  int i, j;
  CComplex x, z;

  z = exp(I * t * PI / 180);

  if (EditAction == 0) {
    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        x.Set(nodelist[i].x, nodelist[i].y);
        x = (x - c) * z + c;
        nodelist[i].x = x.re;
        nodelist[i].y = x.im;
      }
    }
    EnforcePSLG();
    return;
  }

  if (EditAction == 1) {
    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {
        nodelist[linelist[i].n0].IsSelected = TRUE;
        nodelist[linelist[i].n1].IsSelected = TRUE;
      }
    }
    RotateMove(c, t, 0);
    return;
  }

  if (EditAction == 2) {
    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        x.Set(blocklist[i].x, blocklist[i].y);
        x = (x - c) * z + c;
        blocklist[i].x = x.re;
        blocklist[i].y = x.im;

        for (j = 0; j < blockproplist.GetSize(); j++)
          if (blockproplist[j].BlockName == blocklist[i].BlockType)
            if (blockproplist[j].H_c != 0) {
              blocklist[i].MagDir += t;
              break;
            }
      }
    }
    EnforcePSLG();
    return;
  }

  if (EditAction == 3) {
    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        nodelist[arclist[i].n0].IsSelected = TRUE;
        nodelist[arclist[i].n1].IsSelected = TRUE;
      }
    }
    RotateMove(c, t, 0);
    return;
  }

  if (EditAction == 4) {

    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {
        nodelist[linelist[i].n0].IsSelected = TRUE;
        nodelist[linelist[i].n1].IsSelected = TRUE;
      }
    }

    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        nodelist[arclist[i].n0].IsSelected = TRUE;
        nodelist[arclist[i].n1].IsSelected = TRUE;
      }
    }

    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        x.Set(nodelist[i].x, nodelist[i].y);
        x = (x - c) * z + c;
        nodelist[i].x = x.re;
        nodelist[i].y = x.im;
      }
    }

    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        x.Set(blocklist[i].x, blocklist[i].y);
        x = (x - c) * z + c;
        blocklist[i].x = x.re;
        blocklist[i].y = x.im;

        for (j = 0; j < blockproplist.GetSize(); j++)
          if (blockproplist[j].BlockName == blocklist[i].BlockType)
            if (blockproplist[j].H_c != 0) {
              blocklist[i].MagDir += t;
              break;
            }
      }
    }

    EnforcePSLG();
    return;
  }
}

void CFemmeDoc::TranslateMove(double dx, double dy, int EditAction)
{
  int i;

  if (EditAction == 0) {
    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        nodelist[i].x += dx;
        nodelist[i].y += dy;
      }
    }
    EnforcePSLG();
    return;
  }

  if (EditAction == 1) {
    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {
        nodelist[linelist[i].n0].IsSelected = TRUE;
        nodelist[linelist[i].n1].IsSelected = TRUE;
      }
    }
    TranslateMove(dx, dy, 0);
    return;
  }

  if (EditAction == 2) {
    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        blocklist[i].x += dx;
        blocklist[i].y += dy;
      }
    }
    EnforcePSLG();
    return;
  }

  if (EditAction == 3) {
    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        nodelist[arclist[i].n0].IsSelected = TRUE;
        nodelist[arclist[i].n1].IsSelected = TRUE;
      }
    }
    TranslateMove(dx, dy, 0);
    return;
  }

  if (EditAction == 4) {
    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {
        nodelist[linelist[i].n0].IsSelected = TRUE;
        nodelist[linelist[i].n1].IsSelected = TRUE;
      }
    }

    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        nodelist[arclist[i].n0].IsSelected = TRUE;
        nodelist[arclist[i].n1].IsSelected = TRUE;
      }
    }

    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        nodelist[i].x += dx;
        nodelist[i].y += dy;
      }
    }

    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        blocklist[i].x += dx;
        blocklist[i].y += dy;
      }
    }

    EnforcePSLG();
    return;
  }
}

void CFemmeDoc::RotateCopy(CComplex c, double dt, int ncopies, int EditAction)
{
  int i, j, k, nc;
  CComplex x, z;
  double t;

  for (nc = 0; nc < ncopies; nc++) {
    t = ((double)(nc + 1)) * dt;

    z = exp(I * t * PI / 180);

    if (EditAction == 0) {
      CNode node;
      k = (int)nodelist.GetSize();
      for (i = 0; i < k; i++) {
        if (nodelist[i].IsSelected == TRUE) {
          x.Set(nodelist[i].x, nodelist[i].y);
          node = nodelist[i];
          x = (x - c) * z + c;
          node.x = x.re;
          node.y = x.im;
          node.IsSelected = FALSE;
          nodelist.Add(node);
        }
      }
    }

    if (EditAction == 1) {
      CSegment segm;
      CNode n0, n1;
      k = (int)linelist.GetSize();
      for (i = 0; i < k; i++) {
        if (linelist[i].IsSelected == TRUE) {
          n0 = nodelist[linelist[i].n0];
          n1 = nodelist[linelist[i].n1];

          x.Set(n0.x, n0.y);
          x = (x - c) * z + c;
          n0.x = x.re;
          n0.y = x.im;

          x.Set(n1.x, n1.y);
          x = (x - c) * z + c;
          n1.x = x.re;
          n1.y = x.im;

          j = (int)nodelist.GetSize();
          segm = linelist[i];
          segm.n0 = j;
          segm.n1 = j + 1;
          segm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          linelist.Add(segm);
        }
      }
    }

    if (EditAction == 2) {
      CBlockLabel lbl;
      k = (int)blocklist.GetSize();
      for (i = 0; i < k; i++) {
        if (blocklist[i].IsSelected == TRUE) {
          x.Set(blocklist[i].x, blocklist[i].y);
          lbl = blocklist[i];
          x = (x - c) * z + c;
          lbl.x = x.re;
          lbl.y = x.im;
          lbl.IsSelected = FALSE;
          for (j = 0; j < blockproplist.GetSize(); j++)
            if (blockproplist[j].BlockName == blocklist[i].BlockType)
              if (blockproplist[j].H_c != 0) {
                lbl.MagDir += t;
                break;
              }

          blocklist.Add(lbl);
        }
      }
    }

    if (EditAction == 3) {
      CArcSegment asegm;
      CNode n0, n1;
      k = (int)arclist.GetSize();
      for (i = 0; i < k; i++) {
        if (arclist[i].IsSelected == TRUE) {
          n0 = nodelist[arclist[i].n0];
          n1 = nodelist[arclist[i].n1];

          x.Set(n0.x, n0.y);
          x = (x - c) * z + c;
          n0.x = x.re;
          n0.y = x.im;

          x.Set(n1.x, n1.y);
          x = (x - c) * z + c;
          n1.x = x.re;
          n1.y = x.im;

          j = (int)nodelist.GetSize();
          asegm = arclist[i];
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          arclist.Add(asegm);
        }
      }
    }

    if (EditAction == 4) {

      // copy selected nodes;
      CNode node;
      k = (int)nodelist.GetSize();
      for (i = 0; i < k; i++) {
        if (nodelist[i].IsSelected == TRUE) {
          x.Set(nodelist[i].x, nodelist[i].y);
          node = nodelist[i];
          x = (x - c) * z + c;
          node.x = x.re;
          node.y = x.im;
          node.IsSelected = FALSE;
          nodelist.Add(node);
        }
      }

      // copy selected segments;
      CSegment segm;
      CNode n0, n1;
      k = (int)linelist.GetSize();
      for (i = 0; i < k; i++) {
        if (linelist[i].IsSelected == TRUE) {
          n0 = nodelist[linelist[i].n0];
          n1 = nodelist[linelist[i].n1];

          x.Set(n0.x, n0.y);
          x = (x - c) * z + c;
          n0.x = x.re;
          n0.y = x.im;
          n0.IsSelected = 0;

          x.Set(n1.x, n1.y);
          x = (x - c) * z + c;
          n1.x = x.re;
          n1.y = x.im;
          n1.IsSelected = 0;

          j = (int)nodelist.GetSize();
          segm = linelist[i];
          segm.n0 = j;
          segm.n1 = j + 1;
          segm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          linelist.Add(segm);
        }
      }

      // copy selected arc segments
      CArcSegment asegm;
      k = (int)arclist.GetSize();
      for (i = 0; i < k; i++) {
        if (arclist[i].IsSelected == TRUE) {
          n0 = nodelist[arclist[i].n0];
          n1 = nodelist[arclist[i].n1];

          x.Set(n0.x, n0.y);
          x = (x - c) * z + c;
          n0.x = x.re;
          n0.y = x.im;
          n0.IsSelected = 0;

          x.Set(n1.x, n1.y);
          x = (x - c) * z + c;
          n1.x = x.re;
          n1.y = x.im;
          n1.IsSelected = 0;

          j = (int)nodelist.GetSize();
          asegm = arclist[i];
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          arclist.Add(asegm);
        }
      }

      // copy selected block labels
      CBlockLabel lbl;
      k = (int)blocklist.GetSize();
      for (i = 0; i < k; i++) {
        if (blocklist[i].IsSelected == TRUE) {
          x.Set(blocklist[i].x, blocklist[i].y);
          lbl = blocklist[i];
          x = (x - c) * z + c;
          lbl.x = x.re;
          lbl.y = x.im;
          lbl.IsSelected = FALSE;
          for (j = 0; j < blockproplist.GetSize(); j++)
            if (blockproplist[j].BlockName == blocklist[i].BlockType)
              if (blockproplist[j].H_c != 0) {
                lbl.MagDir += t;
                break;
              }

          blocklist.Add(lbl);
        }
      }
    }
  }

  EnforcePSLG();
  return;
}

void CFemmeDoc::TranslateCopy(double incx, double incy, int ncopies, int EditAction)
{
  int i, j, k, nc;
  double dx, dy;
  for (nc = 0; nc < ncopies; nc++) {
    dx = ((double)(nc + 1)) * incx;
    dy = ((double)(nc + 1)) * incy;

    if (EditAction == 0) {
      CNode node;
      k = (int)nodelist.GetSize();
      for (i = 0; i < k; i++) {
        if (nodelist[i].IsSelected == TRUE) {
          node = nodelist[i];
          node.x += dx;
          node.y += dy;
          node.IsSelected = FALSE;
          nodelist.Add(node);
        }
      }
    }

    if (EditAction == 1) {
      CSegment segm;
      CNode n0, n1;
      k = (int)linelist.GetSize();
      for (i = 0; i < k; i++) {
        if (linelist[i].IsSelected == TRUE) {
          n0 = nodelist[linelist[i].n0];
          n1 = nodelist[linelist[i].n1];

          n0.x += dx;
          n0.y += dy;

          n1.x += dx;
          n1.y += dy;

          j = (int)nodelist.GetSize();
          segm = linelist[i];
          segm.n0 = j;
          segm.n1 = j + 1;
          segm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          linelist.Add(segm);
        }
      }
    }

    if (EditAction == 2) {
      CBlockLabel lbl;
      k = (int)blocklist.GetSize();
      for (i = 0; i < k; i++) {
        if (blocklist[i].IsSelected == TRUE) {
          lbl = blocklist[i];
          lbl.x += dx;
          lbl.y += dy;
          lbl.IsSelected = FALSE;
          blocklist.Add(lbl);
        }
      }
    }

    if (EditAction == 3) {
      CArcSegment asegm;
      CNode n0, n1;
      k = (int)arclist.GetSize();
      for (i = 0; i < k; i++) {
        if (arclist[i].IsSelected == TRUE) {
          n0 = nodelist[arclist[i].n0];
          n1 = nodelist[arclist[i].n1];

          n0.x += dx;
          n0.y += dy;
          n0.IsSelected = FALSE;

          n1.x += dx;
          n1.y += dy;
          n1.IsSelected = FALSE;

          j = (int)nodelist.GetSize();
          asegm = arclist[i];
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          arclist.Add(asegm);
        }
      }
    }

    if (EditAction == 4) {
      CNode node;
      k = (int)nodelist.GetSize();
      for (i = 0; i < k; i++) {
        if (nodelist[i].IsSelected == TRUE) {
          node = nodelist[i];
          node.x += dx;
          node.y += dy;
          node.IsSelected = FALSE;
          nodelist.Add(node);
        }
      }

      CSegment segm;
      CNode n0, n1;
      k = (int)linelist.GetSize();
      for (i = 0; i < k; i++) {
        if (linelist[i].IsSelected == TRUE) {
          n0 = nodelist[linelist[i].n0];
          n1 = nodelist[linelist[i].n1];

          n0.x += dx;
          n0.y += dy;
          n0.IsSelected = FALSE;

          n1.x += dx;
          n1.y += dy;
          n1.IsSelected = FALSE;

          j = (int)nodelist.GetSize();
          segm = linelist[i];
          segm.n0 = j;
          segm.n1 = j + 1;
          segm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          linelist.Add(segm);
        }
      }

      CBlockLabel lbl;
      k = (int)blocklist.GetSize();
      for (i = 0; i < k; i++) {
        if (blocklist[i].IsSelected == TRUE) {
          lbl = blocklist[i];
          lbl.x += dx;
          lbl.y += dy;
          lbl.IsSelected = FALSE;
          blocklist.Add(lbl);
        }
      }

      CArcSegment asegm;
      k = (int)arclist.GetSize();
      for (i = 0; i < k; i++) {
        if (arclist[i].IsSelected == TRUE) {
          n0 = nodelist[arclist[i].n0];
          n1 = nodelist[arclist[i].n1];

          n0.x += dx;
          n0.y += dy;
          n0.IsSelected = FALSE;

          n1.x += dx;
          n1.y += dy;
          n1.IsSelected = FALSE;

          j = (int)nodelist.GetSize();
          asegm = arclist[i];
          asegm.n0 = j;
          asegm.n1 = j + 1;
          asegm.IsSelected = FALSE;
          nodelist.Add(n0);
          nodelist.Add(n1);
          arclist.Add(asegm);
        }
      }
    }
  }
  EnforcePSLG();
  return;
}

void CFemmeDoc::EnforcePSLG()
{
  EnforcePSLG(0);
}

void CFemmeDoc::EnforcePSLG(double tol)
{
  /*  need to enforce:
    1) no duplicate point;
    2) no intersections between line segments, lines and arcs, or arcs;
    3) no duplicate block labels;
    4) no overlapping lines or arcs.

    can do this by cleaning out the various lists, and rebuilding them
    using the ``add'' functions that ensure that things come out right.
  */

  CArray<CNode, CNode&> newnodelist;
  CArray<CSegment, CSegment&> newlinelist;
  CArray<CArcSegment, CArcSegment&> newarclist;
  CArray<CBlockLabel, CBlockLabel&> newblocklist;
  int i;
  CComplex p0, p1;
  double d;

  newnodelist.RemoveAll();
  newlinelist.RemoveAll();
  newarclist.RemoveAll();
  newblocklist.RemoveAll();

  for (i = 0; i < nodelist.GetSize(); i++)
    newnodelist.Add(nodelist[i]);
  for (i = 0; i < linelist.GetSize(); i++)
    newlinelist.Add(linelist[i]);
  for (i = 0; i < arclist.GetSize(); i++)
    newarclist.Add(arclist[i]);
  for (i = 0; i < blocklist.GetSize(); i++)
    newblocklist.Add(blocklist[i]);

  nodelist.RemoveAll();
  linelist.RemoveAll();
  arclist.RemoveAll();
  blocklist.RemoveAll();

  // find out what tolerance is so that there are not nodes right on
  // top of each other;
  if (tol == 0) {
    if (newnodelist.GetSize() < 2)
      d = 1.e-08;
    else {
      p0 = newnodelist[0].CC();
      p1 = p0;
      for (i = 1; i < newnodelist.GetSize(); i++) {
        if (newnodelist[i].x < p0.re)
          p0.re = newnodelist[i].x;
        if (newnodelist[i].x > p1.re)
          p1.re = newnodelist[i].x;
        if (newnodelist[i].y < p0.im)
          p0.im = newnodelist[i].y;
        if (newnodelist[i].y > p1.im)
          p1.im = newnodelist[i].y;
      }
      d = abs(p1 - p0) * CLOSE_ENOUGH;
    }
  } else
    d = tol;
  ;
  // put in all of the nodes;
  for (i = 0; i < newnodelist.GetSize(); i++)
    AddNode(newnodelist[i], d);

  // put in all of the lines;
  for (i = 0; i < newlinelist.GetSize(); i++) {
    p0.Set(newnodelist[newlinelist[i].n0].x, newnodelist[newlinelist[i].n0].y);
    p1.Set(newnodelist[newlinelist[i].n1].x, newnodelist[newlinelist[i].n1].y);
    AddSegment(p0, p1, newlinelist[i], d);
  }

  // put in all of the arcs;
  for (i = 0; i < newarclist.GetSize(); i++) {
    p0.Set(newnodelist[newarclist[i].n0].x, newnodelist[newarclist[i].n0].y);
    p1.Set(newnodelist[newarclist[i].n1].x, newnodelist[newarclist[i].n1].y);
    AddArcSegment(p0, p1, newarclist[i], d);
  }

  // put in all of the block labels;
  for (i = 0; i < newblocklist.GetSize(); i++)
    AddBlockLabel(newblocklist[i], d);

  UnselectAll();
  return;
}

BOOL CFemmeDoc::AddNode(CNode& node, double d)
{
  int i, k;
  CSegment segm;
  CArcSegment asegm;
  CComplex c, a0, a1, a2;
  double x, y;
  double R;

  x = node.x;
  y = node.y;

  // test to see if ``too close'' to existing node...
  for (i = 0; i < nodelist.GetSize(); i++)
    if (nodelist[i].GetDistance(x, y) < d)
      return FALSE;

  // can't put a node on top of a block label; do same sort of test.
  for (i = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].GetDistance(x, y) < d)
      return FALSE;

  // if all is OK, add point in to the node list...
  nodelist.Add(node);

  // test to see if node is on an existing line; if so,
  // break into two lines;
  k = (int)linelist.GetSize();
  for (i = 0; i < k; i++) {
    if (fabs(ShortestDistance(x, y, i)) < d) {
      segm = linelist[i];
      linelist[i].n1 = (int)nodelist.GetSize() - 1;
      segm.n0 = (int)nodelist.GetSize() - 1;
      linelist.Add(segm);
    }
  }

  // test to see if node is on an existing arc; if so,
  // break into two arcs;
  k = (int)arclist.GetSize();
  for (i = 0; i < k; i++) {
    if (ShortestDistanceFromArc(CComplex(x, y), arclist[i]) < d) {
      a0.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
      a1.Set(nodelist[arclist[i].n1].x, nodelist[arclist[i].n1].y);
      a2.Set(x, y);
      GetCircle(arclist[i], c, R);
      asegm = arclist[i];
      arclist[i].n1 = (int)nodelist.GetSize() - 1;
      arclist[i].ArcLength = arg((a2 - c) / (a0 - c)) * 180. / PI;
      asegm.n0 = (int)nodelist.GetSize() - 1;
      asegm.ArcLength = arg((a1 - c) / (a2 - c)) * 180. / PI;
      arclist.Add(asegm);
    }
  }
  return TRUE;
}

BOOL CFemmeDoc::AddSegment(CComplex p0, CComplex p1, CSegment& segm, double tol)
{
  int i, j, k, n0, n1;
  double xi, yi, t;
  CComplex p[2];
  CArray<CComplex, CComplex&> newnodes;

  newnodes.RemoveAll();

  n0 = ClosestNode(p0.re, p0.im);
  n1 = ClosestNode(p1.re, p1.im);

  // don't add if line is degenerate
  if (n0 == n1)
    return FALSE;

  // don't add if the line is already in the list;
  for (i = 0; i < linelist.GetSize(); i++) {
    if ((linelist[i].n0 == n0) && (linelist[i].n1 == n1))
      return FALSE;
    if ((linelist[i].n0 == n1) && (linelist[i].n1 == n0))
      return FALSE;
  }

  segm.IsSelected = FALSE;
  segm.n0 = n0;
  segm.n1 = n1;

  // check to see if there are intersections with segments
  for (i = 0; i < linelist.GetSize(); i++)
    if (GetIntersection(n0, n1, i, &xi, &yi) == TRUE)
      newnodes.Add(CComplex(xi, yi));

  // check to see if there are intersections with arcs
  for (i = 0; i < arclist.GetSize(); i++) {
    j = GetLineArcIntersection(segm, arclist[i], p);
    if (j > 0)
      for (k = 0; k < j; k++)
        newnodes.Add(p[k]);
  }

  // add nodes at intersections
  if (tol == 0) {
    if (nodelist.GetSize() < 2)
      t = 1.e-08;
    else {
      CComplex p0, p1;
      p0 = nodelist[0].CC();
      p1 = p0;
      for (i = 1; i < nodelist.GetSize(); i++) {
        if (nodelist[i].x < p0.re)
          p0.re = nodelist[i].x;
        if (nodelist[i].x > p1.re)
          p1.re = nodelist[i].x;
        if (nodelist[i].y < p0.im)
          p0.im = nodelist[i].y;
        if (nodelist[i].y > p1.im)
          p1.im = nodelist[i].y;
      }
      t = abs(p1 - p0) * CLOSE_ENOUGH;
    }
  } else
    t = tol;

  for (i = 0; i < newnodes.GetSize(); i++)
    AddNode(newnodes[i].re, newnodes[i].im, t);

  // Add proposed line segment
  linelist.Add(segm);

  // check to see if proposed line passes through other points;
  // if so, delete line and create lines that link intermediate points;
  // does this by recursive use of AddSegment;
  double d, dmin;
  UnselectAll();
  if (tol == 0)
    dmin = abs(nodelist[n1].CC() - nodelist[n0].CC()) * 1.e-05;
  else
    dmin = tol;

  k = (int)linelist.GetSize() - 1;
  for (i = 0; i < nodelist.GetSize(); i++) {
    if ((i != n0) && (i != n1)) {
      d = ShortestDistance(nodelist[i].x, nodelist[i].y, k);
      // catch a special case...
      if (abs(nodelist[i].CC() - nodelist[n0].CC()) < dmin)
        d = 2. * dmin;
      if (abs(nodelist[i].CC() - nodelist[n1].CC()) < dmin)
        d = 2. * dmin;
      if (d < dmin) {
        linelist[k].ToggleSelect();
        DeleteSelectedSegments();
        AddSegment(n0, i, &segm, dmin);
        AddSegment(i, n1, &segm, dmin);
        i = (int)nodelist.GetSize();
      }
    }
  }

  return TRUE;
}

BOOL CFemmeDoc::AddArcSegment(CComplex p0, CComplex p1, CArcSegment& asegm, double tol)
{

  int i, j, k;
  CSegment segm;
  CArcSegment newarc;
  CComplex c, p[2];
  CArray<CComplex, CComplex&> newnodes;
  double R, d, dmin, t;

  asegm.n0 = ClosestNode(p0.re, p0.im);
  asegm.n1 = ClosestNode(p1.re, p1.im);

  newnodes.RemoveAll();

  // don't add if line is degenerate
  if (asegm.n0 == asegm.n1)
    return FALSE;

  // don't add if the arc is already in the list;
  for (i = 0; i < arclist.GetSize(); i++) {
    if ((arclist[i].n0 == asegm.n0) && (arclist[i].n1 == asegm.n1) && (fabs(arclist[i].ArcLength - asegm.ArcLength) < 1.e-02))
      return FALSE;
    // arcs are ``the same'' if start and end points are the same, and if
    // the arc lengths are relatively close (but a lot farther than
    // machine precision...
  }

  // add proposed arc to the linelist
  asegm.IsSelected = FALSE;

  // check to see if there are intersections
  for (i = 0; i < linelist.GetSize(); i++) {
    j = GetLineArcIntersection(linelist[i], asegm, p);
    if (j > 0)
      for (k = 0; k < j; k++)
        newnodes.Add(p[k]);
  }
  for (i = 0; i < arclist.GetSize(); i++) {
    j = GetArcArcIntersection(asegm, arclist[i], p);
    if (j > 0)
      for (k = 0; k < j; k++)
        newnodes.Add(p[k]);
  }

  // add nodes at intersections
  if (tol == 0) {
    if (nodelist.GetSize() < 2)
      t = 1.e-08;
    else {
      CComplex p0, p1;
      p0 = nodelist[0].CC();
      p1 = p0;
      for (i = 1; i < nodelist.GetSize(); i++) {
        if (nodelist[i].x < p0.re)
          p0.re = nodelist[i].x;
        if (nodelist[i].x > p1.re)
          p1.re = nodelist[i].x;
        if (nodelist[i].y < p0.im)
          p0.im = nodelist[i].y;
        if (nodelist[i].y > p1.im)
          p1.im = nodelist[i].y;
      }
      t = abs(p1 - p0) * CLOSE_ENOUGH;
    }
  } else
    t = tol;

  for (i = 0; i < newnodes.GetSize(); i++)
    AddNode(newnodes[i].re, newnodes[i].im, t);

  // add proposed arc segment;
  arclist.Add(asegm);

  // check to see if proposed arc passes through other points;
  // if so, delete arc and create arcs that link intermediate points;
  // does this by recursive use of AddArcSegment;
  UnselectAll();
  GetCircle(asegm, c, R);
  if (tol == 0)
    dmin = fabs(R * PI * asegm.ArcLength / 180.) * 1.e-05;
  else
    dmin = tol;
  k = (int)arclist.GetSize() - 1;

  for (i = 0; i < nodelist.GetSize(); i++) {
    if ((i != asegm.n0) && (i != asegm.n1)) {
      d = ShortestDistanceFromArc(CComplex(nodelist[i].x, nodelist[i].y), arclist[k]);
      if (d < dmin) {
        CComplex a0, a1, a2;
        a0.Set(nodelist[asegm.n0].x, nodelist[asegm.n0].y);
        a1.Set(nodelist[asegm.n1].x, nodelist[asegm.n1].y);
        a2.Set(nodelist[i].x, nodelist[i].y);
        arclist[k].ToggleSelect();
        DeleteSelectedArcSegments();

        newarc = asegm;
        newarc.n1 = i;
        newarc.ArcLength = arg((a2 - c) / (a0 - c)) * 180. / PI;
        AddArcSegment(newarc, dmin);

        newarc = asegm;
        newarc.n0 = i;
        newarc.ArcLength = arg((a1 - c) / (a2 - c)) * 180. / PI;
        AddArcSegment(newarc, dmin);

        i = (int)nodelist.GetSize();
      }
    }
  }

  return TRUE;
}

BOOL CFemmeDoc::AddBlockLabel(CBlockLabel& blabel, double d)
{
  int i;
  double x, y;
  BOOL AddFlag = TRUE;

  x = blabel.x;
  y = blabel.y;

  // test to see if ``too close'' to existing node...
  for (i = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].GetDistance(x, y) < d)
      AddFlag = FALSE;

  // can't put a block label on top of an existing node...
  for (i = 0; i < nodelist.GetSize(); i++)
    if (nodelist[i].GetDistance(x, y) < d)
      return FALSE;

  // can't put a block label on a line, either...
  for (i = 0; i < linelist.GetSize(); i++)
    if (ShortestDistance(x, y, i) < d)
      return FALSE;

  // if all is OK, add point in to the node list...
  if (AddFlag == TRUE)
    blocklist.Add(blabel);

  return TRUE;
}

BOOL CFemmeDoc::WriteDXF(CString fname)
{
  int i, j, k;
  double x0, y0, x1, y1, R, dt;
  double extmaxx, extminx, extmaxy, extminy;
  BOOL laze[256];
  int nlaze;
  char lay[256];
  CComplex c, p, s;
  FILE* fp;

  if ((fp = fopen(fname, "wt")) == NULL)
    return FALSE;

  if (nodelist.GetSize() < 2) {
    extmaxx = 1;
    extmaxy = 1;
    extminx = 0;
    extminy = 0;
  } else {
    extminx = nodelist[0].x;
    extminy = nodelist[1].y;
    extmaxx = extminx;
    extmaxy = extminy;
    for (i = 1; i < nodelist.GetSize(); i++) {
      if (nodelist[i].x < extminx)
        extminx = nodelist[i].x;
      if (nodelist[i].x > extmaxx)
        extmaxx = nodelist[i].x;
      if (nodelist[i].y < extminy)
        extminy = nodelist[i].y;
      if (nodelist[i].y > extmaxy)
        extmaxy = nodelist[i].y;
    }
  }

  for (i = 0; i < arclist.GetSize(); i++) {
    k = (int)ceil(arclist[i].ArcLength / arclist[i].MaxSideLength);
    dt = arclist[i].ArcLength * PI / (((double)k) * 180.);
    GetCircle(arclist[i], c, R);
    p.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
    s = exp(I * dt);
    for (j = 0; j < k; j++) {
      p = (p - c) * s + c;
      if (p.re < extminx)
        extminx = p.re;
      if (p.re > extmaxx)
        extmaxx = p.re;
      if (p.im < extminy)
        extminy = p.im;
      if (p.im > extmaxy)
        extmaxy = p.im;
    }
  }

  p.Set(extminx, extminy);
  s.Set(extmaxx, extmaxy);
  R = 0.025 * abs(s - p);

  // check out which layers are actually called out;
  for (i = 0, nlaze = 0; i < 256; i++)
    laze[i] = FALSE;
  for (i = 0; i < linelist.GetSize(); i++)
    if (linelist[i].InGroup != 0) {
      for (j = 0, k = FALSE; j < nlaze; j++) {
        if (laze[j] == linelist[i].InGroup) {
          k = TRUE;
          j = nlaze;
        }
      }
      if ((k == FALSE) && (nlaze < 256)) {
        laze[nlaze] = linelist[i].InGroup;
        nlaze++;
      }
    }

  for (i = 0; i < arclist.GetSize(); i++)
    if (arclist[i].InGroup != 0) {
      for (j = 0, k = FALSE; j < nlaze; j++) {
        if (laze[j] == arclist[i].InGroup) {
          k = TRUE;
          j = nlaze;
        }
      }
      if ((k == FALSE) && (nlaze < 256)) {
        laze[nlaze] = arclist[i].InGroup;
        nlaze++;
      }
    }

  fprintf(fp, "  0\nSECTION\n  2\nHEADER\n  9\n");
  fprintf(fp, "$INSBASE\n 10\n0.0000\n 20\n0.0000\n  9\n");
  fprintf(fp, "$EXTMIN\n 10\n%.17g\n 20\n%.17g\n  9\n", extminx - R, extminy - R);
  fprintf(fp, "$EXTMAX\n 10\n%.17g\n 20\n%.17g\n  9\n", extmaxx + R, extmaxy + R);
  fprintf(fp, "$LIMMIN\n 10\n%.17g\n 20\n%.17g\n  9\n", extminx - R, extminy - R);
  fprintf(fp, "$LIMMAX\n 10\n%.17g\n 20\n%.17g\n  9\n", extmaxx + R, extmaxy + R);
  fprintf(fp, "$TEXTSTYLE\n  7\nSTANDARD\n  9\n$CLAYER\n");
  fprintf(fp, "  8\ndefault\n  0\nENDSEC\n  0\n");
  fprintf(fp, "SECTION\n  2\nTABLES\n  0\n");
  fprintf(fp, "TABLE\n  2\nLTYPE\n 70\n4948253\n  0\n");
  fprintf(fp, "LTYPE\n  2\nCONTINUOUS\n 70\n    64\n  3\n");
  fprintf(fp, "Solid line\n 72\n    65\n 73\n     0\n 40\n0.0\n  0\nENDTAB\n  0\n");
  fprintf(fp, "TABLE\n  2\nLAYER\n 70\n     5\n  0\n");
  fprintf(fp, "LAYER\n  2\ndefault\n 70\n    64\n 62\n     7\n  6\n");
  fprintf(fp, "CONTINUOUS\n  0\n");

  for (i = 0; i < nlaze; i++) {
    fprintf(fp, "LAYER\n  2\nlayer%i\n 70\n    64\n 62\n     7\n  6\n", laze[i]);
    fprintf(fp, "CONTINUOUS\n  0\n");
  }

  fprintf(fp, "ENDTAB\n  0\nTABLE\n  2\n");
  fprintf(fp, "STYLE\n 70\n     1\n  0\nSTYLE\n  2\nSTANDARD\n 70\n");
  fprintf(fp, "     0\n 40\n0.0\n 41\n1.0\n 50\n0.0\n 71\n     0\n");
  fprintf(fp, " 42\n0.2\n  3\ntxt\n  4\n\n  0\nENDTAB\n  0\n");
  fprintf(fp, "TABLE\n  2\nVIEW\n 70\n     0\n  0\nENDTAB\n  0\n");
  fprintf(fp, "ENDSEC\n  0\nSECTION\n  2\nBLOCKS\n  0\nENDSEC\n");
  fprintf(fp, "  0\nSECTION\n  2\nENTITIES\n  0\n");

  for (i = 0; i < linelist.GetSize(); i++) {
    x0 = nodelist[linelist[i].n0].x;
    y0 = nodelist[linelist[i].n0].y;
    x1 = nodelist[linelist[i].n1].x;
    y1 = nodelist[linelist[i].n1].y;
    if (linelist[i].InGroup == 0)
      sprintf(lay, "default");
    else
      sprintf(lay, "layer%i", linelist[i].InGroup);
    fprintf(fp, "LINE\n  8\n%s\n 10\n%.17g\n 20\n%.17g\n 30\n0.0\n 11\n%.17g\n 21\n%.17g\n 31\n0.0\n  0\n",
        lay, x0, y0, x1, y1);
  }

  for (i = 0; i < arclist.GetSize(); i++) {
    GetCircle(arclist[i], c, R);
    x0 = arg(nodelist[arclist[i].n0].CC() - c) * 180. / PI;
    x1 = arg(nodelist[arclist[i].n1].CC() - c) * 180. / PI;
    if (x0 < 0)
      x0 += 360.;
    if (x1 < 0)
      x1 += 360.;
    if (arclist[i].InGroup == 0)
      sprintf(lay, "default");
    else
      sprintf(lay, "layer%i", arclist[i].InGroup);
    fprintf(fp, "ARC\n  8\n%s\n", lay);
    fprintf(fp, " 10\n%.17g\n 20\n%.17g\n 30\n%.17g\n 40\n%.17g\n 50\n%.17g\n 51\n%.17g\n  0\n",
        c.re, c.im, 0., R, x0, x1);
  }

  fprintf(fp, "ENDSEC\n  0\nEOF\n");

  fclose(fp);

  return TRUE;
}

void CFemmeDoc::ScaleMove(double bx, double by, double sf, int EditAction)
{
  int i;

  if (EditAction == 0) {
    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        nodelist[i].x = bx + sf * (nodelist[i].x - bx);
        nodelist[i].y = by + sf * (nodelist[i].y - by);
      }
    }
    EnforcePSLG();
    return;
  }

  if (EditAction == 1) {
    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {
        nodelist[linelist[i].n0].IsSelected = TRUE;
        nodelist[linelist[i].n1].IsSelected = TRUE;
      }
    }
    ScaleMove(bx, by, sf, 0);
    return;
  }

  if (EditAction == 2) {
    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        blocklist[i].x = bx + sf * (blocklist[i].x - bx);
        blocklist[i].y = by + sf * (blocklist[i].y - by);
        blocklist[i].MaxArea *= (sf * sf);
      }
    }
    EnforcePSLG();
    return;
  }

  if (EditAction == 3) {
    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        nodelist[arclist[i].n0].IsSelected = TRUE;
        nodelist[arclist[i].n1].IsSelected = TRUE;
      }
    }
    ScaleMove(bx, by, sf, 0);
    return;
  }

  if (EditAction == 4) {
    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {
        nodelist[linelist[i].n0].IsSelected = TRUE;
        nodelist[linelist[i].n1].IsSelected = TRUE;
      }
    }

    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        nodelist[arclist[i].n0].IsSelected = TRUE;
        nodelist[arclist[i].n1].IsSelected = TRUE;
      }
    }

    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        nodelist[i].x = bx + sf * (nodelist[i].x - bx);
        nodelist[i].y = by + sf * (nodelist[i].y - by);
      }
    }

    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        blocklist[i].x = bx + sf * (blocklist[i].x - bx);
        blocklist[i].y = by + sf * (blocklist[i].y - by);
        blocklist[i].MaxArea *= (sf * sf);
      }
    }

    EnforcePSLG();
    return;
  }
}

void CFemmeDoc::MirrorSelected(double x0, double y0, double x1, double y1, int EditAction)
{
  int i, j, k;
  CComplex x, p, y;

  x = x0 + I * y0;
  p = (x1 - x0) + I * (y1 - y0);
  if (abs(p) == 0)
    return;
  p /= abs(p);

  if (EditAction == 0) {
    CNode node;
    k = (int)nodelist.GetSize();
    for (i = 0; i < k; i++) {
      if (nodelist[i].IsSelected == TRUE) {
        node = nodelist[i];
        y = ((node.x + I * node.y) - x) / p;
        y = p * y.Conj() + x;
        node.x = y.re;
        node.y = y.im;
        node.IsSelected = FALSE;
        nodelist.Add(node);
      }
    }
  }

  if (EditAction == 1) {
    CSegment segm;
    CNode n0, n1;
    k = (int)linelist.GetSize();
    for (i = 0; i < k; i++) {
      if (linelist[i].IsSelected == TRUE) {
        n0 = nodelist[linelist[i].n0];
        n1 = nodelist[linelist[i].n1];

        y = ((n0.x + I * n0.y) - x) / p;
        y = p * y.Conj() + x;
        n0.x = y.re;
        n0.y = y.im;

        y = ((n1.x + I * n1.y) - x) / p;
        y = p * y.Conj() + x;
        n1.x = y.re;
        n1.y = y.im;

        j = (int)nodelist.GetSize();
        segm = linelist[i];
        segm.n0 = j;
        segm.n1 = j + 1;
        segm.IsSelected = FALSE;
        nodelist.Add(n0);
        nodelist.Add(n1);
        linelist.Add(segm);
      }
    }
  }

  if (EditAction == 2) {
    CBlockLabel lbl;
    k = (int)blocklist.GetSize();
    for (i = 0; i < k; i++) {
      if (blocklist[i].IsSelected == TRUE) {
        lbl = blocklist[i];
        y = ((lbl.x + I * lbl.y) - x) / p;
        y = p * y.Conj() + x;
        lbl.x = y.re;
        lbl.y = y.im;
        lbl.MagDir = (180. / PI) * arg(p * conj(exp(I * lbl.MagDir * PI / 180.) / p));
        lbl.IsSelected = FALSE;
        blocklist.Add(lbl);
      }
    }
  }

  if (EditAction == 3) {
    CArcSegment asegm;
    CNode n0, n1;
    k = (int)arclist.GetSize();
    for (i = 0; i < k; i++) {
      if (arclist[i].IsSelected == TRUE) {
        n0 = nodelist[arclist[i].n1];
        n1 = nodelist[arclist[i].n0];

        y = ((n0.x + I * n0.y) - x) / p;
        y = p * y.Conj() + x;
        n0.x = y.re;
        n0.y = y.im;
        n0.IsSelected = FALSE;

        y = ((n1.x + I * n1.y) - x) / p;
        y = p * y.Conj() + x;
        n1.x = y.re;
        n1.y = y.im;
        n1.IsSelected = FALSE;

        j = (int)nodelist.GetSize();
        asegm = arclist[i];
        asegm.n0 = j;
        asegm.n1 = j + 1;
        asegm.IsSelected = FALSE;
        nodelist.Add(n0);
        nodelist.Add(n1);
        arclist.Add(asegm);
      }
    }
  }

  if (EditAction == 4) {
    CNode node;
    k = (int)nodelist.GetSize();
    for (i = 0; i < k; i++) {
      if (nodelist[i].IsSelected == TRUE) {
        node = nodelist[i];
        y = ((node.x + I * node.y) - x) / p;
        y = p * y.Conj() + x;
        node.x = y.re;
        node.y = y.im;
        node.IsSelected = FALSE;
        nodelist.Add(node);
      }
    }

    CSegment segm;
    CNode n0, n1;
    k = (int)linelist.GetSize();
    for (i = 0; i < k; i++) {
      if (linelist[i].IsSelected == TRUE) {
        n0 = nodelist[linelist[i].n0];
        n1 = nodelist[linelist[i].n1];

        y = ((n0.x + I * n0.y) - x) / p;
        y = p * y.Conj() + x;
        n0.x = y.re;
        n0.y = y.im;

        y = ((n1.x + I * n1.y) - x) / p;
        y = p * y.Conj() + x;
        n1.x = y.re;
        n1.y = y.im;

        j = (int)nodelist.GetSize();
        segm = linelist[i];
        segm.n0 = j;
        segm.n1 = j + 1;
        segm.IsSelected = FALSE;
        nodelist.Add(n0);
        nodelist.Add(n1);
        linelist.Add(segm);
      }
    }

    CBlockLabel lbl;
    k = (int)blocklist.GetSize();
    for (i = 0; i < k; i++) {
      if (blocklist[i].IsSelected == TRUE) {
        lbl = blocklist[i];
        y = ((lbl.x + I * lbl.y) - x) / p;
        y = p * y.Conj() + x;
        lbl.x = y.re;
        lbl.y = y.im;
        lbl.MagDir = (180. / PI) * arg(p * conj(exp(I * lbl.MagDir * PI / 180.) / p));
        lbl.IsSelected = FALSE;
        blocklist.Add(lbl);
      }
    }

    CArcSegment asegm;
    k = (int)arclist.GetSize();
    for (i = 0; i < k; i++) {
      if (arclist[i].IsSelected == TRUE) {
        n0 = nodelist[arclist[i].n1];
        n1 = nodelist[arclist[i].n0];

        y = ((n0.x + I * n0.y) - x) / p;
        y = p * y.Conj() + x;
        n0.x = y.re;
        n0.y = y.im;
        n0.IsSelected = FALSE;

        y = ((n1.x + I * n1.y) - x) / p;
        y = p * y.Conj() + x;
        n1.x = y.re;
        n1.y = y.im;
        n1.IsSelected = FALSE;

        j = (int)nodelist.GetSize();
        asegm = arclist[i];
        asegm.n0 = j;
        asegm.n1 = j + 1;
        asegm.IsSelected = FALSE;
        nodelist.Add(n0);
        nodelist.Add(n1);
        arclist.Add(asegm);
      }
    }
  }

  EnforcePSLG();
  return;
}

BOOL CFemmeDoc::dxf_line_hook()
{
  MSG msg;

  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if ((msg.message == WM_KEYDOWN) && (msg.wParam == VK_ESCAPE)) {
      MessageBeep(MB_ICONEXCLAMATION);
      return TRUE;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return FALSE;
}

void CFemmeDoc::FancyEnforcePSLG(double tol)
{
  /*
    need to enforce:
    1) no duplicate point;
    2) no intersections between line segments, lines and arcs, or arcs;
    3) no duplicate block labels;
    4) no overlapping lines or arcs.

    can do this by cleaning out the various lists, and rebuilding them
    using the ``add'' functions that ensure that things come out right.
  */

  CArray<CNode, CNode&> newnodelist;
  CArray<CSegment, CSegment&> newlinelist;
  CArray<CArcSegment, CArcSegment&> newarclist;
  CArray<CBlockLabel, CBlockLabel&> newblocklist;
  int i, k;
  CComplex p0, p1;
  double d;

  bLinehook = ImportDXF; // kludge to stop the program from giving a crash if
                         // the user tries to exit during a dxf import.

  CFemmeView* pView;
  POSITION pos;
  pos = GetFirstViewPosition();
  pView = (CFemmeView*)GetNextView(pos);

  FirstDraw = TRUE;
  //	pView->lua_zoomnatural();
  dxf_line_hook();
  pView->EditAction = 4;

  newnodelist.RemoveAll();
  newlinelist.RemoveAll();
  newarclist.RemoveAll();
  newblocklist.RemoveAll();

  for (i = 0; i < nodelist.GetSize(); i++)
    newnodelist.Add(nodelist[i]);
  for (i = 0; i < linelist.GetSize(); i++)
    newlinelist.Add(linelist[i]);
  for (i = 0; i < arclist.GetSize(); i++)
    newarclist.Add(arclist[i]);
  for (i = 0; i < blocklist.GetSize(); i++)
    newblocklist.Add(blocklist[i]);

  nodelist.RemoveAll();
  linelist.RemoveAll();
  arclist.RemoveAll();
  blocklist.RemoveAll();

  pView->InvalidateRect(NULL);
  dxf_line_hook();

  // find out what tolerance is so that there are not nodes right on
  // top of each other;
  if (tol == 0) {
    if (newnodelist.GetSize() < 2)
      d = 1.e-08;
    else {
      p0 = newnodelist[0].CC();
      p1 = p0;
      for (i = 1; i < newnodelist.GetSize(); i++) {
        if (newnodelist[i].x < p0.re)
          p0.re = newnodelist[i].x;
        if (newnodelist[i].x > p1.re)
          p1.re = newnodelist[i].x;
        if (newnodelist[i].y < p0.im)
          p0.im = newnodelist[i].y;
        if (newnodelist[i].y > p1.im)
          p1.im = newnodelist[i].y;
      }
      d = abs(p1 - p0) * CLOSE_ENOUGH;
    }
  } else
    d = tol;

  // put in all of the lines;
  for (i = 0; i < newlinelist.GetSize(); i++) {
    // Add in endpoints of the proposed line;
    AddNode(newnodelist[newlinelist[i].n0], d);
    AddNode(newnodelist[newlinelist[i].n1], d);

    // Add in the proposed line itself;
    p0 = newnodelist[newlinelist[i].n0].CC();
    p1 = newnodelist[newlinelist[i].n1].CC();
    if (AddSegment(p0, p1, newlinelist[i], d) == TRUE) {
      pView->DrawPSLG();
      if (dxf_line_hook()) {
        bLinehook = FALSE;
        return;
      }
    }
  }

  // put in all of the arcs;
  for (i = 0; i < newarclist.GetSize(); i++) {

    // Add in endpoints of the proposed line;
    AddNode(newnodelist[newarclist[i].n0], d);
    AddNode(newnodelist[newarclist[i].n1], d);

    // Add in the proposed arc inself;
    p0 = newnodelist[newarclist[i].n0].CC();
    p1 = newnodelist[newarclist[i].n1].CC();
    if (AddArcSegment(p0, p1, newarclist[i], d) == TRUE) {
      pView->DrawPSLG();
      if (dxf_line_hook()) {
        bLinehook = FALSE;
        return;
      }
    }
  }

  UnselectAll();

  // do one last check to eliminate shallow arcs that
  // link up the same two points as a line segment;
  for (i = 0; i < arclist.GetSize(); i++) {
    if (arclist[i].ArcLength <= 22.5) {
      for (k = 0; k < linelist.GetSize(); k++) {
        if ((arclist[i].n0 == linelist[k].n0) && (arclist[i].n1 == linelist[k].n1))
          arclist[i].IsSelected = TRUE;
        if ((arclist[i].n1 == linelist[k].n0) && (arclist[i].n0 == linelist[k].n1))
          arclist[i].IsSelected = TRUE;
        if (arclist[i].IsSelected)
          k = (int)linelist.GetSize();
      }
    }
  }
  DeleteSelectedArcSegments();

  // put in all of the block labels;
  for (i = 0; i < newblocklist.GetSize(); i++)
    AddBlockLabel(newblocklist[i], d);

  if (SelectOrphans()) {
    CString msg;
    msg = "There are lines or arcs with \"Orphaned\" end points.\n";
    msg += "The lines or arcs could be glitches in the DXF import\n";
    msg += "import, so they should be examined manually.\n";
    msg += "These points and lines will be shown as selected.\n";
    msg += "To redisplay the orphaned points and lines, select\n";
    msg += "View|Show Orphans off of the main menu.";
    MsgBox(msg);
  }

  bLinehook = FALSE;

  return;
}

BOOL CFemmeDoc::SelectOrphans()
{
  int i;
  BOOL bHasOrphans;

  UnselectAll();

  // figure out if there are any orphan segments or arcs
  // and select them so that the will be visible to the user
  // We will first figure out which nodes are elements of only
  // one line or arc.  Then, we will select the lines and arcs
  // that are associated with these orphaned nodes
  for (i = 0; i < linelist.GetSize(); i++) {
    nodelist[linelist[i].n0].IsSelected++;
    nodelist[linelist[i].n1].IsSelected++;
  }
  for (i = 0; i < arclist.GetSize(); i++) {
    nodelist[arclist[i].n0].IsSelected++;
    nodelist[arclist[i].n1].IsSelected++;
  }
  for (i = 0, bHasOrphans = FALSE; i < nodelist.GetSize(); i++) {
    if (nodelist[i].IsSelected != 1)
      nodelist[i].IsSelected = FALSE;
    else
      bHasOrphans = TRUE;
  }
  if (bHasOrphans) {
    // We _have_ orphaned nodes.  Select their associated
    // lines and/or arcs, and give the user a message about it.
    for (i = 0; i < linelist.GetSize(); i++) {
      if (nodelist[linelist[i].n0].IsSelected)
        linelist[i].IsSelected = TRUE;
      if (nodelist[linelist[i].n1].IsSelected)
        linelist[i].IsSelected = TRUE;
    }
    for (i = 0; i < arclist.GetSize(); i++) {
      if (nodelist[arclist[i].n0].IsSelected)
        arclist[i].IsSelected = TRUE;
      if (nodelist[arclist[i].n1].IsSelected)
        arclist[i].IsSelected = TRUE;
    }
  }

  return bHasOrphans;
}