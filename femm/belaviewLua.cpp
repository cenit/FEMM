#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "bv_problem.h"
#include "femm.h"
#include "xyplot.h"
#include "belaviewDoc.h"
#include "belaviewview.h"
#include "MainFrm.h"
#include "windows.h" //necessary for bitmap saving
#include "lua.h"
#include "mycommandlineinfo.h"
#include "promptbox.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern lua_State* lua;
extern BOOL bLinehook;
extern void* pBelaviewDoc;
extern BOOL lua_byebye;
extern int m_luaWindowStatus;
extern CLuaConsoleDlg* LuaConsole;

#define CatchNullDocument()                                    \
  ;                                                            \
  if (pBelaviewDoc == NULL) {                                  \
    CString msg = "No current electrostatics output in focus"; \
    lua_error(L, msg.GetBuffer(1));                            \
    return 0;                                                  \
  }

//-------------------Lua Extensions-------------------------

void CbelaviewDoc::initalise_lua()
{
  // use a messagebox to display errors
  lua_register(lua, "eo_setfocus", lua_switchfocus);
  lua_register(lua, "eo_getpointvalues", lua_getpointvals);
  lua_register(lua, "eo_close", lua_exitpost);
  lua_register(lua, "eo_addcontour", lua_addcontour);
  lua_register(lua, "eo_clearcontour", lua_clearcontour);
  lua_register(lua, "eo_lineintegral", lua_lineintegral);
  lua_register(lua, "eo_selectblock", lua_selectblock);
  lua_register(lua, "eo_groupselectblock", lua_groupselectblock);
  lua_register(lua, "eo_clearblock", lua_clearblock);
  lua_register(lua, "eo_blockintegral", lua_blockintegral);
  lua_register(lua, "eo_zoomnatural", lua_zoomnatural);
  lua_register(lua, "eo_zoomin", lua_zoomin);
  lua_register(lua, "eo_zoomout", lua_zoomout);
  lua_register(lua, "eo_zoom", lua_zoom);
  lua_register(lua, "eo_showgrid", lua_showgrid);
  lua_register(lua, "eo_hidegrid", lua_hidegrid);
  lua_register(lua, "eo_gridsnap", lua_gridsnap);
  lua_register(lua, "eo_setgrid", lua_setgrid);
  lua_register(lua, "eo_showmesh", lua_showmesh);
  lua_register(lua, "eo_hidemesh", lua_hidemesh);
  lua_register(lua, "eo_hidedensityplot", lua_hidedensity);
  lua_register(lua, "eo_showdensityplot", lua_showdensity);
  lua_register(lua, "eo_hidecontourplot", lua_hidecountour);
  lua_register(lua, "eo_showcontourplot", lua_showcountour);
  lua_register(lua, "eo_showvectorplot", lua_vectorplot);
  lua_register(lua, "eo_smooth", lua_smoothing);
  lua_register(lua, "eo_showpoints", lua_showpoints);
  lua_register(lua, "eo_hidepoints", lua_hidepoints);
  lua_register(lua, "eo_shownames", lua_shownames);
  lua_register(lua, "eo_getprobleminfo", lua_getprobleminfo);
  lua_register(lua, "eo_savebitmap", lua_savebitmap);
  lua_register(lua, "eo_getconductorproperties", lua_getcircuitprops);
  lua_register(lua, "eo_savemetafile", lua_saveWMF);
  lua_register(lua, "eo_refreshview", lua_refreshview);
  lua_register(lua, "eo_selectpoint", lua_selectline);
  lua_register(lua, "eo_seteditmode", lua_seteditmode);
  lua_register(lua, "eo_bendcontour", lua_bendcontour);
  lua_register(lua, "eo_makeplot", lua_makeplot);
  lua_register(lua, "eo_selectconductor", lua_selectconductor);
  lua_register(lua, "eo_reload", lua_reload);

  lua_register(lua, "eo_resize", luaResize);
  lua_register(lua, "eo_minimize", luaMinimize);
  lua_register(lua, "eo_maximize", luaMaximize);
  lua_register(lua, "eo_restore", luaRestore);
  lua_register(lua, "eo_gettitle", lua_gettitle);
  lua_register(lua, "eo_get_title", lua_gettitle);

  // compatibility with alternate lua function naming convention
  lua_register(lua, "eo_set_focus", lua_switchfocus);
  lua_register(lua, "eo_get_point_values", lua_getpointvals);
  lua_register(lua, "eo_add_contour", lua_addcontour);
  lua_register(lua, "eo_clear_contour", lua_clearcontour);
  lua_register(lua, "eo_line_integral", lua_lineintegral);
  lua_register(lua, "eo_select_block", lua_selectblock);
  lua_register(lua, "eo_group_select_block", lua_groupselectblock);
  lua_register(lua, "eo_clear_block", lua_clearblock);
  lua_register(lua, "eo_block_integral", lua_blockintegral);
  lua_register(lua, "eo_zoom_natural", lua_zoomnatural);
  lua_register(lua, "eo_zoom_in", lua_zoomin);
  lua_register(lua, "eo_zoom_out", lua_zoomout);
  lua_register(lua, "eo_show_grid", lua_showgrid);
  lua_register(lua, "eo_hide_grid", lua_hidegrid);
  lua_register(lua, "eo_show_mesh", lua_showmesh);
  lua_register(lua, "eo_hide_mesh", lua_hidemesh);
  lua_register(lua, "eo_hide_density_plot", lua_hidedensity);
  lua_register(lua, "eo_show_density_plot", lua_showdensity);
  lua_register(lua, "eo_hide_contour_plot", lua_hidecountour);
  lua_register(lua, "eo_show_contour_plot", lua_showcountour);
  lua_register(lua, "eo_show_points", lua_showpoints);
  lua_register(lua, "eo_hide_points", lua_hidepoints);
  lua_register(lua, "eo_grid_snap", lua_gridsnap);
  lua_register(lua, "eo_set_grid", lua_setgrid);
  lua_register(lua, "eo_get_problem_info", lua_getprobleminfo);
  lua_register(lua, "eo_save_bitmap", lua_savebitmap);
  lua_register(lua, "eo_get_conductor_properties", lua_getcircuitprops);
  lua_register(lua, "eo_save_metafile", lua_saveWMF);
  lua_register(lua, "eo_refresh_view", lua_refreshview);
  lua_register(lua, "eo_select_point", lua_selectline);
  lua_register(lua, "eo_set_edit_mode", lua_seteditmode);
  lua_register(lua, "eo_bend_contour", lua_bendcontour);
  lua_register(lua, "eo_make_plot", lua_makeplot);
  lua_register(lua, "eo_select_conductor", lua_selectconductor);
  lua_register(lua, "eo_show_names", lua_shownames);
  lua_register(lua, "eo_show_vector_plot", lua_vectorplot);

  // functions to access low-level mesh info
  lua_register(lua, "eo_numnodes", lua_numnodes);
  lua_register(lua, "eo_numelements", lua_numelements);
  lua_register(lua, "eo_getnode", lua_getnode);
  lua_register(lua, "eo_getelement", lua_getelement);

  lua_register(lua, "eo_num_nodes", lua_numnodes);
  lua_register(lua, "eo_num_elements", lua_numelements);
  lua_register(lua, "eo_get_node", lua_getnode);
  lua_register(lua, "eo_get_element", lua_getelement);

  pBelaviewDoc = this;
}

