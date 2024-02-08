// FemmeView.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "FemmeDoc.h"
#include "FemmeView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "GRIDDLG.h"
#include "EnterPt.h"
#include "KbdZoom.h"
#include "ArcDlg.h"
#include "CopyDlg.h"
#include "fe_libdlg.h"
#include "scaledlg.h"
#include "MirrorDlg.h"
#include "GroupNumber.h"
#include "PromptBox.h"
#include "MakeABCDlg.h"

#include <winreg.h>
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int Xm, Ym;

extern lua_State* lua;
extern BOOL bLinehook;
extern HANDLE hProc;

/////////////////////////////////////////////////////////////////////////////
// CFemmeView

IMPLEMENT_DYNCREATE(CFemmeView, CView)

BEGIN_MESSAGE_MAP(CFemmeView, CView)
//{{AFX_MSG_MAP(CFemmeView)
ON_COMMAND(ID_NODE_OP, OnNodeOp)
ON_COMMAND(ID_SEGMENT_OP, OnSegmentOp)
ON_COMMAND(ID_BLOCK_OP, OnBlockOp)
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
ON_COMMAND(ID_SET_GRID, OnSetGrid)
ON_COMMAND(ID_SNAP_GRID, OnSnapGrid)
ON_COMMAND(ID_SHOW_MESH, OnShowMesh)
ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_WM_KEYDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_RBUTTONDBLCLK()
ON_COMMAND(ID_ZOOM_NATURAL, OnZoomNatural)
ON_COMMAND(ID_ZOOM_WINDOW, OnZoomWnd)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_MAKE_MESH, OnMakeMesh)
ON_COMMAND(ID_MENU_ANALYZE, OnMenuAnalyze)
ON_COMMAND(ID_MENU_VIEWRES, OnMenuViewres)
ON_COMMAND(ID_ARCSEG_OP, OnArcsegOp)
ON_COMMAND(ID_UNDO, OnUndo)
ON_COMMAND(ID_KBDZOOM, OnKbdZoom)
ON_COMMAND(ID_MOVE_OBJECTS, OnMoveObjects)
ON_COMMAND(ID_COPY_OBJECTS, OnCopyObjects)
ON_COMMAND(ID_DXFIN, OnDxfin)
ON_COMMAND(ID_PURGEMESH, OnPurgemesh)
ON_COMMAND(ID_DXFWRITE, OnDxfwrite)
ON_COMMAND(ID_SELECTWND, OnSelectwnd)
ON_WM_ERASEBKGND()
ON_COMMAND(ID_PAN_DOWN, OnPanDown)
ON_COMMAND(ID_PAN_LEFT, OnPanLeft)
ON_COMMAND(ID_PAN_RIGHT, OnPanRight)
ON_COMMAND(ID_PAN_UP, OnPanUp)
ON_COMMAND(ID_MENU_MATLIB, OnMenuMatlib)
ON_COMMAND(ID_GROUP_OP, OnGroupOp)
ON_COMMAND(ID_OPEN_SELECTED, OnOpenSelected)
ON_COMMAND(ID_EDIT_SCALE, OnEditScale)
ON_COMMAND(ID_EDIT_MIRROR, OnEditMirror)
ON_COMMAND(ID_EDIT_CUT, OnEditCut)
ON_COMMAND(ID_EDIT_COPY_AS_METAFILE, OnEditCopyAsMetafile)
ON_COMMAND(ID_VIEW_SHOWNAMES, OnViewShownames)
ON_COMMAND(ID_FD_SELECTCIRC, OnFDSelectCirc)
ON_COMMAND(ID_VIEW_SHOWORPHANS, OnViewShowOrphans)
ON_COMMAND(ID_CREATERADIUS, OnCreateRadius)
ON_UPDATE_COMMAND_UI(ID_EDIT_EXTERIOR, OnUpdateEditExterior)
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
ON_COMMAND(ID_EDIT_CREATEOPENBOUNDARY, &CFemmeView::OnMakeABC)
END_MESSAGE_MAP()

CFemmeView::~CFemmeView()
{
  ((CFemmApp*)AfxGetApp())->NumViews--;
  if (!((CFemmApp*)AfxGetApp())->NumViews)
    ((CMainFrame*)AfxGetApp()->GetMainWnd())->SetBar(0);
}

/////////////////////////////////////////////////////////////////////////////
// CFemmeView construction/destruction

CFemmeView::CFemmeView()
{
  ((CFemmApp*)AfxGetApp())->NumViews++;

  // Default Colors
  SelColor = dSelColor;
  MeshColor = dMeshColor;
  BlockColor = dBlockColor;
  LineColor = dLineColor;
  GridColor = dGridColor;
  NodeColor = dNodeColor;
  BackColor = dBackColor;
  NameColor = dNameColor;

  // assume some default behaviors if they can't be
  // loaded from disk
  d_action = 0;
  d_mag = 100.;
  d_gridsize = 0.25;
  d_showgrid = TRUE;
  d_snapgrid = FALSE;
  d_showorigin = FALSE;
  d_shownames = TRUE;

  // Load an updated color map, if it exists

  BinDir = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  ScanPreferences();

  // now, set default look for the preprocessor;
  EditAction = d_action;
  ox = 0.;
  oy = 0.;
  mag = d_mag;
  GridSize = d_gridsize;
  GridFlag = d_showgrid;
  SnapFlag = d_snapgrid;
  ShowNames = d_shownames;
  MeshFlag = FALSE;
  FirstPoint = -1;
  ZoomWndFlag = FALSE;
  SelectWndFlag = FALSE;
  CreateRadiusFlag = FALSE;
  SelectCircFlag = FALSE;
  MaxSeg = 1.0;
  ArcAngle = 90.0;
}

void CFemmeView::OnNewDocument()
{
  EditAction = d_action;
  ox = 0.;
  oy = 0.;
  mag = d_mag;
  GridSize = d_gridsize;
  GridFlag = d_showgrid;
  SnapFlag = d_snapgrid;
  ShowNames = d_shownames;
  MeshFlag = FALSE;
  FirstPoint = -1;
  ZoomWndFlag = FALSE;
  SelectWndFlag = FALSE;
  CreateRadiusFlag = FALSE;
  SelectCircFlag = FALSE;

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  if (MFrm == NULL)
    return;
  StatBar = (CStatusBar*)MFrm->GetMessageBar();
  if (StatBar == NULL)
    return;

  // update check boxes in the main menu...
  EditAction = d_action;
  GridFlag = d_showgrid;
  CMenu* MMnu = MFrm->GetMenu();
  MMnu->CheckMenuItem(ID_NODE_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_GROUP_OP, MF_UNCHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, FALSE);
  tc->PressButton(ID_SEGMENT_OP, FALSE);
  tc->PressButton(ID_BLOCK_OP, FALSE);
  tc->PressButton(ID_ARCSEG_OP, FALSE);
  tc->PressButton(ID_GROUP_OP, FALSE);

  if (d_action == 0) {
    tc->PressButton(ID_NODE_OP, TRUE);
    MMnu->CheckMenuItem(ID_NODE_OP, MF_CHECKED);
  }
  if (d_action == 1) {
    tc->PressButton(ID_SEGMENT_OP, TRUE);
    MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_CHECKED);
  }
  if (d_action == 2) {
    tc->PressButton(ID_BLOCK_OP, TRUE);
    MMnu->CheckMenuItem(ID_BLOCK_OP, MF_CHECKED);
  }
  if (d_action == 3) {
    tc->PressButton(ID_ARCSEG_OP, TRUE);
    MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_CHECKED);
  }
  if (d_action == 4) {
    tc->PressButton(ID_GROUP_OP, TRUE);
    MMnu->CheckMenuItem(ID_GROUP_OP, MF_CHECKED);
  }

  pToolBar = &MFrm->m_leftbar;
  tc = &pToolBar->GetToolBarCtrl();
  if (d_showgrid == TRUE) {
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_CHECKED);
    tc->PressButton(ID_SHOW_GRID, TRUE);
  } else {
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SHOW_GRID, FALSE);
  }

  if (d_snapgrid == TRUE) {
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_CHECKED);
    tc->PressButton(ID_SNAP_GRID, TRUE);
  } else {
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SNAP_GRID, FALSE);
  }

  if (d_shownames == TRUE) {
    MMnu->CheckMenuItem(ID_VIEW_SHOWNAMES, MF_CHECKED);
  } else {
    MMnu->CheckMenuItem(ID_VIEW_SHOWNAMES, MF_UNCHECKED);
  }
}

