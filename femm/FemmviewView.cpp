// femmviewView.cpp : implementation of the CFemmviewView class
//

#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "femm.h"
#include "problem.h"
#include "xyplot.h"
#include "femmviewDoc.h"
#include "femmviewView.h"
#include "GridMod.h"
#include "EnterPt.h"
#include "CPlotDlg.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "KbdZoom.h"
#include "CPlotDlg2.h"
#include "cv_DPlotDlg2.h"
#include "MyMsgBox.h"
#include "XYPlotDlg.h"
#include "LIntDlg.h"
#include "BlockInt.h"
#include "CircDlg.h"
#include "BendContourDlg.h"
#include "VPlotDlg.h"
#include "GapIntegral.h"
#include "GapPlotDlg.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int Xm;
extern int Ym;

#define MSKHI (1. + 1.e-08)
#define MSKLO (0. - 1.e-08)

extern lua_State* lua;
extern BOOL bLinehook;

/////////////////////////////////////////////////////////////////////////////
// CFemmviewView

IMPLEMENT_DYNCREATE(CFemmviewView, CView)

BEGIN_MESSAGE_MAP(CFemmviewView, CView)
//{{AFX_MSG_MAP(CFemmviewView)
ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
ON_COMMAND(ID_SNAP_GRID, OnSnapGrid)
ON_COMMAND(ID_SHOW_MESH, OnShowMesh)
ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
ON_COMMAND(ID_SET_GRID, OnSetGrid)
ON_COMMAND(ID_CPLOT, OnCplot)
ON_COMMAND(ID_DPLOT, OnDplot)
ON_COMMAND(ID_ZOOM_NATURAL, OnZoomNatural)
ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
ON_COMMAND(ID_ZOOM_WINDOW, OnZoomWindow)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_COMMAND(ID_RELOAD, OnReload)
ON_COMMAND(ID_MENU_AREA, OnMenuArea)
ON_COMMAND(ID_MENU_CONTOUR, OnMenuContour)
ON_COMMAND(ID_MENU_POINT, OnMenuPoint)
ON_WM_KEYDOWN()
ON_COMMAND(ID_MENU_PLOT, OnMenuPlot)
ON_COMMAND(ID_MENU_INTEGRATE, OnMenuIntegrate)
ON_COMMAND(ID_MENUSHOWPTS, OnMenushowpts)
ON_COMMAND(ID_KBDZOOM, OnKbdZoom)
ON_COMMAND(ID_SMOOTH, OnSmooth)
ON_WM_ERASEBKGND()
ON_WM_RBUTTONDBLCLK()
ON_COMMAND(ID_PAN_DOWN, OnPanDown)
ON_COMMAND(ID_PAN_LEFT, OnPanLeft)
ON_COMMAND(ID_PAN_RIGHT, OnPanRight)
ON_COMMAND(ID_PAN_UP, OnPanUp)
ON_COMMAND(ID_VIEW_CIRCPROPS, OnViewCircprops)
ON_COMMAND(ID_EDIT_COPY_AS_METAFILE, OnEditCopyAsMetafile)
ON_COMMAND(ID_VIEW_BHCURVES, OnViewBHcurves)
ON_COMMAND(ID_VIEW_INFO, OnViewInfo)
ON_COMMAND(ID_VIEW_SHOWNAMES, OnViewShownames)
ON_COMMAND(ID_VPLOT, OnVplot)
ON_WM_SIZE()
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void MyMessageBox(CString s)
{
  CString z;
  char linfeed[3];
  int i;

  sprintf(linfeed, "%c%c", 0x0D, 0x0A);
  for (i = 0; i < s.GetLength(); i++)
    if (s[i] == '\n')
      z += linfeed;
    else
      z += s[i];

  CMyMsgBox msgdlg;
  msgdlg.m_mymsg = z;
  msgdlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFemmviewView construction/destruction

CFemmviewView::CFemmviewView()
{
  ((CFemmApp*)AfxGetApp())->NumViews++;

  // TODO: add construction code here
  d_EditAction = 0;
  d_GridFlag = FALSE;
  d_SnapFlag = FALSE;
  d_MeshFlag = FALSE;
  d_LegendFlag = TRUE;
  d_GreyContours = FALSE;
  d_NumContours = 19;
  d_ShowAr = TRUE;
  d_ShowAi = FALSE;
  d_ShowMask = FALSE;
  d_DensityPlot = 0;
  d_VectorPlot = 0;
  d_PtsFlag = TRUE;
  d_ResetOnReload = FALSE;
  d_Smooth = TRUE;
  d_PlotPoints = 1500;
  d_ShowNames = FALSE;

  ox = 0.;
  oy = 0.;
  mag = 100.; // lower left corner is origin; 100 dpi
  GridSize = 0.25; // 1/4" grid
  Coords = FALSE;
  ZoomWndFlag = FALSE;
  DrawSelected = -1;
  VectorScaleFactor = 1;

  //////////////////////////////
  // Default Colors           //
  //////////////////////////////

  // Greyscale Colormap
  Grey00 = dGrey00;
  Grey01 = dGrey01;
  Grey02 = dGrey02;
  Grey03 = dGrey03;
  Grey04 = dGrey04;
  Grey05 = dGrey05;
  Grey06 = dGrey06;
  Grey07 = dGrey07;
  Grey08 = dGrey08;
  Grey09 = dGrey09;
  Grey10 = dGrey10;
  Grey11 = dGrey11;
  Grey12 = dGrey12;
  Grey13 = dGrey13;
  Grey14 = dGrey14;
  Grey15 = dGrey15;
  Grey16 = dGrey16;
  Grey17 = dGrey17;
  Grey18 = dGrey18;
  Grey19 = dGrey19;

  // BELA Colormap
  Color19 = dColor19;
  Color18 = dColor18;
  Color17 = dColor17;
  Color16 = dColor16;
  Color15 = dColor15;
  Color14 = dColor14;
  Color13 = dColor13;
  Color12 = dColor12;
  Color11 = dColor11;
  Color10 = dColor10;
  Color09 = dColor09;
  Color08 = dColor08;
  Color07 = dColor07;
  Color06 = dColor06;
  Color05 = dColor05;
  Color04 = dColor04;
  Color03 = dColor03;
  Color02 = dColor02;
  Color01 = dColor01;
  Color00 = dColor00;

  // Other colors
  SelColor = dSelColor;
  MeshColor = dMeshColor;
  BlockColor = dBlockColor;
  LineColor = dLineColor;
  RegionColor = dRegionColor;
  GridColor = dGridColor;
  BackColor = dBackColor;
  NodeColor = dNodeColor;
  TextColor = dTextColor;
  RealFluxLineColor = dRealFluxLineColor;
  ImagFluxLineColor = dImagFluxLineColor;
  MaskLineColor = dMaskLineColor;
  NameColor = dNameColor;
  RealVectorColor = dRealVectorColor;
  ImagVectorColor = dImagVectorColor;

  BinDir = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  ScanPreferences();

  // Apply default behaviors
  EditAction = d_EditAction;
  GridFlag = d_GridFlag;
  SnapFlag = d_SnapFlag;
  MeshFlag = d_MeshFlag;
  LegendFlag = d_LegendFlag;
  GreyContours = d_GreyContours;
  NumContours = d_NumContours;
  ShowAr = d_ShowAr;
  ShowAi = d_ShowAi;
  ShowMask = d_ShowMask;
  DensityPlot = d_DensityPlot;
  VectorPlot = d_VectorPlot;
  VectorScaleFactor = 1;
  PtsFlag = d_PtsFlag;
  ShowNames = d_ShowNames;
  bOnDraw = FALSE;
}

CFemmviewView::~CFemmviewView()
{
  ((CFemmApp*)AfxGetApp())->NumViews--;
  if (!((CFemmApp*)AfxGetApp())->NumViews)
    ((CMainFrame*)AfxGetApp()->GetMainWnd())->SetBar(0);
}

BOOL CFemmviewView::PreCreateWindow(CREATESTRUCT& cs)
{
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFemmviewView drawing
void CFemmviewView::ScreenToDwg(int xs, int ys, double* xd, double* yd, RECT* r)
{
  double x, y;
  x = (double)xs;
  y = (double)ys;

  *xd = x / mag + ox;
  *yd = (((double)r->bottom) - y - 1.) / mag + oy;
}

BOOL CFemmviewView::DwgToScreen(double xd, double yd, int* xs, int* ys, RECT* r)
{
  double x, y;
  x = mag * (xd - ox);
  y = ((double)r->bottom) - 1. - mag * (yd - oy);
  //	if((fabs(x)>32768.) || (fabs(y)>32768.)) return FALSE;
  if ((fabs(x) > 2.147483648e9) || (fabs(y) > 2.147483648e9))
    return FALSE;
  *xs = (int)(mag * (xd - ox));
  *ys = ((int)r->bottom) - 1 - ((int)(mag * (yd - oy)));
  return TRUE;
}

void CFemmviewView::DoContours(CDC* pDC, int* p, int side, int Aflag)
{
  int i, j, k;
  static double q[4][4];
  static double* m[4];
  static int u[4];
  double* swap;
  double hi, lo, a, b, z, offset;
  double a0, a1, r0, r1, md;

  CFemmviewDoc* pDoc = GetDocument();

  switch (side) {
  case 0:
    u[0] = p[0];
    u[1] = p[1];
    u[2] = p[1];
    u[3] = p[2];
    break;
  case 1:
    u[0] = p[1];
    u[1] = p[2];
    u[2] = p[2];
    u[3] = p[0];
    break;
  default:
    u[0] = p[2];
    u[1] = p[0];
    u[2] = p[0];
    u[3] = p[1];
    break;
  }

  for (i = 0; i < 4; i++) {
    m[i] = q[i];
    m[i][0] = (double)pDoc->meshnode[u[i]].xs;
    m[i][1] = (double)pDoc->meshnode[u[i]].ys;
    m[i][3] = pDoc->meshnode[u[i]].x;
    if (Aflag == 0)
      m[i][2] = pDoc->meshnode[u[i]].A.re;
    if (Aflag == 1)
      m[i][2] = pDoc->meshnode[u[i]].A.im;
    if (Aflag == 2)
      m[i][2] = pDoc->meshnode[u[i]].msk;
  }

  // scale A for easier analysis.  Not necessary if mask is being displayed,
  // since mask always varies between zero and one.

  if (Aflag < 2) {
    for (i = 0; i < 4; i++)
      m[i][2] = NumContours * (m[i][2] - pDoc->A_Low) / (pDoc->A_High - pDoc->A_Low);
    offset = -NumContours * pDoc->A_Low / (pDoc->A_High - pDoc->A_Low);
  } else {
    for (i = 0; i < 4; i++)
      m[i][2] = NumContours * (m[i][2] - MSKLO) / (MSKHI - MSKLO);
    offset = -NumContours * MSKLO / (MSKHI - MSKLO);
  }

  // swap around so that lowest magnitude is first point in
  // each line...
  if (m[0][2] > m[1][2]) {
    swap = m[1];
    m[1] = m[0];
    m[0] = swap;
  }
  if (m[2][2] > m[3][2]) {
    swap = m[3];
    m[3] = m[2];
    m[2] = swap;
  }

  // figure out which contours lie in this section
  lo = m[0][2];
  if (m[2][2] > lo)
    lo = m[2][2];
  hi = m[1][2];
  if (m[3][2] < hi)
    hi = m[3][2];
  if (fabs(hi - lo) < 0.0000000001)
    return;
  j = (int)ceil(lo);
  k = (int)floor(hi);

  for (i = j; i <= k; i++)
    if ((i >= 0) && (i < NumContours)) {
      z = (double)i;
      a = (z - m[0][2]) / (m[1][2] - m[0][2]);
      b = (z - m[2][2]) / (m[3][2] - m[2][2]);

      if ((pDoc->ProblemType == 1) && (Aflag != 2)) {

        r0 = m[0][3];
        r1 = m[1][3];
        if (fabs(r0) > 1.e-06)
          a0 = (m[0][2] - offset) / r0;
        else
          a0 = 0;
        if (fabs(r1) > 1.e-06)
          a1 = (m[1][2] - offset) / r1;
        else
          a1 = 0;
        md = (a0 + a1) * (r0 + r1) / 4. + offset;

        if ((md > m[0][2]) && (md < m[1][2]) && (fabs(2. * md - m[0][2] - m[1][2]) > 1.e-2)) {
          a = (3. * m[0][2] + m[1][2] - 4. * md + sqrt(pow(3. * m[0][2] + m[1][2] - 4. * md, 2.) - 8. * (m[0][2] + m[1][2] - 2. * md) * (m[0][2] - z))) / (4. * (m[0][2] + m[1][2] - 2 * md));
        }

        r0 = m[2][3];
        r1 = m[3][3];
        if (fabs(r0) > 1.e-06)
          a0 = (m[2][2] - offset) / r0;
        else
          a0 = 0;
        if (fabs(r1) > 1.e-06)
          a1 = (m[3][2] - offset) / r1;
        else
          a1 = 0;
        md = (a0 + a1) * (r0 + r1) / 4. + offset;

        if ((md > m[2][2]) && (md < m[3][2]) && (fabs(2. * md - m[2][2] - m[3][2]) > 1.e-2)) {
          b = (3. * m[2][2] + m[3][2] - 4. * md + sqrt(pow(3. * m[2][2] + m[3][2] - 4. * md, 2.) - 8. * (m[2][2] + m[3][2] - 2. * md) * (m[2][2] - z))) / (4. * (m[2][2] + m[3][2] - 2 * md));
        }
      }

      MyMoveTo(pDC, (int)((m[1][0] - m[0][0]) * a + m[0][0]),
          (int)((m[1][1] - m[0][1]) * a + m[0][1]));
      MyLineTo(pDC, (int)((m[3][0] - m[2][0]) * b + m[2][0]),
          (int)((m[3][1] - m[2][1]) * b + m[2][1]));

      if (Aflag == 0)
        pDC->SetPixel((int)((m[3][0] - m[2][0]) * b + m[2][0]),
            (int)((m[3][1] - m[2][1]) * b + m[2][1]), RealFluxLineColor);

      if (Aflag == 1)
        pDC->SetPixel((int)((m[3][0] - m[2][0]) * b + m[2][0]),
            (int)((m[3][1] - m[2][1]) * b + m[2][1]), ImagFluxLineColor);

      if (Aflag == 2)
        pDC->SetPixel((int)((m[3][0] - m[2][0]) * b + m[2][0]),
            (int)((m[3][1] - m[2][1]) * b + m[2][1]), MaskLineColor);
    }
}
////////////////
void CFemmviewView::PlotFluxDensity(CDC* pDC, int elmnum, int flag)
{
  if (flag == 0)
    return;

  int i, j, k, n, lav;
  double b, bn[3], bh, bl, y, z;
  CComplex b1, b2, h1, h2, J[3], A[3];
  POINT ps[3];
  static double c[64][3], p[3][2];
  CElement elm;
  CFemmviewDoc* pDoc = GetDocument();

  elm = pDoc->meshelem[elmnum];

  if (((pDoc->meshnode[elm.p[0]].xs == pDoc->meshnode[elm.p[1]].xs) && (pDoc->meshnode[elm.p[1]].xs == pDoc->meshnode[elm.p[2]].xs)) || ((pDoc->meshnode[elm.p[0]].ys == pDoc->meshnode[elm.p[1]].ys) && (pDoc->meshnode[elm.p[1]].ys == pDoc->meshnode[elm.p[2]].ys)))
    return;

  if (pDoc->blocklist[elm.lbl].IsSelected == TRUE)
    return;

  const COLORREF mymap[] = {
    Color00, Color01, Color02, Color03, Color04,
    Color05, Color06, Color07, Color08, Color09,
    Color10, Color11, Color12, Color13, Color14,
    Color15, Color16, Color17, Color18, Color19
  };

  const COLORREF greymap[] = {
    Grey00, Grey01, Grey02, Grey03, Grey04,
    Grey05, Grey06, Grey07, Grey08, Grey09,
    Grey10, Grey11, Grey12, Grey13, Grey14,
    Grey15, Grey16, Grey17, Grey18, Grey19
  };

  bl = pDoc->PlotBounds[flag - 1][0];
  bh = pDoc->PlotBounds[flag - 1][1];

  if (pDoc->Frequency != 0)
    switch (flag) {
    case 1: // |B|

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        bn[i] = sqrt(b1.re * b1.re + b1.im * b1.im + b2.re * b2.re + b2.im * b2.im);
      }

      break;

    case 2: // Re(B)

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        bn[i] = sqrt(b1.re * b1.re + b2.re * b2.re);
      }

      break;

    case 3: // Im(B)

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        bn[i] = sqrt(b1.im * b1.im + b2.im * b2.im);
      }

      break;

    case 4: // |H|

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        pDoc->GetH(b1, b2, h1, h2, elmnum);
        bn[i] = sqrt(h1.re * h1.re + h1.im * h1.im + h2.re * h2.re + h2.im * h2.im);
      }

      break;

    case 5: // Re(H)

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        pDoc->GetH(b1, b2, h1, h2, elmnum);
        bn[i] = sqrt(h1.re * h1.re + h2.re * h2.re);
      }

      break;

    case 6: // Im(H)

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        pDoc->GetH(b1, b2, h1, h2, elmnum);
        bn[i] = sqrt(h1.im * h1.im + h2.im * h2.im);
      }

      break;

    case 7: // |J|
      pDoc->GetJA(elmnum, J, A);
      for (i = 0; i < 3; i++)
        bn[i] = abs(J[i]) * 1.e-6;
      break;

    case 8: // Re(J)
      pDoc->GetJA(elmnum, J, A);
      for (i = 0; i < 3; i++)
        bn[i] = fabs(J[i].re) * 1.e-6;
      break;

    case 9: // Im(J)
      pDoc->GetJA(elmnum, J, A);
      for (i = 0; i < 3; i++)
        bn[i] = fabs(J[i].im) * 1.e-6;
      break;

    case 10: // log(|B|)
      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        bn[i] = sqrt(b1.re * b1.re + b1.im * b1.im + b2.re * b2.re + b2.im * b2.im);
        if (bn[i] != 0)
          bn[i] = log10(bn[i]);
        else
          bn[i] = -12.;
        if (bn[i] < -12.)
          bn[i] = -12.;
      }
      break;
    }
  else
    switch (flag) {
    case 1: // |B|

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        bn[i] = sqrt(b1.re * b1.re + b1.im * b1.im + b2.re * b2.re + b2.im * b2.im);
      }
      break;

    case 2: // |H|

      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        pDoc->GetH(b1.re, b2.re, h1.re, h2.re, elmnum);
        bn[i] = sqrt(h1.re * h1.re + h2.re * h2.re);
      }
      break;

    case 3: // |J|

      pDoc->GetJA(elmnum, J, A);
      for (i = 0; i < 3; i++)
        bn[i] = abs(J[i]) * 1.e-6;
      break;

    case 4: // log(|B|)
      for (i = 0; i < 3; i++) {
        if (pDoc->Smooth == TRUE) {
          b1 = elm.b1[i];
          b2 = elm.b2[i];
        } else {
          b1 = elm.B1;
          b2 = elm.B2;
        }
        bn[i] = sqrt(b1.re * b1.re + b1.im * b1.im + b2.re * b2.re + b2.im * b2.im);
        if (bn[i] != 0)
          bn[i] = log10(bn[i]);
        else
          bn[i] = -12.;
        if (bn[i] < -12.)
          bn[i] = -12.;
      }
      break;
    }

  if (bh == bl)
    return;
  else
    for (i = 0; i < 3; i++)
      bn[i] = 20. * (bn[i] - bl) / (bh - bl);

  // find subtriangle edges;
  for (i = 0, n = 0; i < 3; i++) {
    c[n][0] = (double)pDoc->meshnode[elm.p[i]].xs;
    c[n][1] = (double)pDoc->meshnode[elm.p[i]].ys;
    c[n][2] = bn[i];
    n++;
    if (n >= 64)
      return;
    j = i + 1;
    if (j == 3)
      j = 0;
    p[0][0] = (double)pDoc->meshnode[elm.p[i]].xs;
    p[0][1] = (double)pDoc->meshnode[elm.p[i]].ys;
    p[1][0] = (double)pDoc->meshnode[elm.p[j]].xs;
    p[1][1] = (double)pDoc->meshnode[elm.p[j]].ys;
    p[2][0] = bn[i];
    p[2][1] = bn[j];
    if (p[2][0] < p[2][1])
      for (z = ceil(p[2][0]); z < p[2][1]; z++) {
        y = (z - p[2][0]) / (p[2][1] - p[2][0]);
        c[n][0] = (1. - y) * p[0][0] + y * p[1][0];
        c[n][1] = (1. - y) * p[0][1] + y * p[1][1];
        c[n][2] = (1. - y) * p[2][0] + y * p[2][1];
        if ((z >= 0) && (z < 20))
          n++;
        if (n >= 64)
          return;
      }
    else
      for (z = floor(p[2][0]); z > p[2][1]; z--) {
        y = (z - p[2][0]) / (p[2][1] - p[2][0]);
        c[n][0] = (1. - y) * p[0][0] + y * p[1][0];
        c[n][1] = (1. - y) * p[0][1] + y * p[1][1];
        c[n][2] = (1. - y) * p[2][0] + y * p[2][1];
        if ((z >= 0) && (z < 20))
          n++;
        if (n >= 64)
          return;
      }
  }

  // subtriangle vertices are now listed in the c[][] array.
  // there are n vertices.  Now, plot out these subtriangles;

  do {
    // find lowest vertex & neighbors;
    for (i = 1, j = 0; i < n; i++)
      if (c[i][2] < c[j][2])
        j = i;
    i = j - 1;
    if (i < 0)
      i = n - 1;
    k = j + 1;
    if (k >= n)
      k = 0;

    // plot triangle;
    b = (c[i][2] + c[j][2] + c[k][2]) / 3.;
    lav = 19 - ((int)b);
    if (lav > 19)
      lav = 19;
    if (lav < 0)
      lav = 0;

    {
      CPen FillPen, *pOldPen;
      CBrush FillBrush, *pOldBrush;
      BOOL DrawIt = TRUE;

      if (GreyContours == FALSE) {
        if (mymap[lav] == BackColor)
          DrawIt = FALSE;
        else {
          FillBrush.CreateSolidBrush(mymap[lav]);
          FillPen.CreatePen(PS_SOLID, 1, mymap[lav]);
        }
      } else {
        if (greymap[lav] == BackColor)
          DrawIt = FALSE;
        else {
          FillBrush.CreateSolidBrush(greymap[lav]);
          FillPen.CreatePen(PS_SOLID, 1, greymap[lav]);
        }
      }
      if (DrawIt == TRUE) {
        pOldBrush = pDC->SelectObject(&FillBrush);
        pOldPen = pDC->SelectObject(&FillPen);
        ps[0].x = (long)c[i][0];
        ps[0].y = (long)c[i][1];
        ps[1].x = (long)c[j][0];
        ps[1].y = (long)c[j][1];
        ps[2].x = (long)c[k][0];
        ps[2].y = (long)c[k][1];
        pDC->Polygon(ps, 3);
        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
      }
    }

    // remove middle point from the list;
    for (i = j; i < n - 1; i++)
      for (k = 0; k < 3; k++)
        c[i][k] = c[i + 1][k];
    n--;

  } while (n > 2);
}