int CbelaviewDoc::lua_dumpheader(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;

  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  return 0;
}

int CbelaviewDoc::lua_switchfocus(lua_State* L)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  CbelaviewDoc* pDoc;
  CString DocTitle, ThisTitle;
  int n;

  // get title of desired document from Lua
  n = lua_gettop(L);
  if (n == 0)
    return NULL;
  DocTitle.Format("%s", lua_tostring(L, n));
  DocTitle.MakeLower();
  if (DocTitle.Right(4) == ".res")
    DocTitle = DocTitle.Left(DocTitle.GetLength() - 4);

  // get pointer to document template for FemmeDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 6; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (CbelaviewDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetTitle();
    ThisTitle.MakeLower();
    if (ThisTitle.Right(4) == ".res")
      ThisTitle = ThisTitle.Left(ThisTitle.GetLength() - 4);
    if (ThisTitle == DocTitle) {
      pBelaviewDoc = pDoc;
      return NULL;
    }
  }

  // if we can't find it, throw an error message
  CString msg;
  msg.Format("No document that matches %s", DocTitle);
  lua_error(L, msg.GetBuffer(1));
  return NULL;
}

int CbelaviewDoc::lua_getpointvals(lua_State* L)
{
  CatchNullDocument();
  double px, py;

  px = lua_todouble(L, 1);
  py = lua_todouble(L, 2);

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;

  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();

  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CPointVals u;
  CComplex Jtot;

  if (thisDoc->GetPointValues(px, py, u) == TRUE) {
    lua_pushnumber(L, u.V);
    lua_pushnumber(L, u.D.re);
    lua_pushnumber(L, u.D.im);
    lua_pushnumber(L, u.E.re);
    lua_pushnumber(L, u.E.im);
    lua_pushnumber(L, u.e.re);
    lua_pushnumber(L, u.e.im);
    lua_pushnumber(L, u.nrg);
    return 8;
  }

  return 0;
}

int CbelaviewDoc::lua_exitpost(lua_State* L)
{
  CatchNullDocument();
  ((CbelaviewDoc*)pBelaviewDoc)->OnCloseDocument();

  return 0;
}

int CbelaviewDoc::lua_reload(lua_State* L)
{
  CatchNullDocument();

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  // theView->BeginWaitCursor();
  thisDoc->OnReload();
  // theView->EndWaitCursor();
  if (theView->d_ResetOnReload == FALSE)
    thisDoc->FirstDraw = FALSE;
  theView->RedrawView();
  theView->LuaViewInfo();
  return 0;
}