BOOL CFemmeView::PreCreateWindow(CREATESTRUCT& cs)
{
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFemmeView drawing

void CFemmeView::ScreenToDwg(int xs, int ys, double* xd, double* yd, RECT* r)
{
  double x, y;
  x = (double)xs;
  y = (double)ys;

  *xd = x / mag + ox;
  *yd = (((double)r->bottom) - y - 1.) / mag + oy;
}

void CFemmeView::DrawPSLG()
{
  RECT r;
  GetClientRect(&r);
  int i, j, k; // usual iterators...
  int xs, ys, nx, ny;
  double xd, yd, side, R, dt;
  CComplex c, p, s;
  CString lbl;

  CPen* pOldPen;
  CPen penBlue, penRed, penBlack, penGreen, penMesh;
  penBlue.CreatePen(PS_SOLID, 1, LineColor);
  penRed.CreatePen(PS_SOLID, 1, SelColor);
  penBlack.CreatePen(PS_SOLID, 1, NodeColor);
  penGreen.CreatePen(PS_SOLID, 1, BlockColor);
  penMesh.CreatePen(PS_SOLID, 1, MeshColor);

  CFemmeDoc* pDoc = GetDocument();
  CDC* pDC = GetDC();

  CFont fntArial, *pOldFont;
  fntArial.CreateFont(16, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  pOldFont = pDC->SelectObject(&fntArial);

  if (pDoc->FirstDraw == TRUE)
    pDoc->FirstDraw = FALSE;

  // make sure all the right boxes are checked.
  {
    CMainFrame* pFrame = (CMainFrame*)GetTopLevelFrame();
    CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
    CView* pView = pChild->GetActiveView();
    if (pView == this)
      CheckIt();
  }

  // Convert node coordinates to screen coordinates
  for (i = 0; i < pDoc->nodelist.GetSize(); i++)
    DwgToScreen(pDoc->nodelist[i].x, pDoc->nodelist[i].y,
        &(pDoc->nodelist[i].xs), &(pDoc->nodelist[i].ys), &r);
  if (d_showorigin == TRUE)

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

  // draw origin marker if it is enabled
  if (d_showorigin == TRUE) {
    // is the origin in the view area?
    if (DwgToScreen(0., 0., &xs, &ys, &r) == TRUE) {
      if ((xs > -10) && (ys > -10) && (xs < r.right + 10) && (ys < r.bottom + 10)) {
        // ok, the origin is in the view area;
        // draw a cross at the origin.
        pOldPen = pDC->SelectObject(&penBlack);
        MyMoveTo(pDC, xs - 9, ys);
        MyLineTo(pDC, xs + 9, ys);
        MyMoveTo(pDC, xs, ys - 9);
        MyLineTo(pDC, xs, ys + 9);
        pDC->SelectObject(pOldPen);
      }
    }
  }
  // Draw lines linking nodes
  for (i = 0; i < pDoc->linelist.GetSize(); i++) {
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
  for (i = 0; i < pDoc->arclist.GetSize(); i++) {
    if (pDoc->arclist[i].IsSelected == FALSE)
      pOldPen = pDC->SelectObject(&penBlue);
    else
      pOldPen = pDC->SelectObject(&penRed);
    k = (int)ceil(pDoc->arclist[i].ArcLength / pDoc->arclist[i].mySideLength);
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
  for (i = 0; i < pDoc->nodelist.GetSize(); i++) {
    xs = pDoc->nodelist[i].xs;
    ys = pDoc->nodelist[i].ys;

    if (pDoc->nodelist[i].IsSelected == FALSE)
      pOldPen = pDC->SelectObject(&penBlack);
    else
      pOldPen = pDC->SelectObject(&penRed);

    MyMoveTo(pDC, xs - 2, ys - 2);
    MyLineTo(pDC, xs - 2, ys + 2);
    MyLineTo(pDC, xs + 2, ys + 2);
    MyLineTo(pDC, xs + 2, ys - 2);
    MyLineTo(pDC, xs - 2, ys - 2);

    pDC->SelectObject(pOldPen);
  }

  // Draw node block labels
  for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
    DwgToScreen(pDoc->blocklist[i].x, pDoc->blocklist[i].y, &xs, &ys, &r);

    if (pDoc->blocklist[i].IsSelected == FALSE)
      pOldPen = pDC->SelectObject(&penGreen);
    else
      pOldPen = pDC->SelectObject(&penRed);

    MyMoveTo(pDC, xs - 2, ys - 2);
    MyLineTo(pDC, xs - 2, ys + 2);
    MyLineTo(pDC, xs + 2, ys + 2);
    MyLineTo(pDC, xs + 2, ys - 2);
    MyLineTo(pDC, xs - 2, ys - 2);

    // circle approximately showing area constraint...
    if ((pDoc->blocklist[i].MaxArea > 0) && (pDoc->blocklist[i].BlockType != "<No Mesh>")) {
      side = sqrt(pDoc->blocklist[i].MaxArea / PI);
      j = (int)ceil(side * mag);
      /*
        // zoom-safe replacement
        CComplex p;
        int kmax;
        MyMoveTo(pDC,xs+j,ys);
        kmax=(int) (2.*PI*((double) j)/5.);
        if (kmax>36) kmax=36;
        if ((kmax<8) && (kmax>0)) kmax=8;
        for(k=1;k<=kmax;k++)
        {
          p=exp(I*k*2.*PI/((double) kmax))*((double) j);
          MyLineTo(pDC,xs+((int) Re(p)),ys+((int) Im(p)));
        }
      */
      pDC->Arc(xs - j, ys - j, xs + j + 1, ys + j + 1, xs + j + 1, ys, xs + j + 1, ys);
    }

    // draw a line indicating the magnetization direction;
    for (j = 0, k = -1; j < pDoc->blockproplist.GetSize(); j++)
      if (pDoc->blocklist[i].BlockType == pDoc->blockproplist[j].BlockName)
        k = j;
    if (k >= 0)
      if ((pDoc->blockproplist[k].H_c != 0) && (pDoc->blocklist[i].MagDirFctn.GetLength() == 0)) {
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

    if (ShowNames) {
      pDC->SetTextAlign(TA_BASELINE);
      pDC->SetBkMode(TRANSPARENT);
      for (k = 0, lbl = "<None>"; k < pDoc->blockproplist.GetSize(); k++)
        if (pDoc->blockproplist[k].BlockName == pDoc->blocklist[i].BlockType)
          lbl = pDoc->blocklist[i].BlockType;
      if (pDoc->blocklist[i].BlockType == "<No Mesh>")
        lbl = "<No Mesh>";
      pDC->TextOut(xs + 5, ys, lbl);

      pDC->SetTextAlign(TA_TOP);
      for (k = 0, lbl = ""; k < pDoc->circproplist.GetSize(); k++)
        if (pDoc->circproplist[k].CircName == pDoc->blocklist[i].InCircuit) {
          if (pDoc->circproplist[k].CircType == 1)
            lbl.Format("[%s:%i]", pDoc->circproplist[k].CircName,
                pDoc->blocklist[i].Turns);
          else
            lbl = "[" + pDoc->blocklist[i].InCircuit + "]";
        }
      pDC->TextOut(xs + 5, ys, lbl);
    }

    pDC->SelectObject(pOldPen);
  }
  pDC->SelectObject(pOldFont);
  fntArial.DeleteObject();

  ReleaseDC(pDC);
}

BOOL CFemmeView::DwgToScreen(double xd, double yd, int* xs, int* ys, RECT* r)
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

void CFemmeView::OnDraw(CDC* pDC)
{
  CFemmeDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  if (pDoc->NoDraw == TRUE)
    return;

  // TODO: add draw code for native data here
  RECT r;
  GetClientRect(&r);
  int i, j, k; // usual iterators...
  int xs, ys, nx, ny;
  double xd, yd, side, R, dt;
  CComplex c, p, s;
  CString lbl;

  CFont fntArial, *pOldFont;
  fntArial.CreateFont(16, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  pOldFont = pDC->SelectObject(&fntArial);

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
  } else {
    CMainFrame* pFrame = (CMainFrame*)GetTopLevelFrame();
    CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
    CView* pView = pChild->GetActiveView();
    if (pView == this)
      CheckIt();
  }
  if (pDoc->FirstDraw == TRUE)
    OnZoomNatural();

  CPen* pOldPen;
  CPen penBlue, penRed, penNode, penGreen, penMesh, penGrey;
  penBlue.CreatePen(PS_SOLID, 1, LineColor);
  penRed.CreatePen(PS_SOLID, 1, SelColor);
  penNode.CreatePen(PS_SOLID, 1, NodeColor);
  penGreen.CreatePen(PS_SOLID, 1, BlockColor);
  penMesh.CreatePen(PS_SOLID, 1, MeshColor);
  penGrey.CreatePen(PS_SOLID, 1, RGB(220, 220, 220));

  // Convert node coordinates to screen coordinates
  for (i = 0; i < pDoc->nodelist.GetSize(); i++)
    if (DwgToScreen(pDoc->nodelist[i].x, pDoc->nodelist[i].y,
            &(pDoc->nodelist[i].xs), &(pDoc->nodelist[i].ys), &r)
        == FALSE) {
      mag /= 1.1;
      i = -1;
    }

  // Convert meshnode coordinates to screen coordinates
  if (MeshFlag == TRUE)
    for (i = 0; i < pDoc->meshnode.GetSize(); i++)
      DwgToScreen(pDoc->meshnode[i].x, pDoc->meshnode[i].y,
          &(pDoc->meshnode[i].xs), &(pDoc->meshnode[i].ys), &r);

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

  // Draw origin marker if it is enabled
  if (d_showorigin == TRUE) {
    // is the origin in the view area?
    if (DwgToScreen(0., 0., &xs, &ys, &r) == TRUE) {
      if ((xs > -10) && (ys > -10) && (xs < r.right + 10) && (ys < r.bottom + 10)) {
        // ok, the origin is in the view area;
        // draw a cross at the origin.
        pOldPen = pDC->SelectObject(&penNode);
        MyMoveTo(pDC, xs - 9, ys);
        MyLineTo(pDC, xs + 9, ys);
        MyMoveTo(pDC, xs, ys - 9);
        MyLineTo(pDC, xs, ys + 9);
        pDC->SelectObject(pOldPen);
      }
    }
  }

  // Draw mesh if it is enabled...
  if (MeshFlag == TRUE) {
    pOldPen = pDC->SelectObject(&penMesh);
    for (i = 0; i < pDoc->meshline.GetSize(); i++) {
      MyMoveTo(pDC, pDoc->meshnode[pDoc->meshline[i].x].xs,
          pDoc->meshnode[pDoc->meshline[i].x].ys);
      MyLineTo(pDC, pDoc->meshnode[pDoc->meshline[i].y].xs,
          pDoc->meshnode[pDoc->meshline[i].y].ys);
    }
    pDC->SelectObject(pOldPen);

    pOldPen = pDC->SelectObject(&penGrey);
    for (i = 0; i < pDoc->greymeshline.GetSize(); i++) {
      MyMoveTo(pDC, pDoc->meshnode[pDoc->greymeshline[i].x].xs,
          pDoc->meshnode[pDoc->greymeshline[i].x].ys);
      MyLineTo(pDC, pDoc->meshnode[pDoc->greymeshline[i].y].xs,
          pDoc->meshnode[pDoc->greymeshline[i].y].ys);
    }
    pDC->SelectObject(pOldPen);
  }

  // Draw lines linking nodes
  for (i = 0; i < pDoc->linelist.GetSize(); i++) {
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
  for (i = 0; i < pDoc->arclist.GetSize(); i++) {
    if (pDoc->arclist[i].IsSelected == FALSE)
      pOldPen = pDC->SelectObject(&penBlue);
    else
      pOldPen = pDC->SelectObject(&penRed);
    k = (int)ceil(pDoc->arclist[i].ArcLength / pDoc->arclist[i].mySideLength);
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

  // Draw block labels
  for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
    DwgToScreen(pDoc->blocklist[i].x, pDoc->blocklist[i].y, &xs, &ys, &r);

    if (pDoc->blocklist[i].IsSelected == FALSE)
      pOldPen = pDC->SelectObject(&penGreen);
    else
      pOldPen = pDC->SelectObject(&penRed);

    MyMoveTo(pDC, xs - 2, ys - 2);
    MyLineTo(pDC, xs - 2, ys + 2);
    MyLineTo(pDC, xs + 2, ys + 2);
    MyLineTo(pDC, xs + 2, ys - 2);
    MyLineTo(pDC, xs - 2, ys - 2);

    // circle approximately showing area constraint...
    if ((pDoc->blocklist[i].MaxArea > 0) && (pDoc->blocklist[i].BlockType != "<No Mesh>")) {
      side = sqrt(pDoc->blocklist[i].MaxArea / PI);
      j = (int)ceil(side * mag);

      /*
            // zoom-safe replacement
            CComplex p;
            int kmax;
            MyMoveTo(pDC,xs+j,ys);
            kmax=(int) (2.*PI*((double) j)/5.);
            if (kmax>36) kmax=36;
            if ((kmax<8) && (kmax>0)) kmax=8;
            for(k=1;k<=kmax;k++)
            {
              p=exp(I*k*2.*PI/((double) kmax))*((double) j);
              MyLineTo(pDC,xs+((int) Re(p)),ys+((int) Im(p)));
            }
      */
      pDC->Arc(xs - j, ys - j, xs + j + 1, ys + j + 1, xs + j + 1, ys, xs + j + 1, ys);
    }

    // draw a line indicating the magnetization direction;
    for (j = 0, k = -1; j < pDoc->blockproplist.GetSize(); j++)
      if (pDoc->blocklist[i].BlockType == pDoc->blockproplist[j].BlockName)
        k = j;
    if (k >= 0)
      if ((pDoc->blockproplist[k].H_c != 0) && (pDoc->blocklist[i].MagDirFctn.GetLength() == 0)) {
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

    if (ShowNames) {
      pDC->SetTextColor(NameColor);
      pDC->SetTextAlign(TA_BASELINE);
      pDC->SetBkMode(TRANSPARENT);
      for (k = 0, lbl = "<None>"; k < pDoc->blockproplist.GetSize(); k++)
        if (pDoc->blockproplist[k].BlockName == pDoc->blocklist[i].BlockType)
          lbl = pDoc->blocklist[i].BlockType;
      if (pDoc->blocklist[i].BlockType == "<No Mesh>")
        lbl = "<No Mesh>";
      pDC->TextOut(xs + 5, ys, lbl);

      pDC->SetTextAlign(TA_TOP);
      for (k = 0, lbl = ""; k < pDoc->circproplist.GetSize(); k++)
        if (pDoc->circproplist[k].CircName == pDoc->blocklist[i].InCircuit) {
          if (pDoc->circproplist[k].CircType == 1)
            lbl.Format("[%s:%i]", pDoc->circproplist[k].CircName,
                pDoc->blocklist[i].Turns);
          else
            lbl = "[" + pDoc->blocklist[i].InCircuit + "]";
        }
      pDC->TextOut(xs + 5, ys, lbl);
      pDC->SetTextColor(RGB(0, 0, 0));
    }

    pDC->SelectObject(pOldPen);
  }
  pDC->SelectObject(pOldFont);
  fntArial.DeleteObject();
}

void CFemmeView::OnInitialUpdate()
{
  CView::OnInitialUpdate();

  EditAction = d_action;
  GridFlag = d_showgrid;
  SnapFlag = d_snapgrid;
  ShowNames = d_shownames;

  CheckIt();
}

void CFemmeView::CheckIt()
{
  // once again, this is sort of a punt
  // to make sure that the right buttons
  // are checked.  It can be relatively
  // easy to change the internal flags
  // associated with the buttons without
  // taking the time to service the button
  // state otherwise.

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  if (MFrm == NULL)
    MsgBox("Null GetTopLevelFrame");
  else
    StatBar = (CStatusBar*)MFrm->GetMessageBar();

  // update check boxes in the main menu...

  CMenu* MMnu = MFrm->GetMenu();
  MMnu->CheckMenuItem(ID_NODE_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_GROUP_OP, MF_UNCHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, FALSE);
  tc->PressButton(ID_SEGMENT_OP, FALSE);
  tc->PressButton(ID_BLOCK_OP, FALSE);
  tc->PressButton(ID_ARCSEG_OP, FALSE);
  tc->PressButton(ID_GROUP_OP, FALSE);

  if (EditAction == 0) {
    tc->PressButton(ID_NODE_OP, TRUE);
    MMnu->CheckMenuItem(ID_NODE_OP, MF_CHECKED);
  }
  if (EditAction == 1) {
    tc->PressButton(ID_SEGMENT_OP, TRUE);
    MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_CHECKED);
  }
  if (EditAction == 2) {
    tc->PressButton(ID_BLOCK_OP, TRUE);
    MMnu->CheckMenuItem(ID_BLOCK_OP, MF_CHECKED);
  }
  if (EditAction == 3) {
    tc->PressButton(ID_ARCSEG_OP, TRUE);
    MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_CHECKED);
  }
  if (EditAction == 4) {
    tc->PressButton(ID_GROUP_OP, TRUE);
    MMnu->CheckMenuItem(ID_GROUP_OP, MF_CHECKED);
  }

  pToolBar = &MFrm->m_leftbar;
  tc = &pToolBar->GetToolBarCtrl();
  if (GridFlag == TRUE) {
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_CHECKED);
    tc->PressButton(ID_SHOW_GRID, TRUE);
  } else {
    MMnu->CheckMenuItem(ID_SHOW_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SHOW_GRID, FALSE);
  }

  if (SnapFlag == TRUE) {
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_CHECKED);
    tc->PressButton(ID_SNAP_GRID, TRUE);
  } else {
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SNAP_GRID, FALSE);
  }

  if (ShowNames == TRUE) {
    MMnu->CheckMenuItem(ID_VIEW_SHOWNAMES, MF_CHECKED);
  } else {
    MMnu->CheckMenuItem(ID_VIEW_SHOWNAMES, MF_UNCHECKED);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CFemmeView printing

BOOL CFemmeView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CFemmeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CFemmeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFemmeView diagnostics

#ifdef _DEBUG
void CFemmeView::AssertValid() const
{
  CView::AssertValid();
}

void CFemmeView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CFemmeDoc* CFemmeView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFemmeDoc)));
  return (CFemmeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFemmeView message handlers

void CFemmeView::OnNodeOp()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc;
  pDoc = GetDocument();
  pDoc->UnselectAll();
  EditAction = 0;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  MMnu->CheckMenuItem(ID_NODE_OP, MF_CHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_GROUP_OP, MF_UNCHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, TRUE);
  tc->PressButton(ID_SEGMENT_OP, FALSE);
  tc->PressButton(ID_BLOCK_OP, FALSE);
  tc->PressButton(ID_ARCSEG_OP, FALSE);
  tc->PressButton(ID_GROUP_OP, FALSE);

  //	InvalidateRect(NULL);
  DrawPSLG();
}

