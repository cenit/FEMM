// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "femm.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFemmApp theApp;
extern BOOL bLinehook;
/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//{{AFX_MSG_MAP(CChildFrame)
ON_WM_SIZE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
  // TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  if (!CMDIChildWnd::PreCreateWindow(cs))
    return FALSE;

  return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
  // Gets first window to open maximized.  Originally cribbed from:
  //		Visual C++/MFC Frequently Asked Questions
  //		Scot Wingo
  //		Stingray Software
  //		Version 5.0, updated 5/15/97
  if (theApp.luaShowWindow != NULL)
    CMDIChildWnd::ActivateFrame(theApp.luaShowWindow);
  else if (GetMDIFrame()->MDIGetActive())
    CMDIChildWnd::ActivateFrame(nCmdShow);
  else
    CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
  theApp.luaShowWindow = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
  CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::ResizeClient(int nWidth, int nHeight, BOOL bRedraw)
{
  RECT rcWnd;
  GetClientRect(&rcWnd);

  if (nWidth != -1)
    rcWnd.right = nWidth;
  if (nHeight != -1)
    rcWnd.bottom = nHeight;
  if (!::AdjustWindowRectEx(&rcWnd, GetStyle(), (GetMenu() != NULL), GetExStyle()))
    return FALSE;

  UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;

  if (!bRedraw)
    uFlags |= SWP_NOREDRAW;

  return SetWindowPos(NULL, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, uFlags);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
  CMDIChildWnd::OnSize(nType, cx, cy);

  // Idea here is to give the active view a message that says that the window has
  // been minimized.  If the active view is a postprocessor window, the output
  // window may need to be hidden if the window is minimized but still active.

  if (nType == SIZE_MINIMIZED) {
    CView* theView = GetActiveView();
    theView->PostMessage(WM_SIZE, SIZE_MINIMIZED);
  }
}