void CbelaviewView::LuaViewInfo()
{
  OnViewInfo();
}

int CbelaviewDoc::lua_addcontour(lua_State* L)
{
  CatchNullDocument();
  CComplex z;
  int i;
  z.Set(lua_todouble(L, 1), lua_todouble(L, 2));

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  i = (int)thisDoc->contour.GetSize();

  if (i > 0) {
    if (z != thisDoc->contour[i - 1])
      thisDoc->contour.Add(z);
  } else
    thisDoc->contour.Add(z);
  theView->DrawUserContour(FALSE);

  return 0;
}

int CbelaviewDoc::lua_clearcontour(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  theView->EraseUserContour(TRUE);
  thisDoc->contour.RemoveAll();

  return 0;
}

int CbelaviewDoc::lua_lineintegral(lua_State* L)
{
  CatchNullDocument();
  int type;
  double z[4];

  type = (int)lua_todouble(L, 1);
  // 0 - E.t
  // 1 - D.n
  // 2 - Cont length
  // 3 - Force from stress tensor
  // 4 - Torque from stress tensor

  if (type < 0 || type > 4) {
    CString msg;
    msg.Format("Invalid line integral selected %d", type);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  z[0] = z[1] = 0;
  thisDoc->LineIntegral(type, z);

  lua_pushnumber(lua, z[0]);
  lua_pushnumber(lua, z[1]);

  return 2;
}

int CbelaviewDoc::lua_selectblock(lua_State* L)
{
  CatchNullDocument();
  double px, py;

  px = lua_todouble(L, 1);
  py = lua_todouble(L, 2);

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  theView->EditAction = 2;
  int k;

  if (thisDoc->meshelem.GetSize() > 0) {
    k = thisDoc->InTriangle(px, py);
    if (k >= 0) {
      thisDoc->bHasMask = FALSE;
      thisDoc->blocklist[thisDoc->meshelem[k].lbl].ToggleSelect();
      //	RedrawView();
      theView->DrawSelected = thisDoc->meshelem[k].lbl;
      HDC myDCH = GetDC(theView->m_hWnd);
      CDC tempDC;
      CDC* pDC;
      pDC = tempDC.FromHandle(myDCH);

      // CDC *pDC=GetDC(theView->m_hWnd);
      theView->OnDraw(pDC);
      theView->DrawSelected = -1;
      theView->ReleaseDC(pDC);
      thisDoc->UpdateAllViews(theView);
    }
  }

  return 0;
}

int CbelaviewDoc::lua_groupselectblock(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  theView->EditAction = 2;
  int j, k, n;

  if (thisDoc->meshelem.GetSize() > 0) {
    n = lua_gettop(L);
    k = 0;
    if (n > 0)
      k = (int)lua_todouble(L, 1);

    for (j = 0; j < thisDoc->blocklist.GetSize(); j++) {
      if ((thisDoc->blocklist[j].InGroup == k) || (n == 0))
        thisDoc->blocklist[j].ToggleSelect();
      thisDoc->bHasMask = FALSE;
    }

    HDC myDCH = GetDC(theView->m_hWnd);
    CDC tempDC;
    CDC* pDC;
    pDC = tempDC.FromHandle(myDCH);

    // CDC *pDC=GetDC(theView->m_hWnd);
    theView->OnDraw(pDC);
    theView->DrawSelected = -1;
    theView->ReleaseDC(pDC);
    thisDoc->UpdateAllViews(theView);
  }

  return 0;
}

int CbelaviewDoc::lua_clearblock(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  theView->UnselectAll();
  theView->EditAction = 0;
  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_blockintegral(lua_State* L)
{
  // 0 - Stored anergy
  // 1 - Area
  // 2 - Volume
  // 3 - Average D
  // 4 - Average E
  // 5 - Weighted Stress Tensor Force
  // 6 - Weighted Stress Tensor Torque

  CatchNullDocument();
  int type;
  type = (int)lua_todouble(L, 1);

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CComplex z;

  int i;
  BOOL flg = FALSE;
  for (i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      flg = TRUE;
  if (type >= 5)
    for (i = 0; i < thisDoc->meshnode.GetSize(); i++)
      if (thisDoc->meshnode[i].IsSelected == TRUE)
        flg = TRUE;
  if (flg == FALSE) {
    CString msg = "Cannot integrate\nNo area has been selected";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }
  if (type >= 5)
    thisDoc->MakeMask();
  z = thisDoc->BlockIntegral(type);

  lua_pushnumber(L, z.re);
  lua_pushnumber(L, z.im);

  return 2;
}

int CbelaviewDoc::lua_zoomin(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  RECT r;
  double x, y;

  theView->GetClientRect(&r);
  x = r.right;
  y = r.bottom;

  theView->ox = theView->ox + 0.25 * x / theView->mag;
  theView->oy = theView->oy + 0.25 * y / theView->mag;
  theView->mag *= 2.;

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_zoom(lua_State* L)
{
  CatchNullDocument();
  double x[2], y[2], m[2];

  x[0] = lua_todouble(L, 1);
  y[0] = lua_todouble(L, 2);
  x[1] = lua_todouble(L, 3);
  y[1] = lua_todouble(L, 4);

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CRect r;

  theView->ox = x[0];
  theView->oy = y[0];
  theView->GetClientRect(&r);
  m[0] = ((double)(r.right - 1)) / (x[1] - x[0]);
  m[1] = ((double)(r.bottom - 1)) / (y[1] - y[0]);

  if (m[0] < m[1])
    theView->mag = m[0];
  else
    theView->mag = m[1];

  theView->Invalidate(TRUE);
  return 0;
}

int CbelaviewDoc::lua_zoomnatural(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CbelaviewDoc* pDoc = theView->GetDocument();
  double x[2], y[2], m[2], w;
  RECT r;
  int i;

  if (pDoc->meshnode.GetSize() < 2)
    return 0;
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
      theView->GridSize = pow(10., floor(log(w) / log(10.) - 1.));
  }

  theView->ox = x[0];
  theView->oy = y[0];
  theView->GetClientRect(&r);
  m[0] = ((double)(r.right - 1)) / (x[1] - x[0]);
  m[1] = ((double)(r.bottom - 1)) / (y[1] - y[0]);

  if (m[0] < m[1])
    theView->mag = m[0];
  else
    theView->mag = m[1];

  if (pDoc->FirstDraw == TRUE)
    pDoc->FirstDraw = FALSE;
  else
    theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_zoomout(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  RECT r;
  double x, y;

  theView->GetClientRect(&r);
  x = r.right;
  y = r.bottom;
  theView->ox = theView->ox - 0.5 * x / theView->mag;
  theView->oy = theView->oy - 0.5 * y / theView->mag;
  theView->mag /= 2.;

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_showmesh(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_BV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->MeshFlag = TRUE;
  MMnu->CheckMenuItem(ID_SHOW_MESH, MF_CHECKED);
  tc->PressButton(ID_SHOW_MESH, TRUE);

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_hidemesh(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_BV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->MeshFlag = FALSE;
  MMnu->CheckMenuItem(ID_SHOW_MESH, MF_UNCHECKED);
  tc->PressButton(ID_SHOW_MESH, FALSE);

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_hidegrid(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->GridFlag = FALSE;
  MMnu->CheckMenuItem(ID_SHOW_GRID, MF_UNCHECKED);
  tc->PressButton(ID_SHOW_GRID, FALSE);
  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_showgrid(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->GridFlag = TRUE;
  MMnu->CheckMenuItem(ID_SHOW_GRID, MF_CHECKED);
  tc->PressButton(ID_SHOW_GRID, TRUE);
  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_showdensity(lua_State* L)
{
  CatchNullDocument();
  BOOL showlegend;
  BOOL gscale;
  int PlotType, n;
  double m_ub1, m_lb1;
  CString type;

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  n = lua_gettop(L);
  if (n < 3)
    return 0;

  showlegend = (int)lua_todouble(L, 1);
  gscale = (int)lua_todouble(L, 2);
  PlotType = (int)lua_todouble(L, 3);

  if (n >= 5) {
    m_ub1 = lua_todouble(L, 4);
    m_lb1 = lua_todouble(L, 5);
  }

  if (m_lb1 > m_ub1) {
    double temp = m_lb1;
    m_lb1 = m_ub1;
    m_ub1 = temp;
  }

  if ((PlotType > 2) || (PlotType < 0))
    PlotType = 0;

  if (showlegend == -1) {
    // load back the default plot range
    m_lb1 = thisDoc->d_PlotBounds[PlotType][0];
    m_ub1 = thisDoc->d_PlotBounds[PlotType][1];
    showlegend = 1;
    gscale = 0;
    n = 5;
  }

  theView->DensityPlot = PlotType + 1;
  theView->LegendFlag = showlegend;
  theView->GreyContours = gscale;
  if (n >= 5) {
    thisDoc->PlotBounds[PlotType][1] = m_ub1;
    thisDoc->PlotBounds[PlotType][0] = m_lb1;
  }
  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_hidedensity(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  theView->DensityPlot = 0;

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_showcountour(lua_State* L)
{
  CatchNullDocument();
  int m_numcontours;
  double m_alow, m_ahigh;
  CString type;
  m_numcontours = (int)lua_todouble(L, 1);
  m_alow = lua_todouble(L, 2);
  m_ahigh = lua_todouble(L, 3);

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  theView->ShowAr = true;
  thisDoc->A_Low = m_alow;
  thisDoc->A_High = m_ahigh;
  theView->NumContours = m_numcontours;
  theView->RedrawView();

  if (m_numcontours == -1) {
    thisDoc->A_Low = thisDoc->A_lb;
    thisDoc->A_High = thisDoc->A_ub;
    theView->NumContours = 19;
  }

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_hidecountour(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  theView->ShowAr = FALSE;

  theView->RedrawView();
  return 0;
}

int CbelaviewDoc::lua_smoothing(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CString temp;
  temp.Format("%s", lua_tostring(L, 1));
  temp.MakeLower();

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  if (temp == "off") {
    thisDoc->Smooth = FALSE;
    MMnu->CheckMenuItem(ID_SMOOTH, MF_UNCHECKED);
  }
  if (temp == "on") {
    thisDoc->Smooth = TRUE;
    MMnu->CheckMenuItem(ID_SMOOTH, MF_CHECKED);
  }

  if (temp != "on" && temp != "off") {
    CString msg = "Unknown option for smoothing";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_showpoints(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CFrameWnd* MFrm;
  MFrm = theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  theView->PtsFlag = TRUE;
  MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_CHECKED);

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_hidepoints(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CFrameWnd* MFrm;
  MFrm = theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  theView->PtsFlag = FALSE;
  MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_UNCHECKED);

  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_gridsnap(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CString temp;
  temp.Format("%s", lua_tostring(L, 1));
  temp.MakeLower();

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_leftbar;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  if (temp == "off") {
    theView->SnapFlag = FALSE;
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_UNCHECKED);
    tc->PressButton(ID_SNAP_GRID, FALSE);
  } else {
    theView->SnapFlag = TRUE;
    MMnu->CheckMenuItem(ID_SNAP_GRID, MF_CHECKED);
    tc->PressButton(ID_SNAP_GRID, TRUE);
  }

  if (temp != "on" && temp != "off") {
    CString msg = "Unknown option for grid snap";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  return 0;
}

int CbelaviewDoc::lua_setgrid(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CString temp;
  int coords = 0;
  double gridsize;

  gridsize = lua_todouble(L, 1);
  temp.Format("%s", lua_tostring(L, 2));
  temp.MakeLower();

  if (temp == "cart")
    coords = 0;
  if (temp == "polar")
    coords = 1;

  if (temp != "cart" && temp != "polar") {
    CString msg = "Unknown option for set grid";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  theView->GridSize = gridsize;
  thisDoc->Coords = coords;
  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_getprobleminfo(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  lua_pushnumber(L, thisDoc->ProblemType);
  lua_pushnumber(L, thisDoc->Depth);
  lua_pushnumber(L, thisDoc->LengthConv[thisDoc->LengthUnits]);

  return 3;
}

int CbelaviewDoc::lua_savebitmap(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CString filename;

  filename.Format("%s", lua_tostring(L, 1));

  RECT r;
  CDC tempDC;
  CBitmap bitmap;
  CDC* pDC = theView->GetDC();

  theView->GetClientRect(&r);

  tempDC.CreateCompatibleDC(pDC);
  bitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
  tempDC.SelectObject(&bitmap);
  tempDC.Rectangle(0, 0, r.right, r.bottom);

  theView->OnDraw(&tempDC);
  //    tempDC.BitBlt(0, 0, r.right, r.bottom, pDC, 0, 0, SRCCOPY);

  PBITMAPINFO pbmi;

  pbmi = thisDoc->CreateBitmapInfoStruct(theView->m_hWnd, HBITMAP(bitmap));
  thisDoc->CreateBMPFile(theView->m_hWnd, filename.GetBuffer(1), pbmi, HBITMAP(bitmap), tempDC.m_hDC);

  // to save a bitmap file we need
  // a bitmapheader lets use the v5

  //        OpenClipboard();
  // EmptyClipboard();
  //  SetClipboardData(CF_BITMAP, HBITMAP(bitmap));
  //  CloseClipboard();
  return 0;
}

PBITMAPINFO CbelaviewDoc::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
  BITMAP bmp;
  PBITMAPINFO pbmi;
  WORD cClrBits;

  // Retrieve the bitmap's color format, width, and height.
  if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) { // errhandler("GetObject", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy9");
  }
  // Convert the color format to a count of bits.
  cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
  if (cClrBits == 1)
    cClrBits = 1;
  else if (cClrBits <= 4)
    cClrBits = 4;
  else if (cClrBits <= 8)
    cClrBits = 8;
  else if (cClrBits <= 16)
    cClrBits = 16;
  else if (cClrBits <= 24)
    cClrBits = 24;
  else
    cClrBits = 32;

  // Allocate memory for the BITMAPINFO structure. (This structure
  // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
  // data structures.)

  if (cClrBits >= 24)
    pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
        sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * ((int)(1 << cClrBits)));

  // There is no RGBQUAD array for the 24-bit-per-pixel format.

  else
    pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
        sizeof(BITMAPINFOHEADER));

  // Initialize the fields in the BITMAPINFO structure.

  pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  pbmi->bmiHeader.biWidth = bmp.bmWidth;
  pbmi->bmiHeader.biHeight = bmp.bmHeight;
  pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
  pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
  if (cClrBits < 24)
    pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

  // If the bitmap is not compressed, set the BI_RGB flag.
  pbmi->bmiHeader.biCompression = BI_RGB;

  // Compute the number of bytes in the array of color
  // indices and store the result in biSizeImage.
  // For Windows NT/2000, the width must be DWORD aligned unless
  // the bitmap is RLE compressed. This example shows this.
  // For Windows 95/98, the width must be WORD aligned unless the
  // bitmap is RLE compressed.
  pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
      * pbmi->bmiHeader.biHeight;
  // Set biClrImportant to 0, indicating that all of the
  // device colors are important.
  pbmi->bmiHeader.biClrImportant = 0;
  return pbmi;
}

void CbelaviewDoc::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
    HBITMAP hBMP, HDC hDC)
{
  HANDLE hf; // file handle
  BITMAPFILEHEADER hdr; // bitmap file-header
  PBITMAPINFOHEADER pbih; // bitmap info-header
  LPBYTE lpBits; // memory pointer
  DWORD dwTotal; // total count of bytes
  DWORD cb; // incremental count of bytes
  BYTE* hp; // byte pointer
  DWORD dwTmp;

  pbih = (PBITMAPINFOHEADER)pbi;
  lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

  if (!lpBits)
    MsgBox("Critical error on getting bmp info, possible page fault ahoy10");
  //              errhandler("GlobalAlloc", hwnd);

  // Retrieve the color table (RGBQUAD array) and the bits
  // (array of palette indices) from the DIB.
  if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
          DIB_RGB_COLORS)) {
    //    errhandler("GetDIBits", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy11");
  }

  // Create the .BMP file.
  hf = CreateFile(pszFile,
      GENERIC_READ | GENERIC_WRITE,
      (DWORD)0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      (HANDLE)NULL);
  if (hf == INVALID_HANDLE_VALUE) { //    errhandler("CreateFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy12");
  }
  hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
  // Compute the size of the entire file.
  hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
  hdr.bfReserved1 = 0;
  hdr.bfReserved2 = 0;

  // Compute the offset to the array of color indices.
  hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

  // Copy the BITMAPFILEHEADER into the .BMP file.
  if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
          (LPDWORD)&dwTmp, NULL)) {
    MsgBox("Critical error on getting bmp info, possible page fault ahoy13");

    //      errhandler("WriteFile", hwnd);
  }

  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
  if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD),
          (LPDWORD)&dwTmp, (NULL)))
    //        errhandler("WriteFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy14");

  // Copy the array of color indices into the .BMP file.
  dwTotal = cb = pbih->biSizeImage;
  hp = lpBits;
  if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
    //           errhandler("WriteFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy15");

  // Close the .BMP file.
  if (!CloseHandle(hf))
    //           errhandler("CloseHandle", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy16");

  // Free memory.
  GlobalFree((HGLOBAL)lpBits);
}

int CbelaviewDoc::lua_getcircuitprops(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* TheDoc; // note normally thisdoc
  CbelaviewView* theView;
  TheDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = TheDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)TheDoc->GetNextView(pos);

  int NumCircuits, i, k;
  CString circuitname;
  circuitname = lua_tostring(L, 1);
  k = -1;

  // ok we need to find the correct entry for the circuit name
  NumCircuits = (int)TheDoc->circproplist.GetSize();
  for (i = 0; i < NumCircuits; i++) {
    if (TheDoc->circproplist[i].CircName == circuitname) {
      k = i;
      i = NumCircuits; // that will break it
    }
  }

  // trap errors

  if (k == -1) {
    CString msg = "Unknown conductor";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  lua_pushnumber(L, TheDoc->circproplist[k].V);
  lua_pushnumber(L, TheDoc->circproplist[k].q);
  return 2;
}

int CbelaviewDoc::lua_saveWMF(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  CString filename;

  filename.Format("%s", lua_tostring(L, 1));

  RECT r;
  CDC tempDC;
  CDC* pDC = theView->GetDC();

  CMetaFileDC Meta;
  CRgn R;

  Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
  theView->GetClientRect(&r);
  R.CreateRectRgnIndirect(&r);
  Meta.SelectClipRgn(&R);
  theView->OnDraw(&Meta);
  HENHMETAFILE hMeta = Meta.CloseEnhanced();

  DeleteEnhMetaFile(CopyEnhMetaFile(hMeta, filename));
  DeleteEnhMetaFile(hMeta);

  return 0;
}

int CbelaviewDoc::lua_refreshview(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  CDC* pDC = theView->GetDC();

  // erase background
  CBrush backBrush(theView->BackColor);
  CBrush* pOldBrush = pDC->SelectObject(&backBrush);
  CRect rect;
  pDC->GetClipBox(&rect);
  pDC->PatBlt(rect.left, rect.top, rect.Width(),
      rect.Height(), PATCOPY);
  pDC->SelectObject(pOldBrush);

  // redraw the view
  theView->OnDraw(pDC);

  return 0;
}

int CbelaviewDoc::lua_selectline(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* pDoc; // note normally thisdoc
  CbelaviewView* theView;
  pDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = pDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)pDoc->GetNextView(pos);

  theView->EditAction = 1; // make sure things update OK

  double mx, my;
  int i, j, k, m;

  mx = lua_todouble(L, 1);
  my = lua_todouble(L, 2);

  //***************
  {
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
          return 0;

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
        theView->DrawUserContour(FALSE);
        return 0;
      }

      if ((lineno < 0) && (arcno < 0)) {
        pDoc->contour.Add(z);
        theView->DrawUserContour(FALSE);
      }

      if (lineno >= 0) {
        j = (int)pDoc->contour.GetSize();
        if (j > 1) {
          if (abs(pDoc->contour[j - 2] - z) < 1.e-08) {
            // CView::OnLButtonDown(nFlags, point);
            return 0;
          }
        }
        pDoc->contour.Add(z);
        theView->DrawUserContour(FALSE);
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
              // CView::OnLButtonDown(nFlags, point);
              return 0;
            }
          }
          pDoc->contour.Add(y);
          theView->DrawUserContour(FALSE);
        }
      }
    }
  }

  //*************
  return 0;
}