void CFemmeView::OnSegmentOp()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc;
  pDoc = GetDocument();
  pDoc->UnselectAll();
  EditAction = 1;
  FirstPoint = -1;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  MMnu->CheckMenuItem(ID_NODE_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_CHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_GROUP_OP, MF_UNCHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, FALSE);
  tc->PressButton(ID_SEGMENT_OP, TRUE);
  tc->PressButton(ID_BLOCK_OP, FALSE);
  tc->PressButton(ID_ARCSEG_OP, FALSE);
  tc->PressButton(ID_GROUP_OP, FALSE);

  //	InvalidateRect(NULL);
  DrawPSLG();
}

void CFemmeView::OnArcsegOp()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc;
  pDoc = GetDocument();
  pDoc->UnselectAll();
  EditAction = 3;
  FirstPoint = -1;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  MMnu->CheckMenuItem(ID_NODE_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_CHECKED);
  MMnu->CheckMenuItem(ID_GROUP_OP, MF_UNCHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, FALSE);
  tc->PressButton(ID_SEGMENT_OP, FALSE);
  tc->PressButton(ID_BLOCK_OP, FALSE);
  tc->PressButton(ID_ARCSEG_OP, TRUE);
  tc->PressButton(ID_GROUP_OP, FALSE);

  //	InvalidateRect(NULL);
  DrawPSLG();
}
void CFemmeView::OnBlockOp()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc;
  pDoc = GetDocument();
  pDoc->UnselectAll();
  EditAction = 2;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  MMnu->CheckMenuItem(ID_NODE_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_CHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, FALSE);
  tc->PressButton(ID_SEGMENT_OP, FALSE);
  tc->PressButton(ID_BLOCK_OP, TRUE);
  tc->PressButton(ID_ARCSEG_OP, FALSE);
  tc->PressButton(ID_GROUP_OP, FALSE);

  //	InvalidateRect(NULL);
  DrawPSLG();
}

void CFemmeView::OnGroupOp()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc;
  pDoc = GetDocument();
  pDoc->UnselectAll();
  EditAction = 4;

  // update check boxes in the main menu...
  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  MMnu->CheckMenuItem(ID_NODE_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_SEGMENT_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_BLOCK_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_ARCSEG_OP, MF_UNCHECKED);
  MMnu->CheckMenuItem(ID_GROUP_OP, MF_CHECKED);

  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FE_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_NODE_OP, FALSE);
  tc->PressButton(ID_SEGMENT_OP, FALSE);
  tc->PressButton(ID_BLOCK_OP, FALSE);
  tc->PressButton(ID_ARCSEG_OP, FALSE);
  tc->PressButton(ID_GROUP_OP, TRUE);

  //	InvalidateRect(NULL);
  DrawPSLG();
}

void CFemmeView::OnMouseMove(UINT nFlags, CPoint point)
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
  CFemmeDoc* pDoc = GetDocument();

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
  if ((SnapFlag == TRUE) && ((EditAction == 0) || (EditAction == 2))) {
    x = GridSize * floor(0.5 + x / GridSize);
    y = GridSize * floor(0.5 + y / GridSize);
  }

  if ((ZoomWndFlag == 2) || (SelectWndFlag == 2)) {

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

  if (SelectCircFlag == 2) {
    CDC* pDC = GetDC();
    COLORREF ocol;
    CZPix p;
    double R;
    CComplex q;

    int xso, yso, xsi, ysi, xsn, ysn, i, k;
    DwgToScreen(wzx, wzy, &xso, &yso, &r);
    DwgToScreen(mx, my, &xsi, &ysi, &r);
    DwgToScreen(x, y, &xsn, &ysn, &r);

    // Draw old circle off the screen;
    if (ZoomPix.GetSize() > 0) {
      for (k = (int)ZoomPix.GetSize() - 1; k >= 0; k--)
        pDC->SetPixel(ZoomPix[k].x, ZoomPix[k].y, ZoomPix[k].c);
      ZoomPix.RemoveAll();
    }

    // XOR new rectangle onto screen
    R = abs((xso - xsn) + I * (yso - ysn));
    for (i = 1; i <= (int)(2. * PI * R); i++) {
      q = R * exp(I * ((double)i) / R) + (xso + I * yso);
      ocol = pDC->GetPixel((int)Re(q), (int)Im(q));
      p.c = ocol;
      p.x = (int)Re(q);
      p.y = (int)Im(q);
      ZoomPix.Add(p);
      ocol = ocol ^ RGB(255, 255, 255);
      pDC->SetPixel((int)Re(q), (int)Im(q), ocol);
    }

    ReleaseDC(pDC);
  }

  // update mouse location;
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

void CFemmeView::OnZoomIn()
{
  // TODO: Add your command handler code here
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox = ox + 0.25 * x / mag;
  oy = oy + 0.25 * y / mag;
  mag *= 2.;

  InvalidateRect(NULL);
}

void CFemmeView::OnZoomOut()
{
  // TODO: Add your command handler code here
  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox = ox - 0.5 * x / mag;
  oy = oy - 0.5 * y / mag;
  mag /= 2.;

  InvalidateRect(NULL);
}

BOOL CFemmeView::GetBoundingBox(double* x, double* y)
{
  // look at points, block labels, and arcs to get bounding box.
  CFemmeDoc* pDoc = GetDocument();
  int i, j, k;
  double R, dt;
  CComplex p, s, c;

  if (pDoc->nodelist.GetSize() < 2)
    return FALSE;

  x[0] = pDoc->nodelist[0].x;
  x[1] = pDoc->nodelist[0].x;
  y[0] = pDoc->nodelist[0].y;
  y[1] = pDoc->nodelist[0].y;
  for (i = 1; i < pDoc->nodelist.GetSize(); i++) {
    if (pDoc->nodelist[i].x < x[0])
      x[0] = pDoc->nodelist[i].x;
    if (pDoc->nodelist[i].x > x[1])
      x[1] = pDoc->nodelist[i].x;
    if (pDoc->nodelist[i].y < y[0])
      y[0] = pDoc->nodelist[i].y;
    if (pDoc->nodelist[i].y > y[1])
      y[1] = pDoc->nodelist[i].y;
  }

  for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
    if (pDoc->blocklist[i].x < x[0])
      x[0] = pDoc->blocklist[i].x;
    if (pDoc->blocklist[i].x > x[1])
      x[1] = pDoc->blocklist[i].x;
    if (pDoc->blocklist[i].y < y[0])
      y[0] = pDoc->blocklist[i].y;
    if (pDoc->blocklist[i].y > y[1])
      y[1] = pDoc->blocklist[i].y;
  }

  for (i = 0; i < pDoc->arclist.GetSize(); i++) {
    k = (int)ceil(pDoc->arclist[i].ArcLength / pDoc->arclist[i].mySideLength);
    dt = pDoc->arclist[i].ArcLength * PI / (((double)k) * 180.);
    pDoc->GetCircle(pDoc->arclist[i], c, R);
    p.Set(pDoc->nodelist[pDoc->arclist[i].n0].x,
        pDoc->nodelist[pDoc->arclist[i].n0].y);
    s = exp(I * dt);
    for (j = 0; j < k; j++) {
      p = (p - c) * s + c;
      if (p.re < x[0])
        x[0] = p.re;
      if (p.re > x[1])
        x[1] = p.re;
      if (p.im < y[0])
        y[0] = p.im;
      if (p.im > y[1])
        y[1] = p.im;
    }
  }

  return TRUE;
}