///////////////

void CFemmviewView::PlotSelectedElm(CDC* pDC, CElement& elm)
{
  int i;
  POINT p[3];
  CFemmviewDoc* pDoc = GetDocument();

  if (elm.lbl != DrawSelected)
    if (pDoc->blocklist[elm.lbl].IsSelected == FALSE)
      return;

  for (i = 0; i < 3; i++) {
    p[i].x = pDoc->meshnode[elm.p[i]].xs;
    p[i].y = pDoc->meshnode[elm.p[i]].ys;
  }

  CPen FillPen, *pOldPen;
  CBrush FillBrush, *pOldBrush;
  if (pDoc->blocklist[elm.lbl].IsSelected == TRUE) {
    FillBrush.CreateSolidBrush(RegionColor);
    FillPen.CreatePen(PS_SOLID, 1, RegionColor);
  } else {
    FillBrush.CreateSolidBrush(0x00ffffff);
    FillPen.CreatePen(PS_SOLID, 1, 0x00ffffff);
  }
  pOldBrush = pDC->SelectObject(&FillBrush);
  pOldPen = pDC->SelectObject(&FillPen);
  pDC->Polygon(p, 3);
  pDC->SelectObject(pOldPen);
  pDC->SelectObject(pOldBrush);
}

void CFemmviewView::DrawUserContour(BOOL flag)
{
  // FALSE == just draw last line in contour;
  // TRUE  == draw all lines in contour;
  int i, xs, ys, xso, yso;

  CFemmviewDoc* pDoc = GetDocument();
  CDC* pDC = GetDC();
  RECT r;
  GetClientRect(&r);

  for (i = 0; i < pDoc->contour.GetSize(); i++) {
    DwgToScreen(pDoc->contour[i].re, pDoc->contour[i].im, &xs, &ys, &r);
    if ((flag == TRUE) || (i == (int)pDoc->contour.GetSize() - 1)) {
      if (i == 0) {
        SpecialLine(pDC, xs - 2, ys, xs + 2, ys, FALSE);
        SpecialLine(pDC, xs, ys - 2, xs, ys + 2, FALSE);
        xso = xs;
        yso = ys;
      } else {
        SpecialLine(pDC, xso, yso, xs, ys, FALSE);
        xso = xs;
        yso = ys;
      }
    } else {
      xso = xs;
      yso = ys;
    }
  }
  ReleaseDC(pDC);
}

void CFemmviewView::EraseUserContour(BOOL flag)
{
  // FALSE == just draw last line in contour;
  // TRUE  == draw all lines in contour;
  int i, xs, ys, xso, yso;

  CFemmviewDoc* pDoc = GetDocument();
  CDC* pDC = GetDC();
  RECT r;
  GetClientRect(&r);

  for (i = 0; i < pDoc->contour.GetSize(); i++) {
    DwgToScreen(pDoc->contour[i].re, pDoc->contour[i].im, &xs, &ys, &r);
    if ((flag == TRUE) || (i == (int)pDoc->contour.GetSize() - 1)) {
      if (i == 0) {
        SpecialLine(pDC, xs - 2, ys, xs + 2, ys, TRUE);
        SpecialLine(pDC, xs, ys - 2, xs, ys + 2, TRUE);
        xso = xs;
        yso = ys;
      } else {
        SpecialLine(pDC, xso, yso, xs, ys, TRUE);
        xso = xs;
        yso = ys;
      }
    } else {
      xso = xs;
      yso = ys;
    }
  }
  ReleaseDC(pDC);
}

BOOL CFemmviewView::Pump()
{
  // Idea here is to service the message pump during redraws that take a long time.
  // We don't actually have to check the messages every time Pump is called, because that
  // slows things down.  Just check every once in a while when Pump is called.

  static int k = 0;

  if (k++ < 1000)
    return FALSE;
  k = 0;

  MSG msg;

  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_INPUT | PM_QS_PAINT | PM_QS_SENDMESSAGE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return FALSE;
}