int CbelaviewDoc::lua_seteditmode(lua_State* L)
{
  CatchNullDocument();
  CString EditAction;

  EditAction.Format("%s", lua_tostring(L, 1));
  EditAction.MakeLower();

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  if (EditAction == "point") {
    if (theView->EditAction == 1) {
      theView->EraseUserContour(TRUE);
      thisDoc->contour.RemoveAll();
    }
    if (theView->EditAction == 2) {
      int i;
      BOOL flg = FALSE;
      thisDoc->bHasMask = FALSE;
      for (i = 0; i < thisDoc->blocklist.GetSize(); i++) {
        if (thisDoc->blocklist[i].IsSelected == TRUE) {
          thisDoc->blocklist[i].IsSelected = FALSE;
          flg = TRUE;
        }
      }
      if (flg == TRUE)
        theView->RedrawView();
    }
    theView->EditAction = 0;
  }
  if (EditAction == "contour") {
    if (theView->EditAction == 2) {
      int i;
      BOOL flg = FALSE;
      thisDoc->bHasMask = FALSE;
      for (i = 0; i < thisDoc->blocklist.GetSize(); i++) {
        if (thisDoc->blocklist[i].IsSelected == TRUE) {
          thisDoc->blocklist[i].IsSelected = FALSE;
          flg = TRUE;
        }
      }

      if (flg == TRUE)
        theView->RedrawView();
    }
    theView->EditAction = 1;
  }
  if (EditAction == "area") {
    if (theView->EditAction == 1) {
      theView->EraseUserContour(TRUE);
      thisDoc->contour.RemoveAll();
    }
    theView->EditAction = 2;
  }

  return 0;
}