void CFemmeView::OnZoomNatural()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc = GetDocument();
  double x[2], y[2], m[2], w;
  RECT r;

  if (GetBoundingBox(x, y) == FALSE)
    return;

  if (pDoc->FirstDraw == TRUE) {
    if ((x[1] - x[0]) > (y[1] - y[0]))
      w = (x[1] - x[0]);
    else
      w = (y[1] - y[0]);
    if (w != 0)
      GridSize = pow(10., floor(log(w) / log(10.) - 0.5));
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
    InvalidateRect(NULL);
}

void CFemmeView::OnZoomWnd()
{
  // TODO: Add your command handler code here
  ZoomWndFlag = TRUE;
  SelectWndFlag = FALSE;
  SelectCircFlag = FALSE;
  CreateRadiusFlag = FALSE;
}

void CFemmeView::OnSelectwnd()
{
  SelectWndFlag = TRUE;
  ZoomWndFlag = FALSE;
  SelectCircFlag = FALSE;
  CreateRadiusFlag = FALSE;

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_SELECTWND, TRUE);
}

void CFemmeView::OnFDSelectCirc()
{
  SelectCircFlag = TRUE;
  SelectWndFlag = FALSE;
  CreateRadiusFlag = FALSE;
  ZoomWndFlag = FALSE;

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
  tc->PressButton(ID_FD_SELECTCIRC, TRUE);
}

void CFemmeView::OnShowGrid()
{
  // TODO: Add your command handler code here
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

  InvalidateRect(NULL);
}

void CFemmeView::OnSetGrid()
{
  // TODO: Add your command handler code here
  GRIDDLG pDlg;
  CFemmeDoc* pDoc = GetDocument();
  // Send present parameter values to the dialog
  // pDlg.m_rji=TheDoc->vi[0];
  pDlg.m_gridsize = GridSize;
  pDlg.coords = pDoc->Coords;

  // Display dialog and collect data
  if (pDlg.DoModal() == IDOK) {
    GridSize = pDlg.m_gridsize;
    pDoc->Coords = pDlg.coords;
    InvalidateRect(NULL);
  }
}

void CFemmeView::OnSnapGrid()
{
  // TODO: Add your command handler code here
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

void CFemmeView::OnShowMesh()
{
  // TODO: Add your command handler code here
  CFrameWnd* MFrm;
  MFrm = GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  if (MeshFlag == TRUE) {
    MeshFlag = FALSE;
    MMnu->CheckMenuItem(ID_SHOW_MESH, MF_UNCHECKED);
  } else if (MeshUpToDate == TRUE) {
    MeshFlag = TRUE;
    MMnu->CheckMenuItem(ID_SHOW_MESH, MF_CHECKED);
  }
  InvalidateRect(NULL);
}

void CFemmeView::OnEditCopyAsMetafile()
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

void CFemmeView::OnEditCopy()
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

void CFemmeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: Add your message handler code here and/or call default
  // VK_TAB, VK_DELETE -- ones that we want to catch
  CFemmeDoc* pDoc = GetDocument();

  if (((nChar == 'z') || (nChar == 'Z')) && (GetKeyState(VK_CONTROL)))
    OnUndo();

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

  if (nChar == VK_TAB) {
    if ((EditAction == 0) || (EditAction == 2))
      EnterPoint();
    if (EditAction == 4) {
      CGroupNumber dlg;
      dlg.m_groupnumber = 0;
      if (dlg.DoModal() == IDOK) {
        int i, j;
        j = dlg.m_groupnumber;
        // now, the group associated with the nearest entity is in j;
        // toggle the select of all objects in this group;
        for (i = 0; i < pDoc->nodelist.GetSize(); i++)
          if (pDoc->nodelist[i].InGroup == j)
            pDoc->nodelist[i].ToggleSelect();
        for (i = 0; i < pDoc->linelist.GetSize(); i++)
          if (pDoc->linelist[i].InGroup == j)
            pDoc->linelist[i].ToggleSelect();
        for (i = 0; i < pDoc->arclist.GetSize(); i++)
          if (pDoc->arclist[i].InGroup == j)
            pDoc->arclist[i].ToggleSelect();
        for (i = 0; i < pDoc->blocklist.GetSize(); i++)
          if (pDoc->blocklist[i].InGroup == j)
            pDoc->blocklist[i].ToggleSelect();
        DrawPSLG();
      }
    }
  }

  if (nChar == VK_DELETE)
    OnEditCut();

  if (nChar == VK_SPACE)
    OnOpenSelected();

  if (nChar == VK_ESCAPE) {
    pDoc->UnselectAll();
    FirstPoint = -1;
    ZoomWndFlag = 0;
    SelectWndFlag = 0;
    CreateRadiusFlag = 0;

    CMainFrame* MFrm;
    MFrm = (CMainFrame*)GetTopLevelFrame();
    CMenu* MMnu = MFrm->GetMenu();
    CToolBar* pToolBar = &MFrm->m_leftbar;
    CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
    tc->PressButton(ID_SELECTWND, FALSE);

    DrawPSLG();
  }

  if (nChar == VK_F3) {
    int i;
    double x[2], y[2];
    double DefaultMeshSize;

    if (GetBoundingBox(x, y) != FALSE)
      DefaultMeshSize = pow(abs(x[1] - x[0] + I * (y[1] - y[0])) / BoundingBoxFraction, 2.);
    else
      DefaultMeshSize = 0;
    for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
      if (pDoc->blocklist[i].MaxArea > 0)
        pDoc->blocklist[i].MaxArea /= 4.;
      else if (pDoc->SmartMesh)
        pDoc->blocklist[i].MaxArea = DefaultMeshSize / 4.;
    }
    InvalidateRect(NULL);
  }

  if (nChar == VK_F4) {
    int i;
    double x[2], y[2];
    double DefaultMeshSize;

    if ((GetBoundingBox(x, y) != FALSE) && (pDoc->SmartMesh))
      DefaultMeshSize = pow(abs(x[1] - x[0] + I * (y[1] - y[0])) / BoundingBoxFraction, 2.);
    else
      DefaultMeshSize = 0;

    for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
      pDoc->blocklist[i].MaxArea *= 4.;
      if ((pDoc->blocklist[i].MaxArea > DefaultMeshSize) && (DefaultMeshSize > 0))
        pDoc->blocklist[i].MaxArea = DefaultMeshSize;
    }
    InvalidateRect(NULL);
  }

  // Call to base class...
  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFemmeView::EnterPoint()
{
  // TODO: Add your command handler code here
  CEnterPt pDlg;
  double x, y, t, d;
  int i;
  CFemmeDoc* pDoc = GetDocument();
  BOOL flag;

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
    pDoc->UpdateUndo();

    if (pDoc->nodelist.GetSize() < 2)
      d = 1.e-08;
    else {
      CComplex p0, p1, p2;
      p0 = pDoc->nodelist[0].CC();
      p1 = p0;
      for (i = 1; i < pDoc->nodelist.GetSize(); i++) {
        p2 = pDoc->nodelist[i].CC();
        if (p2.re < p0.re)
          p0.re = p2.re;
        if (p2.re > p1.re)
          p1.re = p2.re;
        if (p2.im < p0.im)
          p0.im = p2.im;
        if (p2.im > p1.im)
          p1.im = p2.im;
      }
      d = abs(p1 - p0) * CLOSE_ENOUGH;
    }

    if (EditAction == 0) {
      pDoc->UpdateUndo();
      flag = pDoc->AddNode(x, y, d);
    }
    if (EditAction == 2) {
      pDoc->UpdateUndo();
      flag = pDoc->AddBlockLabel(x, y, d);
    }

    if (flag == TRUE) {
      MeshUpToDate = FALSE;
      if (MeshFlag == TRUE)
        OnShowMesh();
      else
        DrawPSLG();
    }
  }
}

void CFemmeView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (bLinehook != FALSE) {
    CView::OnLButtonDown(nFlags, point);
    return;
  }

  BOOL flag;
  CFemmeDoc* pDoc = GetDocument();

  if (CreateRadiusFlag == TRUE) {
    int k;
    double r;

    CreateRadiusFlag = FALSE;
    if ((k = pDoc->ClosestNode(mx, my)) < 0)
      return; // catch case where no nodes have been drawn yet;
    if (pDoc->CanCreateRadius(k) == FALSE) {
      MsgBox("The specified point is not suitable for\nconversion into a radius");
      return;
    }

    CPromptBox dlg;
    dlg.mytitle = "Enter desired radius dimension:";
    if (dlg.DoModal() == IDOK) {
      r = dlg.ParseDouble();
      if (r > 0) {
        if (pDoc->CreateRadius(k, r)) {
          InvalidateRect(NULL);
          MeshUpToDate = FALSE;
        } else
          MsgBox("Could not make a radius of the prescribed dimension");
      }
    }

    return;
  }

  if (SelectWndFlag == 1) {
    ZoomPix.RemoveAll();
    SelectWndFlag++;
    wzx = mx;
    wzy = my;
    return;
  }
  if (SelectWndFlag == 2)
    return;

  if (SelectCircFlag == 1) {
    ZoomPix.RemoveAll();
    SelectCircFlag++;
    wzx = mx;
    wzy = my;
    return;
  }

  if (SelectCircFlag == 2)
    return;

  if (ZoomWndFlag == 1) {
    ZoomPix.RemoveAll();
    ZoomWndFlag++;
    wzx = mx;
    wzy = my;
    return;
  }

  if (ZoomWndFlag == 2)
    return;

  if (EditAction == 0) {
    pDoc->UpdateUndo();
    flag = pDoc->AddNode(mx, my, 1. / mag);
    if (flag == TRUE) {
      MeshUpToDate = FALSE;
      if (MeshFlag == TRUE)
        OnShowMesh();
      else
        DrawPSLG();
    }
  }

  if (EditAction == 1) {
    if (FirstPoint < 0) {
      if (pDoc->nodelist.GetSize() == 0)
        return;
      FirstPoint = pDoc->ClosestNode(mx, my);
      pDoc->nodelist[FirstPoint].ToggleSelect();
      DrawPSLG();
    } else {
      pDoc->nodelist[FirstPoint].ToggleSelect();
      pDoc->UpdateUndo();
      flag = pDoc->AddSegment(FirstPoint, pDoc->ClosestNode(mx, my));
      FirstPoint = -1;
      if (flag == TRUE) {
        MeshUpToDate = FALSE;
        if (MeshFlag == TRUE)
          OnShowMesh();
        else
          DrawPSLG();
      } else
        DrawPSLG();
    }
  }

  if (EditAction == 2) {
    pDoc->UpdateUndo();
    flag = pDoc->AddBlockLabel(mx, my, 1. / mag);
    if (flag == TRUE) {
      MeshUpToDate = FALSE;
      if (MeshFlag == TRUE)
        OnShowMesh();
      else
        DrawPSLG();
    }
  }

  if (EditAction == 3) {
    if (FirstPoint < 0) {
      if (pDoc->nodelist.GetSize() == 0)
        return;
      FirstPoint = pDoc->ClosestNode(mx, my);
      pDoc->nodelist[FirstPoint].ToggleSelect();
      DrawPSLG();
    } else {
      CArcSegment asegm;
      CArcDlg dlg;
      asegm.n0 = FirstPoint;
      asegm.n1 = pDoc->ClosestNode(mx, my);
      pDoc->nodelist[asegm.n1].ToggleSelect();
      DrawPSLG();

      for (int k = 0; k < pDoc->lineproplist.GetSize(); k++)
        dlg.namelist.Add(pDoc->lineproplist[k].BdryName);

      dlg.cursel = 0;
      dlg.m_MaxSeg = MaxSeg;
      dlg.m_ArcAngle = ArcAngle;
      if (dlg.DoModal() == IDCANCEL) {
        FirstPoint = -1;
        pDoc->UnselectAll();
        DrawPSLG();
        return;
      }
      MaxSeg = asegm.MaxSideLength = dlg.m_MaxSeg;
      ArcAngle = asegm.ArcLength = dlg.m_ArcAngle;
      if (dlg.cursel == 0)
        asegm.BoundaryMarker = "<None>";
      else
        asegm.BoundaryMarker = pDoc->lineproplist[dlg.cursel - 1].BdryName;

      pDoc->UpdateUndo();
      flag = pDoc->AddArcSegment(asegm);
      flag = FALSE;
      FirstPoint = -1;
      pDoc->UnselectAll();
      if (flag == TRUE) {
        MeshUpToDate = FALSE;
        if (MeshFlag == TRUE)
          OnShowMesh();
        else
          DrawPSLG();
      } else
        DrawPSLG();
    }
  }

  CView::OnLButtonDown(nFlags, point);
}