void CFemmviewView::OnDraw(CDC* pDC)
{
  CFemmviewDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  bOnDraw = TRUE;

  RECT r;
  static RECT oldr;
  GetClientRect(&r);
  int i, j, k; // usual iterators...
  int xs, ys, nx, ny;
  double xd, yd, dt, R;
  double xss, yss, rss, rt;
  CComplex p, c, s;

  const COLORREF mymap[] = {
    Color00, Color01, Color02, Color03, Color04,
    Color05, Color06, Color07, Color08, Color09,
    Color10, Color11, Color12, Color13, Color14,
    Color15, Color16, Color17, Color18, Color19
  };

  const COLORREF greymap[] = {
    Grey00, Grey01, Grey02, Grey03, Grey04,
    Grey05, Grey06, Grey07, Grey08, Grey09,
    Grey10, Grey11, Grey12, Grey13, Grey14,
    Grey15, Grey16, Grey17, Grey18, Grey19
  };

  CFont fntArial, *pOldFont;
  fntArial.CreateFont(16, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  pOldFont = pDC->SelectObject(&fntArial);

  if (pDoc->FirstDraw < 0) {
    pDoc->Smooth = d_Smooth;
    pDoc->FirstDraw = FALSE;
  }

  if (pDoc->FirstDraw == TRUE) {
    // Apply default behaviors
    ox = 0.;
    oy = 0.;
    mag = 100.;
    GridSize = 0.25;
    Coords = FALSE;
    ZoomWndFlag = FALSE;
    DrawSelected = -1;
    VectorScaleFactor = 1;
    if (pDoc->bMultiplyDefinedLabels) {
      EditAction = 2;
      ShowNames = TRUE;
    } else {
      EditAction = d_EditAction;
      ShowNames = d_ShowNames;
    }
    GridFlag = d_GridFlag;
    SnapFlag = d_SnapFlag;
    MeshFlag = d_MeshFlag;

    LegendFlag = d_LegendFlag;
    GreyContours = d_GreyContours;
    NumContours = d_NumContours;
    ShowAr = d_ShowAr;
    ShowAi = d_ShowAi;
    ShowMask = d_ShowMask;
    DensityPlot = d_DensityPlot;
    if (pDoc->Frequency == 0) {
      switch (DensityPlot) {
      case 1:
      case 2:
      case 3:
        DensityPlot = 1;
        break;
      case 4:
      case 5:
      case 6:
        DensityPlot = 2;
        break;
      case 7:
      case 8:
      case 9:
        DensityPlot = 3;
        break;
      default:
        DensityPlot = 0;
        break;
      }
    }
    VectorPlot = d_VectorPlot;
    PtsFlag = d_PtsFlag;
    pDoc->Smooth = d_Smooth;

    // catch inconsistent default settings
    if (pDoc->Frequency == 0) {
      if (DensityPlot == 2)
        DensityPlot = 1;
      if (DensityPlot > 1)
        DensityPlot = 0;
      d_ShowAi = FALSE;
    }

    OnZoomNatural();
    LinePix.RemoveAll();
    oldr = r;
    OnViewInfo();
  }

  if ((oldr.right != r.right) || (oldr.bottom != r.bottom)) {
    LinePix.RemoveAll();
    oldr = r;
  }

  if (pDC->IsPrinting() != FALSE) {
    CSize sz;
    RECT cr;
    int wd = pDC->GetDeviceCaps(HORZRES);
    int hd = pDC->GetDeviceCaps(VERTRES);
    int ww = r.right - r.left;
    int hw = r.bottom - r.top;
    int x = wd / ww;
    int y = hd / hw;
    if (y < x)
      x = y;
    pDC->SetMapMode(MM_ISOTROPIC);
    sz.cx = ww;
    sz.cy = hw;
    pDC->SetWindowExt(sz);
    sz.cx = x * ww;
    sz.cy = x * hw;
    pDC->SetViewportExt(sz);
    pDC->GetClipBox(&cr);
    pDC->IntersectClipRect(&r);
  }
  // just give up and make sure that the right
  // buttons are checked every time through OnDraw;
  // otherwise, it's just too easy to get the buttons
  // so that they don't match the actual mode.
  else {
    CMainFrame* pFrame = (CMainFrame*)GetTopLevelFrame();
    CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
    CView* pView = pChild->GetActiveView();
    if (pView == this)
      CheckIt();
  }

  CPen* pOldPen;
  CPen penBlue, penRed, penBlack, penGreen,
      penMesh, penReal, penImag, penMask,
      penNode, penText, penRealVector, penImagVector;
  penBlue.CreatePen(PS_SOLID, 1, LineColor);
  penRed.CreatePen(PS_SOLID, 1, SelColor);
  penReal.CreatePen(PS_SOLID, 1, RealFluxLineColor);
  penImag.CreatePen(PS_SOLID, 1, ImagFluxLineColor);
  penMask.CreatePen(PS_SOLID, 1, MaskLineColor);
  penGreen.CreatePen(PS_SOLID, 1, BlockColor);
  penMesh.CreatePen(PS_SOLID, 1, MeshColor);
  penNode.CreatePen(PS_SOLID, 1, NodeColor);
  penText.CreatePen(PS_SOLID, 1, TextColor);
  penRealVector.CreatePen(PS_SOLID, 1, RealVectorColor);
  penImagVector.CreatePen(PS_SOLID, 1, ImagVectorColor);

  // Convert node coordinates to screen coordinates
  for (i = 0; i < pDoc->nodelist.GetSize(); i++)
    if (DwgToScreen(pDoc->nodelist[i].x, pDoc->nodelist[i].y,
            &(pDoc->nodelist[i].xs), &(pDoc->nodelist[i].ys), &r)
        == FALSE) {
      mag /= 2.;
      i = -1;
    }

  // Convert meshnode coordinates to screen coordinates
  for (i = 0; i < pDoc->meshnode.GetSize(); i++)
    DwgToScreen(pDoc->meshnode[i].x, pDoc->meshnode[i].y,
        &(pDoc->meshnode[i].xs), &(pDoc->meshnode[i].ys), &r);

  // get center and radius of the screen in drawing coords.
  // can then use to find out whether elements are off the screen.
  xs = (r.right + r.left) / 2;
  ys = (r.top + r.bottom) / 2;
  ScreenToDwg(xs, ys, &xss, &yss, &r);
  rss = sqrt(pow((double)r.bottom, 2.) + pow((double)r.right, 2.)) / (2. * mag);

  // Draw selected area...
  if (EditAction == 2) {
    for (i = 0; i < pDoc->meshelem.GetSize(); i++) {
      if ((pDoc->meshelem[i].lbl == DrawSelected) || (DrawSelected == -1)) {
        rt = sqrt(pow(xss - pDoc->meshelem[i].ctr.re, 2.) + pow(yss - pDoc->meshelem[i].ctr.im, 2.));
        if (rt < (sqrt(pDoc->meshelem[i].rsqr) + rss))
          PlotSelectedElm(pDC, pDoc->meshelem[i]);
      }
    }
  }

  // Draw flux densities if they are enabled...
  if (DensityPlot != 0) {
    for (i = 0; i < pDoc->meshelem.GetSize(); i++) {
      if ((pDoc->meshelem[i].lbl == DrawSelected) || (DrawSelected == -1)) {
        rt = abs(CComplex(xss, yss) - pDoc->meshelem[i].ctr);
        if (rt < (sqrt(pDoc->meshelem[i].rsqr) + rss)) {
          PlotFluxDensity(pDC, i, DensityPlot);
          Pump();
        }
      }
    }
  }

  // Draw grid if it is enabled...
  if (GridFlag == TRUE) {
    int skip;
    BOOL drawgrid = TRUE;

    ScreenToDwg((int)r.right, (int)r.top, &xd, &yd, &r);
    nx = (int)(floor(xd / GridSize) - ceil(ox / GridSize));
    ny = (int)(floor(yd / GridSize) - ceil(oy / GridSize));

    if (nx > 0) {
      if ((r.right / nx) < 2)
        drawgrid = FALSE;
    }

    if (drawgrid == FALSE) {
      StatBar->SetPaneText(0, "Grid too dense to display.", TRUE);
    } else {
      skip = __min((nx / 20) + 1, (ny / 20) + 1);
      skip = (int)pow(2., floor(log((double)skip) / log(2.)));
      for (i = 0, xd = GridSize * ceil(ox / GridSize); i <= nx; i++, xd += GridSize) {
        for (j = 0, yd = GridSize * ceil(oy / GridSize); j <= ny; j++, yd += GridSize) {
          if ((i == skip * (i / skip)) || (j == skip * (j / skip))) {
            DwgToScreen(xd, yd, &xs, &ys, &r);
            pDC->SetPixel((int)xs, (int)ys, GridColor);
          }
        }
      }
    }
  }

  // Draw mesh if it is enabled...
  if (MeshFlag == TRUE) {
    int pi, po, OnBoundary;

    pOldPen = pDC->SelectObject(&penMesh);
    for (i = 0; i < pDoc->meshelem.GetSize(); i++) {
      if ((pDoc->meshelem[i].lbl == DrawSelected) || (DrawSelected == -1)) {

        OnBoundary = FALSE;
        for (j = 0; j < 3; j++)
          if (pDoc->meshelem[i].n[j] == 1)
            OnBoundary = TRUE;

        for (j = 0; j < 3; j++) {
          k = j + 1;
          if (k == 3)
            k = 0;
          pi = pDoc->meshelem[i].p[j];
          po = pDoc->meshelem[i].p[k];

          if ((po > pi) || (OnBoundary == TRUE)) {
            MyMoveTo(pDC, pDoc->meshnode[pi].xs,
                pDoc->meshnode[pi].ys);
            MyLineTo(pDC, pDoc->meshnode[po].xs,
                pDoc->meshnode[po].ys);
          }
        }
        Pump();
      }
    }
    pDC->SelectObject(pOldPen);
  }

  // Draw contour lines, if they are enabled.
  pOldPen = pDC->SelectObject(&penReal);
  if (ShowAr == TRUE)
    for (i = 0; i < pDoc->meshelem.GetSize(); i++) {
      if ((pDoc->meshelem[i].lbl == DrawSelected) || (DrawSelected == -1)) {
        rt = sqrt(pow(xss - pDoc->meshelem[i].ctr.re, 2.) + pow(yss - pDoc->meshelem[i].ctr.im, 2.));
        if (rt < (sqrt(pDoc->meshelem[i].rsqr) + rss))
          for (j = 0; j < 3; j++)
            DoContours(pDC, pDoc->meshelem[i].p, j, 0);
        Pump();
      }
    }

  pDC->SelectObject(&penImag);
  if (ShowAi == TRUE)
    for (i = 0; i < pDoc->meshelem.GetSize(); i++) {
      if ((pDoc->meshelem[i].lbl == DrawSelected) || (DrawSelected == -1)) {
        rt = sqrt(pow(xss - pDoc->meshelem[i].ctr.re, 2.) + pow(yss - pDoc->meshelem[i].ctr.im, 2.));
        if (rt < (sqrt(pDoc->meshelem[i].rsqr) + rss))
          for (j = 0; j < 3; j++)
            DoContours(pDC, pDoc->meshelem[i].p, j, 1);
        Pump();
      }
    }

  pDC->SelectObject(&penMask);
  if (ShowMask == TRUE)
    for (i = 0; i < pDoc->meshelem.GetSize(); i++) {
      if ((pDoc->meshelem[i].lbl == DrawSelected) || (DrawSelected == -1)) {
        rt = sqrt(pow(xss - pDoc->meshelem[i].ctr.re, 2.) + pow(yss - pDoc->meshelem[i].ctr.im, 2.));
        if (rt < (sqrt(pDoc->meshelem[i].rsqr) + rss))
          for (j = 0; j < 3; j++)
            DoContours(pDC, pDoc->meshelem[i].p, j, 2);
        Pump();
      }
    }

  pDC->SelectObject(pOldPen);

  // Draw lines linking nodes
  for (i = 0; i < pDoc->linelist.GetSize(); i++)
    if (pDoc->linelist[i].Hidden == FALSE) {
      if (pDoc->linelist[i].IsSelected == FALSE)
        pOldPen = pDC->SelectObject(&penBlue);
      else {
        //			if (EditAction==2) pOldPen = pDC->SelectObject( &penGreen );
        //			else
        pOldPen = pDC->SelectObject(&penRed);
      }

      MyMoveTo(pDC, pDoc->nodelist[pDoc->linelist[i].n0].xs,
          pDoc->nodelist[pDoc->linelist[i].n0].ys);
      MyLineTo(pDC, pDoc->nodelist[pDoc->linelist[i].n1].xs,
          pDoc->nodelist[pDoc->linelist[i].n1].ys);

      pDC->SelectObject(pOldPen);
    }

  // Draw Arc Segments;
  for (i = 0; i < pDoc->arclist.GetSize(); i++)
    if (pDoc->arclist[i].Hidden == FALSE) {
      if (pDoc->arclist[i].IsSelected == FALSE)
        pOldPen = pDC->SelectObject(&penBlue);
      else {
        //			if (EditAction==2) pOldPen = pDC->SelectObject( &penGreen );
        //			else
        pOldPen = pDC->SelectObject(&penRed);
      }
      k = (int)ceil(pDoc->arclist[i].ArcLength / pDoc->arclist[i].MaxSideLength);
      dt = pDoc->arclist[i].ArcLength * PI / (((double)k) * 180.);

      pDoc->GetCircle(pDoc->arclist[i], c, R);
      p.Set(pDoc->nodelist[pDoc->arclist[i].n0].x,
          pDoc->nodelist[pDoc->arclist[i].n0].y);
      DwgToScreen(p.re, p.im, &xs, &ys, &r);
      MyMoveTo(pDC, xs, ys);
      s = exp(I * dt);
      for (j = 0; j < k; j++) {
        p = (p - c) * s + c;
        DwgToScreen(p.re, p.im, &xs, &ys, &r);
        MyLineTo(pDC, xs, ys);
      }
      pDC->SelectObject(pOldPen);
    }

  // Draw node points
  if (PtsFlag == TRUE)
    for (i = 0; i < pDoc->nodelist.GetSize(); i++) {
      xs = pDoc->nodelist[i].xs;
      ys = pDoc->nodelist[i].ys;

      if (pDoc->nodelist[i].IsSelected == FALSE)
        pOldPen = pDC->SelectObject(&penNode);
      else
        pOldPen = pDC->SelectObject(&penRed);

      MyMoveTo(pDC, xs - 2, ys - 2);
      MyLineTo(pDC, xs - 2, ys + 2);
      MyLineTo(pDC, xs + 2, ys + 2);
      MyLineTo(pDC, xs + 2, ys - 2);
      MyLineTo(pDC, xs - 2, ys - 2);

      pDC->SelectObject(pOldPen);
    }

  // Draw vectors
  if ((VectorPlot > 0) && (pDoc->nodelist.GetSize() > 0)) {
    CPointVals u;
    BOOL drawgrid = TRUE;
    BOOL PlotB;
    double MaxVal;
    int xp, yp;
    CComplex vr, va, c0, c1;
    int ilo, ihi, jlo, jhi;

    if ((VectorPlot == 1) || (VectorPlot == 3) || (VectorPlot == 5)) {
      MaxVal = pDoc->B_High;
      PlotB = TRUE;
    } else {
      MaxVal = pDoc->H_High;
      PlotB = FALSE;
    }

    ScreenToDwg((int)r.right, (int)r.top, &xd, &yd, &r);
    nx = (int)(floor(xd / GridSize) - ceil(ox / GridSize));
    ny = (int)(floor(yd / GridSize) - ceil(oy / GridSize));

    if (nx > 0) {
      if ((r.right / nx) < 2)
        drawgrid = FALSE;
    }

    if (drawgrid == FALSE) {
      StatBar->SetPaneText(0, "Vectors too dense to display.", TRUE);
    } else {

      // VectorPlot = 1 -> B_re;
      // VectorPlot = 2 -> H_re;
      // VectorPlot = 3 -> B_im;
      // VectorPlot = 4 -> H_im;
      // VectorPlot = 5 -> B_re & B_im;
      // VectorPlot = 6 -> H_re & H_im;

      int MakePlots = (VectorPlot + 1) / 2;

      for (k = 0; k < pDoc->meshelem.GetSize(); k++) {
        rt = abs(CComplex(xss, yss) - pDoc->meshelem[k].ctr);
        if (rt < (sqrt(pDoc->meshelem[k].rsqr) + rss)) {
          c0 = pDoc->meshelem[k].ctr - sqrt(pDoc->meshelem[k].rsqr) * (1. + I);
          c1 = pDoc->meshelem[k].ctr + sqrt(pDoc->meshelem[k].rsqr) * (1. + I);
          c0 /= GridSize;
          c1 /= GridSize;
          ilo = (int)floor(Re(c0));
          ihi = (int)ceil(Re(c1));
          jlo = (int)floor(Im(c0));
          jhi = (int)ceil(Im(c1));
          for (i = ilo; i <= ihi; i++) {
            for (j = jlo; j <= jhi; j++) {
              xd = GridSize * ((double)i);
              yd = GridSize * ((double)j);
              if (pDoc->InTriangleTest(xd, yd, k)) {
                pDoc->GetPointValues(xd, yd, k, u);

                // Plot real vector
                if (MakePlots & 1) {
                  pOldPen = pDC->SelectObject(&penRealVector);
                  if (PlotB)
                    vr = Re(u.B1) + I * Re(u.B2);
                  else
                    vr = Re(u.H1) + I * Re(u.H2);
                  vr *= (GridSize * VectorScaleFactor / MaxVal);
                  DwgToScreen(xd, yd, &xs, &ys, &r);
                  MyMoveTo(pDC, xs, ys);
                  xp = xs;
                  yp = ys;
                  DwgToScreen(xd + Re(vr), yd + Im(vr), &xs, &ys, &r);
                  MyLineTo(pDC, xs, ys);
                  if ((xs == xp) && (ys == yp))
                    pDC->SetPixel((int)xs, (int)ys, RealVectorColor);
                  else if (abs(vr) != 0) {
                    xp = xs;
                    yp = ys;
                    va = exp(I * 3. * PI / 4.) * vr / 3.;
                    DwgToScreen(xd + Re(vr + va), yd + Im(vr + va), &xs, &ys, &r);
                    MyLineTo(pDC, xs, ys);
                    MyMoveTo(pDC, xp, yp);
                    va *= exp(I * PI / 2.);
                    DwgToScreen(xd + Re(vr + va), yd + Im(vr + va), &xs, &ys, &r);
                    MyLineTo(pDC, xs, ys);
                  }
                  pDC->SelectObject(pOldPen);
                }

                // Plot imag vector
                if (MakePlots & 2) {
                  pOldPen = pDC->SelectObject(&penImagVector);
                  if (PlotB)
                    vr = Im(u.B1) + I * Im(u.B2);
                  else
                    vr = Im(u.H1) + I * Im(u.H2);
                  vr *= (GridSize * VectorScaleFactor / MaxVal);
                  DwgToScreen(xd, yd, &xs, &ys, &r);
                  MyMoveTo(pDC, xs, ys);
                  xp = xs;
                  yp = ys;
                  DwgToScreen(xd + Re(vr), yd + Im(vr), &xs, &ys, &r);
                  MyLineTo(pDC, xs, ys);
                  if ((xs == xp) && (ys == yp))
                    pDC->SetPixel((int)xs, (int)ys, ImagVectorColor);
                  else if (abs(vr) != 0) {
                    xp = xs;
                    yp = ys;
                    va = exp(I * 3. * PI / 4.) * vr / 3.;
                    DwgToScreen(xd + Re(vr + va), yd + Im(vr + va), &xs, &ys, &r);
                    MyLineTo(pDC, xs, ys);
                    MyMoveTo(pDC, xp, yp);
                    va *= exp(I * PI / 2.);
                    DwgToScreen(xd + Re(vr + va), yd + Im(vr + va), &xs, &ys, &r);
                    MyLineTo(pDC, xs, ys);
                  }
                  pDC->SelectObject(pOldPen);
                }
                Pump();
              }
            }
          }
        } // end of if(rt<...
      }
    }
  }

  if (EditAction == 1)
    DrawUserContour(TRUE);

  // draw block labels/names
  if (ShowNames) {
    pDC->SetTextColor(NameColor);
    pOldPen = pDC->SelectObject(&penGreen);
    CString lbl;
    for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
      DwgToScreen(pDoc->blocklist[i].x, pDoc->blocklist[i].y, &xs, &ys, &r);

      MyMoveTo(pDC, xs - 2, ys - 2);
      MyLineTo(pDC, xs - 2, ys + 2);
      MyLineTo(pDC, xs + 2, ys + 2);
      MyLineTo(pDC, xs + 2, ys - 2);
      MyLineTo(pDC, xs - 2, ys - 2);

      // draw arrows indicating magnetization direction
      // if the block is a permanent magnet
      if ((pDoc->blockproplist[pDoc->blocklist[i].BlockType].H_c != 0) && (pDoc->blocklist[i].MagDirFctn.GetLength() == 0)) {
        int vx, vy;
        CComplex ar;
        ar = (cos(pDoc->blocklist[i].MagDir * PI / 180)) + I * (sin(pDoc->blocklist[i].MagDir * PI / 180));
        vx = xs - ((int)(10. * ar.re));
        vy = ys + ((int)(10. * ar.im));
        MyMoveTo(pDC, vx, vy);
        vx = xs + ((int)(10. * ar.re));
        vy = ys - ((int)(10. * ar.im));
        MyLineTo(pDC, vx, vy);
        ar /= (1. + I) / sqrt(2.);
        MyLineTo(pDC, vx - ((int)(6. * ar.re)), vy + ((int)(6 * ar.im)));
        MyMoveTo(pDC, vx, vy);
        ar *= I;
        MyLineTo(pDC, vx - ((int)(6. * ar.re)), vy + ((int)(6 * ar.im)));
      }

      pDC->SetTextAlign(TA_BASELINE);
      pDC->SetBkMode(TRANSPARENT);
      lbl = pDoc->blockproplist[pDoc->blocklist[i].BlockType].BlockName;
      pDC->TextOut(xs + 5, ys, lbl);

      pDC->SetTextAlign(TA_TOP);

      if (pDoc->blocklist[i].InCircuit >= 0) {
        k = pDoc->blocklist[i].InCircuit;
        if (pDoc->circproplist[k].CircType == 1)
          lbl.Format("[%s:%i]", pDoc->circproplist[k].CircName,
              pDoc->blocklist[i].Turns);
        else
          lbl = "[" + pDoc->circproplist[k].CircName + "]";
        pDC->TextOut(xs + 5, ys, lbl);
      }
    }
    pDC->SelectObject(pOldPen);
  }

  // Draw Legend if it is enabled;
  if ((LegendFlag == TRUE) && (DensityPlot != 0)) {
    CBrush* pOldBrush;
    char cc[80];
    double dta;
    CBrush BackBrush;

    pOldPen = pDC->SelectObject(&penText);
    pDC->SetTextColor(TextColor);
    pDC->SetTextAlign(TA_TOP);
    pDC->SetTextAlign(TA_LEFT);
    pDC->SetBkMode(TRANSPARENT);

    BackBrush.CreateSolidBrush(BackColor);
    pOldBrush = pDC->SelectObject(&BackBrush);
    pDC->Rectangle(r.right - 212 - 6, r.top + 6, r.right - 6, r.top + 24 + 21 * 16);
    pDC->SelectObject(pOldBrush);

    if (pDoc->Frequency != 0) {
      if (DensityPlot == 1)
        sprintf(cc, "Density Plot: |B|, Tesla");
      if (DensityPlot == 2)
        sprintf(cc, "Density Plot: |B_re|, Tesla");
      if (DensityPlot == 3)
        sprintf(cc, "Density Plot: |B_im|, Tesla");
      if (DensityPlot == 4)
        sprintf(cc, "Density Plot: |H|, A/m");
      if (DensityPlot == 5)
        sprintf(cc, "Density Plot: |H_re|, A/m");
      if (DensityPlot == 6)
        sprintf(cc, "Density Plot: |H_im|, A/m");
      if (DensityPlot == 7)
        sprintf(cc, "Density Plot: |J|, MA/m^2");
      if (DensityPlot == 8)
        sprintf(cc, "Density Plot: |J_re|, MA/m^2");
      if (DensityPlot == 9)
        sprintf(cc, "Density Plot: |J_im|, MA/m^2");
      if (DensityPlot == 10)
        sprintf(cc, "Density Plot: log10(|B|), log(Tesla)");
    } else {
      if (DensityPlot == 1)
        sprintf(cc, "Density Plot: |B|, Tesla");
      if (DensityPlot == 2)
        sprintf(cc, "Density Plot: |H|, A/m");
      if (DensityPlot == 3)
        sprintf(cc, "Density Plot: |J|, MA/m^2");
      if (DensityPlot == 4)
        sprintf(cc, "Density Plot: log10(|B|), log(Tesla)");
    }

    pDC->TextOut(r.right - 212, r.top + 16 * 20 + 16, cc, (int)strlen(cc));

    dta = (pDoc->PlotBounds[DensityPlot - 1][1] - pDoc->PlotBounds[DensityPlot - 1][0]) / 20.;

    for (i = 0; i < 20; i++) {

      {
        CBrush FillBrush;

        if (GreyContours == FALSE)
          FillBrush.CreateSolidBrush(mymap[i]);
        else
          FillBrush.CreateSolidBrush(greymap[i]);
        pOldBrush = pDC->SelectObject(&FillBrush);
        pDC->Rectangle(r.right - 212, r.top + 16 * i + 12, r.right - 187, r.top + 12 + 16 * (i + 1));
        pDC->SelectObject(pOldBrush);
      }

      {
        double B_lb = pDoc->PlotBounds[DensityPlot - 1][0];
        if (i == 0)
          sprintf(cc, "%.3e : >%.3e", ((double)(19 - i)) * dta + B_lb,
              ((double)(20 - i)) * dta + B_lb);
        else if (i == 19)
          sprintf(cc, "<%.3e : %.3e", ((double)(19 - i)) * dta + B_lb,
              ((double)(20 - i)) * dta + B_lb);
        else
          sprintf(cc, "%.3e : %.3e", ((double)(19 - i)) * dta + B_lb,
              ((double)(20 - i)) * dta + B_lb);
        pDC->TextOut(r.right - 182, r.top + 16 * i + 12, cc, (int)strlen(cc));
      }
    }
    pDC->SelectObject(pOldPen);
  }

  pDC->SelectObject(pOldFont);
  fntArial.DeleteObject();

  bOnDraw = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CFemmviewView printing

BOOL CFemmviewView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CFemmviewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CFemmviewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFemmviewView diagnostics

#ifdef _DEBUG
void CFemmviewView::AssertValid() const
{
  CView::AssertValid();
}

void CFemmviewView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CFemmviewDoc* CFemmviewView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFemmviewDoc)));
  return (CFemmviewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFemmviewView message handlers

void CFemmviewView::OnMouseMove(UINT nFlags, CPoint point)
{
  if ((bLinehook == NormalLua) || (bLinehook == HiddenLua)) {
    StatBar->SetPaneText(0, "EXECUTING LUASCRIPT -- HIT <ESC> TO ABORT", TRUE);
    CView::OnMouseMove(nFlags, point);
    return;
  }

  if (bLinehook == ImportDXF) {
    StatBar->SetPaneText(0, "IMPORTING DXF -- HIT <ESC> TO ABORT", TRUE);
    CView::OnMouseMove(nFlags, point);
    return;
  }

  if (bLinehook == BuildMask) {
    StatBar->SetPaneText(0, "BUILDING STRESS TENSOR MASK -- HIT <ESC> TO ABORT", TRUE);
    CView::OnMouseMove(nFlags, point);
    return;
  }

  static char statmsg[256];
  double x, y, q;
  RECT r;
  CFemmviewDoc* pDoc = GetDocument();

  // get size of current client area and convert mouse position into
  // position on the actual drawing.
  GetClientRect(&r);
  q = pow(10., floor(log(mag) / log(10.) + 0.5));
  x = (double)point.x;
  y = (double)(r.bottom - point.y - 1);
  x = x / mag + ox;
  y = y / mag + oy;
  x = floor(x * q + 0.5) / q;
  y = floor(y * q + 0.5) / q;

  // process Snap to Grid command
  if ((SnapFlag == TRUE) && (EditAction != 2) && (ZoomWndFlag == 0)) {
    x = GridSize * floor(0.5 + x / GridSize);
    y = GridSize * floor(0.5 + y / GridSize);
  }

  // draw box for window zoom;
  if (ZoomWndFlag == 2) {
    CDC* pDC = GetDC();
    COLORREF ocol;
    CZPix p;

    int xso, yso, xsi, ysi, xsn, ysn, i, k, lo, hi;
    DwgToScreen(wzx, wzy, &xso, &yso, &r);
    DwgToScreen(mx, my, &xsi, &ysi, &r);
    DwgToScreen(x, y, &xsn, &ysn, &r);

    // Draw old rectangle off the screen;
    if (ZoomPix.GetSize() > 0) {
      for (k = (int)ZoomPix.GetSize() - 1; k >= 0; k--)
        pDC->SetPixel(ZoomPix[k].x, ZoomPix[k].y, ZoomPix[k].c);
      ZoomPix.RemoveAll();
    }

    // XOR new rectangle onto screen
    if (xso < xsn) {
      lo = xso, hi = xsn;
    } else {
      lo = xsn;
      hi = xso;
    }
    for (i = lo; i <= hi; i += 4) {
      ocol = pDC->GetPixel(i, ysn);
      p.c = ocol;
      p.x = i;
      p.y = ysn;
      ZoomPix.Add(p);
      ocol = ocol ^ RGB(255, 255, 255);
      pDC->SetPixel(i, ysn, ocol);
      ocol = pDC->GetPixel(i, yso);
      p.c = ocol;
      p.x = i;
      p.y = yso;
      ZoomPix.Add(p);
      ocol = ocol ^ RGB(255, 255, 255);
      pDC->SetPixel(i, yso, ocol);
    }
    if (yso < ysn) {
      lo = yso, hi = ysn;
    } else {
      lo = ysn;
      hi = yso;
    }
    for (i = lo; i <= hi; i += 4) {
      ocol = pDC->GetPixel(xso, i);
      p.c = ocol;
      p.x = xso;
      p.y = i;
      ZoomPix.Add(p);
      ocol = ocol ^ RGB(255, 255, 255);
      pDC->SetPixel(xso, i, ocol);
      ocol = pDC->GetPixel(xsn, i);
      p.c = ocol;
      p.x = xsn;
      p.y = i;
      ZoomPix.Add(p);
      ocol = ocol ^ RGB(255, 255, 255);
      pDC->SetPixel(xsn, i, ocol);
    }

    ReleaseDC(pDC);
  }

  // update mouse position;
  mx = x;
  my = y;

  if ((pDoc->Coords == FALSE) && (pDoc->ProblemType == 0))
    sprintf(statmsg, "(x=%.4f,y=%.4f)", x, y);
  else if ((pDoc->Coords == FALSE) && (pDoc->ProblemType == 1))
    sprintf(statmsg, "(r=%.4f,z=%.4f)", x, y);
  else
    sprintf(statmsg, "(%.4f at %.4f deg)", sqrt(x * x + y * y), atan2(y, x) * 180 / PI);
  StatBar->SetPaneText(0, statmsg, TRUE);

  CView::OnMouseMove(nFlags, point);
}

void CFemmviewView::OnZoomIn()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;

  ox = ox + 0.25 * x / mag;
  oy = oy + 0.25 * y / mag;
  mag *= 2.;

  RedrawView();
}