int CbelaviewDoc::lua_bendcontour(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  thisDoc->BendContour(lua_todouble(L, 1), lua_todouble(L, 2));
  theView->InvalidateRect(NULL);

  return 0;
}

int CbelaviewDoc::lua_makeplot(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* pDoc;
  CbelaviewView* pView;
  pDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = pDoc->GetFirstViewPosition();
  pView = (CbelaviewView*)pDoc->GetNextView(pos);

  int PlotType, npoints, FileFormat, n;
  CString ToFile;

  //	if ((pView->EditAction!=1) || (pDoc->contour.GetSize()==0)){
  if (pDoc->contour.GetSize() == 0) {
    CString outmsg;
    outmsg.Format("*** Cannot create a plot;\r\n*** No contour has been defined\r\n");
    LuaConsole->ToOutput(outmsg);
    return TRUE;
  }

  n = lua_gettop(L);

  if (n > 0)
    PlotType = (int)lua_todouble(L, 1);
  else
    return FALSE;

  if (n > 1)
    npoints = (int)lua_todouble(L, 2);
  else
    npoints = pView->d_PlotPoints;

  CXYPlot xyplot;
  pDoc->GetLineValues(xyplot, PlotType, npoints);

  if (n < 3) {
    CMetaFileDC Meta;
    Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
    HGLOBAL BoundingBox = GlobalAlloc(0, 256);
    xyplot.MakePlot(&Meta, (char*)BoundingBox);
    HENHMETAFILE hMeta = Meta.CloseEnhanced();
    if (hMeta == NULL)
      MsgBox("No Handle...");
    if (pView->OpenClipboard() == FALSE)
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
  } else {
    ToFile.Format("%s", lua_tostring(L, 3));

    if (n > 3) {
      FileFormat = (int)lua_todouble(L, 4);
      if (xyplot.ToDisk(FileFormat, ToFile) == FALSE)
        LuaConsole->ToOutput("*** Couldn't write data to disk\r\n");
    } else {
      CMetaFileDC Meta;
      Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
      HGLOBAL BoundingBox = GlobalAlloc(0, 256);
      xyplot.MakePlot(&Meta, (char*)BoundingBox);
      HENHMETAFILE hMeta = Meta.CloseEnhanced();
      if (hMeta == NULL)
        MsgBox("No Handle...");
      DeleteEnhMetaFile(CopyEnhMetaFile(hMeta, ToFile));
      DeleteEnhMetaFile(hMeta);
    }
  }

  return TRUE;
}