void CFemmeView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (bLinehook != FALSE) {
    CView::OnLButtonUp(nFlags, point);
    return;
  }

  if (ZoomWndFlag == 2) {
    ZoomPix.RemoveAll();
    ZoomWndFlag = 0;
    if ((mx == wzx) && (my == wzy)) {
      InvalidateRect(NULL);
      return;
    }
    RECT r;
    double z, m[2];
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

    ox = wzx;
    oy = wzy;
    if (m[0] < m[1])
      mag = m[0];
    else
      mag = m[1];
    InvalidateRect(NULL);
  }

  if (SelectWndFlag == 2) {

    CMainFrame* MFrm;
    MFrm = (CMainFrame*)GetTopLevelFrame();
    CMenu* MMnu = MFrm->GetMenu();
    CToolBar* pToolBar = &MFrm->m_leftbar;
    CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
    tc->PressButton(ID_SELECTWND, FALSE);

    int i, k;
    double x, y, z;
    CFemmeDoc* pDoc = GetDocument();
    SelectWndFlag = 0;
    if ((mx == wzx) && (my == wzy)) {
      InvalidateRect(NULL);
      return;
    }
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

    if ((EditAction == 0) || (EditAction == 4)) {
      for (i = 0; i < pDoc->nodelist.GetSize(); i++) {
        x = pDoc->nodelist[i].x;
        y = pDoc->nodelist[i].y;
        if ((x <= mx) && (x >= wzx) && (y <= my) && (y >= wzy))
          pDoc->nodelist[i].IsSelected = TRUE;
      }
    }

    if ((EditAction == 2) || (EditAction == 4)) {
      for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
        x = pDoc->blocklist[i].x;
        y = pDoc->blocklist[i].y;
        if ((x <= mx) && (x >= wzx) && (y <= my) && (y >= wzy))
          pDoc->blocklist[i].IsSelected = TRUE;
      }
    }

    if ((EditAction == 1) || (EditAction == 4)) {
      for (i = 0; i < pDoc->linelist.GetSize(); i++) {
        k = 0;
        x = pDoc->nodelist[pDoc->linelist[i].n0].x;
        y = pDoc->nodelist[pDoc->linelist[i].n0].y;
        if ((x <= mx) && (x >= wzx) && (y <= my) && (y >= wzy))
          k++;
        x = pDoc->nodelist[pDoc->linelist[i].n1].x;
        y = pDoc->nodelist[pDoc->linelist[i].n1].y;
        if ((x <= mx) && (x >= wzx) && (y <= my) && (y >= wzy))
          k++;

        if (k == 2)
          pDoc->linelist[i].IsSelected = TRUE;
      }
    }

    if ((EditAction == 3) || (EditAction == 4)) {
      for (i = 0; i < pDoc->arclist.GetSize(); i++) {
        k = 0;
        x = pDoc->nodelist[pDoc->arclist[i].n0].x;
        y = pDoc->nodelist[pDoc->arclist[i].n0].y;
        if ((x <= mx) && (x >= wzx) && (y <= my) && (y >= wzy))
          k++;
        x = pDoc->nodelist[pDoc->arclist[i].n1].x;
        y = pDoc->nodelist[pDoc->arclist[i].n1].y;
        if ((x <= mx) && (x >= wzx) && (y <= my) && (y >= wzy))
          k++;

        if (k == 2)
          pDoc->arclist[i].IsSelected = TRUE;
      }
    }
    InvalidateRect(NULL);
  }

  if (SelectCircFlag == 2) {

    CMainFrame* MFrm;
    MFrm = (CMainFrame*)GetTopLevelFrame();
    CMenu* MMnu = MFrm->GetMenu();
    CToolBar* pToolBar = &MFrm->m_leftbar;
    CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();
    tc->PressButton(ID_FD_SELECTCIRC, FALSE);

    int i, k;
    double R;
    CComplex c, q;
    CFemmeDoc* pDoc = GetDocument();
    SelectCircFlag = 0;
    if ((mx == wzx) && (my == wzy)) {
      InvalidateRect(NULL);
      return;
    }

    R = abs((mx - wzx) + I * (my - wzy));
    c = wzx + I * wzy;

    if ((EditAction == 0) || (EditAction == 4)) {
      for (i = 0; i < pDoc->nodelist.GetSize(); i++) {
        q = pDoc->nodelist[i].CC();
        if (abs(q - c) <= R)
          pDoc->nodelist[i].IsSelected = TRUE;
      }
    }

    if ((EditAction == 2) || (EditAction == 4)) {
      for (i = 0; i < pDoc->blocklist.GetSize(); i++) {
        q = pDoc->blocklist[i].x + I * pDoc->blocklist[i].y;
        if (abs(q - c) <= R)
          pDoc->blocklist[i].IsSelected = TRUE;
      }
    }

    if ((EditAction == 1) || (EditAction == 4)) {
      for (i = 0; i < pDoc->linelist.GetSize(); i++) {

        k = 0;
        q = pDoc->nodelist[pDoc->linelist[i].n0].CC();
        if (abs(q - c) <= R)
          k++;
        q = pDoc->nodelist[pDoc->linelist[i].n1].CC();
        if (abs(q - c) <= R)
          k++;

        if (k == 2)
          pDoc->linelist[i].IsSelected = TRUE;
      }
    }

    if ((EditAction == 3) || (EditAction == 4)) {
      for (i = 0; i < pDoc->arclist.GetSize(); i++) {
        k = 0;
        q = pDoc->nodelist[pDoc->arclist[i].n0].CC();
        if (abs(q - c) <= R)
          k++;
        q = pDoc->nodelist[pDoc->arclist[i].n1].CC();
        if (abs(q - c) <= R)
          k++;

        if (k == 2)
          pDoc->arclist[i].IsSelected = TRUE;
      }
    }
    InvalidateRect(NULL);
  }

  CView::OnLButtonUp(nFlags, point);
}

void CFemmeView::OnRButtonDown(UINT nFlags, CPoint point)
{
  if (bLinehook != FALSE) {
    CView::OnRButtonDown(nFlags, point);
    return;
  }

  CFemmeDoc* pDoc = GetDocument();
  int i, j;

  // toggle select of nearest point
  if (EditAction == 0) {
    j = pDoc->ClosestNode(mx, my);
    if (j >= 0)
      pDoc->nodelist[j].ToggleSelect();
    DrawPSLG();
  }

  // toggle select of nearest segment
  if (EditAction == 1) {
    j = pDoc->ClosestSegment(mx, my);
    if (j >= 0)
      pDoc->linelist[j].ToggleSelect();
    DrawPSLG();
  }

  // toggle select of nearest arc segment
  if (EditAction == 3) {
    j = pDoc->ClosestArcSegment(mx, my);
    if (j >= 0)
      pDoc->arclist[j].ToggleSelect();
    DrawPSLG();
  }
  // toggle select of nearest blocklabel
  if (EditAction == 2) {
    j = pDoc->ClosestBlockLabel(mx, my);
    if (j >= 0)
      pDoc->blocklist[j].ToggleSelect();
    DrawPSLG();
  }

  if (EditAction == 4) {
    int clnode, clseg, claseg, cllbl;
    double dnode, dseg, daseg, dlbl, d;

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

    // find distance to closest block label;
    cllbl = pDoc->ClosestBlockLabel(mx, my);
    if (cllbl >= 0)
      dlbl = pDoc->blocklist[cllbl].GetDistance(mx, my);
    else
      dlbl = -1.;

    // now, compare to find the closest entity;
    j = -1;
    if (clnode >= 0) {
      d = dnode;
      j = pDoc->nodelist[clnode].InGroup;
    }
    if ((dseg < d) && (clseg >= 0)) {
      d = dseg;
      j = pDoc->linelist[clseg].InGroup;
    }
    if ((daseg < d) && (claseg >= 0)) {
      d = daseg;
      j = pDoc->arclist[claseg].InGroup;
    }
    if ((dlbl < d) && (cllbl >= 0)) {
      d = dlbl;
      j = pDoc->blocklist[cllbl].InGroup;
    }

    if (j < 1)
      return; // return if the closest object is associated with
              // the default group, or if there are no objects.

    // now, the group associated with the nearest entity is in j;
    // toggle the select of all objects in this group;
    for (i = 0; i < pDoc->nodelist.GetSize(); i++)
      if (pDoc->nodelist[i].InGroup == j)
        pDoc->nodelist[i].ToggleSelect();
    for (i = 0; i < pDoc->linelist.GetSize(); i++)
      if (pDoc->linelist[i].InGroup == j)
        pDoc->linelist[i].ToggleSelect();
    for (i = 0; i < pDoc->arclist.GetSize(); i++)
      if (pDoc->arclist[i].InGroup == j)
        pDoc->arclist[i].ToggleSelect();
    for (i = 0; i < pDoc->blocklist.GetSize(); i++)
      if (pDoc->blocklist[i].InGroup == j)
        pDoc->blocklist[i].ToggleSelect();
    DrawPSLG();
  }

  CView::OnRButtonDown(nFlags, point);
}

void CFemmeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CView::OnLButtonDblClk(nFlags, point);
}

void CFemmeView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
  CFemmeDoc* pDoc = GetDocument();
  if (EditAction == 0) {
    int i = pDoc->ClosestNode(mx, my);
    if (i >= 0) {
      char s[256];
      int j;
      CString ss;

      sprintf(s, "Closest node:  (%g,%g)\n", pDoc->nodelist[i].x,
          pDoc->nodelist[i].y);
      ss += s;
      ss += "Nodal Property:  ";
      sprintf(s, "<None>\n");
      for (j = 0; j < pDoc->nodeproplist.GetSize(); j++) {
        if (pDoc->nodeproplist[j].PointName == pDoc->nodelist[i].BoundaryMarker) {
          sprintf(s, "%s\n", (const char*)pDoc->nodelist[i].BoundaryMarker);
        }
      }
      ss += s;
      sprintf(s, "In Group:  %i", pDoc->nodelist[i].InGroup);
      ss += s;

      AfxMessageBox(ss, MB_ICONINFORMATION);
      pDoc->nodelist[i].ToggleSelect();
      DrawPSLG();
    }
  }

  if (EditAction == 1) {
    int i = pDoc->ClosestSegment(mx, my);
    int j;
    CString ss;

    if (i >= 0) {
      char s[256];
      sprintf(s, "Length of closest segment: %g\n",
          pDoc->LineLength(i));

      ss += s;
      ss += "Boundary Property:  ";

      sprintf(s, "<None>\n");
      for (j = 0; j < pDoc->lineproplist.GetSize(); j++) {
        if (pDoc->lineproplist[j].BdryName == pDoc->linelist[i].BoundaryMarker) {
          sprintf(s, "%s\n", (const char*)pDoc->linelist[i].BoundaryMarker);
        }
      }
      ss += s;

      ss += "Grid spacing:  ";
      if (pDoc->linelist[i].MaxSideLength <= 0)
        sprintf(s, "<Auto>\n");
      else
        sprintf(s, "%g\n", pDoc->linelist[i].MaxSideLength);
      ss += s;
      sprintf(s, "In Group:  %i", pDoc->linelist[i].InGroup);
      ss += s;

      if (pDoc->linelist[i].Hidden == TRUE)
        ss += "\nHidden in postprocessor";

      AfxMessageBox(ss, MB_ICONINFORMATION);
      pDoc->linelist[i].ToggleSelect();
      DrawPSLG();
    }
  }

  if (EditAction == 2) {
    int i = pDoc->ClosestBlockLabel(mx, my);
    int j, k;
    if (i >= 0) {
      char s[256];
      sprintf(s, "Closest block label:  (%g,%g)\n", pDoc->blocklist[i].x,
          pDoc->blocklist[i].y);
      CString ss = s;
      ss += "Block Material:  ";
      k = -1;
      sprintf(s, "<None>\n");
      if (pDoc->blocklist[i].BlockType == "<No Mesh>") {
        sprintf(s, "<No Mesh>\n");
      } else
        for (j = 0; j < pDoc->blockproplist.GetSize(); j++) {
          if (pDoc->blockproplist[j].BlockName == pDoc->blocklist[i].BlockType) {
            sprintf(s, "%s\n", (const char*)pDoc->blocklist[i].BlockType);
            k = j;
          }
        }
      ss += s;

      ss += "In Circuit:  ";
      sprintf(s, "<None>\n");
      for (j = 0; j < pDoc->circproplist.GetSize(); j++) {
        if (pDoc->circproplist[j].CircName == pDoc->blocklist[i].InCircuit) {
          sprintf(s, "%s\n", (const char*)pDoc->blocklist[i].InCircuit);
        }
      }
      ss += s;

      ss += "Mesh size:  ";
      if (pDoc->blocklist[i].MaxArea == 0)
        sprintf(s, "<Auto>\n");
      else
        sprintf(s, "%g\n",
            floor(2.e07 * sqrt(pDoc->blocklist[i].MaxArea / PI)
                + 0.5)
                / 1.e07);
      ss += s;

      sprintf(s, "In Group:  %i", pDoc->blocklist[i].InGroup);
      ss += s;

      // if it has magnetized material....
      if (k >= 0) {
        if (pDoc->blockproplist[k].H_c > 0) {
          sprintf(s, "\nMag Direction:  %g deg",
              pDoc->blocklist[i].MagDir);
          ss += s;
        }
      }
      AfxMessageBox(ss, MB_ICONINFORMATION);
      pDoc->blocklist[i].ToggleSelect();
      DrawPSLG();
    }
  }

  if (EditAction == 3) {
    int i = pDoc->ClosestArcSegment(mx, my);
    if (i >= 0) {
      char s[256];
      CString ss;
      int j;
      double R;
      CComplex c;

      sprintf(s, "Angle spanned by closest arc: %g deg\n",
          pDoc->arclist[i].ArcLength);
      ss += s;
      pDoc->GetCircle(pDoc->arclist[i], c, R);
      c = Chop(c);
      sprintf(s, "Radius of associated circle: %g\n", R);
      ss += s;
      sprintf(s, "Center of associated circle: (%g,%g)\n", Re(c), Im(c));
      ss += s;
      ss += "Boundary Property:  ";

      sprintf(s, "<None>\n");
      for (j = 0; j < pDoc->lineproplist.GetSize(); j++) {
        if (pDoc->lineproplist[j].BdryName == pDoc->arclist[i].BoundaryMarker) {
          sprintf(s, "%s\n", (const char*)pDoc->arclist[i].BoundaryMarker);
        }
      }
      ss += s;
      sprintf(s, "Grid spacing:  %g deg\n", pDoc->arclist[i].MaxSideLength);
      ss += s;
      sprintf(s, "In Group:  %i", pDoc->arclist[i].InGroup);
      ss += s;
      if (pDoc->arclist[i].Hidden == TRUE)
        ss += "\nHidden in postprocessor";

      AfxMessageBox(ss, MB_ICONINFORMATION);
      pDoc->arclist[i].ToggleSelect();
      DrawPSLG();
    }
  }
  CView::OnRButtonDblClk(nFlags, point);
}

void CFemmeView::OnMakeMesh()
{
  CFemmeDoc* TheDoc = GetDocument();

  ASSERT_VALID(TheDoc);
  CString pn = TheDoc->GetPathName();
  if (pn.GetLength() == 0) {
    MsgBox("A data file must be loaded,\nor the current data must saved.");
    return;
  }

  if (TheDoc->OnSaveDocument(pn) == FALSE)
    return;

  if (bLinehook == FALSE)
    BeginWaitCursor();
  if (TheDoc->HasPeriodicBC() == TRUE) {
    if (TheDoc->FunnyOnWritePoly() == FALSE) {
      if (bLinehook == FALSE)
        EndWaitCursor();
      TheDoc->UnselectAll();
      return;
    }
  } else {
    if (TheDoc->OnWritePoly() == FALSE) {
      if (bLinehook == FALSE)
        EndWaitCursor();
      return;
    }
  }
  BOOL LoadMesh = TheDoc->LoadMesh();
  if (bLinehook == FALSE)
    EndWaitCursor();

  if (LoadMesh == TRUE) {
    MeshUpToDate = TRUE;
    if (MeshFlag == FALSE)
      OnShowMesh();
    else
      InvalidateRect(NULL);
    CString s;
    s.Format("Created mesh with %i nodes", TheDoc->meshnode.GetSize());
    if (TheDoc->greymeshline.GetSize() != 0)
      s += "\nGrey mesh lines denote regions\nthat have no block label.";
    if (bLinehook == FALSE)
      AfxMessageBox(s, MB_ICONINFORMATION);
    else
      lua_pushnumber(lua, (int)TheDoc->meshnode.GetSize());
  }
}

void CFemmeView::OnMenuAnalyze()
{
  int i, j, k;
  CFemmeDoc* TheDoc = GetDocument();
  BOOL bFlag = FALSE;
  BOOL bExteriorDefined = TRUE;

  ASSERT_VALID(TheDoc);

  // check to see if all blocklabels are kosher...
  if (TheDoc->blocklist.GetSize() == 0) {
    MsgBox("No block information has been defined\nCannot analyze the problem");
    return;
  }

  for (i = 0, bFlag = FALSE; i < TheDoc->blocklist.GetSize(); i++) {
    for (k = 0, j = 0; k < TheDoc->blockproplist.GetSize(); k++)
      if (TheDoc->blocklist[i].BlockType != TheDoc->blockproplist[k].BlockName)
        j++;
    if ((j == TheDoc->blockproplist.GetSize()) && (TheDoc->blocklist[i].BlockType != "<No Mesh>")) {
      if (bFlag == FALSE)
        OnBlockOp();
      bFlag = TRUE;
      TheDoc->blocklist[i].IsSelected = TRUE;
    }
  }

  if (bFlag == TRUE) {
    InvalidateRect(NULL);
    CString ermsg = "Material properties have not\n";
    ermsg += "been defined for all block labels.\n";
    ermsg += "Cannot analyze the problem";
    MsgBox(ermsg);
    return;
  }

  if (TheDoc->ProblemType == 1) {
    // check to see if all of the input points are on r>=0 for axisymmetric problems.
    for (k = 0; k < TheDoc->nodelist.GetSize(); k++)
      if (TheDoc->nodelist[k].x < -(1.e-6)) {
        InvalidateRect(NULL);
        CString ermsg = "The problem domain must lie in\n";
        ermsg += "r>=0 for axisymmetric problems.\n";
        ermsg += "Cannot analyze the problem.";
        MsgBox(ermsg);
        return;
      }

    // check to see if all block defined to be in an axisymmetric external region are linear.
    for (k = 0; k < TheDoc->blocklist.GetSize(); k++) {
      if (TheDoc->blocklist[k].IsExternal) {

        if ((TheDoc->extRo == 0) || (TheDoc->extRi == 0))
          bExteriorDefined = FALSE;

        for (i = 0; i < TheDoc->blockproplist.GetSize(); i++)
          if (TheDoc->blocklist[k].BlockType == TheDoc->blockproplist[i].BlockName) {
            if (TheDoc->blockproplist[i].BHpoints != 0)
              bFlag = TRUE;
            else if (TheDoc->blockproplist[i].mu_x != TheDoc->blockproplist[i].mu_y)
              bFlag = TRUE;
          }
      }
    }
    if (bFlag) {
      InvalidateRect(NULL);
      CString ermsg = "Only linear istropic materials are\n";
      ermsg += "allowed in axisymmetric external regions.\n";
      ermsg += "Cannot analyze the problem";
      MsgBox(ermsg);
      return;
    }

    if (!bExteriorDefined) {
      InvalidateRect(NULL);
      CString ermsg = "Some block labels have been specific as placed in\n";
      ermsg += "an axisymmetric exterior region, but no properties\n";
      ermsg += "have been adequately defined for the exterior region\n";
      ermsg += "Cannot analyze the problem";
      MsgBox(ermsg);
      return;
    }
  }

  CString pn = TheDoc->GetPathName();
  if (pn.GetLength() == 0) {
    MsgBox("A data file must be loaded,\nor the current data must saved.");
    return;
  }
  if (TheDoc->OnSaveDocument(pn) == FALSE)
    return;

  if (bLinehook == FALSE)
    BeginWaitCursor();
  if (TheDoc->HasPeriodicBC() == TRUE) {
    if (TheDoc->FunnyOnWritePoly() == FALSE) {
      if (bLinehook == FALSE)
        EndWaitCursor();
      TheDoc->UnselectAll();
      return;
    }
  } else {
    if (TheDoc->OnWritePoly() == FALSE) {
      if (bLinehook == FALSE)
        EndWaitCursor();
      return;
    }
  }
  if (bLinehook == FALSE)
    EndWaitCursor();

  char CommandLine[512];
  CString rootname = "\"" + pn.Left(pn.ReverseFind('.')) + "\"";

  if (bLinehook == HiddenLua)
    sprintf(CommandLine, "\"%sfkn.exe\" %s bLinehook", (const char*)BinDir, (const char*)rootname);
  else
    sprintf(CommandLine, "\"%sfkn.exe\" %s", (const char*)BinDir, (const char*)rootname);

  CString MyPath = pn.Left(pn.ReverseFind('\\'));

  STARTUPINFO StartupInfo2 = { 0 };
  PROCESS_INFORMATION ProcessInfo2;
  StartupInfo2.cb = sizeof(STARTUPINFO);
  StartupInfo2.dwFlags = STARTF_FORCEOFFFEEDBACK;
  if (bLinehook == HiddenLua) {
    StartupInfo2.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
    //<DP> SHOWNOACTIVATE doesn't steal focus to others </DP>
    StartupInfo2.wShowWindow = SW_SHOWMINNOACTIVE;
  }
  if (CreateProcess(NULL, CommandLine, NULL, NULL, FALSE,
          0, NULL, MyPath, &StartupInfo2, &ProcessInfo2)) {
    if (bLinehook != FALSE) {
      DWORD ExitCode;
      hProc = ProcessInfo2.hProcess;
      do {
        GetExitCodeProcess(ProcessInfo2.hProcess, &ExitCode);
        ((CFemmApp*)AfxGetApp())->line_hook(lua, NULL);
        Sleep(1);
      } while (ExitCode == STILL_ACTIVE);
      hProc = NULL;

      if (ExitCode == 1)
        MsgBox("Material properties have not been defined for all regions");
      if (ExitCode == 2)
        MsgBox("problem loading mesh");
      if (ExitCode == 3)
        MsgBox("problem renumbering node points");
      if (ExitCode == 4)
        MsgBox("couldn't allocate enough space for matrices");
      if (ExitCode == 5)
        MsgBox("Couldn't solve the problem");
      if (ExitCode == 6)
        MsgBox("couldn't write results to disk");
      if (ExitCode == 7)
        MsgBox("problem loading input file");
    }
    CloseHandle(ProcessInfo2.hProcess);
    CloseHandle(ProcessInfo2.hThread);
  } else {
    MsgBox("Problem executing the solver");
    return;
  }
}

