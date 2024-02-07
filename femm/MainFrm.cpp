// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "femm.h"

#include "lua.h"
#include "luadebug.h"
#include "luaconsoledlg.h"

#include "MainFrm.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int m_luaWindowStatus;
extern CLuaConsoleDlg* LuaConsole;
extern BOOL bLinehook;
extern HANDLE hProc;
extern lua_State* lua;
extern CFemmApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_COMMAND(ID_PT_PROP, OnPtProp)
ON_WM_INITMENU()
ON_COMMAND(ID_VIEW_LUACONSOLE, OnViewLuaConsole)
ON_WM_CLOSE()
ON_COMMAND(ID_PREFERENCES, OnPreferences)
ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
ON_COMMAND(ID_HELP_LICENSE, OnHelpLicense)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
  ID_SEPARATOR // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return -1; // fail to create
  }

  // force the status bar to use a more readable font size
  LOGFONT lf; // Used to create the CFont.
  memset(&lf, 0, sizeof(LOGFONT)); // Clear out structure.
  lf.lfHeight = 16;
  strcpy(lf.lfFaceName, "MS Sans Serif");
  m_StatusBarFont.CreateFontIndirect(&lf); // Create the font.
  m_wndStatusBar.SetFont(&m_StatusBarFont);

  // create output window;
  EnableDocking(0);

  if (m_dlgBar.Create(IDD_OUTBOX, this) != 0) {
    m_dlgBar.SetWindowText("FEMM Output");
    CRect rc;
    GetClientRect(&rc);
    ClientToScreen(&rc);
    CPoint p = rc.BottomRight();
    p.x -= 175;
    p.y -= 300;
    m_dlgBar.SetWindowPos(this, p.x, p.y, 0, 0, SWP_NOSIZE);
  }
  theApp.bShowOutputWindow = theApp.d_bShowOutputWindow;

  // flash up title;
  {
    CString s;
    s = "Femm 4.2\r\n";
    s += "Magnetics finite element suite\r\n";
    s += "Copyright 1998-2023\r\n\r\n";
    s += "David Meeker\r\ndmeeker@ieee.org\r\n";
    s += "https://www.femm.info";
    m_dlgBar.SetDlgItemText(IDC_OUTBOX, s);
  }

  //<DP>
  if (!m_wndToolBar1.Create(this) || !m_wndToolBar1.LoadToolBar(IDR_MAIN1)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_wndToolBar1.SetBarStyle(m_wndToolBar1.GetBarStyle() | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  if (!m_wndToolBar2.Create(this, WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_wndToolBar2.LoadToolBar(IDR_MAIN2)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  //</DP>

  if (!m_FV_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_FV_toolBar1.LoadToolBar(IDR_FEMMVIEWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_FV_toolBar1.SetWindowText("Femmview Toolbar");

  if (!m_leftbar.Create(this, WS_CHILD | CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_leftbar.LoadToolBar(IDR_LEFTBAR)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_leftbar.SetWindowText("Zoom/Pan Toolbar");

  if (!m_FE_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_FE_toolBar1.LoadToolBar(IDR_FEMMETYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_FE_toolBar1.SetWindowText("Femme Mode/View Commands");

  if (!m_BD_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_BD_toolBar1.LoadToolBar(IDR_BELADRAWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_BD_toolBar1.SetWindowText("Beladraw Mode/View Commands");

  if (!m_BV_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_BV_toolBar1.LoadToolBar(IDR_BELAVIEWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_BV_toolBar1.SetWindowText("Belaview Toolbar");

  if (!m_HD_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_HD_toolBar1.LoadToolBar(IDR_HDRAWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_HD_toolBar1.SetWindowText("hdraw Mode/View Commands");

  if (!m_HV_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_HV_toolBar1.LoadToolBar(IDR_HVIEWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_HV_toolBar1.SetWindowText("hview Toolbar");

  if (!m_CD_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_CD_toolBar1.LoadToolBar(IDR_HDRAWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_CD_toolBar1.SetWindowText("hdraw Mode/View Commands");

  if (!m_CV_toolBar1.Create(this, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI | CBRS_SIZE_DYNAMIC) || !m_CV_toolBar1.LoadToolBar(IDR_CVIEWTYPE)) {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }
  m_CV_toolBar1.SetWindowText("hview Toolbar");

  // Make toolbars dockable
  m_wndToolBar1.EnableDocking(CBRS_ALIGN_ANY); //<DP>
  m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY); //<DP>
  m_BD_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_HD_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_CD_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_FE_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_FV_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_BV_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_HV_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_CV_toolBar1.EnableDocking(CBRS_ALIGN_ANY);
  m_leftbar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);

  DockControlBar(&m_wndToolBar1);
  DockControlBar(&m_wndToolBar2);

  DockControlBarLeftOf(&m_FE_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_BD_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_HD_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_CD_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_FV_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_BV_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_HV_toolBar1, &m_wndToolBar1);
  DockControlBarLeftOf(&m_CV_toolBar1, &m_wndToolBar1);

  DockControlBarLeftOf(&m_leftbar, &m_wndToolBar2);

  m_LuaConsole.Create(IDD_LUACONSOLE);

  if ((m_luaWindowStatus == SW_SHOW) && (theApp.d_luaconsole))
    m_LuaConsole.ShowWindow(SW_SHOW);
  LuaConsole = &m_LuaConsole;

  lua_register(lua, "main_minimize", luaMinimize);
  lua_register(lua, "main_maximize", luaMaximize);
  lua_register(lua, "main_restore", luaRestore);
  lua_register(lua, "main_resize", luaResize);

#ifdef MDITAB
  // CMDITabs must be created last to ensure correct layout
  // ------------------------------------------------------
  bool m_bTop = false;
  bool m_bImages = true;
  DWORD dwStyle = (m_bTop ? MT_TOP : 0) | (m_bImages ? MT_IMAGES : 0);
  m_wndMDITabs.Create(this, dwStyle);
#endif

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CMDIFrameWnd::PreCreateWindow(cs))
    return FALSE;

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
  CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);

#ifdef MDITAB
  m_wndMDITabs.Update(); // sync the mditabctrl with all views
#endif
}

void CMainFrame::OnPtProp()
{
  if ((m_dlgBar.GetStyle() & WS_VISIBLE) != 0) {
    m_dlgBar.ShowWindow(SW_HIDE);
    theApp.bShowOutputWindow = FALSE;
  } else {
    m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);
    theApp.bShowOutputWindow = TRUE;
  }
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
  CMDIFrameWnd::OnInitMenu(pMenu);

  /*
    if( (m_toolBar.GetStyle() & WS_VISIBLE) != 0 ){
      // make sure that toolbar entry is checked
      pMenu->CheckMenuItem(ID_SHOW_TBAR, MF_CHECKED);
    }
    else{
      // make sure that toolbar entry is not checked
      pMenu->CheckMenuItem(ID_SHOW_TBAR, MF_UNCHECKED);
    }
  */
  if ((m_dlgBar.GetStyle() & WS_VISIBLE) != 0)
    pMenu->CheckMenuItem(ID_PT_PROP, MF_CHECKED);
  else
    pMenu->CheckMenuItem(ID_PT_PROP, MF_UNCHECKED);

  if ((LuaConsole->GetStyle() & WS_VISIBLE) != 0)
    pMenu->CheckMenuItem(ID_VIEW_LUACONSOLE, MF_CHECKED);
  else
    pMenu->CheckMenuItem(ID_VIEW_LUACONSOLE, MF_UNCHECKED);
}

void CMainFrame::OnViewLuaConsole()
{
  if ((m_LuaConsole.GetStyle() & WS_VISIBLE) != 0)
    m_LuaConsole.ShowWindow(SW_HIDE);
  else
    m_LuaConsole.ShowWindow(SW_SHOW);
}

void CMainFrame::SetBar(int n)
{
  // Displays the requested toolbar configuration
  switch (n) {
  // Note--the toolbars that are being turned on
  // need to be last in the list, or bad things
  // will happen.
  case 1: // Show Femme toolbar(s)
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 2: // Show Femmview toolbar
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 3: // Show Beladraw toolbar(s)
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BD_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 4: // Show Belaview toolbar
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BV_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 5: // Show hdraw toolbar(s)
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 6: // Show hview toolbar
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 7: // Show cdraw toolbar(s)
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  case 8: // Show cview toolbar
    ShowControlBar(&m_BD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_FE_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_BV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_HV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, TRUE, FALSE);
    ShowControlBar(&m_leftbar, TRUE, FALSE);
    break;

  default: // Show no toolbar
    ShowControlBar(&m_BV_toolBar1, FALSE, FALSE);
    ShowControlBar(&m_BD_toolBar1, FALSE, FALSE);
    ShowControlBar(&m_FV_toolBar1, FALSE, FALSE);
    ShowControlBar(&m_FE_toolBar1, FALSE, FALSE);
    ShowControlBar(&m_HV_toolBar1, FALSE, FALSE);
    ShowControlBar(&m_HD_toolBar1, FALSE, FALSE);
    ShowControlBar(&m_CD_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_CV_toolBar1, FALSE, TRUE);
    ShowControlBar(&m_leftbar, FALSE, FALSE);
    m_dlgBar.ShowWindow(SW_HIDE);
    break;
  }
}

void CMainFrame::OnClose()
{
  if ((bLinehook == NormalLua) || (bLinehook == HiddenLua)) {
    bLinehook = FALSE;
    if (hProc != NULL)
      TerminateProcess(hProc, NULL);
    hProc = NULL;
    PostMessage(WM_CLOSE);
    lua_error(lua, NULL);
  }

  if (bLinehook != ImportDXF)
    CFrameWnd::OnClose();
}

void CMainFrame::OnPreferences()
{
  CPreferences dlg;

  if (dlg.DoModal() == IDOK) {
    // do a really ugly kludge to get the views
    // to reload the color mappingsjust in case
    // they have been changed.
    theApp.ForceViewPrefReload();
  }
}

void CMainFrame::OnHelpFinder()
{
  CString manualname = theApp.GetExecutablePath() + "manual.pdf";
  if (((int)ShellExecute(m_hWnd, "open", manualname, "", "", SW_SHOWMAXIMIZED)) <= 32) {
    CString mymsg;
    mymsg.Format("Couldn't open %s\n", manualname);
    mymsg += "The most likely cause is that no viewer is registered\n";
    mymsg += "for PDF files.  If you have a PDF viewer such as Acrobat\n";
    mymsg += "Reader or Ghostview installed, you can open this file manually.";
    MsgBox(mymsg);
  }
}

void CMainFrame::OnHelpLicense()
{

  CString licensename = theApp.GetExecutablePath() + "license.txt";
  if (((int)ShellExecute(m_hWnd, "open", licensename, "", "", SW_SHOWNORMAL)) <= 32) {
    CString mymsg;
    mymsg.Format("Couldn't open %s\n", licensename);
    mymsg += "The most likely cause is that no viewer is registered\n";
    mymsg += "for RTF files.  If you have an RTF viewer such as Wordpad,\n";
    mymsg += "Word, or OpenOffice installed, you can open this file manually.";
    MsgBox(mymsg);
  }
}

int CMainFrame::luaMinimize(lua_State* L)
{
  //	theApp.GetMainWnd()->ShowWindow(SW_MINIMIZE);
  theApp.GetMainWnd()->ShowWindow(SW_SHOWMINNOACTIVE);
  m_luaWindowStatus = SW_SHOWMINNOACTIVE;
  return 0;
}

int CMainFrame::luaMaximize(lua_State* L)
{
  theApp.GetMainWnd()->ShowWindow(SW_MAXIMIZE);

  return 0;
}

int CMainFrame::luaRestore(lua_State* L)
{
  theApp.GetMainWnd()->ShowWindow(SW_RESTORE);

  return 0;
}

int CMainFrame::luaResize(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return 0;

  int nWidth = (int)lua_todouble(L, 1);
  int nHeight = (int)lua_todouble(L, 2);
  CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
  pMainFrame->ResizeClient(nWidth, nHeight, TRUE);

  return 0;
}

BOOL CMainFrame::ResizeClient(int nWidth, int nHeight, BOOL bRedraw)
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

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
  CRect rect;
  DWORD dw;
  UINT n;

  // get MFC to adjust the dimensions of all docked ToolBars
  // so that GetWindowRect will be accurate
  RecalcLayout(TRUE);

  LeftOf->GetWindowRect(&rect);
  rect.OffsetRect(1, 1);
  dw = LeftOf->GetBarStyle();
  n = 0;
  n = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
  n = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
  n = (dw & CBRS_ALIGN_LEFT && n == 0) ? AFX_IDW_DOCKBAR_LEFT : n;
  n = (dw & CBRS_ALIGN_RIGHT && n == 0) ? AFX_IDW_DOCKBAR_RIGHT : n;

  // When we take the default parameters on rect, DockControlBar will dock
  // each Toolbar on a separate line. By calculating a rectangle, we
  // are simulating a Toolbar being dragged to that location and docked.
  DockControlBar(Bar, n, &rect);
}