int CbelaviewDoc::lua_selectconductor(lua_State* L)
{
  CatchNullDocument();
  int n = lua_gettop(L);
  if (n == 0)
    return 0;

  int j, i;
  CString name;
  name = lua_tostring(L, 1);

  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  for (i = 0, j = -1; i < thisDoc->circproplist.GetSize(); i++) {

    if (name == thisDoc->circproplist[i].CircName) {
      j = i;
      break;
    }
  }

  if (j < 0)
    return 0;

  // now, the group associated with the nearest entity is in j;
  // toggle the select of all objects in this group;
  for (i = 0; i < thisDoc->nodelist.GetSize(); i++)
    if (thisDoc->nodelist[i].InConductor == j)
      thisDoc->nodelist[i].ToggleSelect();
  for (i = 0; i < thisDoc->linelist.GetSize(); i++)
    if (thisDoc->linelist[i].InConductor == j)
      thisDoc->linelist[i].ToggleSelect();
  for (i = 0; i < thisDoc->arclist.GetSize(); i++)
    if (thisDoc->arclist[i].InConductor == j)
      thisDoc->arclist[i].ToggleSelect();
  for (i = 0; i < thisDoc->meshnode.GetSize(); i++) {
    if (thisDoc->meshnode[i].Q == j) {
      thisDoc->meshnode[i].IsSelected = 1 - thisDoc->meshnode[i].IsSelected;
    }
  }
  theView->RedrawView();

  return 0;
}