void CFemmeView::OnMenuViewres()
{
  CFemmeDoc* TheDoc = GetDocument();

  ASSERT_VALID(TheDoc);
  CString pn = TheDoc->GetPathName();
  if (pn.GetLength() == 0) {
    MsgBox("No results to display");
    return;
  }

  CString rootname = "\"" + pn.Left(pn.ReverseFind('.')) + ".ans\"";
  ((CFemmApp*)AfxGetApp())->MOCheckIfAlreadyOpen(rootname);
  AfxGetApp()->OpenDocumentFile(rootname);
}

void CFemmeView::OnUndo()
{
  CFemmeDoc* TheDoc = GetDocument();
  TheDoc->Undo();
  TheDoc->UnselectAll();
  InvalidateRect(NULL);
}

void CFemmeView::OnKbdZoom()
{
  // TODO: Add your command handler code here
  CFemmeDoc* pDoc = GetDocument();
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
    InvalidateRect(NULL);
  }
}

void CFemmeView::OnMoveObjects()
{
  CCopyDlg dlg;
  CFemmeDoc* pDoc = GetDocument();

  dlg.m_aboutx = 0;
  dlg.m_abouty = 0;
  dlg.m_shiftangle = 0;
  dlg.m_deltax = 0;
  dlg.m_deltay = 0;
  dlg.IsMove = TRUE;
  dlg.m_ncopies = 0;

  if (dlg.DoModal() == IDOK) {
    if (dlg.BtnState == 0) // Rotate
    {
      pDoc->UpdateUndo();
      pDoc->RotateMove(CComplex(dlg.m_aboutx, dlg.m_abouty),
          dlg.m_shiftangle, EditAction);
      pDoc->meshnode.RemoveAll();
      pDoc->meshline.RemoveAll();
      pDoc->greymeshline.RemoveAll();
      MeshFlag = FALSE;
      MeshUpToDate = FALSE;
      InvalidateRect(NULL);
    }
    if (dlg.BtnState == 1) // Translate
    {
      pDoc->UpdateUndo();
      pDoc->TranslateMove(dlg.m_deltax, dlg.m_deltay, EditAction);
      pDoc->meshnode.RemoveAll();
      pDoc->meshline.RemoveAll();
      MeshFlag = FALSE;
      MeshUpToDate = FALSE;
      InvalidateRect(NULL);
    }
  }
}

void CFemmeView::OnCopyObjects()
{
  CCopyDlg dlg;
  CFemmeDoc* pDoc = GetDocument();

  dlg.m_aboutx = 0;
  dlg.m_abouty = 0;
  dlg.m_shiftangle = 0;
  dlg.m_deltax = 0;
  dlg.m_deltay = 0;
  dlg.IsMove = FALSE;
  dlg.m_ncopies = 1;

  if (dlg.DoModal() == IDOK) {
    if (dlg.BtnState == 0) // Rotate
    {
      pDoc->UpdateUndo();
      pDoc->RotateCopy(CComplex(dlg.m_aboutx, dlg.m_abouty),
          dlg.m_shiftangle, dlg.m_ncopies, EditAction);
      pDoc->meshnode.RemoveAll();
      pDoc->meshline.RemoveAll();
      pDoc->greymeshline.RemoveAll();
      MeshFlag = FALSE;
      MeshUpToDate = FALSE;
      InvalidateRect(NULL);
    }
    if (dlg.BtnState == 1) // Translate
    {
      pDoc->UpdateUndo();
      pDoc->TranslateCopy(dlg.m_deltax, dlg.m_deltay, dlg.m_ncopies, EditAction);
      pDoc->meshnode.RemoveAll();
      pDoc->meshline.RemoveAll();
      pDoc->greymeshline.RemoveAll();
      MeshFlag = FALSE;
      MeshUpToDate = FALSE;
      InvalidateRect(NULL);
    }
  }
}

void CFemmeView::OnDxfin()
{
  CString dxffile;
  CFileDialog* fname_dia;
  CFemmeDoc* pDoc = GetDocument();

  fname_dia = new CFileDialog(
      TRUE,
      "dxf | * ",
      dxffile,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "CAD Drawing (*.dxf) | *.dxf; *.DXF | All Files (*.*) | *.*||",
      NULL);

  if (fname_dia->DoModal() == IDCANCEL) {
    delete[] fname_dia;
    return;
  }
  dxffile = fname_dia->GetPathName();
  delete[] fname_dia;

  if (pDoc->ReadDXF(dxffile) == TRUE) {
    if (bLinehook == FALSE)
      EndWaitCursor();
    MeshFlag = FALSE;
    InvalidateRect(NULL);
  } else {
    MsgBox("Couldn't read specified DXF file\nMake sure the the specified file\nis in ASCII DXF r12 format");
    pDoc->NoDraw = FALSE;
    MeshFlag = FALSE;
    InvalidateRect(NULL);
  }
}

void CFemmeView::OnPurgemesh()
{

  CFemmeDoc* pDoc = GetDocument();
  pDoc->meshnode.RemoveAll();
  pDoc->meshnode.FreeExtra();
  pDoc->meshline.RemoveAll();
  pDoc->greymeshline.RemoveAll();
  pDoc->meshline.FreeExtra();
  pDoc->greymeshline.FreeExtra();
  MeshUpToDate = FALSE;
  if (MeshFlag == TRUE) {
    MeshFlag = FALSE;
    InvalidateRect(NULL);
  }
}

void CFemmeView::OnDxfwrite()
{

  CString dxffile;
  CFileDialog* fname_dia;
  CFemmeDoc* pDoc = GetDocument();

  fname_dia = new CFileDialog(
      FALSE,
      "dxf | * ",
      dxffile,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "CAD Drawing (*.dxf) | *.dxf; *.DXF | All Files (*.*) | *.*||",
      NULL);

  if (fname_dia->DoModal() == IDCANCEL) {
    delete[] fname_dia;
    return;
  }
  dxffile = fname_dia->GetPathName();
  delete[] fname_dia;

  if (bLinehook == FALSE)
    BeginWaitCursor();
  if (pDoc->WriteDXF(dxffile) == TRUE) {
    if (bLinehook == FALSE)
      EndWaitCursor();
    pDoc->FirstDraw = TRUE;
    MeshFlag = FALSE;
    InvalidateRect(NULL);
  } else {
    MsgBox("Couldn't write specified DXF file");
    if (bLinehook == FALSE)
      EndWaitCursor();
  }
}