void CFemmviewView::OnZoomOut()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox = ox - 0.5 * x / mag;
  oy = oy - 0.5 * y / mag;
  mag /= 2.;

  RedrawView();
}

void CFemmviewView::OnZoomNatural()
{
  CFemmviewDoc* pDoc = GetDocument();
  double x[2], y[2], m[2], w;
  RECT r;
  int i;

  if (pDoc->meshnode.GetSize() < 2)
    return;
  x[0] = pDoc->meshnode[0].x;
  x[1] = pDoc->meshnode[0].x;
  y[0] = pDoc->meshnode[0].y;
  y[1] = pDoc->meshnode[0].y;
  for (i = 1; i < pDoc->meshnode.GetSize(); i++) {
    if (pDoc->meshnode[i].x < x[0])
      x[0] = pDoc->meshnode[i].x;
    if (pDoc->meshnode[i].x > x[1])
      x[1] = pDoc->meshnode[i].x;
    if (pDoc->meshnode[i].y < y[0])
      y[0] = pDoc->meshnode[i].y;
    if (pDoc->meshnode[i].y > y[1])
      y[1] = pDoc->meshnode[i].y;
  }

  if (pDoc->FirstDraw == TRUE) {
    if ((x[1] - x[0]) > (y[1] - y[0]))
      w = x[1] - x[0];
    else
      w = y[1] - y[0];
    if (w != 0)
      GridSize = pow(10., floor(log(w) / log(10.) - 1.));
  }

  ox = x[0];
  oy = y[0];
  GetClientRect(&r);
  m[0] = ((double)(r.right - 1)) / (x[1] - x[0]);
  m[1] = ((double)(r.bottom - 1)) / (y[1] - y[0]);

  if (m[0] < m[1])
    mag = m[0];
  else
    mag = m[1];

  if (pDoc->FirstDraw == TRUE)
    pDoc->FirstDraw = FALSE;
  else
    RedrawView();
}

void CFemmviewView::OnKbdZoom()
{
  CFemmviewDoc* pDoc = GetDocument();
  CKbdZoom dlg;
  RECT r;
  double m[2], x[4], z;

  GetClientRect(&r);
  ScreenToDwg(r.right, r.top, &dlg.m_scr_right, &dlg.m_scr_top, &r);
  dlg.m_scr_top = floor(1000. * dlg.m_scr_top) / 1000.;
  dlg.m_scr_right = floor(1000. * dlg.m_scr_right) / 1000.;
  dlg.m_scr_bottom = oy;
  dlg.m_scr_left = ox;

  if (dlg.DoModal() == IDOK) {
    x[0] = dlg.m_scr_left;
    x[1] = dlg.m_scr_bottom;
    x[2] = dlg.m_scr_right;
    x[3] = dlg.m_scr_top;

    if (x[0] > x[2]) {
      z = x[2];
      x[2] = x[0];
      x[0] = z;
    }
    if (x[1] > x[3]) {
      z = x[3];
      x[3] = x[1];
      x[1] = z;
    }

    ox = x[0];
    oy = x[1];
    m[0] = ((double)(r.right - 1)) / (x[2] - x[0]);
    m[1] = ((double)(r.bottom - 1)) / (x[3] - x[1]);
    if (m[0] < m[1])
      mag = m[0];
    else
      mag = m[1];
    RedrawView();
  }
}

void CFemmviewView::OnZoomWindow()
{
  ZoomWndFlag = TRUE;
  ZoomPix.RemoveAll();
}

void CFemmviewView::OnShowGrid()
{
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  if (GridFlag == TRUE) {
    GridFlag = FALSE;
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SHOW_GRID, FALSE);
  } else {
    GridFlag = TRUE;
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_CHECKED);
    tc->PressButton(ID_SHOW_GRID, TRUE);
  }
  RedrawView();
}

void CFemmviewView::OnSetGrid()
{
  CGridMod pDlg;
  CFemmviewDoc* pDoc = GetDocument();

  // Send present parameter values to the dialog
  // pDlg.m_rji=TheDoc->vi[0];
  pDlg.m_gridsize = GridSize;
  pDlg.coords = pDoc->Coords;

  // Display dialog and collect data
  if (pDlg.DoModal() == IDOK) {
    GridSize = pDlg.m_gridsize;
    pDoc->Coords = pDlg.coords;
    RedrawView();
  }
}

void CFemmviewView::OnSnapGrid()
{
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  if (SnapFlag == TRUE) {
    SnapFlag = FALSE;
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SNAP_GRID, FALSE);
  } else {
    SnapFlag = TRUE;
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_CHECKED);
    tc->PressButton(ID_SNAP_GRID, TRUE);
  }
}

void CFemmviewView::OnShowMesh()
{
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  if (MeshFlag == TRUE) {
    MeshFlag = FALSE;
    MMnu->CheckMenuItem(ID_SHOW_MESH, MF_UNCHECKED);
    tc->PressButton(ID_SHOW_MESH, FALSE);
  } else {
    MeshFlag = TRUE;
    MMnu->CheckMenuItem(ID_SHOW_MESH, MF_CHECKED);
    tc->PressButton(ID_SHOW_MESH, TRUE);
  }

  RedrawView();
}

void CFemmviewView::OnCplot()
{
  CFemmviewDoc* pDoc = GetDocument();

  if (pDoc->Frequency != 0) {
    CCPlotDlg dlg;
    dlg.m_showar = ShowAr;
    dlg.m_showai = ShowAi;
    dlg.m_showmask = ShowMask;
    dlg.m_numcontours = NumContours;
    dlg.m_alow = pDoc->A_Low;
    dlg.m_ahigh = pDoc->A_High;
    dlg.Alb = pDoc->A_lb;
    dlg.Aub = pDoc->A_ub;
    if (dlg.DoModal() == IDOK) {
      ShowAr = dlg.m_showar;
      ShowAi = dlg.m_showai;
      ShowMask = dlg.m_showmask;
      pDoc->A_Low = dlg.m_alow;
      pDoc->A_High = dlg.m_ahigh;
      NumContours = dlg.m_numcontours;
      RedrawView();
    }
  } else {
    CCplotDlg2 dlg;
    dlg.m_showa = ShowAr;
    dlg.m_showmask = ShowMask;
    dlg.m_numcontours = NumContours;
    dlg.m_alow = pDoc->A_Low;
    dlg.m_ahigh = pDoc->A_High;
    dlg.Alb = pDoc->A_lb;
    dlg.Aub = pDoc->A_ub;
    if (dlg.DoModal() == IDOK) {
      ShowAr = dlg.m_showa;
      ShowMask = dlg.m_showmask;
      ShowAi = FALSE;
      pDoc->A_Low = dlg.m_alow;
      pDoc->A_High = dlg.m_ahigh;
      NumContours = dlg.m_numcontours;
      RedrawView();
    }
  }
}

void CFemmviewView::OnDplot()
{
  CFemmviewDoc* pDoc = GetDocument();
  int i, k;

  cvCDPlotDlg2 dlg;

  if (!DensityPlot)
    dlg.m_showit = FALSE;
  else
    dlg.m_showit = TRUE;
  dlg.m_showlegend = LegendFlag;
  dlg.m_gscale = GreyContours;
  dlg.DensityPlot = DensityPlot;

  if (pDoc->Frequency == 0) {
    dlg.listtype = 1;
    for (i = 0; i < 4; i++)
      for (k = 0; k < 2; k++) {
        dlg.PlotBounds[i][k] = pDoc->PlotBounds[i][k];
        dlg.d_PlotBounds[i][k] = pDoc->d_PlotBounds[i][k];
      }
  } else {
    dlg.listtype = 2;
    for (i = 0; i < 10; i++)
      for (k = 0; k < 2; k++) {
        dlg.PlotBounds[i][k] = pDoc->PlotBounds[i][k];
        dlg.d_PlotBounds[i][k] = pDoc->d_PlotBounds[i][k];
      }
  }

  if (dlg.DoModal() == IDOK) {
    if (dlg.m_showit)
      DensityPlot = dlg.DensityPlot;
    else
      DensityPlot = 0;
    LegendFlag = dlg.m_showlegend;
    GreyContours = dlg.m_gscale;
    for (i = 0; i < 10; i++)
      for (k = 0; k < 2; k++)
        pDoc->PlotBounds[i][k] = dlg.PlotBounds[i][k];
    RedrawView();
  }
}

void CFemmviewView::OnInitialUpdate()
{
  CView::OnInitialUpdate();
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  if (MFrm == NULL)
    MsgBox("Null GetTopLevelFrame");
  else {
    StatBar = (CStatusBar*)MFrm->GetMessageBar();
    Dspl = &MFrm->m_dlgBar;
  }
}

