// cviewView.cpp : implementation of the CcviewView class
//

#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "femm.h"
#include "cv_problem.h"
#include "xyplot.h"
#include "cviewDoc.h"
#include "cviewView.h"
#include "GridMod.h"
#include "EnterPt.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "KbdZoom.h"
#include "cv_CPlotDlg2.h"
#include "cv_DPlotDlg2.h"
#include "cv_VPlotDlg.h"
#include "MyMsgBox.h"
#include "cv_XYPlotDlg.h"
#include "cv_LIntDlg.h"
#include "cv_BlockInt.h"
#include "cv_CircDlg.h"
#include "BendContourDlg.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int Xm, Ym;

#define MSKHI (1. + 1.e-08)
#define MSKLO (0. - 1.e-08)

extern lua_State* lua;
extern BOOL bLinehook;

/////////////////////////////////////////////////////////////////////////////
// CcviewView

IMPLEMENT_DYNCREATE(CcviewView, CView)

BEGIN_MESSAGE_MAP(CcviewView, CView)
//{{AFX_MSG_MAP(CcviewView)
ON_COMMAND(ID_SMOOTH, OnSmooth)
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
ON_WM_ERASEBKGND()
ON_WM_RBUTTONDBLCLK()
ON_WM_SIZE()
ON_COMMAND(ID_PAN_DOWN, OnPanDown)
ON_COMMAND(ID_PAN_LEFT, OnPanLeft)
ON_COMMAND(ID_PAN_RIGHT, OnPanRight)
ON_COMMAND(ID_PAN_UP, OnPanUp)
ON_COMMAND(ID_VIEW_CIRCPROPS, OnViewCircprops)
ON_COMMAND(ID_EDIT_COPY_AS_METAFILE, OnEditCopyAsMetafile)
ON_COMMAND(ID_VIEW_INFO, OnViewInfo)
ON_COMMAND(ID_VIEW_SHOWBLOCKNAMES, OnViewShowblocknames)
ON_COMMAND(ID_VPLOT, OnVplot)
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void MyMessageBox(CString s);

/////////////////////////////////////////////////////////////////////////////
// CcviewView construction/destruction

CcviewView::CcviewView()
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
  d_ShowAr = FALSE;
  d_ShowAi = FALSE;
  d_ShowMask = FALSE;
  d_DensityPlot = 1;
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
  bOnDraw = FALSE;
}

CcviewView::~CcviewView()
{
  ((CFemmApp*)AfxGetApp())->NumViews--;
  if (!((CFemmApp*)AfxGetApp())->NumViews)
    ((CMainFrame*)AfxGetApp()->GetMainWnd())->SetBar(0);
}