BOOL CFemmeView::OnEraseBkgnd(CDC* pDC)
{

  // idea is to always make the background white.
  // source code copied from:
  // Frequently Asked Questions About Foundation Classes

  // Set brush to desired background color
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

void CFemmeView::OnPanDown()
{

  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  //	ox=ox-0.25*x/mag;
  oy -= 0.25 * y / mag;

  InvalidateRect(NULL);
}

void CFemmeView::OnPanLeft()
{

  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox -= 0.25 * x / mag;
  //	oy-=0.25*y/mag;

  InvalidateRect(NULL);
}

void CFemmeView::OnPanRight()
{

  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  ox += 0.25 * x / mag;
  //	oy-=0.25*y/mag;

  InvalidateRect(NULL);
}

void CFemmeView::OnPanUp()
{

  RECT r;
  double x, y;

  GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  //	ox+=0.25*x/mag;
  oy += 0.25 * y / mag;

  InvalidateRect(NULL);
}

void CFemmeView::OnMenuMatlib()
{
  fe_CLibDlg dlg;

  dlg.pDoc = GetDocument();
  dlg.BinDir = BinDir;

  dlg.DoModal();
}

void CFemmeView::OnOpenSelected()
{

  CFemmeDoc* pDoc = GetDocument();

  if (EditAction == 1)
    pDoc->OpSegDlg();
  if (EditAction == 2) {
    if (pDoc->OpBlkDlg() == TRUE)
      InvalidateRect(NULL);
  }
  if (EditAction == 3) {
    pDoc->OpArcSegDlg();
    InvalidateRect(NULL);
  }
  if (EditAction == 0)
    pDoc->OpNodeDlg();

  if (EditAction == 4)
    pDoc->OpGrpDlg();

  pDoc->UnselectAll();
  DrawPSLG();
}

void CFemmeView::OnEditScale()
{

  CScaleDlg dlg;
  CFemmeDoc* pDoc = GetDocument();
  double x[2], y[2], R, dt;
  int i, j, k;
  CComplex p, s, c;
  BOOL bFlag = FALSE;

  // look at points, block labels, and arcs to get bounding box.
  for (i = 0; i < pDoc->nodelist.GetSize(); i++)
    if (pDoc->nodelist[i].IsSelected == TRUE) {
      if (bFlag == FALSE) {
        bFlag = TRUE;
        x[0] = pDoc->nodelist[i].x;
        x[1] = x[0];
        y[0] = pDoc->nodelist[i].y;
        y[1] = y[0];
      }
      if (pDoc->nodelist[i].x < x[0])
        x[0] = pDoc->nodelist[i].x;
      if (pDoc->nodelist[i].x > x[1])
        x[1] = pDoc->nodelist[i].x;
      if (pDoc->nodelist[i].y < y[0])
        y[0] = pDoc->nodelist[i].y;
      if (pDoc->nodelist[i].y > y[1])
        y[1] = pDoc->nodelist[i].y;
    }

  for (i = 0; i < pDoc->blocklist.GetSize(); i++)
    if (pDoc->blocklist[i].IsSelected == TRUE) {
      if (bFlag == FALSE) {
        bFlag = TRUE;
        x[0] = pDoc->blocklist[i].x;
        x[1] = x[0];
        y[0] = pDoc->blocklist[i].y;
        y[1] = y[0];
      }
      if (pDoc->blocklist[i].x < x[0])
        x[0] = pDoc->blocklist[i].x;
      if (pDoc->blocklist[i].x > x[1])
        x[1] = pDoc->blocklist[i].x;
      if (pDoc->blocklist[i].y < y[0])
        y[0] = pDoc->blocklist[i].y;
      if (pDoc->blocklist[i].y > y[1])
        y[1] = pDoc->blocklist[i].y;
    }

  for (i = 0; i < pDoc->linelist.GetSize(); i++)
    if (pDoc->linelist[i].IsSelected == TRUE) {
      for (j = 0; j < 2; j++) {
        if (j == 0)
          k = pDoc->linelist[i].n0;
        else
          k = pDoc->linelist[i].n1;
        if (bFlag == FALSE) {
          bFlag = TRUE;
          x[0] = pDoc->nodelist[k].x;
          x[1] = x[0];
          y[0] = pDoc->nodelist[k].y;
          y[1] = y[0];
        }
        if (pDoc->nodelist[k].x < x[0])
          x[0] = pDoc->nodelist[k].x;
        if (pDoc->nodelist[k].x > x[1])
          x[1] = pDoc->nodelist[k].x;
        if (pDoc->nodelist[k].y < y[0])
          y[0] = pDoc->nodelist[k].y;
        if (pDoc->nodelist[k].y > y[1])
          y[1] = pDoc->nodelist[k].y;
      }
    }

  for (i = 0; i < pDoc->arclist.GetSize(); i++)
    if (pDoc->arclist[i].IsSelected == TRUE) {
      k = (int)ceil(pDoc->arclist[i].ArcLength / pDoc->arclist[i].mySideLength);
      dt = pDoc->arclist[i].ArcLength * PI / (((double)k) * 180.);
      pDoc->GetCircle(pDoc->arclist[i], c, R);
      p.Set(pDoc->nodelist[pDoc->arclist[i].n0].x,
          pDoc->nodelist[pDoc->arclist[i].n0].y);
      s = exp(I * dt);
      for (j = 0; j < k; j++) {
        p = (p - c) * s + c;
        if (bFlag == FALSE) {
          bFlag = TRUE;
          x[0] = p.re;
          x[1] = x[0];
          y[0] = p.im;
          y[1] = y[0];
        }
        if (p.re < x[0])
          x[0] = p.re;
        if (p.re > x[1])
          x[1] = p.re;
        if (p.im < y[0])
          y[0] = p.im;
        if (p.im > y[1])
          y[1] = p.im;
      }
    }

  if (bFlag == FALSE)
    return;

  dlg.m_scalefactor = 1.;
  dlg.m_basex = (x[1] + x[0]) / 2.;
  dlg.m_basey = (y[1] + y[0]) / 2.;

  if (dlg.DoModal() == IDOK) {
    pDoc->UpdateUndo();
    pDoc->ScaleMove(dlg.m_basex, dlg.m_basey, dlg.m_scalefactor, EditAction);
    InvalidateRect(NULL);
  }
}

void CFemmeView::OnEditMirror()
{

  CMirrorDlg dlg;
  CFemmeDoc* pDoc = GetDocument();

  if (dlg.DoModal() == IDOK) {
    pDoc->UpdateUndo();
    pDoc->MirrorSelected(dlg.m_pax, dlg.m_pay,
        dlg.m_pbx, dlg.m_pby, EditAction);
    InvalidateRect(NULL);
  }
}

void CFemmeView::OnEditCut()
{

  CFemmeDoc* pDoc = GetDocument();
  BOOL flag;

  pDoc->UpdateUndo();
  if (EditAction == 0)
    flag = pDoc->DeleteSelectedNodes();
  if (EditAction == 1)
    flag = pDoc->DeleteSelectedSegments();
  if (EditAction == 2)
    flag = pDoc->DeleteSelectedBlockLabels();
  if (EditAction == 3)
    flag = pDoc->DeleteSelectedArcSegments();
  if (EditAction == 4) {
    flag = FALSE;
    if (pDoc->DeleteSelectedSegments() == TRUE)
      flag = TRUE;
    if (pDoc->DeleteSelectedArcSegments() == TRUE)
      flag = TRUE;
    if (pDoc->DeleteSelectedNodes() == TRUE)
      flag = TRUE;
    if (pDoc->DeleteSelectedBlockLabels() == TRUE)
      flag = TRUE;
  }
  if (flag == TRUE) {
    MeshUpToDate = FALSE;
    if (MeshFlag == TRUE)
      OnShowMesh();
    else
      InvalidateRect(NULL);
  }
}

BOOL CFemmeView::ScanPreferences()
{
  FILE* fp;
  CString fname;

  fname = BinDir + "femme.cfg";

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

      if (_strnicmp(q, "<BkgndColor>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        BackColor = RGB(cr, cg, cb);
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

      if (_strnicmp(q, "<LineColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        LineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GridColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        GridColor = RGB(cr, cg, cb);
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

      if (_strnicmp(q, "<EditAction>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_action);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<PixelsPerUnit>", 15) == 0) {
        v = StripKey(s);
        sscanf(v, "%lf", &d_mag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GridSize>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%lf", &d_gridsize);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowGrid>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_showgrid);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowOrigin>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_showorigin);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<SnapGrid>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_snapgrid);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowNames>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_shownames);
        q[0] = NULL;
      }
    }
    fclose(fp);
    return TRUE;
  }

  return FALSE;
}

void WriteColor(char* cname, COLORREF c, FILE* fp)
{
  fprintf(fp, "<%s> = %i	%i	%i\n", cname,
      GetRValue(c), GetGValue(c), GetBValue(c));
}

BOOL CFemmeView::WritePreferences()
{
  FILE* fp;
  CString fname;
  CFemmeDoc* pDoc = GetDocument();

  fname = BinDir + "femme.cfg";

  fp = fopen(fname, "wt");
  if (fp != NULL) {
    WriteColor("SelColor", SelColor, fp);
    WriteColor("BkgndColor", BackColor, fp);
    WriteColor("MeshColor", MeshColor, fp);
    WriteColor("BlockColor", BlockColor, fp);
    WriteColor("LineColor", LineColor, fp);
    WriteColor("GridColor", GridColor, fp);
    WriteColor("NodeColor", NodeColor, fp);
    WriteColor("NameColor", NameColor, fp);
    fprintf(fp, "<EditAction> = %i\n", d_action);
    fprintf(fp, "<PixelsPerUnit> = %g\n", d_mag);
    fprintf(fp, "<GridSize> = %g\n", d_gridsize);
    fprintf(fp, "<ShowGrid> = %i\n", d_showgrid);
    fprintf(fp, "<SnapGrid> = %i\n", d_snapgrid);
    fprintf(fp, "<ShowNames> = %i\n", d_shownames);
    fprintf(fp, "<ShowOrigin> = %i\n", d_showorigin);
    fprintf(fp, "<ProblemType> = %i\n", pDoc->d_type);
    fprintf(fp, "<LengthUnits> = %i\n", pDoc->d_length);
    fprintf(fp, "<Frequency> = %g\n", pDoc->d_freq);
    fprintf(fp, "<Precision>  = %g\n", pDoc->d_prec);
    fprintf(fp, "<MinAngle>   = %g\n", pDoc->d_minangle);
    fprintf(fp, "<Depth> = %g\n", pDoc->d_depth);
    fprintf(fp, "<Coordinates> = %i\n", pDoc->d_coord);

    fclose(fp);
  } else
    return FALSE;

  return TRUE;
}

void CFemmeView::OnViewShownames()
{

  ShowNames = 1 - ShowNames;
  InvalidateRect(NULL);
}

void CFemmeView::MyMoveTo(CDC* pDC, int x, int y)
{
  Xm = x;
  Ym = y;
}

void CFemmeView::MyLineTo(CDC* pDC, int x, int y)
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

void CFemmeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
  if (bActivate && pActivateView == this) {
    CMainFrame* MFrm;
    MFrm = (CMainFrame*)GetTopLevelFrame();
    if (!MFrm->IsIconic()) {
      MFrm->SetBar(1);

      MFrm->m_dlgBar.ShowWindow(SW_HIDE);
      CheckIt();
    }
  }

  CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CFemmeView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;

  return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CFemmeView::OnViewShowOrphans()
{
  CFemmeDoc* pDoc = GetDocument();

  OnGroupOp();
  pDoc->SelectOrphans();
  InvalidateRect(NULL);
}

void CFemmeView::OnUpdateEditExterior(CCmdUI* pCmdUI)
{
  if (GetDocument()->ProblemType)
    pCmdUI->Enable(TRUE);
  else
    pCmdUI->Enable(FALSE);
}

void CFemmeView::OnCreateRadius()
{
  SelectWndFlag = FALSE;
  ZoomWndFlag = FALSE;
  SelectCircFlag = FALSE;
  CreateRadiusFlag = TRUE;

  StatBar->SetPaneText(0, "SELECT POINT AT WHICH A RADIUS IS DESIRED", TRUE);
}
void CFemmeView::OnMakeABC()
{
  CMakeABCDlg dlg;
  CFemmeDoc* pDoc = GetDocument();

  dlg.n = 0;

  double xx[2], yy[2];
  if (GetBoundingBox(xx, yy) == 0)
    return;

  if (pDoc->ProblemType == 0) // 2D planar case
  {
    dlg.abcr = 0.75 * abs((xx[0] + I * yy[0]) - (xx[1] + I * yy[1]));
    dlg.abcx = (xx[0] + xx[1]) / 2.0;
    dlg.abcy = (yy[0] + yy[1]) / 2.0;
  } else { // Axi case
    dlg.abcx = 0;
    dlg.abcy = (yy[0] + yy[1]) / 2.0;
    dlg.abcr = 1.5 * abs(xx[1] + I * (yy[1] - yy[0]) / 2.0);
  }

  dlg.abcn = 7; // suggested default number of layers

  if (dlg.DoModal() == IDOK) {
    pDoc->UpdateUndo();
    CString LuaCmd;
    LuaCmd.Format("mi_makeABC(%i,%g,%g,%g,%i)", dlg.abcn, dlg.abcr, dlg.abcx, dlg.abcy, dlg.n);
    lua_dostring(lua, LuaCmd);
  }
}