void CFemmviewView::DisplayPointProperties(double px, double py)
{
  CFemmviewDoc* pDoc = GetDocument();
  CPointVals u;
  CComplex Jtot;
  if (pDoc->nodelist.GetSize() == 0)
    return;
  if (pDoc->GetPointValues(px, py, u) == TRUE) {
    char c[80];
    CString s, outbox;
    if (pDoc->ProblemType >= 0) {
      if (pDoc->Frequency == 0) {
        if (pDoc->ProblemType == 0) {
          s.Format("Point: x=%g, y=%g\r\n", px, py);
          outbox = s;
          s.Format("A   = %g Wb/m\r\n", u.A.re);
          outbox += s;
          s.Format("|B| = %g T\r\n", sqrt(u.B1.re * u.B1.re + u.B2.re * u.B2.re));
          outbox += s;
          s.Format("Bx  = %g T\r\n", u.B1.re);
          outbox += s;
          s.Format("By  = %g T\r\n", u.B2.re);
          outbox += s;
          s.Format("|H| = %g A/m\r\n", sqrt(u.H1.re * u.H1.re + u.H2.re * u.H2.re));
          outbox += s;
          s.Format("Hx  = %g A/m\r\n", u.H1.re);
          outbox += s;
          s.Format("Hy  = %g A/m\r\n", u.H2.re);
          outbox += s;

          if (u.Hc == 0) {
            s.Format("mu_x= %g (rel)\r\n", u.mu1.re);
            outbox += s;
            s.Format("mu_y = %g (rel)\r\n", u.mu2.re);
            outbox += s;
            if (Re(u.mu12) != 0) {
              s.Format("mu_xy= %g (rel)\r\n", u.mu12.re);
              outbox += s;
            }
          } else {
            double bh = abs(u.B1 * u.H1 + u.B2 * u.H2);
            s.Format("B.H = %g J/m^3 (%g MGOe)\r\n", bh, muo * bh * 100.);
            outbox += s;
          }
          s.Format("E   = %g J/m^3\r\n", u.E);
          outbox += s;
          s.Format("J   = %g MA/m^2\r\n", u.Js.re);
          outbox += s;
          if (u.ff > 0) {
            s.Format("Winding Fill = %.2f%%\r\n", u.ff * 100.);
            outbox += s;
          }
          Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
          OutputWindowText = outbox;
        } else {
          s.Format("Point: r=%g, z=%g\r\n", px, py);
          outbox = s;
          s.Format("Flux= %g Wb\r\n", u.A.re);
          outbox += s;
          s.Format("|B| = %g T\r\n", sqrt(u.B1.re * u.B1.re + u.B2.re * u.B2.re));
          outbox += s;
          s.Format("Br  = %g T\r\n", u.B1.re);
          outbox += s;
          s.Format("Bz  = %g T\r\n", u.B2.re);
          outbox += s;
          s.Format("|H| = %g A/m\r\n", sqrt(u.H1.re * u.H1.re + u.H2.re * u.H2.re));
          outbox += s;
          s.Format("Hr  = %g A/m\r\n", u.H1.re);
          outbox += s;
          s.Format("Hz  = %g A/m\r\n", u.H2.re);
          outbox += s;
          if (u.Hc == 0) {
            s.Format("mu_r= %g (rel)\r\n", u.mu1.re);
            outbox += s;
            s.Format("mu_z= %g (rel)\r\n", u.mu2.re);
            outbox += s;
            if (Re(u.mu12) != 0) {
              s.Format("mu_rz= %g (rel)\r\n", u.mu12.re);
              outbox += s;
            }
          } else {
            double bh = abs(u.B1 * u.H1 + u.B2 * u.H2);
            s.Format("B.H = %g J/m^3 (%g MGOe)\r\n", bh, muo * bh * 100.);
            outbox += s;
          }
          s.Format("E   = %g J/m^3\r\n", u.E);
          outbox += s;
          s.Format("J   = %g MA/m^2\r\n", u.Js.re);
          outbox += s;
          if (u.ff > 0) {
            s.Format("Winding Fill = %.2f%%\r\n", u.ff * 100.);
            outbox += s;
          }
          Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
          OutputWindowText = outbox;
        }
      }
      // Harmonic problems...
      else {
        if (pDoc->ProblemType == 0) {
          s.Format("Point: x=%g, y=%g", px, py);
          outbox = s;
          s.Format("A   = %s Wb/m", u.A.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("|B| = %g T", sqrt(u.B1.re * u.B1.re + u.B2.re * u.B2.re + u.B1.im * u.B1.im + u.B2.im * u.B2.im));
          outbox += "\r\n" + s;
          s.Format("Bx  = %s T", u.B1.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("By  = %s T", u.B2.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("|H| = %g A/m", sqrt(u.H1.re * u.H1.re + u.H2.re * u.H2.re + u.H1.im * u.H1.im + u.H2.im * u.H2.im));
          outbox += "\r\n" + s;
          s.Format("Hx  = %s A/m", u.H1.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("Hy  = %s A/m", u.H2.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("mu_x = %s (rel)", u.mu1.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("mu_y = %s (rel)", u.mu2.ToStringAlt(c));
          outbox += "\r\n" + s;
          if ((Re(u.mu12) != 0) | (Re(u.mu12) != 0)) {
            s.Format("mu_xy= %s (rel)", u.mu12.ToStringAlt(c));
            outbox += "\r\n" + s;
          }
          Jtot = u.Je + u.Js;
          s.Format("J= %s MA/m^2", Jtot.ToStringAlt(c));
          outbox += "\r\n" + s;
          if (u.ff > 0) {
            s.Format("\r\nWinding Fill = %.2f%%", u.ff * 100.);
            outbox += s;
          }
          Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
          OutputWindowText = outbox;
        }
        if (pDoc->ProblemType == 1) {
          s.Format("Point: r=%g, z=%g", px, py);
          outbox = s;
          s.Format("Flux= %s Wb", u.A.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("|B| = %g T", sqrt(u.B1.re * u.B1.re + u.B2.re * u.B2.re + u.B1.im * u.B1.im + u.B2.im * u.B2.im));
          outbox += "\r\n" + s;
          s.Format("Br  = %s T", u.B1.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("Bz  = %s T", u.B2.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("|H| = %g A/m", sqrt(u.H1.re * u.H1.re + u.H2.re * u.H2.re + u.H1.im * u.H1.im + u.H2.im * u.H2.im));
          outbox += "\r\n" + s;
          s.Format("Hr  = %s A/m", u.H1.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("Hz  = %s A/m", u.H2.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("mu_r = %s (rel)", u.mu1.ToStringAlt(c));
          outbox += "\r\n" + s;
          s.Format("mu_z = %s (rel)", u.mu2.ToStringAlt(c));
          outbox += "\r\n" + s;
          if ((Re(u.mu12) != 0) | (Re(u.mu12) != 0)) {
            s.Format("mu_rz= %s (rel)", u.mu12.ToStringAlt(c));
            outbox += "\r\n" + s;
          }
          Jtot = u.Je + u.Js;
          s.Format("J= %s MA/m^2", Jtot.ToStringAlt(c));
          outbox += "\r\n" + s;
          if (u.ff > 0) {
            s.Format("\r\nWinding Fill = %.2f%%", u.ff * 100.);
            outbox += s;
          }
          Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
          OutputWindowText = outbox;
        }
      }
    }
  }
}

void CFemmviewView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if ((bLinehook != FALSE) || (bOnDraw != FALSE)) {
    CView::OnLButtonDown(nFlags, point);
    return;
  }

  int i, j, k, m;

  if (ZoomWndFlag == 1) {
    ZoomWndFlag++;
    wzx = mx;
    wzy = my;
    return;
  }

  if (ZoomWndFlag == 2)
    return;

  if (EditAction == 0)
    DisplayPointProperties(mx, my);

  if (EditAction == 1) {
    CFemmviewDoc* pDoc = GetDocument();
    if (pDoc->nodelist.GetSize() > 0) {
      i = pDoc->ClosestNode(mx, my);
      CComplex x, y, z;
      double R, d1, d2;
      int lineno, arcno, flag = 0;
      z.Set(pDoc->nodelist[i].x, pDoc->nodelist[i].y);

      if (pDoc->contour.GetSize() > 0) {

        // check to see if point is the same as last point in the contour;
        y = pDoc->contour[pDoc->contour.GetSize() - 1];
        if ((y.re == z.re) && (y.im == z.im))
          return;

        j = pDoc->ClosestNode(y.re, y.im);
        x.Set(pDoc->nodelist[j].x, pDoc->nodelist[j].y);

        // check to see if this point and the last point are ends of an
        // input segment;
        lineno = -1;
        d1 = 1.e08;

        if (abs(x - y) < 1.e-08) {
          for (k = 0; k < pDoc->linelist.GetSize(); k++) {
            if ((pDoc->linelist[k].n0 == j) && (pDoc->linelist[k].n1 == i)) {
              d2 = fabs(pDoc->ShortestDistanceFromSegment(mx, my, k));
              if (d2 < d1) {
                lineno = k;
                d1 = d2;
              }
            }
            if ((pDoc->linelist[k].n0 == i) && (pDoc->linelist[k].n1 == j)) {
              d2 = fabs(pDoc->ShortestDistanceFromSegment(mx, my, k));
              if (d2 < d1) {
                lineno = k;
                d1 = d2;
              }
            }
          }
        }

        // check to see if this point and last point are ends of an
        //  arc segment; if so, add entire arc to the contour;
        arcno = -1;
        if (abs(x - y) < 1.e-08) {
          for (k = 0; k < pDoc->arclist.GetSize(); k++) {
            if ((pDoc->arclist[k].n0 == j) && (pDoc->arclist[k].n1 == i)) {
              d2 = pDoc->ShortestDistanceFromArc(CComplex(mx, my),
                  pDoc->arclist[k]);
              if (d2 < d1) {
                arcno = k;
                lineno = -1;
                flag = TRUE;
                d1 = d2;
              }
            }
            if ((pDoc->arclist[k].n0 == i) && (pDoc->arclist[k].n1 == j)) {
              d2 = pDoc->ShortestDistanceFromArc(CComplex(mx, my),
                  pDoc->arclist[k]);
              if (d2 < d1) {
                arcno = k;
                lineno = -1;
                flag = FALSE;
                d1 = d2;
              }
            }
          }
        }

      } else {
        pDoc->contour.Add(z);
        DrawUserContour(FALSE);
        return;
      }

      if ((lineno < 0) && (arcno < 0)) {
        pDoc->contour.Add(z);
        DrawUserContour(FALSE);
      }

      if (lineno >= 0) {
        j = (int)pDoc->contour.GetSize();
        if (j > 1) {
          if (abs(pDoc->contour[j - 2] - z) < 1.e-08) {
            CView::OnLButtonDown(nFlags, point);
            return;
          }
        }
        pDoc->contour.Add(z);
        DrawUserContour(FALSE);
      }
      if (arcno >= 0) {
        k = arcno;
        pDoc->GetCircle(pDoc->arclist[k], x, R);
        j = (int)ceil(pDoc->arclist[k].ArcLength / pDoc->arclist[k].MaxSideLength);
        if (flag == TRUE)
          z = exp(I * pDoc->arclist[k].ArcLength * PI / (180. * ((double)j)));
        else
          z = exp(-I * pDoc->arclist[k].ArcLength * PI / (180. * ((double)j)));
        for (i = 0; i < j; i++) {
          y = (y - x) * z + x;
          m = (int)pDoc->contour.GetSize();
          if (m > 1) {
            if (abs(pDoc->contour[m - 2] - y) < 1.e-08) {
              CView::OnLButtonDown(nFlags, point);
              return;
            }
          }
          pDoc->contour.Add(y);
          DrawUserContour(FALSE);
        }
      }
    }
  }

  if (EditAction == 2) {
    CFemmviewDoc* pDoc = GetDocument();

    if (pDoc->meshelem.GetSize() > 0) {
      k = pDoc->InTriangle(mx, my);
      if (k >= 0) {
        pDoc->bHasMask = FALSE;
        pDoc->blocklist[pDoc->meshelem[k].lbl].ToggleSelect();
        DrawSelected = pDoc->meshelem[k].lbl;
        CDC* pDC = GetDC();
        OnDraw(pDC);
        DrawSelected = -1;
        ReleaseDC(pDC);
      }
      /*			else{ // if not in an element, select nearest arc or line
              CFemmviewDoc *pDoc=GetDocument();
              int clseg,claseg;
              int myType=0;
              double dseg,daseg;

              // find distance to closest segment;
              clseg=pDoc->ClosestSegment(mx,my);
              if (clseg>=0){
                dseg=pDoc->ShortestDistance(mx,my,clseg);
                myType+=1;
              }
              else dseg=-1.;

              // find distance to closest arc segment;
              claseg=pDoc->ClosestArcSegment(mx,my);
              if (claseg>=0)
              {
                daseg=pDoc->ShortestDistanceFromArc(CComplex(mx,my),pDoc->arclist[claseg]);
                myType+=2;
              }
              else daseg=-1.;

              switch(myType)
              {
                case 1: pDoc->linelist[clseg].ToggleSelect(); break;
                case 2: pDoc->arclist[claseg].ToggleSelect(); break;
                case 3:
                  if (dseg<daseg) pDoc->linelist[clseg].ToggleSelect();
                  else pDoc->arclist[claseg].ToggleSelect();
                  break;
                default: return;
              }

              CDC *pDC=GetDC();
              OnDraw(pDC);
              ReleaseDC(pDC);

            } */
    }
  }

  CView::OnLButtonDown(nFlags, point);
}

void CFemmviewView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (bLinehook != FALSE) {
    CView::OnLButtonUp(nFlags, point);
    return;
  }

  if (ZoomWndFlag > 0) {
    ZoomPix.RemoveAll();
    ZoomWndFlag = 0;
    if ((mx == wzx) && (my == wzy)) {
      RedrawView();
      return;
    }
    RECT r;
    double z, cmag, m[2];
    if (mx < wzx) {
      z = wzx;
      wzx = mx;
      mx = z;
    }
    if (my < wzy) {
      z = wzy;
      wzy = my;
      my = z;
    }
    GetClientRect(&r);
    m[0] = ((double)r.right) / (mx - wzx);
    m[1] = ((double)r.bottom) / (my - wzy);

    if (m[0] < m[1])
      cmag = m[0];
    else
      cmag = m[1];

    mag = cmag;
    ox = wzx;
    oy = wzy;

    RedrawView();
  }

  CView::OnLButtonUp(nFlags, point);
}

void CFemmviewView::OnRButtonDown(UINT nFlags, CPoint point)
{
  if (bLinehook != FALSE) {
    CView::OnRButtonDown(nFlags, point);
    return;
  }

  if (EditAction == 1) {
    CComplex z;
    int i;
    CFemmviewDoc* pDoc = GetDocument();
    z.Set(mx, my);
    i = (int)pDoc->contour.GetSize();
    if (i > 0) {
      if (z != pDoc->contour[i - 1])
        pDoc->contour.Add(z);
    } else
      pDoc->contour.Add(z);
    DrawUserContour(FALSE);
  }

  CView::OnRButtonDown(nFlags, point);
}

void CFemmviewView::OnEditCopyAsMetafile()
{
  // puts a metafile drawing of the screen on the clipboard
  CMetaFileDC Meta;
  RECT r;
  CRgn R;

  Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
  GetClientRect(&r);
  R.CreateRectRgnIndirect(&r);
  Meta.SelectClipRgn(&R);
  OnDraw(&Meta);
  HENHMETAFILE hMeta = Meta.CloseEnhanced();

  OpenClipboard();
  EmptyClipboard();
  SetClipboardData(CF_ENHMETAFILE, hMeta);
  CloseClipboard();
}

void CFemmviewView::OnEditCopy()
{
  // puts a bitmap of the screen on the clipboard
  RECT r;
  CDC tempDC;
  CBitmap bitmap;
  CBitmap* oldbitmap;
  CDC* pDC = GetDC();

  GetClientRect(&r);
  tempDC.CreateCompatibleDC(pDC);
  bitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
  oldbitmap = tempDC.SelectObject(&bitmap);

  tempDC.BitBlt(0, 0, r.right, r.bottom, pDC, 0, 0, SRCCOPY);

  OpenClipboard();
  EmptyClipboard();
  SetClipboardData(CF_BITMAP, HBITMAP(bitmap));
  CloseClipboard();
  ReleaseDC(pDC);
}

void CFemmviewView::OnReload()
{
  CFemmviewDoc* pDoc = GetDocument();
  if (bLinehook == FALSE)
    BeginWaitCursor();
  pDoc->OnReload();
  if (bLinehook == FALSE)
    EndWaitCursor();
  if (d_ResetOnReload == FALSE)
    pDoc->FirstDraw = FALSE;
  RedrawView();
  OnViewInfo();
}

void CFemmviewView::OnMenuArea()
{
  CFemmviewDoc* pDoc = GetDocument();
  if (EditAction == 1) {
    EraseUserContour(TRUE);
    pDoc->contour.RemoveAll();
  }
  EditAction = 2;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_CHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_UNCHECKED);
  tc->PressButton(ID_MENU_AREA, TRUE);
  tc->PressButton(ID_MENU_CONTOUR, FALSE);
  tc->PressButton(ID_MENU_POINT, FALSE);
}

void CFemmviewView::OnMenushowpts()
{
  CFrameWnd* MFrm;
  MFrm = GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  if (PtsFlag == FALSE) {
    PtsFlag = TRUE;
    MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_CHECKED);
  } else {
    PtsFlag = FALSE;
    MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_UNCHECKED);
  }
  RedrawView();
}

void CFemmviewView::OnMenuContour()
{
  CFemmviewDoc* pDoc = GetDocument();

  if (EditAction == 2) {
    int i;
    BOOL flg = FALSE;
    pDoc->bHasMask = FALSE;
    for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
      if (pDoc->blocklist[i].IsSelected == TRUE) {
        pDoc->blocklist[i].IsSelected = FALSE;
        flg = TRUE;
      }
    }

    for (i = 0; i < pDoc->linelist.GetSize(); i++) {
      if (pDoc->linelist[i].IsSelected == TRUE) {
        pDoc->linelist[i].IsSelected = FALSE;
        flg = TRUE;
      }
    }

    for (i = 0; i < pDoc->arclist.GetSize(); i++) {
      if (pDoc->arclist[i].IsSelected == TRUE) {
        pDoc->arclist[i].IsSelected = FALSE;
        flg = TRUE;
      }
    }

    if (flg == TRUE)
      RedrawView();
  }

  EditAction = 1;

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_CHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_UNCHECKED);
  tc->PressButton(ID_MENU_AREA, FALSE);
  tc->PressButton(ID_MENU_CONTOUR, TRUE);
  tc->PressButton(ID_MENU_POINT, FALSE);
}

void CFemmviewView::OnMenuPoint()
{
  CFemmviewDoc* pDoc = GetDocument();

  if (EditAction == 1) {
    EraseUserContour(TRUE);
    pDoc->contour.RemoveAll();
  }
  if (EditAction == 2) {
    int i;
    BOOL flg = FALSE;
    pDoc->bHasMask = FALSE;
    for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
      if (pDoc->blocklist[i].IsSelected == TRUE) {
        pDoc->blocklist[i].IsSelected = FALSE;
        flg = TRUE;
      }
    }

    for (i = 0; i < pDoc->linelist.GetSize(); i++) {
      if (pDoc->linelist[i].IsSelected == TRUE) {
        pDoc->linelist[i].IsSelected = FALSE;
        flg = TRUE;
      }
    }

    for (i = 0; i < pDoc->arclist.GetSize(); i++) {
      if (pDoc->arclist[i].IsSelected == TRUE) {
        pDoc->arclist[i].IsSelected = FALSE;
        flg = TRUE;
      }
    }

    if (flg == TRUE)
      RedrawView();
  }

  EditAction = 0;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_CHECKED);
  tc->PressButton(ID_MENU_AREA, FALSE);
  tc->PressButton(ID_MENU_CONTOUR, FALSE);
  tc->PressButton(ID_MENU_POINT, TRUE);
}

void CFemmviewView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CFemmviewDoc* pDoc = GetDocument();

  if (nChar == VK_LEFT)
    OnPanLeft();
  if (nChar == VK_RIGHT)
    OnPanRight();
  if (nChar == VK_UP)
    OnPanUp();
  if (nChar == VK_DOWN)
    OnPanDown();
  if (nChar == VK_PRIOR)
    OnZoomIn(); // page up
  if (nChar == VK_NEXT)
    OnZoomOut(); // page down
  if (nChar == VK_HOME)
    OnZoomNatural();

  if ((nChar == VK_TAB) && (EditAction != 2))
    EnterPoint();

  if ((nChar == VK_DELETE) && (EditAction == 1)) {
    int k;
    k = (int)pDoc->contour.GetSize();

    if (k > 1) {
      EraseUserContour(FALSE);
      pDoc->contour.RemoveAt(k - 1);
      DrawUserContour(TRUE);
    } else if (k != 0) {
      EraseUserContour(TRUE);
      pDoc->contour.RemoveAt(0);
    }
  }

  if ((nChar == VK_ESCAPE) && (EditAction == 1)) {
    EraseUserContour(TRUE);
    pDoc->contour.RemoveAll();
  }

  if ((nChar == VK_SHIFT) && (EditAction == 1)) {
    if (pDoc->contour.GetSize() > 1) {
      CBendContourDlg dlg;
      dlg.DoModal();
      if (dlg.m_angle != 0) {
        EraseUserContour(FALSE);
        pDoc->BendContour(dlg.m_angle, dlg.m_anglestep);
        DrawUserContour(TRUE);
      }
    }
  }

  if ((nChar == VK_ESCAPE) && (EditAction == 2)) {
    BOOL flg = FALSE;
    int i;

    for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
      if (pDoc->blocklist[i].IsSelected == TRUE) {
        flg = TRUE;
        pDoc->blocklist[i].IsSelected = FALSE;
      }
    }

    for (i = 0; i < pDoc->linelist.GetSize(); i++) {
      if (pDoc->linelist[i].IsSelected == TRUE) {
        flg = TRUE;
        pDoc->linelist[i].IsSelected = FALSE;
      }
    }

    for (i = 0; i < pDoc->arclist.GetSize(); i++) {
      if (pDoc->arclist[i].IsSelected == TRUE) {
        flg = TRUE;
        pDoc->arclist[i].IsSelected = FALSE;
      }
    }
    if (flg == TRUE)
      RedrawView();
  }

  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFemmviewView::EnterPoint()
{
  CEnterPt pDlg;
  double x, y, t;
  CFemmviewDoc* pDoc = GetDocument();

  // Send present parameter values to the dialog
  // pDlg.m_rji=TheDoc->vi[0];

  if (pDoc->Coords == FALSE) {
    pDlg.m_coord1 = mx;
    pDlg.m_coord2 = my;
    if (pDoc->ProblemType == 0) {
      pDlg.m_label1 = "x-coord";
      pDlg.m_label2 = "y-coord";
    } else {
      pDlg.m_label1 = "r-coord";
      pDlg.m_label2 = "z-coord";
    }
  } else {
    pDlg.m_coord1 = sqrt(mx * mx + my * my);
    pDlg.m_coord2 = atan2(my, mx) * 180 / PI;
    pDlg.m_label1 = "radius";
    pDlg.m_label2 = "degrees";
  }

  // Display dialog and collect data
  if (pDlg.DoModal() == IDOK) {
    if (pDoc->Coords == FALSE) {
      x = pDlg.m_coord1;
      y = pDlg.m_coord2;
    } else {
      t = pDlg.m_coord2 * PI / 180;
      x = pDlg.m_coord1 * cos(t);
      y = pDlg.m_coord1 * sin(t);
    }
    if (EditAction == 0)
      DisplayPointProperties(x, y);
    if (EditAction == 1) {
      CComplex z;
      z.Set(x, y);
      pDoc->contour.Add(z);
      DrawUserContour(FALSE);
    }
  }
}

void CFemmviewView::SpecialLine(CDC* pDC, int X0, int Y0, int X1, int Y1, int flag)
{
  double l;
  int i, j, k, p, x, y, z;
  int hi, lo, zj;
  BOOL InLine, DrawIt;
  COLORREF c;
  COLORREF Red;
  RECT r;

  GetClientRect(&r);
  Red = SelColor;

  int x0, y0, x1, y1, xb, yb;

  x0 = X0;
  y0 = Y0;
  x1 = X1;
  y1 = Y1;
  xb = r.right;
  yb = r.bottom;
  DrawIt = TRUE;

  // test to see if any of the endpoints are offscreen.
  // if they are, crop them to the screen size
  if ((x0 < 0) || (x0 > xb) || (x1 < 0) || (x1 > xb) || (y0 < 0) || (y0 > yb) || (y1 < 0) || (y1 > yb)) {
    DrawIt = FALSE;
    double z, dx0, dx1, dy0, dy1, dxb, dyb;
    dx0 = (double)x0;
    dy0 = (double)y0;
    dx1 = (double)x1;
    dy1 = (double)y1;
    dxb = (double)xb;
    dyb = (double)yb;

    if ((x1 - x0) != 0) {
      // test left side;
      z = (-dx0) / (dx1 - dx0);
      if ((z >= 0.) && (z <= 1.)) {
        DrawIt = TRUE;
        if (x0 < 0) {
          dx0 = 0;
          x0 = 0;
          dy0 = dy0 + z * (dy1 - dy0);
          y0 = (int)dy0;
        } else {
          dx1 = 0;
          x1 = 0;
          dy1 = dy0 + z * (dy1 - dy0);
          y1 = (int)dy1;
        }
      }

      // test right side;
      z = (dxb - dx0) / (dx1 - dx0);
      if ((z >= 0.) && (z <= 1.)) {
        DrawIt = TRUE;
        if (x0 > xb) {
          dx0 = dxb;
          x0 = xb;
          dy0 = dy0 + z * (dy1 - dy0);
          y0 = (int)dy0;
        } else {
          dx1 = dxb;
          x1 = xb;
          dy1 = dy0 + z * (dy1 - dy0);
          y1 = (int)dy1;
        }
      }
    }

    if ((y1 - y0) != 0) {
      // test top side;
      z = (-dy0) / (dy1 - dy0);
      if ((z >= 0.) && (z <= 1.)) {
        DrawIt = TRUE;
        if (y0 < 0) {
          dy0 = 0;
          y0 = 0;
          dx0 = dx0 + z * (dx1 - dx0);
          x0 = (int)dx0;
        } else {
          dy1 = 0;
          y1 = 0;
          dx1 = dx0 + z * (dx1 - dx0);
          x1 = (int)dx1;
        }
      }

      // test bottom side;
      z = (dyb - dy0) / (dy1 - dy0);
      if ((z >= 0.) && (z <= 1.)) {
        DrawIt = TRUE;
        if (y0 > dyb) {
          dy0 = dyb;
          y0 = yb;
          dx0 = dx0 + z * (dx1 - dx0);
          x0 = (int)dx0;
        } else {
          dy1 = dyb;
          y1 = yb;
          dx1 = dx0 + z * (dx1 - dx0);
          x1 = (int)dx1;
        }
      }
    }
  }

  if (!DrawIt)
    return;

  l = (double)((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
  l = sqrt(l);
  k = (int)l;
  for (i = 0; i <= k; i++) {
    if (k != 0) {
      x = x0 + ((x1 - x0) * i) / k;
      y = y0 + ((y1 - y0) * i) / k;
    } else {
      x = x0;
      y = y0;
    }

    z = x + y * r.right;

    // check if this point is already in the line
    // in the case that a red line is supposed to be drawn;

    if (flag == FALSE) {
      // bisection search to see if the pixel under
      // consideration is already in LinePix
      InLine = FALSE;
      lo = 0;
      hi = (int)LinePix.GetSize() - 1;
      if (hi >= 0) {
        zj = LinePix[0].z;
        if (z <= zj) {
          hi = 0;
          if (z == zj)
            InLine = TRUE;
        }
        zj = LinePix[hi].z;
        if (z >= zj) {
          hi += 1;
          lo = hi;
          if (z == zj)
            InLine = TRUE;
        }
        while ((hi - lo) > 1) {
          j = (lo + hi) / 2;
          zj = LinePix[j].z;
          if (zj == z) {
            InLine = TRUE;
            break;
          } else if (zj < z)
            lo = j;
          else
            hi = j;
        }
      } else
        hi = 0;

      if (InLine == FALSE) {
        CPixel NewPixel;
        NewPixel.z = z;
        NewPixel.c = pDC->GetPixel(x, y);
        if (hi == (int)LinePix.GetSize())
          LinePix.Add(NewPixel);
        else
          LinePix.InsertAt(hi, NewPixel);
      }
    }

    // if line is to be erased, get entry out of the pixel list
    if (flag != FALSE) {
      // bisection search to find the pixel
      InLine = FALSE;
      lo = 0;
      hi = (int)LinePix.GetSize() - 1;
      if (hi >= 0) {
        zj = LinePix[0].z;
        if (z <= zj) {
          hi = 0;
          if (z == zj) {
            InLine = TRUE;
            p = 0;
          }
        }
        zj = LinePix[hi].z;
        if (z >= zj) {
          lo = hi;
          if (z == zj) {
            InLine = TRUE;
            p = hi;
          }
        }
        while ((hi - lo) > 1) {
          j = (lo + hi) / 2;
          zj = LinePix[j].z;
          if (zj == z) {
            InLine = TRUE;
            p = j;
            break;
          } else if (zj < z)
            lo = j;
          else
            hi = j;
        }
      }

      if (InLine == TRUE) {
        c = LinePix[p].c;
        LinePix.RemoveAt(p);
      }
    }

    if (pDC->PtVisible(x, y) != FALSE) {
      if (flag == FALSE)
        pDC->SetPixel(x, y, Red);
      else if (InLine == TRUE)
        pDC->SetPixel(x, y, c);
    }
  }
}

void CFemmviewView::OnMenuPlot()
{

  CFemmviewDoc* pDoc = GetDocument();
  int PlotType, npoints, FileFormat, myAGE;
  BOOL ToFile;
  CXYPlot xyplot;

  if (pDoc->contour.GetSize() == 0) {
    // if no AGE, throw error message and return
    if (pDoc->agelist.GetSize() == 0) {
      MsgBox("Cannot create a plot;\nNo contour has been defined");
      return;
    }

    // otherwise, invoke functionality to plot air gap values
    CGapPlotDlg dlg;
    dlg.pagelist = &pDoc->agelist;
    dlg.m_npoints = d_PlotPoints;
    if (dlg.DoModal() == IDOK) {
      PlotType = dlg.XYPlotType;
      FileFormat = dlg.FileFormat;
      ToFile = dlg.m_ToFile;
      npoints = dlg.m_npoints;
      myAGE = dlg.m_age;
      if (bLinehook == FALSE)
        BeginWaitCursor();
      pDoc->GetGapValues(xyplot, PlotType, npoints, myAGE);
      if (bLinehook == FALSE)
        EndWaitCursor();
    } else
      return;
  } else {
    // do normal plot along a defined contour
    CXYPlotDlg dlg;
    if (pDoc->Frequency == 0)
      dlg.ListType = 0;
    else
      dlg.ListType = 1;
    dlg.m_npoints = d_PlotPoints;
    if (dlg.DoModal() == IDOK) {
      PlotType = dlg.XYPlotType;
      FileFormat = dlg.FileFormat;
      ToFile = dlg.m_ToFile;
      npoints = dlg.m_npoints;
      if (bLinehook == FALSE)
        BeginWaitCursor();
      pDoc->GetLineValues(xyplot, PlotType, npoints);
      if (bLinehook == FALSE)
        EndWaitCursor();
    } else
      return;
  }

  if (ToFile == FALSE) {
    CMetaFileDC Meta;
    Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
    HGLOBAL BoundingBox = GlobalAlloc(0, 256);
    xyplot.MakePlot(&Meta, (char*)BoundingBox);
    HENHMETAFILE hMeta = Meta.CloseEnhanced();
    if (hMeta == NULL)
      MsgBox("No Handle...");
    if (OpenClipboard() == FALSE)
      MsgBox("Cannot access the Clipboard");
    else {
      EmptyClipboard();
      if (SetClipboardData(CF_ENHMETAFILE, hMeta) == NULL)
        MsgBox("Couldn't SetClipboardData");
      if (SetClipboardData(CF_TEXT, BoundingBox) == NULL)
        MsgBox("Couldn't SetClipboardData");
      CloseClipboard();
      // fire up plot viewer;
      ((CFemmApp*)AfxGetApp())->CreateNewDocument(8);
    }
  } else
    xyplot.ToDisk(FileFormat);
}

void CFemmviewView::OnMenuIntegrate()
{
  CFemmviewDoc* pDoc = GetDocument();
  int inttype;
  int bBlocksAreSelected = FALSE;

  if ((EditAction == 0) && (pDoc->agelist.GetSize() == 0)) {
    MsgBox("Cannot integrate;\nNo contour or area\nhas been defined.");
    return;
  }

  if ((EditAction == 1) && (pDoc->contour.GetSize() == 0) && (pDoc->agelist.GetSize() == 0)) {
    MsgBox("Cannot integrate;\nNo contour has been defined");
    return;
  }

  if (EditAction == 2) {
    int i;
    for (i = 0; i < pDoc->blocklist.GetSize(); i++)
      if (pDoc->blocklist[i].IsSelected == TRUE)
        bBlocksAreSelected = TRUE;
    if ((bBlocksAreSelected == FALSE) && (pDoc->agelist.GetSize() == 0)) {
      MsgBox("Cannot integrate;\nNo area has been selected");
      return;
    }
  }

  if ((EditAction == 1) && (pDoc->contour.GetSize() > 0)) {
    CLIntDlg dlg;
    int inttype;

    if (dlg.DoModal() == IDOK) {
      inttype = dlg.linttype;
      char s[512];
      char c[80];
      char d[80];
      CComplex* z;
      switch (inttype) {
      case 0:
        z = (CComplex*)calloc(2, sizeof(CComplex));
        pDoc->LineIntegral(inttype, z);
        if (pDoc->Frequency == 0)
          sprintf(s, "Normal flux = %g Webers\nAverage B.n = %g Tesla", z[0].re, z[1].re);
        else
          sprintf(s, "Normal flux = %s Webers\nAverage B.n = %s Tesla", z[0].ToStringAlt(c), z[1].ToStringAlt(d));
        MyMessageBox(s);
        free(z);
        break;

      case 1:
        z = (CComplex*)calloc(2, sizeof(CComplex));
        if (bLinehook == FALSE)
          BeginWaitCursor();
        pDoc->LineIntegral(inttype, z);
        if (bLinehook == FALSE)
          EndWaitCursor();
        if (pDoc->Frequency == 0)
          sprintf(s, "MMF drop along contour = %g Amp-turns\nAverage H.t = %g Amp/Meter", z[0].re, z[1].re);
        else
          sprintf(s, "MMF drop along contour = %s Amp-turns\nAverage H.t = %s Amp/Meter", z[0].ToStringAlt(c), z[1].ToStringAlt(d));

        MyMessageBox(s);
        free(z);
        break;
      case 2:
        z = (CComplex*)calloc(1, sizeof(CComplex));
        pDoc->LineIntegral(inttype, z);
        sprintf(s, "Contour length = %g meters\nSurface Area = %g meter^2", z[0].re, z[0].im);
        MyMessageBox(s);
        free(z);
        break;
      case 3:
        z = (CComplex*)calloc(4, sizeof(CComplex));
        if (bLinehook == FALSE)
          BeginWaitCursor();
        pDoc->LineIntegral(inttype, z);
        if (bLinehook == FALSE)
          EndWaitCursor();
        if (pDoc->Frequency == 0) {
          if (pDoc->ProblemType == 0)
            sprintf(s, "Force in x-direction = %g N\nForce in y-direction = %g N\n", z[0].re, z[1].re);
          else
            sprintf(s, "Force in z-direction = %g N", z[1].re);
          MyMessageBox(s);
        } else {
          CString ss;
          if (pDoc->ProblemType == 0) {
            sprintf(s, "DC Force:\n x-direction = %g N\n y-direction = %g N\n", z[2].re, z[3].re);
            ss = s;
            sprintf(s, "\n%f Hz Force:\n x-direction = %s N\n y-direction = %s N",
                pDoc->Frequency * 2., z[0].ToStringAlt(c), z[1].ToStringAlt(d));
            ss += s;
          } else {
            sprintf(s, "DC Force:\n z-direction = %g N", z[3].re);
            ss = s;
            sprintf(s, "\n%g Hz Force:\n z-direction = %s N",
                pDoc->Frequency * 2., z[1].ToStringAlt(c));
            ss += s;
          }

          MyMessageBox(ss);
          // AfxMessageBox(ss,MB_ICONINFORMATION);
        }
        free(z);
        break;

      case 4:
        z = (CComplex*)calloc(2, sizeof(CComplex));
        if (bLinehook == FALSE)
          BeginWaitCursor();
        pDoc->LineIntegral(inttype, z);
        if (bLinehook == FALSE)
          EndWaitCursor();
        if (pDoc->Frequency == 0) {
          if (pDoc->ProblemType == 0)
            sprintf(s, "Torque about (0,0) = %g N*m", z[0].re);
          else
            sprintf(s, "No net torque in axisymmetric case.");

          MyMessageBox(s);
          // AfxMessageBox(s,MB_ICONINFORMATION);
        } else {
          CString ss;
          if (pDoc->ProblemType == 0) {
            sprintf(s, "Torque about (0,0):\nDC Torque = %g N*m\n", z[1].re);
            ss = s;
            sprintf(s, "%g Hz Torque = %s N*m",
                pDoc->Frequency * 2., z[0].ToStringAlt(c));
            ss += s;
          } else {
            sprintf(s, "No net torque in axisymmetric case");
            ss = s;
          }

          MyMessageBox(ss);
          // AfxMessageBox(ss,MB_ICONINFORMATION);
        }

        free(z);
        break;

      case 5:
        z = (CComplex*)calloc(2, sizeof(CComplex));
        pDoc->LineIntegral(inttype, z);
        if (pDoc->Frequency == 0)
          sprintf(s, "Result = %g Webers^2\nAverage (B.n)^2 = %g Tesla^2", z[0].re, z[1].re);
        else
          sprintf(s, "Result = %s Webers^2\nAverage (B.n)^2 = %s Tesla^2", z[0].ToStringAlt(c), z[1].ToStringAlt(d));

        MyMessageBox(s);
        free(z);
        break;

      default:
        sprintf(s, "This space left intentionally blank");
        break;
      }
    }
    return;
  }

  if ((EditAction == 2) && (bBlocksAreSelected)) {
    CBlockInt dlg;

    if (dlg.DoModal() == IDOK) {
      inttype = dlg.binttype;
      CComplex z, z1, z2, z3, z4;
      char s[512];
      char c[80];
      char d[80];

      switch (inttype) {
      case 0:
        z = pDoc->BlockIntegral(inttype);
        if (pDoc->Frequency == 0)
          sprintf(s, "%g Henry Amp^2", z.re);
        else
          sprintf(s, "%s Henry Amp^2", z.ToStringAlt(c));
        break;
      case 1:
        z = pDoc->BlockIntegral(inttype);
        z1 = pDoc->BlockIntegral(5);
        if (pDoc->Frequency == 0)
          sprintf(s, "Integral of A over selection:\n%g Henry Amp meter^2\nIntegral of A / selected area:\n%g Henry Amp", z.re, z.re / z1.re);
        else
          sprintf(s, "Integral of A over selection:\n%s Henry Amp meter^2\nIntegral of A / selected area:\n%s Henry Amp", z.ToStringAlt(c),
              (z / z1.re).ToStringAlt(d));
        break;
      case 2:
        z = pDoc->BlockIntegral(inttype);
        sprintf(s, "%g Joules", z.re);
        break;

      case 3:
        z = pDoc->BlockIntegral(inttype);
        sprintf(s, "%g Watts", z.re);
        break;

      case 4:
        z = pDoc->BlockIntegral(inttype);
        sprintf(s, "%g Watts", z.re);
        break;

      case 5:
        z = pDoc->BlockIntegral(inttype);
        sprintf(s, "%g meter^2", z.re);
        break;

      case 6:
        z = pDoc->BlockIntegral(inttype);
        sprintf(s, "%g Watts", z.re);
        break;

      case 7:
        z = pDoc->BlockIntegral(inttype);
        if (pDoc->Frequency == 0)
          sprintf(s, "%g Amps", z.re);
        else
          sprintf(s, "%s Amps", z.ToStringAlt(c));
        break;

      case 8:
        z1 = pDoc->BlockIntegral(8);
        z2 = pDoc->BlockIntegral(9);
        if ((pDoc->ProblemType == 0) && (pDoc->Frequency == 0))
          sprintf(s, "x-component: %g Tesla meter^3\ny-component: %g Tesla meter^3", z1.re, z2.re);
        if ((pDoc->ProblemType == 0) && (pDoc->Frequency != 0))
          sprintf(s, "x-component: %s Tesla meter^3\ny-component: %s Tesla meter^3", z1.ToStringAlt(c), z2.ToStringAlt(d));
        if ((pDoc->ProblemType == 1) && (pDoc->Frequency == 0))
          sprintf(s, "r-component: %g Tesla meter^3\nz-component: %g Tesla meter^3", z1.re, z2.re);
        if ((pDoc->ProblemType == 1) && (pDoc->Frequency != 0))
          sprintf(s, "r-component: %s Tesla meter^3\nz-component: %s Tesla meter^3", z1.ToStringAlt(c), z2.ToStringAlt(d));
        break;
      case 9:
        z = pDoc->BlockIntegral(10);
        sprintf(s, "%g meter^3", z.re);
        break;
      case 10: // Lorentz Force

        // just report 1X results for incremental permeability AC solutions
        if (pDoc->bIncremental == 1) {
          z3 = pDoc->BlockIntegral(28);
          z4 = pDoc->BlockIntegral(29);
          if (pDoc->ProblemType == 0) {
            if (pDoc->Frequency == 0)
              sprintf(s, "Interaction force:\nx-component: %s N\ny-component: %s N", z3.ToStringAlt(c), z4.ToStringAlt(d));
            else
              sprintf(s, "1x Frequency force:\nx-component: %s N\ny-component: %s N", z3.ToStringAlt(c), z4.ToStringAlt(d));
          } else {
            sprintf(s, "1x Frequency force:\nr-component: 0. N\nz-component: %s N", z4.ToStringAlt(d));
          }
          break;
        }

        if (pDoc->bIncremental == 2) {
          z1 = pDoc->BlockIntegral(11);
          z2 = pDoc->BlockIntegral(12);
          z3 = pDoc->BlockIntegral(28);
          z4 = pDoc->BlockIntegral(29);
          if (pDoc->ProblemType == 0)
            sprintf(s, "Self force:\nx-component: %g N\ny-component: %g N\nInteraction force:\nx-component: %s N\ny-component: %s N", z1.re, z2.re, z3.ToStringAlt(c), z4.ToStringAlt(d));
          else
            sprintf(s, "Self force:\nr-component: 0. N\nz-component: %g NInteraction Frequency force:\nr-component: 0. N\nz-component: %s N", z2.re, z4.ToStringAlt(d));
          break;
        }

        // otherwise, report DC force (and 2X force as applicable)
        z1 = pDoc->BlockIntegral(11);
        z2 = pDoc->BlockIntegral(12);
        if (pDoc->Frequency != 0) {
          z3 = pDoc->BlockIntegral(13);
          z4 = pDoc->BlockIntegral(14);
        }
        if ((pDoc->ProblemType == 0) && (pDoc->Frequency == 0))
          sprintf(s, "x-component: %g N\ny-component: %g N", z1.re, z2.re);
        if ((pDoc->ProblemType == 1) && (pDoc->Frequency == 0))
          sprintf(s, "r-component: %g N\nz-component: %g N", 0., z2.re);
        if ((pDoc->ProblemType == 0) && (pDoc->Frequency != 0))
          sprintf(s, "Steady-state force:\nx-component: %g N\ny-component: %g N\n2x Frequency force:\nx-component: %s N\ny-component: %s N",
              z1.re, z2.re, z3.ToStringAlt(c), z4.ToStringAlt(d));
        if ((pDoc->ProblemType == 1) && (pDoc->Frequency != 0))
          sprintf(s, "Steady-state force:\nr-component: 0. N\nz-component: %g N\n2x Frequency force:\nr-component: 0. N\nz-component: %s N",
              z2.re, z4.ToStringAlt(d));
        break;

      case 11: // Lorentz Torque

        if (pDoc->ProblemType == 1) {
          sprintf(s, "No Lorentz torque possible in axisymmetric case");
          break;
        }

        // just report 1X results for incremental permeability AC solutions
        if (pDoc->bIncremental == 1) {
          z4 = pDoc->BlockIntegral(30);
          if (pDoc->Frequency == 0)
            sprintf(s, "Interaction torque about (0,0): %s N*m", z4.ToStringAlt(d));
          else
            sprintf(s, "1x Freq torque about (0,0): %s N*m", z4.ToStringAlt(d));
          break;
        }

        // report self- and interaction-components for frozen permeability DC solutions
        if (pDoc->bIncremental == 2) {
          z1 = pDoc->BlockIntegral(15);
          z4 = pDoc->BlockIntegral(30);
          sprintf(s, "Self torque about (0,0): %g\nInteraction torque about (0,0): %s N*m", z1.re, z4.ToStringAlt(d));
          break;
        }
        // otherwise, report DC torque (and 2X torque as applicable)
        z1 = pDoc->BlockIntegral(15);
        z2 = pDoc->BlockIntegral(16);
        if ((pDoc->ProblemType == 0) && (pDoc->Frequency == 0))
          sprintf(s, "Torque about (0,0): %g N*m", z1.re);
        if ((pDoc->ProblemType == 0) && (pDoc->Frequency != 0))
          sprintf(s, "Torque about (0,0):\nSteady State: %g N*m\n2x Frequency: %s N*m",
              z1.re, z2.ToStringAlt(d));

        if (pDoc->ProblemType == 1)
          sprintf(s, "No Lorentz torque possible in axisymmetric case");
        break;
      case 12:
        z = pDoc->BlockIntegral(17);
        sprintf(s, "%g Joules", z.re);
        break;

      // Henrotte Force
      case 13:

        // just report 1X force as the force if this is an incremental permeability AC problem
        if (pDoc->bIncremental == 1) {
          z3 = pDoc->BlockIntegral(25);
          z4 = pDoc->BlockIntegral(26);
          if (pDoc->ProblemType == 0) {
            if (pDoc->Frequency == 0)
              sprintf(s, "Interaction force:\nx-component: %s N\ny-component: %s N", z3.ToStringAlt(c), z4.ToStringAlt(d));
            else
              sprintf(s, "1x Frequency force:\nx-component: %s N\ny-component: %s N", z3.ToStringAlt(c), z4.ToStringAlt(d));
          } else {
            sprintf(s, "1x Frequency force:\nr-component: 0. N\nz-component: %s N", z4.ToStringAlt(d));
          }
        } else if (pDoc->bIncremental == 2) {
          z1 = pDoc->BlockIntegral(18);
          z2 = pDoc->BlockIntegral(19);
          z3 = pDoc->BlockIntegral(25);
          z4 = pDoc->BlockIntegral(26);
          if (pDoc->ProblemType == 0)
            sprintf(s, "Self force:\nx-component: %g N\ny-component: %g N\nInteraction force:\nx-component: %s N\ny-component: %s N", z1.re, z2.re, z3.ToStringAlt(c), z4.ToStringAlt(d));
          else
            sprintf(s, "Self force:\nr-component: 0. N\nz-component: %g N\nInteraction force:\nr-component: 0. N\nz-component: %s N", z2.re, z4.ToStringAlt(d));
        }
        // otherwise. report DC force (and 2X force if applicable)
        else {
          z1 = pDoc->BlockIntegral(18);
          z2 = pDoc->BlockIntegral(19);
          if (pDoc->Frequency != 0) {
            z3 = pDoc->BlockIntegral(20);
            z4 = pDoc->BlockIntegral(21);
          }
          if ((pDoc->ProblemType == 0) && (pDoc->Frequency == 0))
            sprintf(s, "x-component: %g N\ny-component: %g N", z1.re, z2.re);
          if ((pDoc->ProblemType == 1) && (pDoc->Frequency == 0))
            sprintf(s, "r-component: %g N\nz-component: %g N", 0., z2.re);
          if ((pDoc->ProblemType == 0) && (pDoc->Frequency != 0))
            sprintf(s, "Steady-state force:\nx-component: %g N\ny-component: %g N\n2x Frequency force:\nx-component: %s N\ny-component: %s N",
                z1.re, z2.re, z3.ToStringAlt(c), z4.ToStringAlt(d));
          if ((pDoc->ProblemType == 1) && (pDoc->Frequency != 0))
            sprintf(s, "Steady-state force:\nr-component: 0. N\nz-component: %g N\n2x Frequency force:\nr-component: 0. N\nz-component: %s N",
                z2.re, z4.ToStringAlt(d));
        }

        if (ShowMask)
          InvalidateRect(NULL);
        break;

      // Henrotte Torque
      case 14:
        if (pDoc->ProblemType == 1) {
          sprintf(s, "No torque possible in axisymmetric case");
          break;
        }

        // just report 1X torque as the torque if this is an incremental permeability AC problem
        if (pDoc->bIncremental == 1) {
          z4 = pDoc->BlockIntegral(27);
          if (pDoc->Frequency == 0)
            sprintf(s, "Interaction torque about (0,0): %s N*m", z4.ToStringAlt(d));
          else
            sprintf(s, "1x Freq torque about (0,0): %s N*m", z4.ToStringAlt(d));
        } else if (pDoc->bIncremental == 2) {
          z1 = pDoc->BlockIntegral(22);
          z4 = pDoc->BlockIntegral(27);
          sprintf(s, "Self torque about (0,0): %g N*m\nInteraction torque about (0,0): %s N*m", z1.re, z4.ToStringAlt(d));
        }
        // otherwise report DC torque (and 2X torque if applicable)
        else {
          z1 = pDoc->BlockIntegral(22);
          if (pDoc->Frequency == 0)
            sprintf(s, "Torque about (0,0): %g N*m", z1.re);
          else {
            z2 = pDoc->BlockIntegral(23);
            sprintf(s, "Torque about (0,0):\nSteady State: %g N*m\n2x Frequency: %s N*m", z1.re, z2.ToStringAlt(d));
          }
        }

        if (ShowMask)
          InvalidateRect(NULL);
        break;

      case 15:
        z1 = pDoc->BlockIntegral(24);
        if (pDoc->ProblemType)
          sprintf(s, "Integral of R^2: %g m^5", Re(z1));
        else
          sprintf(s, "Integral of (x^2 + y^2): %g m^5", Re(z1));
        break;
      case 16:
        z = Re(pDoc->BlockIntegral(6) / pDoc->BlockIntegral(10));
        sprintf(s, "%g Watts/meter^3", z.re);
        break;
      default:
        sprintf(s, "This space left intentionally blank");
        break;
      }
      MyMessageBox(s);
    }
    return;
  }

  // Otherwise, prompt for air gap integrals!!!
  // Works for any EditAction so long as no other integration region has been defined
  // and at least one Air Gap Element exists
  GapIntegral dlg;
  CString s;
  double y1, y2;
  CComplex z1, z2;
  char c[80], d[80];
  int n;

  dlg.pagelist = &pDoc->agelist;
  dlg.Frequency = pDoc->Frequency;
  dlg.bIncremental = pDoc->bIncremental;
  n = lua_gettop(lua);
  lua_pop(lua, n);
  if (dlg.DoModal() == IDOK) {
    switch (dlg.myIntType) {
    case 0: // Torque
      if (pDoc->Frequency == 0) {
        lua_pushstring(lua, dlg.BdryName);
        lua_pushnumber(lua, 0);
        pDoc->lua_gapintegral(lua);
        s.Format("Torque = %g Nm", lua_todouble(lua, -1));
        n = lua_gettop(lua);
        lua_pop(lua, n);
      } else {
        lua_pushstring(lua, dlg.BdryName);
        lua_pushnumber(lua, 0); // DC part of torque
        pDoc->lua_gapintegral(lua);
        y1 = lua_todouble(lua, -1);
        n = lua_gettop(lua);
        lua_pop(lua, n);

        lua_pushstring(lua, dlg.BdryName);
        lua_pushnumber(lua, 3); // 2X part of torque
        pDoc->lua_gapintegral(lua);
        z1 = lua_tonumber(lua, -1);
        n = lua_gettop(lua);
        lua_pop(lua, n);

        s.Format("DC Torque = %g N*m\n2X Torque = %s N*m", y1, z1.ToStringAlt(c));
      }
      MyMessageBox(s);
      break;

    case 1: // Force
      if (pDoc->Frequency == 0) {
        lua_pushstring(lua, dlg.BdryName);
        lua_pushnumber(lua, 1);
        pDoc->lua_gapintegral(lua);
        y1 = lua_todouble(lua, -2);
        y2 = lua_todouble(lua, -1);
        n = lua_gettop(lua);
        lua_pop(lua, n);
        s.Format("Fx = %g N\nFy = %g N", y1, y2);
      } else {
        lua_pushstring(lua, dlg.BdryName);
        lua_pushnumber(lua, 1); // DC part of torque
        pDoc->lua_gapintegral(lua);
        y1 = lua_todouble(lua, -2);
        y2 = lua_todouble(lua, -1);
        n = lua_gettop(lua);
        lua_pop(lua, n);

        lua_pushstring(lua, dlg.BdryName);
        lua_pushnumber(lua, 4); // 2X part of torque
        pDoc->lua_gapintegral(lua);
        z1 = lua_tonumber(lua, -2);
        z2 = lua_tonumber(lua, -1);
        n = lua_gettop(lua);
        lua_pop(lua, n);

        s.Format("DC Fx = %g N\nDC Fy = %g N\n2X Fx = %s N\n2X Fy = %s N", y1, y2, z1.ToStringAlt(c), z2.ToStringAlt(d));
      }
      MyMessageBox(s);
      break;

    case 2: // Energy
      lua_pushstring(lua, dlg.BdryName);
      lua_pushnumber(lua, 2);
      pDoc->lua_gapintegral(lua);
      y1 = lua_todouble(lua, -1);
      n = lua_gettop(lua);
      lua_pop(lua, n);

      if (pDoc->Frequency == 0)
        s.Format("Stored Energy = %g J", y1);
      else
        s.Format("Average Stored Energy = %g J", y1);
      MyMessageBox(s);
      break;

    case 3: // Interaction Torque
      lua_pushstring(lua, dlg.BdryName);
      lua_pushnumber(lua, 5);
      pDoc->lua_gapintegral(lua);
      z1 = lua_tonumber(lua, -1);
      n = lua_gettop(lua);
      lua_pop(lua, n);

      s.Format("Torque = %s Nm", z1.ToStringAlt(c));
      MyMessageBox(s);
      break;

    case 4: // Interaction Force
      lua_pushstring(lua, dlg.BdryName);
      lua_pushnumber(lua, 6);
      pDoc->lua_gapintegral(lua);
      z2 = lua_tonumber(lua, -2);
      z1 = lua_tonumber(lua, -1);
      n = lua_gettop(lua);
      lua_pop(lua, n);

      s.Format("1X Fx = %s Nm\n1X Fx = %s", z1.ToStringAlt(c), z2.ToStringAlt(d));
      MyMessageBox(s);
      break;

    default:
      break;
    }
  }
}

void CFemmviewView::OnSmooth()
{
  CFemmviewDoc* pDoc = GetDocument();
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  if (pDoc->Smooth == TRUE) {
    pDoc->Smooth = FALSE;
    MMnu->CheckMenuItem(ID_SMOOTH, MF_UNCHECKED);
  } else {
    pDoc->Smooth = TRUE;
    MMnu->CheckMenuItem(ID_SMOOTH, MF_CHECKED);
  }
  RedrawView();
}

BOOL CFemmviewView::OnEraseBkgnd(CDC* pDC)
{
  CBrush backBrush(BackColor);

  // Save old brush
  CBrush* pOldBrush = pDC->SelectObject(&backBrush);

  CRect rect;
  pDC->GetClipBox(&rect); // Erase the area needed

  pDC->PatBlt(rect.left, rect.top, rect.Width(),
      rect.Height(), PATCOPY);
  pDC->SelectObject(pOldBrush);

  return TRUE;
}

void CFemmviewView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
  CFemmviewDoc* pDoc = GetDocument();

  if (EditAction == 0) {
    int i = pDoc->ClosestNode(mx, my);
    if (i >= 0) {
      char s[256];
      sprintf(s, "Closest input node:\n(%g,%g)", pDoc->nodelist[i].x,
          pDoc->nodelist[i].y);
      AfxMessageBox(s, MB_ICONINFORMATION);
    }
  }

  if (EditAction == 1) {
    if (pDoc->contour.GetSize() > 1) {
      CBendContourDlg dlg;
      dlg.DoModal();
      if (dlg.m_angle != 0) {
        EraseUserContour(FALSE);
        pDoc->BendContour(dlg.m_angle, dlg.m_anglestep);
        DrawUserContour(TRUE);
      }
    }
  }

  CView::OnRButtonDblClk(nFlags, point);
}

void CFemmviewView::OnPanDown()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  oy -= 0.25 * y / mag;

  RedrawView();
}

void CFemmviewView::OnPanLeft()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox -= 0.25 * x / mag;

  RedrawView();
}