int CbelaviewDoc::lua_shownames(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  int flg = (int)lua_todouble(L, 1);
  if (flg != 0)
    flg = 1;
  theView->ShowNames = flg;
  theView->InvalidateRect(NULL);

  return 0;
}

int CbelaviewDoc::lua_vectorplot(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);

  int n = lua_gettop(L);
  if (n > 0)
    theView->VectorPlot = (int)lua_todouble(L, 1);
  if (n > 1)
    theView->VectorScaleFactor = lua_todouble(L, 2);

  theView->InvalidateRect(NULL);

  return 0;
}

int CbelaviewDoc::luaResize(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();
  int n = lua_gettop(L);
  if (n != 2)
    return 0;

  int nWidth = (int)lua_todouble(L, 1);
  int nHeight = (int)lua_todouble(L, 2);
  pChildFrm->ResizeClient(nWidth, nHeight, TRUE);

  return 0;
}

int CbelaviewDoc::luaMinimize(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_MINIMIZE);

  return 0;
}

int CbelaviewDoc::luaMaximize(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_MAXIMIZE);

  return 0;
}

int CbelaviewDoc::luaRestore(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  CbelaviewView* theView;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CbelaviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_RESTORE);

  return 0;
}

int CbelaviewDoc::lua_gettitle(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;

  lua_pushstring(L, thisDoc->GetTitle());

  return 1;
}