BOOL CcviewView::PreCreateWindow(CREATESTRUCT& cs)
{
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CcviewView drawing
void CcviewView::ScreenToDwg(int xs, int ys, double* xd, double* yd, RECT* r)
{
  double x, y;
  x = (double)xs;
  y = (double)ys;

  *xd = x / mag + ox;
  *yd = (((double)r->bottom) - y - 1.) / mag + oy;
}

BOOL CcviewView::DwgToScreen(double xd, double yd, int* xs, int* ys, RECT* r)
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

void CcviewView::DoContours(CDC* pDC, int* p, int side, int Aflag)
{
  int i, j, k;
  static double q[4][4];
  static double* m[4];
  static int u[4];
  double* swap;
  double hi, lo, a, b, z, offset;

  CcviewDoc* pDoc = GetDocument();

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
      m[i][2] = Re(pDoc->meshnode[u[i]].v);
    if (Aflag == 1)
      m[i][2] = Im(pDoc->meshnode[u[i]].v);
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
void CcviewView::PlotFluxDensity(CDC* pDC, int elmnum, int flag)
{

  int i, j, k, n, lav;
  double b, bn[3], bh, bl, y, z;
  CComplex b1[3], b2[3], Ex, Ey;
  POINT ps[3];
  static double c[64][3], p[3][2];
  CElement elm;
  CcviewDoc* pDoc = GetDocument();

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

  for (i = 0; i < 3; i++) {
    switch (DensityPlot) {
    // |Voltage|
    case 1:
      bn[i] = abs(pDoc->meshnode[elm.p[i]].v);
      break;

    // |Re(Voltage)|
    case 2:
      bn[i] = Re(pDoc->meshnode[elm.p[i]].v);
      break;

    // |Im(Voltage)|
    case 3:
      bn[i] = Im(pDoc->meshnode[elm.p[i]].v);
      break;

    // |J|
    case 4:
      if (pDoc->Smooth == TRUE)
        bn[i] = abs(abs(elm.jx[i]) + I * abs(elm.jy[i]));
      else
        bn[i] = abs(abs(elm.Jx) + I * abs(elm.Jy));
      break;

    // |Re(J)|
    case 5:
      if (pDoc->Smooth == TRUE)
        bn[i] = abs(Re(elm.jx[i].re) + I * Re(elm.jy[i]));
      else
        bn[i] = abs(Re(elm.Jx) + I * Re(elm.Jy));
      break;

    // |Im(J)|
    case 6:
      if (pDoc->Smooth == TRUE)
        bn[i] = abs(Im(elm.jx[i]) + I * Im(elm.jy[i]));
      else
        bn[i] = abs(Im(elm.Jx) + I * Im(elm.Jy));
      break;

    // |E|
    case 7:
      if (pDoc->Smooth == TRUE) {
        Ex = elm.jx[i] / pDoc->blockproplist[elm.blk].Kx;
        Ey = elm.jy[i] / pDoc->blockproplist[elm.blk].Ky;
      } else {
        Ex = elm.Jx / pDoc->blockproplist[elm.blk].Kx;
        Ey = elm.Jy / pDoc->blockproplist[elm.blk].Ky;
      }
      bn[i] = abs(abs(Ex) + I * abs(Ey));
      break;

    // |Re(E)|
    case 8:
      if (pDoc->Smooth == TRUE) {
        Ex = Re(elm.jx[i] / pDoc->blockproplist[elm.blk].Kx);
        Ey = Re(elm.jy[i] / pDoc->blockproplist[elm.blk].Ky);
      } else {
        Ex = Re(elm.Jx / pDoc->blockproplist[elm.blk].Kx);
        Ey = Re(elm.Jy / pDoc->blockproplist[elm.blk].Ky);
      }
      bn[i] = abs(abs(Ex) + I * abs(Ey));
      break;

    // |Im(E)|
    case 9:
      if (pDoc->Smooth == TRUE) {
        Ex = Im(elm.jx[i] / pDoc->blockproplist[elm.blk].Kx);
        Ey = Im(elm.jy[i] / pDoc->blockproplist[elm.blk].Ky);
      } else {
        Ex = Im(elm.Jx / pDoc->blockproplist[elm.blk].Kx);
        Ey = Im(elm.Jy / pDoc->blockproplist[elm.blk].Ky);
      }
      bn[i] = abs(abs(Ex) + I * abs(Ey));
      break;

    default:
      bn[i] = 0;
      break;
    }
  }

  flag = 1;

  bl = pDoc->PlotBounds[DensityPlot - 1][0];
  bh = pDoc->PlotBounds[DensityPlot - 1][1];

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

void CcviewView::PlotSelectedElm(CDC* pDC, CElement& elm)
{
  int i;
  POINT p[3];
  CcviewDoc* pDoc = GetDocument();

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

void CcviewView::DrawUserContour(BOOL flag)
{
  // FALSE == just draw last line in contour;
  // TRUE  == draw all lines in contour;
  int i, xs, ys, xso, yso;

  CcviewDoc* pDoc = GetDocument();
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

void CcviewView::EraseUserContour(BOOL flag)
{
  // FALSE == just draw last line in contour;
  // TRUE  == draw all lines in contour;
  int i, xs, ys, xso, yso;

  CcviewDoc* pDoc = GetDocument();
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

BOOL CcviewView::Pump()
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

void CcviewView::OnDraw(CDC* pDC)
{
  CcviewDoc* pDoc = GetDocument();
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
    VectorPlot = d_VectorPlot;
    VectorScaleFactor = 1;
    PtsFlag = d_PtsFlag;
    pDoc->Smooth = d_Smooth;

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
      for (i = 0, xd = GridSize * ceil(ox / GridSize); i <= nx; i++, xd += GridSize)
        for (j = 0, yd = GridSize * ceil(oy / GridSize); j <= ny; j++, yd += GridSize) {
          if ((i == skip * (i / skip)) || (j == skip * (j / skip))) {
            DwgToScreen(xd, yd, &xs, &ys, &r);
            pDC->SetPixel((int)xs, (int)ys, GridColor);
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
      else
        pOldPen = pDC->SelectObject(&penRed);

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
      else
        pOldPen = pDC->SelectObject(&penRed);
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
    BOOL PlotJ;
    double MaxVal;
    int xp, yp;
    CComplex vr, va, c0, c1;
    CComplex px, py;
    int ilo, ihi, jlo, jhi;

    if ((VectorPlot == 1) || (VectorPlot == 3) || (VectorPlot == 5)) {
      MaxVal = pDoc->d_PlotBounds[3][1];
      PlotJ = TRUE;
    } else {
      MaxVal = pDoc->d_PlotBounds[6][1];
      PlotJ = FALSE;
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
                px = u.Jx;
                py = u.Jy;
                if (!PlotJ) {
                  px /= u.Kx;
                  py /= u.Ky;
                }

                // plot real arrows
                if (MakePlots & 1) {
                  pOldPen = pDC->SelectObject(&penRealVector);
                  vr = (Re(px) + I * Re(py)) * GridSize * VectorScaleFactor / MaxVal;
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

                // plot imaginary arrows
                if (MakePlots & 2) {
                  pOldPen = pDC->SelectObject(&penImagVector);
                  vr = (Im(px) + I * Im(py)) * GridSize * VectorScaleFactor / MaxVal;
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
              }
            }
          }
        } // end of if(rt<...
      }
    }
  }

  if (EditAction == 1)
    DrawUserContour(TRUE);

  // draw block names/labels
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

      pDC->SetTextAlign(TA_BASELINE);
      pDC->SetBkMode(TRANSPARENT);
      lbl = pDoc->blockproplist[pDoc->blocklist[i].BlockType].BlockName;
      pDC->TextOut(xs + 5, ys, lbl);
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
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextAlign(TA_TOP);
    pDC->SetTextAlign(TA_LEFT);
    BackBrush.CreateSolidBrush(BackColor);
    pOldBrush = pDC->SelectObject(&BackBrush);
    pDC->Rectangle(r.right - 212 - 6, r.top + 6, r.right - 6, r.top + 24 + 21 * 16);
    pDC->SelectObject(pOldBrush);

    if (DensityPlot == 1)
      sprintf(cc, "Density Plot: |V|, Volts");
    if (DensityPlot == 2)
      sprintf(cc, "Density Plot: |Re(V)|, Volts");
    if (DensityPlot == 3)
      sprintf(cc, "Density Plot: |Im(V)|, Volts");
    if (DensityPlot == 4)
      sprintf(cc, "Density Plot: |J|, A/m^2");
    if (DensityPlot == 5)
      sprintf(cc, "Density Plot: |Re(J)|, A/m^2");
    if (DensityPlot == 6)
      sprintf(cc, "Density Plot: |Im(J)|, A/m^2");
    if (DensityPlot == 7)
      sprintf(cc, "Density Plot: |E|, V/m");
    if (DensityPlot == 8)
      sprintf(cc, "Density Plot: |Re(E)|, V/m");
    if (DensityPlot == 9)
      sprintf(cc, "Density Plot: |Im(E)|, V/m");
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

      double Blb = pDoc->PlotBounds[DensityPlot - 1][0];
      double Bub = pDoc->PlotBounds[DensityPlot - 1][1];
      if (i == 0)
        sprintf(cc, "%.3e : >%.3e", ((double)(19 - i)) * dta + Blb,
            ((double)(20 - i)) * dta + Blb);
      else if (i == 19)
        sprintf(cc, "<%.3e : %.3e", ((double)(19 - i)) * dta + Blb,
            ((double)(20 - i)) * dta + Blb);
      else
        sprintf(cc, "%.3e : %.3e", ((double)(19 - i)) * dta + Blb,
            ((double)(20 - i)) * dta + Blb);
      pDC->TextOut(r.right - 182, r.top + 16 * i + 12, cc, (int)strlen(cc));
    }
    pDC->SelectObject(pOldPen);
  }

  pDC->SelectObject(pOldFont);
  fntArial.DeleteObject();
  bOnDraw = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CcviewView printing

BOOL CcviewView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CcviewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CcviewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CcviewView diagnostics

#ifdef _DEBUG
void CcviewView::AssertValid() const
{
  CView::AssertValid();
}

void CcviewView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CcviewDoc* CcviewView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcviewDoc)));
  return (CcviewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CcviewView message handlers

void CcviewView::OnMouseMove(UINT nFlags, CPoint point)
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
  CcviewDoc* pDoc = GetDocument();

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

void CcviewView::OnZoomIn()
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

void CcviewView::OnZoomOut()
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

void CcviewView::OnZoomNatural()
{
  CcviewDoc* pDoc = GetDocument();
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

void CcviewView::OnKbdZoom()
{
  CcviewDoc* pDoc = GetDocument();
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

void CcviewView::OnZoomWindow()
{
  ZoomWndFlag = TRUE;
  ZoomPix.RemoveAll();
}

void CcviewView::OnShowGrid()
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

void CcviewView::OnSetGrid()
{
  CGridMod pDlg;
  CcviewDoc* pDoc = GetDocument();

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

void CcviewView::OnSnapGrid()
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

void CcviewView::OnShowMesh()
{
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_CV_toolBar1;
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

void CcviewView::OnCplot()
{
  CcviewDoc* pDoc = GetDocument();

  cvCCPlotDlg2 dlg;
  dlg.m_showa = ShowAr;
  dlg.m_showa2 = ShowAi;
  dlg.m_showmask = ShowMask;
  dlg.m_numcontours = NumContours;
  dlg.m_alow = pDoc->A_Low;
  dlg.m_ahigh = pDoc->A_High;
  dlg.Alb = __min(pDoc->A_lb.re, pDoc->A_lb.im);
  dlg.Aub = __max(pDoc->A_ub.re, pDoc->A_ub.im);
  if (dlg.DoModal() == IDOK) {
    ShowAr = dlg.m_showa;
    ShowAi = dlg.m_showa2;
    ShowMask = dlg.m_showmask;
    pDoc->A_Low = dlg.m_alow;
    pDoc->A_High = dlg.m_ahigh;
    NumContours = dlg.m_numcontours;
    RedrawView();
  }
}

void CcviewView::OnDplot()
{
  CcviewDoc* pDoc = GetDocument();
  int i, k;

  cvCDPlotDlg2 dlg;

  if (!DensityPlot)
    dlg.m_showit = FALSE;
  else
    dlg.m_showit = TRUE;
  dlg.m_showlegend = LegendFlag;
  dlg.m_gscale = GreyContours;
  dlg.DensityPlot = DensityPlot;
  dlg.listtype = 0;

  for (i = 0; i < 9; i++)
    for (k = 0; k < 2; k++) {
      dlg.PlotBounds[i][k] = pDoc->PlotBounds[i][k];
      dlg.d_PlotBounds[i][k] = pDoc->d_PlotBounds[i][k];
    }

  if (dlg.DoModal() == IDOK) {
    if (dlg.m_showit)
      DensityPlot = dlg.DensityPlot;
    else
      DensityPlot = 0;
    LegendFlag = dlg.m_showlegend;
    GreyContours = dlg.m_gscale;
    for (i = 0; i < 9; i++)
      for (k = 0; k < 2; k++)
        pDoc->PlotBounds[i][k] = dlg.PlotBounds[i][k];
    RedrawView();
  }
}

void CcviewView::OnInitialUpdate()
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

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_CV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
}

void CcviewView::DisplayPointProperties(double px, double py)
{
  CcviewDoc* pDoc = GetDocument();
  char c[80];
  CPointVals u;
  CComplex Jtot;
  if (pDoc->nodelist.GetSize() == 0)
    return;
  if (pDoc->GetPointValues(px, py, u) == TRUE) {
    CString outbox, s;
    if (pDoc->ProblemType >= 0) {
      if (pDoc->ProblemType == 0) {
        s.Format("Point: x=%g, y=%g", px, py);
        outbox = s;
        s.Format("V   = %s V", u.v.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("|J| = %g A/m^2", abs(abs(u.Jx) + I * abs(u.Jy)));
        outbox += "\r\n" + s;
        s.Format("Jx  = %s A/m^2", u.Jx.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("Jy  = %s A/m^2", u.Jy.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("|E| = %g V/m", abs(abs(u.Ex) + I * abs(u.Ey)));
        outbox += "\r\n" + s;
        s.Format("Ex  = %s V/m", u.Ex.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("Ey  = %s V/m", u.Ey.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("ex  = %s (rel)", u.ex.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("ey  = %s (rel)", u.ey.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("ox  = %g S/m", u.ox);
        outbox += "\r\n" + s;
        s.Format("oy  = %g S/m", u.ox);
        outbox += "\r\n" + s;
        s.Format("|Jc| = %g A/m^2", abs(abs(u.Jcx) + I * abs(u.Jcy)));
        outbox += "\r\n" + s;
        s.Format("|Jd| = %g A/m^2", abs(abs(u.Jdx) + I * abs(u.Jdy)));
        outbox += "\r\n" + s;

        Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
        OutputWindowText = outbox;
      } else {
        s.Format("Point: r=%g, z=%g", px, py);
        outbox = s;
        s.Format("V   = %s V", u.v.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("|J| = %g A/m^2", abs(abs(u.Jx) + I * abs(u.Jy)));
        outbox += "\r\n" + s;
        s.Format("Jr  = %s A/m^2", u.Jx.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("Jz  = %s A/m^2", u.Jy.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("|E| = %g V/m", abs(abs(u.Ex) + I * abs(u.Ey)));
        outbox += "\r\n" + s;
        s.Format("Er  = %s V/m", u.Ex.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("Ez  = %s V/m", u.Ey.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("er  = %s (rel)", u.ex.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("ez  = %s (rel)", u.ey.ToStringAlt(c));
        outbox += "\r\n" + s;
        s.Format("or  = %g S/m", u.ox);
        outbox += "\r\n" + s;
        s.Format("oz  = %g S/m", u.ox);
        outbox += "\r\n" + s;
        s.Format("|Jc| = %g A/m^2", abs(abs(u.Jcx) + I * abs(u.Jcy)));
        outbox += "\r\n" + s;
        s.Format("|Jd| = %g A/m^2", abs(abs(u.Jdx) + I * abs(u.Jdy)));
        outbox += "\r\n" + s;

        Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
        OutputWindowText = outbox;
      }
    }
  }
}

void CcviewView::OnLButtonDown(UINT nFlags, CPoint point)
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
    CcviewDoc* pDoc = GetDocument();
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
    CcviewDoc* pDoc = GetDocument();

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
    }
  }

  CView::OnLButtonDown(nFlags, point);
}

void CcviewView::OnLButtonUp(UINT nFlags, CPoint point)
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

void CcviewView::OnRButtonDown(UINT nFlags, CPoint point)
{
  if (bLinehook != FALSE) {
    CView::OnRButtonDown(nFlags, point);
    return;
  }

  if (EditAction == 1) {
    CComplex z;
    int i;
    CcviewDoc* pDoc = GetDocument();
    z.Set(mx, my);
    i = (int)pDoc->contour.GetSize();
    if (i > 0) {
      if (z != pDoc->contour[i - 1])
        pDoc->contour.Add(z);
    } else
      pDoc->contour.Add(z);
    DrawUserContour(FALSE);
  }

  if (EditAction == 2) {
    CcviewDoc* pDoc = GetDocument();
    int clnode, clseg, claseg, i, j;
    double dnode, dseg, daseg, d;

    // find distance to closest node;
    clnode = pDoc->ClosestNode(mx, my);
    if (clnode >= 0)
      dnode = pDoc->nodelist[clnode].GetDistance(mx, my);
    else
      dnode = -1.;

    // find distance to closest segment;
    clseg = pDoc->ClosestSegment(mx, my);
    if (clseg >= 0)
      dseg = pDoc->ShortestDistance(mx, my, clseg);
    else
      dseg = -1.;

    // find distance to closest arc segment;
    claseg = pDoc->ClosestArcSegment(mx, my);
    if (claseg >= 0)
      daseg = pDoc->ShortestDistanceFromArc(CComplex(mx, my), pDoc->arclist[claseg]);
    else
      daseg = -1.;

    // now, compare to find the closest entity;
    j = -1;
    if (clnode >= 0) {
      d = dnode;
      j = pDoc->nodelist[clnode].InConductor;
    }
    if ((dseg < d) && (clseg >= 0)) {
      d = dseg;
      j = pDoc->linelist[clseg].InConductor;
    }
    if ((daseg < d) && (claseg >= 0)) {
      d = daseg;
      j = pDoc->arclist[claseg].InConductor;
    }

    if (j < 0)
      return; // return if the closest object is associated with
              // the default group, or if there are no objects.

    pDoc->bHasMask = FALSE;

    // now, the group associated with the nearest entity is in j;
    // toggle the select of all objects in this group;
    for (i = 0; i < pDoc->nodelist.GetSize(); i++)
      if (pDoc->nodelist[i].InConductor == j)
        pDoc->nodelist[i].ToggleSelect();
    for (i = 0; i < pDoc->linelist.GetSize(); i++)
      if (pDoc->linelist[i].InConductor == j)
        pDoc->linelist[i].ToggleSelect();
    for (i = 0; i < pDoc->arclist.GetSize(); i++)
      if (pDoc->arclist[i].InConductor == j)
        pDoc->arclist[i].ToggleSelect();
    for (i = 0; i < pDoc->meshnode.GetSize(); i++) {
      if (pDoc->meshnode[i].Q == j) {
        pDoc->meshnode[i].IsSelected = 1 - pDoc->meshnode[i].IsSelected;
      }
    }
    //	DrawPSLG();
    RedrawView();
  }

  CView::OnRButtonDown(nFlags, point);
}

void CcviewView::OnEditCopyAsMetafile()
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

void CcviewView::OnEditCopy()
{
  // puts a bitmap of the screen on the clipboard
  RECT r;
  CDC tempDC;
  CBitmap bitmap;
  CDC* pDC = GetDC();

  GetClientRect(&r);
  tempDC.CreateCompatibleDC(pDC);
  bitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
  tempDC.SelectObject(&bitmap);

  tempDC.BitBlt(0, 0, r.right, r.bottom, pDC, 0, 0, SRCCOPY);

  OpenClipboard();
  EmptyClipboard();
  SetClipboardData(CF_BITMAP, HBITMAP(bitmap));
  CloseClipboard();
}

void CcviewView::OnReload()
{
  CcviewDoc* pDoc = GetDocument();
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

void CcviewView::OnMenuArea()
{
  CcviewDoc* pDoc = GetDocument();
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
  pToolBar = &MFrm->m_CV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_CHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_UNCHECKED);
  tc->PressButton(ID_MENU_AREA, TRUE);
  tc->PressButton(ID_MENU_CONTOUR, FALSE);
  tc->PressButton(ID_MENU_POINT, FALSE);
}

void CcviewView::OnMenushowpts()
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

void CcviewView::OnMenuContour()
{
  CcviewDoc* pDoc = GetDocument();

  if (EditAction == 2) {
    BOOL flg = FALSE;
    pDoc->bHasMask = FALSE;
    UnselectAll();

    RedrawView();
  }

  EditAction = 1;

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_CV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_CHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_UNCHECKED);
  tc->PressButton(ID_MENU_AREA, FALSE);
  tc->PressButton(ID_MENU_CONTOUR, TRUE);
  tc->PressButton(ID_MENU_POINT, FALSE);
}

void CcviewView::OnMenuPoint()
{
  CcviewDoc* pDoc = GetDocument();

  if (EditAction == 1) {
    EraseUserContour(TRUE);
    pDoc->contour.RemoveAll();
  }
  if (EditAction == 2) {
    BOOL flg = FALSE;
    pDoc->bHasMask = FALSE;
    UnselectAll();
    RedrawView();
  }

  EditAction = 0;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_CV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  MMnu->CheckMenuItem(ID_MENU_AREA, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_CONTOUR, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_MENU_POINT, MF_CHECKED);
  tc->PressButton(ID_MENU_AREA, FALSE);
  tc->PressButton(ID_MENU_CONTOUR, FALSE);
  tc->PressButton(ID_MENU_POINT, TRUE);
}

void CcviewView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CcviewDoc* pDoc = GetDocument();

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
    UnselectAll();
    RedrawView();
  }

  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CcviewView::UnselectAll()
{
  CcviewDoc* pDoc = GetDocument();
  int i;

  pDoc->bHasMask = FALSE;
  for (i = 0; i < pDoc->blocklist.GetSize(); i++)
    pDoc->blocklist[i].IsSelected = FALSE;
  for (i = 0; i < pDoc->meshnode.GetSize(); i++)
    pDoc->meshnode[i].IsSelected = FALSE;
  for (i = 0; i < pDoc->nodelist.GetSize(); i++)
    pDoc->nodelist[i].IsSelected = FALSE;
  for (i = 0; i < pDoc->linelist.GetSize(); i++)
    pDoc->linelist[i].IsSelected = FALSE;
  for (i = 0; i < pDoc->arclist.GetSize(); i++)
    pDoc->arclist[i].IsSelected = FALSE;
}

void CcviewView::EnterPoint()
{
  CEnterPt pDlg;
  double x, y, t;
  CcviewDoc* pDoc = GetDocument();

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

void CcviewView::SpecialLine(CDC* pDC, int X0, int Y0, int X1, int Y1, int flag)
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

void CcviewView::OnMenuPlot()
{
  cvCXYPlotDlg dlg;
  CcviewDoc* pDoc = GetDocument();
  int PlotType, npoints, FileFormat;
  BOOL ToFile;

  if ((EditAction != 1) || (pDoc->contour.GetSize() == 0)) {
    MsgBox("Cannot create a plot;\nNo contour has been defined");
    return;
  }

  dlg.m_npoints = d_PlotPoints;
  if (dlg.DoModal() == IDOK) {
    PlotType = dlg.XYPlotType;
    FileFormat = dlg.FileFormat;
    ToFile = dlg.m_ToFile;
    npoints = dlg.m_npoints;
    CXYPlot xyplot;
    if (bLinehook == FALSE)
      BeginWaitCursor();
    pDoc->GetLineValues(xyplot, PlotType, npoints);
    if (bLinehook == FALSE)
      EndWaitCursor();
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

void CcviewView::OnMenuIntegrate()
{
  CcviewDoc* pDoc = GetDocument();
  int inttype;

  if (EditAction == 0) {
    MsgBox("Cannot integrate;\nNo contour or area\nhas been defined.");
    return;
  }

  if ((EditAction == 1) && (pDoc->contour.GetSize() == 0)) {
    MsgBox("Cannot integrate;\nNo contour has been defined");
    return;
  }

  if (EditAction == 2) {
    int i;
    BOOL flg = FALSE;
    for (i = 0; i < pDoc->blocklist.GetSize(); i++)
      if (pDoc->blocklist[i].IsSelected == TRUE)
        flg = TRUE;
    for (i = 0; i < pDoc->meshnode.GetSize(); i++)
      if (pDoc->meshnode[i].IsSelected == TRUE)
        flg = TRUE;
    if (flg == FALSE) {
      MsgBox("Cannot integrate;\nNo area has been selected");
      return;
    }
  }

  if (EditAction == 1) {
    cvCLIntDlg dlg;
    int inttype;
    CComplex z[4];
    char c[80], d[80], s[512];
    CString ss;

    dlg.Frequency = pDoc->Frequency;
    if (dlg.DoModal() == IDOK) {
      inttype = dlg.linttype;
      switch (inttype) {
      case 0:
        pDoc->LineIntegral(inttype, z);
        sprintf(s, "Voltage Difference = %s Volts", z[0].ToStringAlt(c));
        MyMessageBox(s);
        break;

      case 1:
        if (bLinehook == FALSE)
          BeginWaitCursor();
        pDoc->LineIntegral(inttype, z);
        if (bLinehook == FALSE)
          EndWaitCursor();
        sprintf(s, "Current = %s A\nAverage Current = %s A/m^2",
            z[0].ToStringAlt(c), z[1].ToStringAlt(d));
        MyMessageBox(s);
        break;

      case 2:
        pDoc->LineIntegral(inttype, z);
        sprintf(s, "Contour length = %g meters\nSurface Area = %g meter^2", Re(z[0]), Re(z[1]));
        MyMessageBox(s);
        break;

      case 4:
        if (bLinehook == FALSE)
          BeginWaitCursor();
        pDoc->LineIntegral(inttype, z);
        if (bLinehook == FALSE)
          EndWaitCursor();
        if (pDoc->ProblemType == 0) {
          sprintf(s, "DC Force:\n x-direction = %g N\n y-direction = %g N\n", z[2].re, z[3].re);
          ss = s;
          sprintf(s, "\n%g Hz Force:\n x-direction = %s N\n y-direction = %s N",
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
        break;

      case 5:
        if (bLinehook == FALSE)
          BeginWaitCursor();
        pDoc->LineIntegral(inttype, z);
        if (bLinehook == FALSE)
          EndWaitCursor();
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
        break;

      default:
        sprintf(s, "This space left intentionally blank");
        break;
      }
    }
  }

  if (EditAction == 2) {
    cvCBlockInt dlg;
    char c[80], d[80];

    dlg.Frequency = pDoc->Frequency;
    if (dlg.DoModal() == IDOK) {
      inttype = dlg.binttype;
      CComplex z1, z2, z3, z4;
      char s[512];
      // Average Temp over volume
      // Block cross-section area
      // Block volume
      // Average F over volume
      // Average G over volume
      switch (inttype) {
      case 0:
        z1 = pDoc->BlockIntegral(0);
        sprintf(s, "%g W", Re(z1));
        break;

      case 1:
        z1 = pDoc->BlockIntegral(1);
        sprintf(s, "%g VAR", abs(z1));
        break;
      case 2:
        z1 = pDoc->BlockIntegral(2);
        sprintf(s, "%g VA", abs(z1));
        break;

      case 3:
        z1 = pDoc->BlockIntegral(3);
        sprintf(s, "%g J", Re(z1));
        break;

      case 4:
        z1 = pDoc->BlockIntegral(4);
        sprintf(s, "%g m^2", Re(z1));
        break;

      case 5:
        z1 = pDoc->BlockIntegral(5);
        sprintf(s, "%g m^3", Re(z1));
        break;

      // Weighted Stress Tensor Force
      case 6:
        if (!pDoc->MakeMask())
          return;
        z1 = pDoc->BlockIntegral(6);
        z2 = pDoc->BlockIntegral(7);
        z3 = pDoc->BlockIntegral(8);
        z4 = pDoc->BlockIntegral(9);
        if (pDoc->ProblemType == 0) {
          sprintf(s, "Steady-state force:\nx-component: %g N\ny-component: %g N\n2x Frequency force:\nx-component: %s N\ny-component: %s N",
              z1.re, z2.re, z3.ToStringAlt(c), z4.ToStringAlt(d));
        } else {
          sprintf(s, "Steady-state force:\nr-component: 0. N\nz-component: %g N\n2x Frequency force:\nr-component: 0. N\nz-component: %s N",
              z2.re, z4.ToStringAlt(d));
        }
        if (ShowMask)
          InvalidateRect(NULL);
        break;

      // Weighted Stress Tensor Torque
      case 7:
        if (!pDoc->MakeMask())
          return;
        z1 = pDoc->BlockIntegral(10);
        z2 = pDoc->BlockIntegral(11);
        if (pDoc->ProblemType == 0) {
          sprintf(s, "Torque about (0,0):\nSteady State: %g N*m\n2x Frequency: %s N*m",
              z1.re, z2.ToStringAlt(d));
        } else {
          sprintf(s, "No torque possible in axisymmetric case");
        }
        if (ShowMask)
          InvalidateRect(NULL);
        break;

      default:
        sprintf(s, "Invalid integral selection");
        break;
      }
      MyMessageBox(s);
    }
  }
}

void CcviewView::OnSmooth()
{
  CcviewDoc* pDoc = GetDocument();
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

BOOL CcviewView::OnEraseBkgnd(CDC* pDC)
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

void CcviewView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
  CcviewDoc* pDoc = GetDocument();

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

void CcviewView::OnPanDown()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  oy -= 0.25 * y / mag;

  RedrawView();
}

void CcviewView::OnPanLeft()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox -= 0.25 * x / mag;

  RedrawView();
}

void CcviewView::OnPanRight()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox += 0.25 * x / mag;

  RedrawView();
}

void CcviewView::OnPanUp()
{
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  oy += 0.25 * y / mag;

  RedrawView();
}

void CcviewView::OnViewCircprops()
{
  cvCCircDlg dlg;

  dlg.TheDoc = GetDocument();
  dlg.DoModal();
}

void CcviewView::RedrawView()
{
  LinePix.RemoveAll();
  InvalidateRect(NULL);
}

BOOL CcviewView::ScanPreferences()
{
  FILE* fp;
  CString fname;

  fname = BinDir + "cview.cfg";

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

      if (_strnicmp(q, "<NameColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        NameColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MaskColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        MaskLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorRColor>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RealVectorColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorIColor>", 13) == 0) {
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

      if (_strnicmp(q, "<ShowNames>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_ShowNames);
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

BOOL CcviewView::WritePreferences()
{
  FILE* fp;
  CString fname;
  CcviewDoc* pDoc = GetDocument();

  fname = BinDir + "cview.cfg";

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
    WriteColor("NameColor", NameColor, fp);
    WriteColor("MaskColor", MaskLineColor, fp);
    WriteColor("VectorRColor", RealVectorColor, fp);
    WriteColor("VectorIColor", ImagVectorColor, fp);
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
    fprintf(fp, "<ShowNames> = %i\n", d_ShowNames);
    fprintf(fp, "<LineIntegralPoints> = %i\n",
        pDoc->d_LineIntegralPoints);

    fclose(fp);
  } else
    return FALSE;

  return TRUE;
}

void CcviewView::CheckIt()
{
  // make sure all of the right things are checked
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar *pToolBar1, *pToolBar2;
  pToolBar1 = &MFrm->m_CV_toolBar1;
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

  CcviewDoc* pDoc = GetDocument();
  if (pDoc->Smooth == FALSE)
    MMnu->CheckMenuItem(ID_SMOOTH, MF_UNCHECKED);
  else
    MMnu->CheckMenuItem(ID_SMOOTH, MF_CHECKED);

  if (ShowNames == FALSE)
    MMnu->CheckMenuItem(ID_VIEW_SHOWBLOCKNAMES, MF_UNCHECKED);
  else
    MMnu->CheckMenuItem(ID_VIEW_SHOWBLOCKNAMES, MF_CHECKED);

  if ((theApp.bShowOutputWindow) && (!IsMinimized()) && (!MFrm->IsIconic())) {
    MFrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
    MFrm->m_dlgBar.SetDlgItemText(IDC_OUTBOX, OutputWindowText);
  }
}

void CcviewView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  CcviewDoc* pDoc = GetDocument();

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

void CcviewView::OnViewInfo()
{
  CcviewDoc* pDoc = GetDocument();
  CString s, z, outbox;

  // first line;
  outbox.Format("Title: %s\r\n", pDoc->GetTitle());

  // second line;
  switch (pDoc->LengthUnits) {
  case 0:
    s = "Length Units: Inches";
    z = " in";
    break;
  case 1:
    s = "Length Units: Millimeters";
    z = " mm";
    break;
  case 2:
    s = "Length Units: Centimeters";
    z = " cm";
    break;
  case 4:
    s = "Length Units: Mils";
    z = " mil";
    break;
  case 5:
    s = "Length Units: Micrometers";
    z = " um";
    break;
  default:
    s = "Length Units: Meters";
    z = " m";
    break;
  }
  outbox += "\r\n" + s;

  // third line
  if (pDoc->ProblemType == TRUE)
    s = "Axisymmetric Solution";
  else {
    s.Format("2-D Planar (Depth: %g",
        pDoc->Depth / pDoc->LengthConv[pDoc->LengthUnits]);
    s = s + z + ")";
  }
  outbox += "\r\n" + s;

  // fourth line
  s.Format("Frequency: %g Hz", pDoc->Frequency);
  outbox += "\r\n" + s;

  // fifth line
  s.Format("");
  outbox += "\r\n" + s;

  // sixth line
  s.Format("%i Nodes", pDoc->meshnode.GetSize());
  outbox += "\r\n" + s;

  // seventh line
  s.Format("%i Elements", pDoc->meshelem.GetSize());
  outbox += "\r\n" + s;

  // display it
  Dspl->SetDlgItemText(IDC_OUTBOX, outbox);
  OutputWindowText = outbox;
}

void CcviewView::OnViewShowblocknames()
{
  // TODO: Add your command handler code here
  ShowNames = 1 - ShowNames;
  RedrawView();
}

void CcviewView::OnVplot()
{
  // TODO: Add your command handler code here
  cvCVPlotDlg dlg;

  dlg.m_vectorscalefactor = VectorScaleFactor;
  dlg.m_plottype = VectorPlot;
  if (dlg.DoModal() == IDOK) {
    VectorScaleFactor = dlg.m_vectorscalefactor;
    VectorPlot = dlg.m_plottype;
    InvalidateRect(NULL);
  }
}

void CcviewView::MyMoveTo(CDC* pDC, int x, int y)
{
  Xm = x;
  Ym = y;
}

void CcviewView::MyLineTo(CDC* pDC, int x, int y)
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

BOOL CcviewView::IsMinimized()
{
  CChildFrame* pChildFrm = (CChildFrame*)GetParentFrame();
  if (pChildFrm->GetStyle() & WS_MINIMIZE)
    return TRUE;
  return FALSE;
}

void CcviewView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
  if ((bActivate) && (pActivateView == this)) {
    CMainFrame* MFrm;
    MFrm = (CMainFrame*)GetTopLevelFrame();
    if (!MFrm->IsIconic()) {
      MFrm->SetBar(8);
      CheckIt();
    }
    if ((theApp.bShowOutputWindow) && (!IsMinimized()) && (!MFrm->IsIconic())) {
      MFrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
      MFrm->m_dlgBar.SetDlgItemText(IDC_OUTBOX, OutputWindowText);
    }
  }

  CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CcviewView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;
  return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CcviewView::OnSize(UINT nType, int cx, int cy)
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

  if ((nType == SIZE_MAXIMIZED) || (nType == SIZE_RESTORED)) {
    CMainFrame* MFrm = (CMainFrame*)theApp.GetMainWnd();
    if ((theApp.bShowOutputWindow) && (!IsMinimized()) && (!MFrm->IsIconic())) {
      MFrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
      MFrm->m_dlgBar.SetDlgItemText(IDC_OUTBOX, OutputWindowText);
    }
  }
}