void CFemmviewView::OnPanRight()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox += 0.25 * x / mag;

  RedrawView();
}

void CFemmviewView::OnPanUp()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  oy += 0.25 * y / mag;

  RedrawView();
}

void CFemmviewView::OnViewCircprops()
{
  CCircDlg dlg;

  dlg.TheDoc = GetDocument();
  dlg.DoModal();
}

void CFemmviewView::RedrawView()
{
  LinePix.RemoveAll();
  InvalidateRect(NULL);
}

BOOL CFemmviewView::ScanPreferences()
{
  FILE* fp;
  CString fname;

  fname = BinDir + "femmview.cfg";

  fp = fopen(fname, "rt");
  if (fp != NULL) {
    BOOL flag = FALSE;
    char s[1024];
    char q[1024];
    char* v;
    int cr, cg, cb;

    // parse the file
    while (fgets(s, 1024, fp) != NULL) {
      sscanf(s, "%s", q);

      if (_strnicmp(q, "<SelColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        SelColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MeshColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        MeshColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<BlockColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        BlockColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<NodeColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        NodeColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<NameColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        NameColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<BkgndColor>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        BackColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<TextColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        TextColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<FluxRColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RealFluxLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<FluxIColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        ImagFluxLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MaskColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        MaskLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorRColor>", 14) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RealVectorColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorIColor>", 14) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        ImagVectorColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<LineColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        LineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<RegionColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RegionColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GridColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        GridColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color00>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color00 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color01>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color01 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color02>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color02 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color03>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color03 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color04>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color04 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color05>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color05 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color06>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color06 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color07>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color07 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color08>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color08 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color09>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color09 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color10>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color10 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color11>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color11 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color12>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color12 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color13>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color13 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color14>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color14 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color15>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color15 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color16>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color16 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color17>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color17 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color18>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color18 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color19>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color19 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey00>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey00 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey01>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey01 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey02>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey02 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey03>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey03 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey04>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey04 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey05>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey05 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey06>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey06 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey07>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey07 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey08>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey08 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey09>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey09 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey10>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey10 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey11>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey11 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey12>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey12 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey13>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey13 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey14>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey14 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey15>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey15 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey16>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey16 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey17>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey17 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey18>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey18 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey19>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey19 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<EditAction>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_EditAction);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<DensityPlot>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_DensityPlot);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorPlot>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_VectorPlot);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GridFlag>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_GridFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<SnapFlag>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_SnapFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MeshFlag>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_MeshFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<LegendFlag>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_LegendFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<NumContours>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_NumContours);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowAr>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_ShowAr);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowAi>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_ShowAi);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowMask>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_ShowMask);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GreyContours>", 14) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_GreyContours);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<PtsFlag>", 9) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_PtsFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Smooth>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_Smooth);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<PlotPoints>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_PlotPoints);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ResetOnReload>", 15) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_ResetOnReload);
        q[0] = NULL;
      }
    }
    fclose(fp);
  } else
    return FALSE;

  return TRUE;
}