///////////////////////////////////////////////////////
// Functions that provide low-level access to the mesh
///////////////////////////////////////////////////////

int CbelaviewDoc::lua_numnodes(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;

  lua_pushnumber(L, (int)thisDoc->meshnode.GetSize());

  return 1;
}

int CbelaviewDoc::lua_numelements(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;

  lua_pushnumber(L, (int)thisDoc->meshelem.GetSize());

  return 1;
}

int CbelaviewDoc::lua_getnode(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;

  int k = (int)lua_todouble(L, 1);
  k--;
  if ((k < 0) || (k >= thisDoc->meshnode.GetSize()))
    return 0;

  lua_pushnumber(L, thisDoc->meshnode[k].x);
  lua_pushnumber(L, thisDoc->meshnode[k].y);

  return 2;
}

int CbelaviewDoc::lua_getelement(lua_State* L)
{
  CatchNullDocument();
  CbelaviewDoc* thisDoc;
  thisDoc = (CbelaviewDoc*)pBelaviewDoc;

  int k = (int)lua_todouble(L, 1);
  k--;
  if ((k < 0) || (k >= thisDoc->meshelem.GetSize()))
    return 0;

  lua_pushnumber(L, thisDoc->meshelem[k].p[0] + 1);
  lua_pushnumber(L, thisDoc->meshelem[k].p[1] + 1);
  lua_pushnumber(L, thisDoc->meshelem[k].p[2] + 1);
  lua_pushnumber(L, Re(thisDoc->meshelem[k].ctr));
  lua_pushnumber(L, Im(thisDoc->meshelem[k].ctr));
  lua_pushnumber(L, thisDoc->ElmArea(k));
  lua_pushnumber(L, thisDoc->blocklist[thisDoc->meshelem[k].lbl].InGroup);

  return 7;
}