void WriteColor(char* cname, COLORREF c, FILE* fp);

BOOL CFemmviewView::WritePreferences()
{
  FILE* fp;
  CString fname;
  CFemmviewDoc* pDoc = GetDocument();

  fname = BinDir + "femmview.cfg";

  fp = fopen(fname, "wt");
  if (fp != NULL) {
    WriteColor("SelColor", SelColor, fp);
    WriteColor("BkgndColor", BackColor, fp);
    WriteColor("MeshColor", MeshColor, fp);
    WriteColor("BlockColor", BlockColor, fp);
    WriteColor("LineColor", LineColor, fp);
    WriteColor("GridColor", GridColor, fp);
    WriteColor("NodeColor", NodeColor, fp);
    WriteColor("RegionColor", RegionColor, fp);
    WriteColor("TextColor", TextColor, fp);
    WriteColor("FluxRColor", RealFluxLineColor, fp);
    WriteColor("FluxIColor", ImagFluxLineColor, fp);
    WriteColor("MaskColor", MaskLineColor, fp);
    WriteColor("VectorRColor", RealVectorColor, fp);
    WriteColor("VectorIColor", ImagVectorColor, fp);
    WriteColor("NameColor", NameColor, fp);
    WriteColor("Color00", Color00, fp);
    WriteColor("Color01", Color01, fp);
    WriteColor("Color02", Color02, fp);
    WriteColor("Color03", Color03, fp);
    WriteColor("Color04", Color04, fp);
    WriteColor("Color05", Color05, fp);
    WriteColor("Color06", Color06, fp);
    WriteColor("Color07", Color07, fp);
    WriteColor("Color08", Color08, fp);
    WriteColor("Color09", Color09, fp);
    WriteColor("Color10", Color10, fp);
    WriteColor("Color11", Color11, fp);
    WriteColor("Color12", Color12, fp);
    WriteColor("Color13", Color13, fp);
    WriteColor("Color14", Color14, fp);
    WriteColor("Color15", Color15, fp);
    WriteColor("Color16", Color16, fp);
    WriteColor("Color17", Color17, fp);
    WriteColor("Color18", Color18, fp);
    WriteColor("Color19", Color19, fp);
    WriteColor("Grey00", Grey00, fp);
    WriteColor("Grey01", Grey01, fp);
    WriteColor("Grey02", Grey02, fp);
    WriteColor("Grey03", Grey03, fp);
    WriteColor("Grey04", Grey04, fp);
    WriteColor("Grey05", Grey05, fp);
    WriteColor("Grey06", Grey06, fp);
    WriteColor("Grey07", Grey07, fp);
    WriteColor("Grey08", Grey08, fp);
    WriteColor("Grey09", Grey09, fp);
    WriteColor("Grey10", Grey10, fp);
    WriteColor("Grey11", Grey11, fp);
    WriteColor("Grey12", Grey12, fp);
    WriteColor("Grey13", Grey13, fp);
    WriteColor("Grey14", Grey14, fp);
    WriteColor("Grey15", Grey15, fp);
    WriteColor("Grey16", Grey16, fp);
    WriteColor("Grey17", Grey17, fp);
    WriteColor("Grey18", Grey18, fp);
    WriteColor("Grey19", Grey19, fp);

    fprintf(fp, "<EditAction> = %i\n", d_EditAction);
    fprintf(fp, "<DensityPlot> = %i\n", d_DensityPlot);
    fprintf(fp, "<VectorPlot> = %i\n", d_VectorPlot);
    fprintf(fp, "<GridFlag> = %i\n", d_GridFlag);
    fprintf(fp, "<SnapFlag> = %i\n", d_SnapFlag);
    fprintf(fp, "<MeshFlag> = %i\n", d_MeshFlag);
    fprintf(fp, "<LegendFlag> = %i\n", d_LegendFlag);
    fprintf(fp, "<NumContours> = %i\n", d_NumContours);
    fprintf(fp, "<ShowAr> = %i\n", d_ShowAr);
    fprintf(fp, "<ShowAi> = %i\n", d_ShowAi);
    fprintf(fp, "<ShowMask> = %i\n", d_ShowMask);
    fprintf(fp, "<GreyContours> = %i\n", d_GreyContours);
    fprintf(fp, "<PtsFlag> = %i\n", d_PtsFlag);
    fprintf(fp, "<ResetOnReload> = %i\n", d_ResetOnReload);
    fprintf(fp, "<Smooth> = %i\n", d_Smooth);
    fprintf(fp, "<PlotPoints> = %i\n", d_PlotPoints);
    fprintf(fp, "<LineIntegralPoints> = %i\n",
        pDoc->d_LineIntegralPoints);
    fprintf(fp, "<ShiftH> = %i\n", pDoc->d_ShiftH);

    fclose(fp);
  } else
    return FALSE;

  return TRUE;
}

void CFemmviewView::CheckIt()
{
  // make sure all of the right things are checked
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar *pToolBar1, *pToolBar2;
  pToolBar1 = &MFrm->m_FV_toolBar1;
  pToolBar2 = &MFrm->m_leftbar;
  CToolBarCtrl* tc1 = &pToolBar1->GetToolBarCtrl();
  CToolBarCtrl* tc2 = &pToolBar2->GetToolBarCtrl();
  if (SnapFlag == FALSE) {
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_UNCHECKED);
    tc2->PressButton(ID_SNAP_GRID, FALSE);
  } else {
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_CHECKED);
    tc2->PressButton(ID_SNAP_GRID, TRUE);
  }

  if (GridFlag == FALSE) {
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_UNCHECKED);
    tc2->PressButton(ID_SHOW_GRID, FALSE);
  } else {
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_CHECKED);
    tc2->PressButton(ID_SHOW_GRID, TRUE);
  }

  if (MeshFlag == FALSE) {
    MMnu->CheckMenuItem(ID_SHOW_MESH, MF_UNCHECKED);
    tc1->PressButton(ID_SHOW_MESH, FALSE);
  } else {
    MMnu->CheckMenuItem(ID_SHOW_MESH, MF_CHECKED);
    tc1->PressButton(ID_SHOW_MESH, TRUE);
  }

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_UNCHECKED);
  tc1->PressButton(ID_MENU_AREA, FALSE);
  tc1->PressButton(ID_MENU_CONTOUR, FALSE);
  tc1->PressButton(ID_MENU_POINT, FALSE);

  if (EditAction == 2) {
    MMnu->CheckMenuItem(ID_MENU_AREA, MF_CHECKED);
    tc1->PressButton(ID_MENU_AREA, TRUE);
  }
  if (EditAction == 1) {
    MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_CHECKED);
    tc1->PressButton(ID_MENU_CONTOUR, TRUE);
  }
  if (EditAction == 0) {
    MMnu->CheckMenuItem(ID_MENU_POINT, MF_CHECKED);
    tc1->PressButton(ID_MENU_POINT, TRUE);
  }

  if (PtsFlag == TRUE)
    MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_CHECKED);
  else
    MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_UNCHECKED);

  CFemmviewDoc* pDoc = GetDocument();
  if (pDoc->Smooth == FALSE)
    MMnu->CheckMenuItem(ID_SMOOTH, MF_UNCHECKED);
  else
    MMnu->CheckMenuItem(ID_SMOOTH, MF_CHECKED);

  if (ShowNames == FALSE) {
    MMnu->CheckMenuItem(ID_VIEW_SHOWNAMES, MF_UNCHECKED);
  } else {
    MMnu->CheckMenuItem(ID_VIEW_SHOWNAMES, MF_CHECKED);
  }

  if ((theApp.bShowOutputWindow) && (!IsMinimized()) && (!MFrm->IsIconic())) {
    MFrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
    MFrm->m_dlgBar.SetDlgItemText(IDC_OUTBOX, OutputWindowText);
  }
}

void CFemmviewView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  CFemmviewDoc* pDoc = GetDocument();

  if (EditAction == 1) {
    if (pDoc->contour.GetSize() > 1) {
      CBendContourDlg dlg;
      dlg.DoModal();
      if (dlg.m_angle != 0) {
        EraseUserContour(FALSE);
        pDoc->BendContour(dlg.m_angle, dlg.m_anglestep);
        DrawUserContour(TRUE);
      }
    }
  }

  if (EditAction == 2) {
    CFemmviewDoc* pDoc = GetDocument();
    int i, k, grp, sel;

    if (pDoc->meshelem.GetSize() > 0) {
      k = pDoc->InTriangle(mx, my);
      if (k >= 0) {
        pDoc->bHasMask = FALSE;
        grp = pDoc->blocklist[pDoc->meshelem[k].lbl].InGroup;
        sel = pDoc->blocklist[pDoc->meshelem[k].lbl].IsSelected;
        for (i = 0; i < pDoc->blocklist.GetSize(); i++)
          if ((pDoc->blocklist[i].InGroup == grp) && (pDoc->blocklist[i].IsSelected != sel)) {
            pDoc->blocklist[i].IsSelected = sel;
            DrawSelected = i;
            CDC* pDC = GetDC();
            OnDraw(pDC);
            DrawSelected = -1;
            ReleaseDC(pDC);
          }
      }
    }
  }

  CView::OnLButtonDblClk(nFlags, point);
}

void CFemmviewView::OnViewBHcurves()
{
  CXYPlotDlg dlg;
  CFemmviewDoc* pDoc = GetDocument();
  int PlotType, npoints, FileFormat;
  int i, k;
  BOOL ToFile;

  for (k = 0, i = 0; k < pDoc->blockproplist.GetSize(); k++)
    if (pDoc->blockproplist[k].BHpoints > 0)
      i++;
  if (i == 0) {
    MsgBox("No nonlinear materials in this solution");
    return;
  }

  dlg.ListType = 2;
  dlg.pDoc = pDoc;
  dlg.m_npoints = d_PlotPoints;

  if (dlg.DoModal() == IDOK) {
    // figure out the right material index;
    PlotType = dlg.XYPlotType;
    for (k = 0, i = 0; k < pDoc->blockproplist.GetSize(); k++) {
      if (pDoc->blockproplist[k].BHpoints > 0) {
        if (i == PlotType) {
          PlotType = k;
          k = (int)pDoc->blockproplist.GetSize();
        } else
          i++;
      }
    }

    FileFormat = dlg.FileFormat;
    ToFile = dlg.m_ToFile;
    npoints = dlg.m_npoints;
    CXYPlot xyplot;

    if (pDoc->Frequency == 0) {
      xyplot.Create(npoints, 2);
      strcpy(xyplot.lbls[0], "H, A/m");
      strcpy(xyplot.lbls[1], "B, T");
    } else {
      xyplot.Create(npoints, 3);
      strcpy(xyplot.lbls[0], "H, A/m");
      strcpy(xyplot.lbls[1], "Re[B], T");
      strcpy(xyplot.lbls[2], "Im[B], T");
    }

    CMaterialProp* blk = &(pDoc->blockproplist[PlotType]);
    double b;
    double db = (blk->Bdata[blk->BHpoints - 1]) / ((double)(npoints - 1));
    CComplex B, H;

    for (i = 1, b = db; i < npoints; i++, b += db) {
      H = blk->GetH(CComplex(b));
      B = abs(H) * b / H;
      H = abs(H);
      xyplot.M[i][0] = Re(H);
      xyplot.M[i][1] = Re(B);
      if (pDoc->Frequency != 0)
        xyplot.M[i][2] = Im(B);
    }

    if (ToFile == FALSE) {
      CMetaFileDC Meta;
      Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
      HGLOBAL BoundingBox = GlobalAlloc(0, 256);
      xyplot.MakePlot(&Meta, (char*)BoundingBox);
      HENHMETAFILE hMeta = Meta.CloseEnhanced();
      if (hMeta == NULL)
        MsgBox("No Handle...");
      if (OpenClipboard() == FALSE)
        MsgBox("Cannot access the Clipboard");
      else {
        EmptyClipboard();
        if (SetClipboardData(CF_ENHMETAFILE, hMeta) == NULL)
          MsgBox("Couldn't SetClipboardData");
        if (SetClipboardData(CF_TEXT, BoundingBox) == NULL)
          MsgBox("Couldn't SetClipboardData");
        CloseClipboard();

        // fire up plot viewer;
        ((CFemmApp*)AfxGetApp())->CreateNewDocument(8);
      }
    } else
      xyplot.ToDisk(FileFormat);
  }
}

void CFemmviewView::OnViewInfo()
{
  CFemmviewDoc* pDoc = GetDocument();
  CString s, z, tmp;

  // first line;
  s.Format("Title: %s\r\n", pDoc->GetTitle());

  // second line;
  switch (pDoc->LengthUnits) {
  case 0:
    s += "Length Units: Inches\r\n";
    z = " in";
    break;
  case 1:
    s += "Length Units: Millimeters\r\n";
    z = " mm";
    break;
  case 2:
    s += "Length Units: Centimeters\r\n";
    z = " cm";
    break;
  case 4:
    s += "Length Units: Mils\r\n";
    z = " mil";
    break;
  case 5:
    s += "Length Units: Micrometers\r\n";
    z = " um";
    break;
  default:
    s += "Length Units: Meters\r\n";
    z = " m";
    break;
  }

  // third line
  if (pDoc->ProblemType == TRUE)
    s += "Axisymmetric Solution\r\n";
  else {
    tmp.Format("2-D Planar (Depth: %g", pDoc->Depth / pDoc->LengthConv[pDoc->LengthUnits]);
    s += tmp + z + ")\r\n";
  }

  // fourth line
  tmp.Format("Frequency: %g Hz\r\n\r\n", pDoc->Frequency);
  s += tmp;

  // sixth line
  tmp.Format("%i Nodes\r\n", pDoc->meshnode.GetSize());
  s += tmp;

  // seventh line
  tmp.Format("%i Elements", pDoc->meshelem.GetSize());
  s += tmp;

  // display it
  Dspl->SetDlgItemText(IDC_OUTBOX, s);
  OutputWindowText = s;
}
void CFemmviewView::OnViewShownames()
{
  ShowNames = 1 - ShowNames;
  RedrawView();
}

void CFemmviewView::OnVplot()
{
  // TODO: Add your command handler code here
  CVPlotDlg dlg;
  CFemmviewDoc* pDoc = GetDocument();

  dlg.m_vectorscalefactor = VectorScaleFactor;
  dlg.m_plottype = VectorPlot;
  if (pDoc->Frequency == 0)
    dlg.ListType = 0;
  else
    dlg.ListType = 1;

  if (dlg.DoModal() == IDOK) {
    VectorScaleFactor = dlg.m_vectorscalefactor;
    VectorPlot = dlg.m_plottype;
    InvalidateRect(NULL);
  }
}

void CFemmviewView::MyMoveTo(CDC* pDC, int x, int y)
{
  Xm = x;
  Ym = y;
}

void CFemmviewView::MyLineTo(CDC* pDC, int x, int y)
{
  // trivial case
  if ((x == Xm) && (y == Ym))
    return;

  RECT r;
  int Zm;
  GetClientRect(&r);
  Zm = __max(__max(r.right, r.bottom), __max(r.left, r.top));
  Zm = (Zm * 3) / 2;

  // as long as all the coordinates are small enough,
  // everything works normally
  if ((abs(Xm) < Zm) && (abs(Ym) < Zm) && (abs(x) < Zm) && (abs(y) < Zm)) {
    pDC->MoveTo(Xm, Ym);
    pDC->LineTo(x, y);
  } else {
    // otherwise, we have to do some limiting to make sure
    // that the plotting routines don't go berzerk.
    CComplex p, q, pc, qc;
    double a, b, c, u0, u1;

    p = (Xm + I * Ym) / ((double)Zm);
    q = (x + I * y) / ((double)Zm);
    pc = conj(p);
    qc = conj(q);

    c = Re(p * pc - 1.);
    b = Re(-2. * p * pc + pc * q + p * qc);
    a = Re(p * pc - pc * q - p * qc + q * qc);

    if (fabs(a) != 0) {
      b /= a;
      c /= a;
      if ((b * b - 4. * c) > 0) {
        // there is some possible intersection with the viewable region...
        u0 = -b / 2. + sqrt(b * b - 4. * c) / 2.;
        u1 = -b / 2. - sqrt(b * b - 4. * c) / 2.;
        if (u1 < u0) {
          c = u0;
          u0 = u1;
          u1 = c;
        }
        if ((u0 < 1) && (u1 > 0)) {
          if (u0 < 0)
            u0 = 0;
          if (u1 > 1)
            u1 = 1;
          pc = p * (1. - u0) + q * u0;
          qc = p * (1. - u1) + q * u1;
          pc *= ((double)Zm);
          qc *= ((double)Zm);
          pDC->MoveTo((int)Re(pc), (int)Im(pc));
          pDC->LineTo((int)Re(qc), (int)Im(qc));
        }
      }
    }
  }

  // update the current position
  Xm = x;
  Ym = y;
}

BOOL CFemmviewView::IsMinimized()
{
  CChildFrame* pChildFrm = (CChildFrame*)GetParentFrame();
  if (pChildFrm->GetStyle() & WS_MINIMIZE)
    return TRUE;
  return FALSE;
}

void CFemmviewView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
  if ((bActivate) && (pActivateView == this)) {
    CMainFrame* MFrm;
    MFrm = (CMainFrame*)GetTopLevelFrame();
    if (!MFrm->IsIconic()) {
      MFrm->SetBar(2);
      CheckIt();
    }
    if ((theApp.bShowOutputWindow) && (!IsMinimized()) && (!MFrm->IsIconic())) {

      MFrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
      MFrm->m_dlgBar.SetDlgItemText(IDC_OUTBOX, OutputWindowText);
    }
  }

  CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CFemmviewView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;
  return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CFemmviewView::OnSize(UINT nType, int cx, int cy)
{
  CView::OnSize(nType, cx, cy);

  // The child frame makes a special effort to pass down the
  // WM_SIZE message when a window is minimized.  The view
  // uses this message to minimize the output window, if it is visible.
  if (nType == SIZE_MINIMIZED) {
    CMainFrame* MFrm = (CMainFrame*)theApp.GetMainWnd();
    if (MFrm->m_dlgBar.GetStyle() & WS_VISIBLE)
      MFrm->m_dlgBar.ShowWindow(SW_HIDE);
  }
}

//	if((theApp.bShowOutputWindow) && (!IsMinimized()) &&
//		(!MFrm->IsIconic()) && ((MFrm->m_dlgBar.GetStyle() & WS_VISIBLE)!=0)){
//			MFrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
//			MFrm->m_dlgBar.SetDlgItemText(IDC_OUTBOX,OutputWindowText);
//	}