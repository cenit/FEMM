#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "problem.h"
#include "femm.h"
#include "xyplot.h"
#include "femmviewDoc.h"
#include "femmviewview.h"
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
extern void* pFemmviewdoc;
extern CLuaConsoleDlg* LuaConsole;
extern BOOL lua_byebye;
extern int m_luaWindowStatus;

#define CatchNullDocument()                       \
  ;                                               \
  if (pFemmviewdoc == NULL) {                     \
    CString msg = "No magnetics output in focus"; \
    lua_error(L, msg.GetBuffer(1));               \
    return 0;                                     \
  }

//-------------------Lua Extensions-------------------------

void CFemmviewDoc::initalise_lua()
{
  // use a messagebox to display errors
  lua_register(lua, "mo_setfocus", lua_switchfocus);
  lua_register(lua, "mo_getpointvalues", lua_getpointvals);
  lua_register(lua, "mo_close", lua_exitpost);
  lua_register(lua, "mo_addcontour", lua_addcontour);
  lua_register(lua, "mo_clearcontour", lua_clearcontour);
  lua_register(lua, "mo_lineintegral", lua_lineintegral);
  lua_register(lua, "mo_selectblock", lua_selectblock);
  lua_register(lua, "mo_groupselectblock", lua_groupselectblock);
  lua_register(lua, "mo_clearblock", lua_clearblock);
  lua_register(lua, "mo_blockintegral", lua_blockintegral);
  lua_register(lua, "mo_zoomnatural", lua_zoomnatural);
  lua_register(lua, "mo_zoomin", lua_zoomin);
  lua_register(lua, "mo_zoomout", lua_zoomout);
  lua_register(lua, "mo_zoom", lua_zoom);
  lua_register(lua, "mo_showgrid", lua_showgrid);
  lua_register(lua, "mo_hidegrid", lua_hidegrid);
  lua_register(lua, "mo_gridsnap", lua_gridsnap);
  lua_register(lua, "mo_setgrid", lua_setgrid);
  lua_register(lua, "mo_showmesh", lua_showmesh);
  lua_register(lua, "mo_hidemesh", lua_hidemesh);
  lua_register(lua, "mo_hidedensityplot", lua_hidedensity);
  lua_register(lua, "mo_showdensityplot", lua_showdensity);
  lua_register(lua, "mo_hidecontourplot", lua_hidecountour);
  lua_register(lua, "mo_showcontourplot", lua_showcountour);
  lua_register(lua, "mo_showvectorplot", lua_vectorplot);
  lua_register(lua, "mo_smooth", lua_smoothing);
  lua_register(lua, "mo_showpoints", lua_showpoints);
  lua_register(lua, "mo_hidepoints", lua_hidepoints);
  lua_register(lua, "mo_shownames", lua_shownames);
  lua_register(lua, "mo_getprobleminfo", lua_getprobleminfo);
  lua_register(lua, "mo_savebitmap", lua_savebitmap);
  lua_register(lua, "mo_getcircuitproperties", lua_getcircuitprops);
  lua_register(lua, "mo_savemetafile", lua_saveWMF);
  lua_register(lua, "mo_refreshview", lua_refreshview);
  lua_register(lua, "mo_selectpoint", lua_selectline);
  lua_register(lua, "mo_seteditmode", lua_seteditmode);
  lua_register(lua, "mo_bendcontour", lua_bendcontour);
  lua_register(lua, "mo_makeplot", lua_makeplot);
  lua_register(lua, "mo_reload", lua_reload);
  lua_register(lua, "mo_gradient", lua_gradient);
  lua_register(lua, "mo_gettitle", lua_gettitle);
  lua_register(lua, "mo_get_title", lua_gettitle);
  lua_register(lua, "mo_set_weighting_scheme", lua_weighting);
  lua_register(lua, "mo_setweightingscheme", lua_weighting);

  lua_register(lua, "mo_resize", luaResize);
  lua_register(lua, "mo_minimize", luaMinimize);
  lua_register(lua, "mo_maximize", luaMaximize);
  lua_register(lua, "mo_restore", luaRestore);

  // compatibility with alternate lua function naming convention
  lua_register(lua, "mo_set_focus", lua_switchfocus);
  lua_register(lua, "mo_get_point_values", lua_getpointvals);
  lua_register(lua, "mo_add_contour", lua_addcontour);
  lua_register(lua, "mo_clear_contour", lua_clearcontour);
  lua_register(lua, "mo_line_integral", lua_lineintegral);
  lua_register(lua, "mo_select_block", lua_selectblock);
  lua_register(lua, "mo_group_select_block", lua_groupselectblock);
  lua_register(lua, "mo_clear_block", lua_clearblock);
  lua_register(lua, "mo_block_integral", lua_blockintegral);
  lua_register(lua, "mo_zoom_natural", lua_zoomnatural);
  lua_register(lua, "mo_zoom_in", lua_zoomin);
  lua_register(lua, "mo_zoom_out", lua_zoomout);
  lua_register(lua, "mo_show_grid", lua_showgrid);
  lua_register(lua, "mo_hide_grid", lua_hidegrid);
  lua_register(lua, "mo_show_mesh", lua_showmesh);
  lua_register(lua, "mo_hide_mesh", lua_hidemesh);
  lua_register(lua, "mo_hide_density_plot", lua_hidedensity);
  lua_register(lua, "mo_show_density_plot", lua_showdensity);
  lua_register(lua, "mo_hide_contour_plot", lua_hidecountour);
  lua_register(lua, "mo_show_contour_plot", lua_showcountour);
  lua_register(lua, "mo_show_vector_plot", lua_vectorplot);
  lua_register(lua, "mo_show_points", lua_showpoints);
  lua_register(lua, "mo_hide_points", lua_hidepoints);
  lua_register(lua, "mo_grid_snap", lua_gridsnap);
  lua_register(lua, "mo_set_grid", lua_setgrid);
  lua_register(lua, "mo_get_problem_info", lua_getprobleminfo);
  lua_register(lua, "mo_save_bitmap", lua_savebitmap);
  lua_register(lua, "mo_get_circuit_properties", lua_getcircuitprops);
  lua_register(lua, "mo_save_metafile", lua_saveWMF);
  lua_register(lua, "mo_refresh_view", lua_refreshview);
  lua_register(lua, "mo_select_point", lua_selectline);
  lua_register(lua, "mo_set_edit_mode", lua_seteditmode);
  lua_register(lua, "mo_bend_contour", lua_bendcontour);
  lua_register(lua, "mo_make_plot", lua_makeplot);
  lua_register(lua, "mo_show_names", lua_shownames);

  // functions to access low-level mesh info
  lua_register(lua, "mo_numnodes", lua_numnodes);
  lua_register(lua, "mo_numelements", lua_numelements);
  lua_register(lua, "mo_getnode", lua_getnode);
  lua_register(lua, "mo_getelement", lua_getelement);

  lua_register(lua, "mo_num_nodes", lua_numnodes);
  lua_register(lua, "mo_num_elements", lua_numelements);
  lua_register(lua, "mo_get_node", lua_getnode);
  lua_register(lua, "mo_get_element", lua_getelement);

  lua_register(lua, "mo_get_gap_b", lua_getAGEflux);
  lua_register(lua, "mo_getgapb", lua_getAGEflux);
  lua_register(lua, "mo_gap_integral", lua_gapintegral);
  lua_register(lua, "mo_gapintegral", lua_gapintegral);
  lua_register(lua, "mo_get_gap_a", lua_getgapa);
  lua_register(lua, "mo_getgapa", lua_getgapa);
  lua_register(lua, "mo_getgapharmonics", lua_getgapharmonics);
  lua_register(lua, "mo_get_gap_harmonics", lua_getgapharmonics);

  pFemmviewdoc = this;
}

int CFemmviewDoc::lua_switchfocus(lua_State* L)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  CFemmviewDoc* pDoc;
  CString DocTitle, ThisTitle;
  int n;

  // get title of desired document from Lua
  n = lua_gettop(L);
  if (n == 0)
    return NULL;
  DocTitle.Format("%s", lua_tostring(L, n));
  DocTitle.MakeLower();
  if (DocTitle.Right(4) == ".ans")
    DocTitle = DocTitle.Left(DocTitle.GetLength() - 4);

  // get pointer to document template for FemmeDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 5; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (CFemmviewDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetTitle();
    ThisTitle.MakeLower();
    if (ThisTitle.Right(4) == ".ans")
      ThisTitle = ThisTitle.Left(ThisTitle.GetLength() - 4);
    if (ThisTitle == DocTitle) {
      pFemmviewdoc = pDoc;
      return NULL;
    }
  }

  // if we can't find it, throw an error message
  CString msg;
  msg.Format("No document that matches %s", DocTitle);
  lua_error(L, msg.GetBuffer(1));
  return NULL;
}

int CFemmviewDoc::lua_dumpheader(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;

  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  return 0;
}

int CFemmviewDoc::lua_getpointvals(lua_State* L)
{
  // for compatibility with 4.0 and 4.1 Lua implementation
  if (((CFemmApp*)AfxGetApp())->CompatibilityMode != 0)
    return ((CFemmviewDoc*)pFemmviewdoc)->old_lua_getpointvals(L);

  CatchNullDocument();
  double px, py;

  px = lua_todouble(L, 1);
  py = lua_todouble(L, 2);

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;

  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();

  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CPointVals u;
  CComplex Jtot;

  if (thisDoc->GetPointValues(px, py, u) == TRUE) {
    lua_pushnumber(L, u.A);
    lua_pushnumber(L, u.B1);
    lua_pushnumber(L, u.B2);
    lua_pushnumber(L, u.c);
    lua_pushnumber(L, u.E);
    lua_pushnumber(L, u.H1);
    lua_pushnumber(L, u.H2);
    lua_pushnumber(L, u.Je);
    lua_pushnumber(L, u.Js);
    lua_pushnumber(L, u.mu1);
    lua_pushnumber(L, u.mu2);
    lua_pushnumber(L, u.Pe);
    lua_pushnumber(L, u.Ph);
    lua_pushnumber(L, u.ff);
    return 14;
  }

  return 0;
}

int CFemmviewDoc::lua_exitpost(lua_State* L)
{
  CatchNullDocument();
  ((CFemmviewDoc*)pFemmviewdoc)->OnCloseDocument();

  return 0;
}

int CFemmviewDoc::lua_reload(lua_State* L)
{
  CatchNullDocument();

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  // theView->BeginWaitCursor();
  thisDoc->OnReload();
  // theView->EndWaitCursor();
  if (theView->d_ResetOnReload == FALSE)
    thisDoc->FirstDraw = FALSE;
  theView->RedrawView();
  theView->LuaViewInfo();
  return 0;
}

void CFemmviewView::LuaViewInfo()
{
  OnViewInfo();
}

int CFemmviewDoc::lua_addcontour(lua_State* L)
{
  CatchNullDocument();
  CComplex z;
  int i;
  z.Set(lua_todouble(L, 1), lua_todouble(L, 2));

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  i = (int)thisDoc->contour.GetSize();

  if (i > 0) {
    if (z != thisDoc->contour[i - 1])
      thisDoc->contour.Add(z);
  } else
    thisDoc->contour.Add(z);
  theView->DrawUserContour(FALSE);

  return 0;
}

int CFemmviewDoc::lua_clearcontour(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  theView->EraseUserContour(TRUE);
  thisDoc->contour.RemoveAll();

  return 0;
}

int CFemmviewDoc::lua_lineintegral(lua_State* L)
{
  // for compatibility with 4.0 and 4.1 Lua implementation
  if (((CFemmApp*)AfxGetApp())->CompatibilityMode != 0)
    return ((CFemmviewDoc*)pFemmviewdoc)->old_lua_lineintegral(L);

  CatchNullDocument();
  int type;
  CComplex* z;
  z = (CComplex*)calloc(4, sizeof(CComplex));

  type = (int)lua_todouble(L, 1);
  // 0- B.n
  // 1 - H.t
  // 2 - Cont length
  // 3 - Force from stress tensor
  // 4 - Torque from stress tensor
  // 5 - (B.n)^2

  if (type < 0 || type > 5) {
    CString msg;
    msg.Format("Invalid line integral selected %d", type);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  thisDoc->LineIntegral(type, z);

  switch (type) {
  case 2: // length result
    lua_pushnumber(lua, z[0].re); // contour length
    lua_pushnumber(lua, z[0].im); // swept area
    free(z);
    return 2;

  case 3: // force results
    if (thisDoc->Frequency != 0) {
      lua_pushnumber(lua, z[2].re);
      lua_pushnumber(lua, z[3].re);
      lua_pushnumber(lua, z[0]);
      lua_pushnumber(lua, z[1]);

      free(z);
      return 4;
    } else {
      lua_pushnumber(lua, z[0].re);
      lua_pushnumber(lua, z[1].re);

      free(z);
      return 2;
    }

  case 4: // torque results
    if (thisDoc->Frequency != 0) {
      lua_pushnumber(lua, z[1].re);
      lua_pushnumber(lua, z[0]);

      free(z);
      return 2;
    } else {
      lua_pushnumber(lua, z[0].re);
      lua_pushnumber(lua, 0);

      free(z);
      return 2;
    }

  default:
    lua_pushnumber(lua, z[0]);
    lua_pushnumber(lua, z[1]);

    free(z);
    return 2;
  }

  free(z);
  return 0;
}

int CFemmviewDoc::lua_selectblock(lua_State* L)
{
  CatchNullDocument();
  double px, py;

  px = lua_todouble(L, 1);
  py = lua_todouble(L, 2);

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
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

int CFemmviewDoc::lua_groupselectblock(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
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
    /*
        for(j=0;j<thisDoc->linelist.GetSize();j++)
        {
          if ((thisDoc->linelist[j].InGroup==k) || (n==0))
          thisDoc->linelist[j].ToggleSelect();
          thisDoc->bHasMask=FALSE;
        }

        for(j=0;j<thisDoc->arclist.GetSize();j++)
        {
          if ((thisDoc->arclist[j].InGroup==k) || (n==0))
          thisDoc->arclist[j].ToggleSelect();
          thisDoc->bHasMask=FALSE;
        }
    */
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

int CFemmviewDoc::lua_clearblock(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  thisDoc->bHasMask = FALSE;
  for (int i = 0; i < thisDoc->blocklist.GetSize(); i++) {
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      thisDoc->blocklist[i].IsSelected = FALSE;
  }

  theView->EditAction = 0;

  theView->RedrawView();

  //	HDC myDCH=GetDC(theView->m_hWnd);
  //	CDC tempDC;
  //		CDC *pDC;
  //	pDC=tempDC.FromHandle(myDCH);

  // CDC *pDC=GetDC(theView->m_hWnd);
  //	theView->OnDraw(pDC);
  //	theView->DrawSelected=-1;
  //	theView->ReleaseDC(pDC);
  //	thisDoc->UpdateAllViews(theView);

  return 0;
}

int CFemmviewDoc::lua_blockintegral(lua_State* L)
{
  // for compatibility with 4.0 and 4.1 Lua implementation
  if (((CFemmApp*)AfxGetApp())->CompatibilityMode != 0)
    return ((CFemmviewDoc*)pFemmviewdoc)->old_lua_blockintegral(L);

  CatchNullDocument();
  int type;
  type = (int)lua_todouble(L, 1);

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  CatchNullDocument();

  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CComplex z;

  int i;
  BOOL flg = FALSE;
  for (i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      flg = TRUE;
  if (flg == FALSE) {
    CString msg = "Cannot integrate\nNo area has been selected";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  z = thisDoc->BlockIntegral(type);

  lua_pushnumber(L, z);

  return 1;
}

int CFemmviewDoc::lua_zoomin(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_zoom(lua_State* L)
{
  CatchNullDocument();
  double x[2], y[2], m[2];

  x[0] = lua_todouble(L, 1);
  y[0] = lua_todouble(L, 2);
  x[1] = lua_todouble(L, 3);
  y[1] = lua_todouble(L, 4);

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_zoomnatural(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CFemmviewDoc* pDoc = theView->GetDocument();
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

int CFemmviewDoc::lua_zoomout(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
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

int CFemmviewDoc::lua_showmesh(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->MeshFlag = TRUE;
  MMnu->CheckMenuItem(ID_SHOW_MESH, MF_CHECKED);
  tc->PressButton(ID_SHOW_MESH, TRUE);

  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_hidemesh(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_FV_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->MeshFlag = FALSE;
  MMnu->CheckMenuItem(ID_SHOW_MESH, MF_UNCHECKED);
  tc->PressButton(ID_SHOW_MESH, FALSE);

  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_hidegrid(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_showgrid(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_showdensity(lua_State* L)
{
  CatchNullDocument();
  BOOL showlegend;
  BOOL gscale;
  double m_ub1, m_lb1;
  CString type;
  int btnState;
  // type real,imag,mag;

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  showlegend = (int)lua_todouble(L, 1);
  gscale = (int)lua_todouble(L, 2);
  m_ub1 = lua_todouble(L, 3);
  m_lb1 = lua_todouble(L, 4);

  if (m_lb1 > m_ub1) {
    double temp = m_lb1;
    m_lb1 = m_ub1;
    m_ub1 = temp;
  }

  type.Format("%s", lua_tostring(L, 5));

  btnState = 0;

  if (thisDoc->Frequency == 0) {
    if ((type == "mag") || (type == "bmag"))
      btnState = 1;
    if ((type == "real") || (type == "breal"))
      btnState = 1;
    if ((type == "imag") || (type == "bimag"))
      btnState = 1;
    if (type == "hmag")
      btnState = 2;
    if (type == "hreal")
      btnState = 2;
    if (type == "himag")
      btnState = 2;
    if (type == "jmag")
      btnState = 3;
    if (type == "jreal")
      btnState = 3;
    if (type == "jimag")
      btnState = 3;
    if (type == "logb")
      btnState = 4;
  } else {
    if ((type == "mag") || (type == "bmag"))
      btnState = 1;
    if ((type == "real") || (type == "breal"))
      btnState = 2;
    if ((type == "imag") || (type == "bimag"))
      btnState = 3;
    if (type == "hmag")
      btnState = 4;
    if (type == "hreal")
      btnState = 5;
    if (type == "himag")
      btnState = 6;
    if (type == "jmag")
      btnState = 7;
    if (type == "jreal")
      btnState = 8;
    if (type == "jimag")
      btnState = 9;
    if (type == "logb")
      btnState = 10;
  }

  if (showlegend == -1) {
    m_lb1 = thisDoc->d_PlotBounds[btnState - 1][0];
    m_ub1 = thisDoc->d_PlotBounds[btnState - 1][1];
    showlegend = 1;
    gscale = 0;
  }

  theView->DensityPlot = btnState;
  theView->LegendFlag = showlegend;
  theView->GreyContours = gscale;
  if (btnState > 0) {
    thisDoc->PlotBounds[btnState - 1][0] = m_lb1;
    thisDoc->PlotBounds[btnState - 1][1] = m_ub1;
  }
  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_hidedensity(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  theView->DensityPlot = 0;

  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_showcountour(lua_State* L)
{
  CatchNullDocument();
  int m_numcontours;
  double m_alow, m_ahigh;
  CString type;
  m_numcontours = (int)lua_todouble(L, 1);
  m_alow = lua_todouble(L, 2);
  m_ahigh = lua_todouble(L, 3);
  type = lua_tostring(L, 4);
  type.MakeLower();

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  if (thisDoc->Frequency != 0) {
    if (type == "real") {
      theView->ShowAr = true;
      theView->ShowAi = false;
    }
    if (type == "imag") {
      theView->ShowAi = true;
      theView->ShowAr = false;
    }
    if (type == "both") {
      theView->ShowAi = theView->ShowAr = true;
    }

    thisDoc->A_Low = m_alow;
    thisDoc->A_High = m_ahigh;
    theView->NumContours = m_numcontours;
    theView->RedrawView();
  } else {
    theView->ShowAr = true;
    theView->ShowAi = FALSE;
    thisDoc->A_Low = m_alow;
    thisDoc->A_High = m_ahigh;
    theView->NumContours = m_numcontours;
    theView->RedrawView();
  }

  if (m_numcontours == -1) {
    thisDoc->A_Low = thisDoc->A_lb;
    thisDoc->A_High = thisDoc->A_ub;
    theView->NumContours = 19;
  }

  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_hidecountour(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  theView->ShowAr = FALSE;
  theView->ShowAi = FALSE;

  theView->RedrawView();
  return 0;
}

int CFemmviewDoc::lua_smoothing(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_showpoints(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CFrameWnd* MFrm;
  MFrm = theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  theView->PtsFlag = TRUE;
  MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_CHECKED);

  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_hidepoints(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CFrameWnd* MFrm;
  MFrm = theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();

  theView->PtsFlag = FALSE;
  MMnu->CheckMenuItem(ID_MENUSHOWPTS, MF_UNCHECKED);

  theView->RedrawView();

  return 0;
}

int CFemmviewDoc::lua_gridsnap(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_setgrid(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_getprobleminfo(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  lua_pushnumber(L, thisDoc->ProblemType);
  lua_pushnumber(L, thisDoc->Frequency);
  lua_pushnumber(L, thisDoc->Depth);
  lua_pushnumber(L, thisDoc->LengthConv[thisDoc->LengthUnits]);

  return 4;
}

int CFemmviewDoc::lua_savebitmap(lua_State* L)
{

  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CString filename;

  filename.Format("%s", lua_tostring(L, 1));

  RECT r;
  CDC tempDC;
  CBitmap bitmap;
  CBitmap* oldbitmap;

  CDC* pDC = theView->GetDC();

  theView->GetClientRect(&r);

  tempDC.CreateCompatibleDC(pDC);
  bitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
  oldbitmap = tempDC.SelectObject(&bitmap);
  tempDC.Rectangle(0, 0, r.right, r.bottom);

  theView->OnDraw(&tempDC);
  //    tempDC.BitBlt(0, 0, r.right, r.bottom, pDC, 0, 0, SRCCOPY);

  PBITMAPINFO pbmi;

  pbmi = thisDoc->CreateBitmapInfoStruct(theView->m_hWnd, HBITMAP(bitmap));
  thisDoc->CreateBMPFile(theView->m_hWnd, filename.GetBuffer(1), pbmi, HBITMAP(bitmap), tempDC.m_hDC);

  tempDC.SelectObject(oldbitmap);
  theView->ReleaseDC(pDC);
  tempDC.DeleteDC();
  bitmap.DeleteObject();

  // to save a bitmap file we need
  // a bitmapheader lets use the v5

  //        OpenClipboard();
  // EmptyClipboard();
  //  SetClipboardData(CF_BITMAP, HBITMAP(bitmap));
  //  CloseClipboard();
  return 0;
}

PBITMAPINFO CFemmviewDoc::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
  BITMAP bmp;
  PBITMAPINFO pbmi;
  WORD cClrBits;

  // Retrieve the bitmap's color format, width, and height.
  if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) { // errhandler("GetObject", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy25");
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

void CFemmviewDoc::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
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
    MsgBox("Critical error on getting bmp info, possible page fault ahoy26");
  //              errhandler("GlobalAlloc", hwnd);

  // Retrieve the color table (RGBQUAD array) and the bits
  // (array of palette indices) from the DIB.
  if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
          DIB_RGB_COLORS)) {
    //    errhandler("GetDIBits", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy27");
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
    MsgBox("Critical error on getting bmp info, possible page fault ahoy28");
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
    MsgBox("Critical error on getting bmp info, possible page fault ahoy29");

    //      errhandler("WriteFile", hwnd);
  }

  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
  if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD),
          (LPDWORD)&dwTmp, (NULL)))
    //        errhandler("WriteFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy30");

  // Copy the array of color indices into the .BMP file.
  dwTotal = cb = pbih->biSizeImage;
  hp = lpBits;
  if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
    //           errhandler("WriteFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy31");

  // Close the .BMP file.
  if (!CloseHandle(hf))
    //           errhandler("CloseHandle", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy32");

  // Free memory.
  GlobalFree((HGLOBAL)lpBits);
}

int CFemmviewDoc::lua_getcircuitprops(lua_State* L)
{
  // for compatibility with 4.0 and 4.1 Lua implementation
  if (((CFemmApp*)AfxGetApp())->CompatibilityMode != 0)
    return ((CFemmviewDoc*)pFemmviewdoc)->old_lua_getcircuitprops(L);

  CatchNullDocument();
  CFemmviewDoc* TheDoc; // note normally thisdoc
  CFemmviewView* theView;
  TheDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = TheDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)TheDoc->GetNextView(pos);

  int NumCircuits, k;
  CString circuitname;
  circuitname = lua_tostring(L, 1);
  k = -1;

  // ok we need to find the correct entry for the circuit name
  NumCircuits = (int)TheDoc->circproplist.GetSize();
  for (int i = 0; i < NumCircuits; i++) {
    if (TheDoc->circproplist[i].CircName == circuitname) {
      k = i;
      i = NumCircuits; // that will break it
    }
  }

  // trap errors

  if (k == -1) {
    CString msg = "Unknown circuit";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  CComplex amps, volts, fluxlinkage;

  amps = TheDoc->circproplist[k].Amps;
  volts = TheDoc->GetVoltageDrop(k);
  fluxlinkage = TheDoc->GetFluxLinkage(k);

  lua_pushnumber(L, amps);
  lua_pushnumber(L, volts);
  lua_pushnumber(L, fluxlinkage);

  return 3;
}

int CFemmviewDoc::lua_saveWMF(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
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

int CFemmviewDoc::lua_refreshview(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_selectline(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* pDoc; // note normally thisdoc
  CFemmviewView* theView;
  pDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = pDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)pDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_seteditmode(lua_State* L)
{
  CatchNullDocument();
  CString EditAction;

  EditAction.Format("%s", lua_tostring(L, 1));
  EditAction.MakeLower();

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

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

int CFemmviewDoc::lua_bendcontour(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  thisDoc->BendContour(lua_todouble(L, 1), lua_todouble(L, 2));
  theView->InvalidateRect(NULL);

  return 0;
}

int CFemmviewDoc::lua_makeplot(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* pDoc;
  CFemmviewView* pView;
  pDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = pDoc->GetFirstViewPosition();
  pView = (CFemmviewView*)pDoc->GetNextView(pos);

  int PlotType, npoints, FileFormat, n;
  CString ToFile;

  //	if ((pView->EditAction!=1) || (pDoc->contour.GetSize()==0)){
  if (pDoc->contour.GetSize() == 0) {
    CString outmsg;
    outmsg.Format("*** Cannot create a plot;\r\n*** No contour has been defined\r\n");
    LuaConsole->ToOutput(outmsg);
    return FALSE;
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

int CFemmviewDoc::lua_shownames(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  int flg = (int)lua_todouble(L, 1);
  if (flg != 0)
    flg = 1;
  theView->ShowNames = flg;
  theView->InvalidateRect(NULL);

  return 0;
}

int CFemmviewDoc::lua_vectorplot(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  int n = lua_gettop(L);
  if (n > 0)
    theView->VectorPlot = (int)lua_todouble(L, 1);
  if (n > 1)
    theView->VectorScaleFactor = lua_todouble(L, 2);

  theView->InvalidateRect(NULL);

  return 0;
}

/*
int CFemmviewDoc::lua_gradient(lua_State *L)
{
  // computes the gradients of the B field by differentiating
  // the shape functions that are used to represent the smoothed
  // B in an element.

    CatchNullDocument();
    CFemmviewDoc * thisDoc;
    CFemmviewView * theView;
    thisDoc=(CFemmviewDoc *)pFemmviewdoc;
    POSITION pos;
    pos=thisDoc->GetFirstViewPosition();
    theView=(CFemmviewView *)thisDoc->GetNextView(pos);

    double x,y;
    int i,n[3];
    double b[3],c[3],da;
    CComplex dbxdx,dbxdy,dbydx,dbydy;
    CElement elm;

    x=lua_todouble(L,1);
    y=lua_todouble(L,2);
    i=thisDoc->InTriangle(x,y);
    if(i<0){
        for(i=0;i<8;i++) lua_pushnumber(L,0);
        return 8;
    }

    elm=thisDoc->meshelem[i];
    for(i=0;i<3;i++) n[i]=elm.p[i];
    b[0]=thisDoc->meshnode[n[1]].y - thisDoc->meshnode[n[2]].y;
    b[1]=thisDoc->meshnode[n[2]].y - thisDoc->meshnode[n[0]].y;
    b[2]=thisDoc->meshnode[n[0]].y - thisDoc->meshnode[n[1]].y;
    c[0]=thisDoc->meshnode[n[2]].x - thisDoc->meshnode[n[1]].x;
    c[1]=thisDoc->meshnode[n[0]].x - thisDoc->meshnode[n[2]].x;
    c[2]=thisDoc->meshnode[n[1]].x - thisDoc->meshnode[n[0]].x;
    da=(b[0]*c[1]-b[1]*c[0])*thisDoc->LengthConv[thisDoc->LengthUnits];

  dbxdx=0; dbxdy=0; dbydx=0; dbydy=0;
    for(i=0;i<3;i++)
    {
    dbxdx+=elm.b1[i]*b[i]/da;
    dbxdy+=elm.b1[i]*c[i]/da;
    dbydx+=elm.b2[i]*b[i]/da;
    dbydy+=elm.b2[i]*c[i]/da;
  }

  if (thisDoc->ProblemType==PLANAR)
  {
    // in a source-free region, we can get a little bit
    // of smoothing by enforcing the dependencies that
    // are implied by the differential equation.
    dbxdx=(dbxdx-dbydy)/2.;
    dbydy=-dbxdx;
    dbydx=(dbydx+dbxdy)/2;
    dbxdy=dbydx;
  }

    lua_pushnumber(L,Re(dbxdx));
    lua_pushnumber(L,Im(dbxdx));
  lua_pushnumber(L,Re(dbxdy));
    lua_pushnumber(L,Im(dbxdy));
  lua_pushnumber(L,Re(dbydx));
    lua_pushnumber(L,Im(dbydx));
  lua_pushnumber(L,Re(dbydy));
    lua_pushnumber(L,Im(dbydy));

    return 8;
}
*/

int CFemmviewDoc::lua_gradient(lua_State* L)
{
  // computes the gradients of the B field by differentiating
  // the shape functions that are used to represent the smoothed
  // B in an element.

  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  int i;
  double xo, yo, x, y, da, p0, p1, p2;
  CComplex dbxdx, dbxdy, dbydx, dbydy, Mx, My;

  xo = lua_todouble(L, 1);
  yo = lua_todouble(L, 2);

  dbxdx = 0;
  dbxdy = 0;
  dbydx = 0;
  dbydy = 0;

  for (i = 0; i < thisDoc->meshelem.GetSize(); i++) {
    thisDoc->GetMagnetization(i, Mx, My);
    da = muo * thisDoc->ElmArea(i) / thisDoc->LengthConv[thisDoc->LengthUnits];
    x = Re(thisDoc->meshelem[i].ctr);
    y = Im(thisDoc->meshelem[i].ctr);

    p0 = PI * pow(pow(x - xo, 2.) + pow(y - yo, 2.), 3.);
    p1 = (-3. * pow(x - xo, 2.) + pow(y - yo, 2.)) * (y - yo);
    p2 = (x - xo) * (pow(x - xo, 2.) - 3. * pow(y - yo, 2.));
    dbxdx += (da * (-(My * p1) + Mx * p2)) / p0;
    dbydx += (da * (-(Mx * p1) - My * p2)) / p0;
    dbxdy += (da * (-(Mx * p1) - My * p2)) / p0;
    dbydy += -(da * (-(My * p1) + Mx * p2)) / p0;
  }

  lua_pushnumber(L, Re(dbxdx));
  lua_pushnumber(L, Im(dbxdx));
  lua_pushnumber(L, Re(dbxdy));
  lua_pushnumber(L, Im(dbxdy));
  lua_pushnumber(L, Re(dbydx));
  lua_pushnumber(L, Im(dbydx));
  lua_pushnumber(L, Re(dbydy));
  lua_pushnumber(L, Im(dbydy));

  return 8;
}

int CFemmviewDoc::luaResize(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();
  int n = lua_gettop(L);
  if (n != 2)
    return 0;

  int nWidth = (int)lua_todouble(L, 1);
  int nHeight = (int)lua_todouble(L, 2);
  pChildFrm->ResizeClient(nWidth, nHeight, TRUE);

  return 0;
}

int CFemmviewDoc::luaMinimize(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_MINIMIZE);

  return 0;
}

int CFemmviewDoc::luaMaximize(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_MAXIMIZE);

  return 0;
}

int CFemmviewDoc::luaRestore(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_RESTORE);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//
// old versions for compatibility
//
////////////////////////////////////////////////////////////////////////////////////

int CFemmviewDoc::old_lua_lineintegral(lua_State* L)
{
  CatchNullDocument();
  int type;
  CComplex* z;
  z = (CComplex*)calloc(4, sizeof(CComplex));

  type = (int)lua_todouble(L, 1);
  // 0- B.n
  // 1 - H.t
  // 2 - Cont length
  // 3 - Force from stress tensor
  // 4 - Torque from stress tensor
  // 5 - (B.n)^2

  if (type < 0 || type > 5) {
    CString msg;
    msg.Format("Invalid line integral selected %d", type);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  thisDoc->LineIntegral(type, z);

  switch (type) {
  case 3: // force results
    if (thisDoc->Frequency != 0) {
      lua_pushnumber(lua, z[2].re);
      lua_pushnumber(lua, 0);
      lua_pushnumber(lua, z[3].re);
      lua_pushnumber(lua, 0);
      lua_pushnumber(lua, z[0].re);
      lua_pushnumber(lua, z[0].im);
      lua_pushnumber(lua, z[1].re);
      lua_pushnumber(lua, z[1].im);
      free(z);
      return 8;
    } else {
      lua_pushnumber(lua, z[0].re);
      lua_pushnumber(lua, 0);
      lua_pushnumber(lua, z[1].re);
      lua_pushnumber(lua, 0);
      free(z);
      return 4;
    }

  case 4: // torque results
    if (thisDoc->Frequency != 0) {
      lua_pushnumber(lua, z[1].re);
      lua_pushnumber(lua, 0);
      lua_pushnumber(lua, z[0].re);
      lua_pushnumber(lua, z[0].im);
      free(z);
      return 4;
    } else {
      lua_pushnumber(lua, z[0].re);
      lua_pushnumber(lua, 0);
      lua_pushnumber(lua, 0);
      lua_pushnumber(lua, 0);

      free(z);
      return 4;
    }

  default:
    lua_pushnumber(lua, z[0].re);
    lua_pushnumber(lua, z[0].im);

    lua_pushnumber(lua, z[1].re);
    lua_pushnumber(lua, z[1].im);

    free(z);
    return 4;
  }

  free(z);
  return 0;
}

int CFemmviewDoc::old_lua_getpointvals(lua_State* L)
{
  CatchNullDocument();
  double px, py;

  px = lua_todouble(L, 1);
  py = lua_todouble(L, 2);

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;

  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();

  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CPointVals u;
  CComplex Jtot;

  if (thisDoc->GetPointValues(px, py, u) == TRUE) {
    lua_pushnumber(L, u.A.re);
    lua_pushnumber(L, u.A.im);
    lua_pushnumber(L, u.B1.re);
    lua_pushnumber(L, u.B1.im);
    lua_pushnumber(L, u.B2.re);
    lua_pushnumber(L, u.B2.im);
    lua_pushnumber(L, u.c);
    lua_pushnumber(L, u.E);
    lua_pushnumber(L, u.H1.re);
    lua_pushnumber(L, u.H1.im);
    lua_pushnumber(L, u.H2.re);
    lua_pushnumber(L, u.H2.im);
    lua_pushnumber(L, u.Je.re);
    lua_pushnumber(L, u.Je.im);
    lua_pushnumber(L, u.Js.re);
    lua_pushnumber(L, u.Js.im);
    lua_pushnumber(L, u.mu1.re);
    lua_pushnumber(L, u.mu1.im);
    lua_pushnumber(L, u.mu2.re);
    lua_pushnumber(L, u.mu2.im);
    lua_pushnumber(L, u.Pe);
    lua_pushnumber(L, u.Ph);
    lua_pushnumber(L, u.ff);
    return 23;
  }

  return 0;
}

int CFemmviewDoc::old_lua_blockintegral(lua_State* L)
{
  CatchNullDocument();
  int type;
  type = (int)lua_todouble(L, 1);

  CFemmviewDoc* thisDoc;
  CFemmviewView* theView;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;
  CatchNullDocument();

  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)thisDoc->GetNextView(pos);

  CComplex z;

  int i;
  BOOL flg = FALSE;
  for (i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      flg = TRUE;
  if (flg == FALSE) {
    CString msg = "Cannot integrate\nNo area has been selected";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }
  z = thisDoc->BlockIntegral(type);

  lua_pushnumber(L, z.re);
  lua_pushnumber(L, z.im);

  return 2;
}

int CFemmviewDoc::old_lua_getcircuitprops(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* TheDoc; // note normally thisdoc
  CFemmviewView* theView;
  TheDoc = (CFemmviewDoc*)pFemmviewdoc;
  POSITION pos;
  pos = TheDoc->GetFirstViewPosition();
  theView = (CFemmviewView*)TheDoc->GetNextView(pos);

  int NumCircuits, k;
  CString circuitname;
  circuitname = lua_tostring(L, 1);
  k = -1;

  // ok we need to find the correct entry for the circuit name
  NumCircuits = (int)TheDoc->circproplist.GetSize();
  for (int i = 0; i < NumCircuits; i++) {
    if (TheDoc->circproplist[i].CircName == circuitname) {
      k = i;
      i = NumCircuits; // that will break it
    }
  }

  // trap errors

  if (k == -1) {
    CString msg = "Unknown circuit";
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  CComplex amps, volts, fluxlinkage;

  amps = TheDoc->circproplist[k].Amps;
  volts = TheDoc->GetVoltageDrop(k);
  fluxlinkage = TheDoc->GetFluxLinkage(k);

  lua_pushnumber(L, Re(amps));
  lua_pushnumber(L, Im(amps));
  lua_pushnumber(L, Re(volts));
  lua_pushnumber(L, Im(volts));
  lua_pushnumber(L, Re(fluxlinkage));
  lua_pushnumber(L, Im(fluxlinkage));

  return 6;
}

int CFemmviewDoc::lua_gettitle(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  lua_pushstring(L, thisDoc->GetTitle());

  return 1;
}

///////////////////////////////////////////////////////
// Functions that provide low-level access to the mesh
///////////////////////////////////////////////////////

int CFemmviewDoc::lua_numnodes(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  lua_pushnumber(L, (int)thisDoc->meshnode.GetSize());

  return 1;
}

int CFemmviewDoc::lua_numelements(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  lua_pushnumber(L, (int)thisDoc->meshelem.GetSize());

  return 1;
}

int CFemmviewDoc::lua_getnode(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  int k = (int)lua_todouble(L, 1);
  k--;
  if ((k < 0) || (k >= thisDoc->meshnode.GetSize()))
    return 0;

  lua_pushnumber(L, thisDoc->meshnode[k].x);
  lua_pushnumber(L, thisDoc->meshnode[k].y);

  return 2;
}

int CFemmviewDoc::lua_getelement(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

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

int CFemmviewDoc::lua_weighting(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  thisDoc->WeightingScheme = (int)lua_todouble(L, 1);
  thisDoc->bHasMask = FALSE;

  return 0;
}

int CFemmviewDoc::lua_getAGEflux(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  int i, k;
  CString myBdryName;
  CComplex br = 0;
  CComplex bt = 0;
  double n, tta;

  myBdryName.Format("%s", lua_tostring(L, 1));

  // figure out which AGE is being asked for
  i = -1;
  for (k = 0; k < thisDoc->agelist.GetSize(); k++)
    if (thisDoc->agelist[k].BdryName == myBdryName)
      i = k;

  // if the requested AGE exists, roll up the flux density
  // at the specified angle.  Note: angle is specified in degrees
  if (i >= 0) {
    tta = lua_todouble(L, 2) * PI / 180;
    for (k = 0; k < thisDoc->agelist[i].nn; k++) {
      n = thisDoc->agelist[i].nh[k];
      br += thisDoc->agelist[i].brc[k] * cos(n * tta) + thisDoc->agelist[i].brs[k] * sin(n * tta);
      bt += thisDoc->agelist[i].btc[k] * cos(n * tta) + thisDoc->agelist[i].bts[k] * sin(n * tta);
    }
  }

  lua_pushnumber(L, br);
  lua_pushnumber(L, bt);

  return 2;
}

int CFemmviewDoc::lua_gapintegral(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  CString myBdryName;
  int i, k, IntegralType;

  myBdryName.Format("%s", lua_tostring(L, 1));

  // figure out which AGE is being asked for
  i = -1;
  for (k = 0; k < thisDoc->agelist.GetSize(); k++)
    if (thisDoc->agelist[k].BdryName == myBdryName)
      i = k;

  if (i < 0) {
    // Case where named AGE is not found
    lua_pushnumber(L, 0);
    return 1;
  }

  IntegralType = (int)lua_todouble(L, 2);
  double R = (thisDoc->agelist[i].ri + thisDoc->agelist[i].ro) / 2.;

  if (IntegralType == 0) // DC torque version using harmonic solution in airgap
  {
    int k;
    double tq = 0;

    for (k = 0; k < thisDoc->agelist[i].nn; k++) {
      tq += Re(thisDoc->agelist[i].brc[k] * conj(thisDoc->agelist[i].btc[k]) + thisDoc->agelist[i].brs[k] * conj(thisDoc->agelist[i].bts[k]));
    }
    tq *= (PI * R * R * thisDoc->Depth) / muo;

    if (thisDoc->Frequency != 0)
      tq /= 2.;

    lua_pushnumber(L, tq);

    return 1;
  }

  /*
    if (IntegralType==0) // DC torque
    {
      int k;
      CComplex tq=0;

      double dt=(PI/180.)*(thisDoc->agelist[i].totalArcLength/thisDoc->agelist[i].totalArcElements);
      CComplex br,bt;

      for(k=0;k<thisDoc->agelist[i].totalArcElements;k++)
      {
        br = thisDoc->agelist[i].br[k];
        bt = thisDoc->agelist[i].bt[k];
        // add in torque contribution for this element
        tq = tq + (br*conj(bt) + conj(br)*bt)/(2.*muo)*R*dt*R*thisDoc->Depth;
      }

      tq *= 360./((double) thisDoc->agelist[i].totalArcLength);
      if (thisDoc->Frequency!=0) tq/=2.;

      lua_pushnumber(L,tq);

      return 1;
    }
  */
  if (IntegralType == 3) // 2X torque
  {
    int k;
    CComplex tq = 0;

    if (thisDoc->Frequency != 0) {
      for (k = 0; k < thisDoc->agelist[i].nn; k++) {
        tq += thisDoc->agelist[i].brc[k] * thisDoc->agelist[i].btc[k] + thisDoc->agelist[i].brs[k] * thisDoc->agelist[i].bts[k];
      }
      tq *= (PI * R * R * thisDoc->Depth) / (2. * muo);
    }

    lua_pushnumber(L, tq);

    return 1;
  }

  if (IntegralType == 1) // DC Force
  {
    int k;
    CComplex fx = 0;
    CComplex fy = 0;
    CComplex dfx, dfy;

    if (round(thisDoc->agelist[i].totalArcLength) == 360) {
      for (k = 1; k < thisDoc->agelist[i].nn; k++) {

        dfx = (((thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) * conj(thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) + (thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) * conj(thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) + (thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k]) * conj(thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) + (thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) * conj(thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k])));

        dfy = (((-thisDoc->agelist[i].brc[k] + thisDoc->agelist[i].bts[k]) * conj(thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) + (thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) * conj(thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) + (thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) * conj(thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) + (-thisDoc->agelist[i].brs[k - 1] + thisDoc->agelist[i].btc[k - 1]) * conj(thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k])));

        fx += Re(dfx);
        fy += Re(dfy);
      }
      fx *= thisDoc->Depth * PI * R / (4. * muo);
      fy *= thisDoc->Depth * PI * R / (4. * muo);
      if (thisDoc->Frequency != 0) {
        fx /= 2.;
        fy /= 2.;
      }
    }

    lua_pushnumber(L, fx);
    lua_pushnumber(L, fy);

    return 2;
  }

  if (IntegralType == 4) // 2X Force
  {
    int k;
    CComplex fx = 0;
    CComplex fy = 0;
    CComplex dfx, dfy;

    if ((round(thisDoc->agelist[i].totalArcLength) == 360) && (thisDoc->Frequency != 0)) {
      for (k = 1; k < thisDoc->agelist[i].nn; k++) {

        dfx = (thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) * (thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) + (thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) * (thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k]);

        dfy = (thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) * (thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) - (thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) * (thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k]);

        fx += dfx;
        fy += dfy;
      }
      fx *= thisDoc->Depth * PI * R / (4. * muo);
      fy *= thisDoc->Depth * PI * R / (4. * muo);
    }

    lua_pushnumber(L, fx);
    lua_pushnumber(L, fy);

    return 2;
  }

  if (IntegralType == 5) // Incremental Torque
  {
    int k;
    CComplex tq = 0;

    for (k = 0; k < thisDoc->agelist[i].nn; k++) {
      tq += thisDoc->agelist[i].btcPrev[k] * thisDoc->agelist[i].brc[k] + thisDoc->agelist[i].brcPrev[k] * thisDoc->agelist[i].btc[k] + thisDoc->agelist[i].btsPrev[k] * thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].brsPrev[k] * thisDoc->agelist[i].bts[k];
    }
    tq *= (PI * R * R * thisDoc->Depth) / muo;

    lua_pushnumber(L, tq);

    return 1;
  }

  if (IntegralType == 6) // Incremental Force
  {
    int k;
    CComplex fx = 0;
    CComplex fy = 0;
    CComplex dfx, dfy;

    if ((round(thisDoc->agelist[i].totalArcLength) == 360) && (thisDoc->Frequency != 0)) {
      for (k = 1; k < thisDoc->agelist[i].nn; k++) {

        dfx = (thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) * (thisDoc->agelist[i].brsPrev[k - 1] - thisDoc->agelist[i].btcPrev[k - 1]) + (thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) * (thisDoc->agelist[i].brsPrev[k] + thisDoc->agelist[i].btcPrev[k]) + (thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k]) * (thisDoc->agelist[i].brcPrev[k - 1] + thisDoc->agelist[i].btsPrev[k - 1]) + (thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) * (thisDoc->agelist[i].brcPrev[k] - thisDoc->agelist[i].btsPrev[k]);

        dfy = (thisDoc->agelist[i].brsPrev[k] + thisDoc->agelist[i].btcPrev[k]) * (thisDoc->agelist[i].brc[k - 1] + thisDoc->agelist[i].bts[k - 1]) - (thisDoc->agelist[i].brsPrev[k - 1] - thisDoc->agelist[i].btcPrev[k - 1]) * (thisDoc->agelist[i].brc[k] - thisDoc->agelist[i].bts[k]) + (thisDoc->agelist[i].brs[k] + thisDoc->agelist[i].btc[k]) * (thisDoc->agelist[i].brcPrev[k - 1] + thisDoc->agelist[i].btsPrev[k - 1]) - (thisDoc->agelist[i].brs[k - 1] - thisDoc->agelist[i].btc[k - 1]) * (thisDoc->agelist[i].brcPrev[k] - thisDoc->agelist[i].btsPrev[k]);

        fx += dfx;
        fy += dfy;
      }
      fx *= thisDoc->Depth * PI * R / (2. * muo);
      fy *= thisDoc->Depth * PI * R / (2. * muo);
    }

    lua_pushnumber(L, fx);
    lua_pushnumber(L, fy);

    return 2;
  }

  if (IntegralType == 2) // (Time-Average) Stored Energy
  {
    int k, n;
    CComplex W = 0;

    double Ri = thisDoc->agelist[i].ri / R;
    double Ro = thisDoc->agelist[i].ro / R;
    double dr = R * (Ro - Ri);

    for (k = 0; k < thisDoc->agelist[i].nn; k++) {
      n = thisDoc->agelist[i].nh[k];

      if (n != 0)
        W += (thisDoc->agelist[i].brs[k] * thisDoc->agelist[i].brs[k].Conj() + thisDoc->agelist[i].brc[k] * thisDoc->agelist[i].brc[k].Conj() + thisDoc->agelist[i].bts[k] * thisDoc->agelist[i].bts[k].Conj() + thisDoc->agelist[i].btc[k] * thisDoc->agelist[i].btc[k].Conj()) * dr;
      else
        W += 2 * dr * thisDoc->agelist[i].btc[k] * thisDoc->agelist[i].btc[k].Conj();
    }
    W = Re(W) * (PI * R * thisDoc->Depth) / (2. * muo);
    if (thisDoc->Frequency != 0)
      W /= 2;

    lua_pushnumber(L, W);

    return 1;
  }

  return 0;
}

int CFemmviewDoc::lua_getgapa(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  int i, k;
  CString myBdryName;
  CComplex ac = 0;
  double n, tta, R;

  myBdryName.Format("%s", lua_tostring(L, 1));

  // figure out which AGE is being asked for
  i = -1;
  for (k = 0; k < thisDoc->agelist.GetSize(); k++)
    if (thisDoc->agelist[k].BdryName == myBdryName)
      i = k;

  // if the requested AGE exists, roll up the flux density
  // at the specified angle.  Note: angle is specified in degrees
  if (i >= 0) {
    R = (thisDoc->agelist[i].ri + thisDoc->agelist[i].ro) / 2.;
    tta = lua_todouble(L, 2) * PI / 180;
    for (k = 0; k < thisDoc->agelist[i].nn; k++) {
      n = thisDoc->agelist[i].nh[k];
      if (n == 0)
        ac += thisDoc->agelist[i].aco;
      else
        ac += (R / n) * (-thisDoc->agelist[i].brs[k] * cos(n * tta) + thisDoc->agelist[i].brc[k] * sin(n * tta));
    }
  }

  lua_pushnumber(L, ac);

  return 1;
}

int CFemmviewDoc::lua_getgapharmonics(lua_State* L)
{
  CatchNullDocument();
  CFemmviewDoc* thisDoc;
  thisDoc = (CFemmviewDoc*)pFemmviewdoc;

  CString myBdryName;
  int i, k;

  if (lua_gettop(L) == 0)
    return 0;

  // figure out which AGE is being asked for
  myBdryName.Format("%s", lua_tostring(L, 1));
  for (k = 0, i = -1; k < thisDoc->agelist.GetSize(); k++)
    if (thisDoc->agelist[k].BdryName == myBdryName)
      i = k;

  if (i < 0)
    return 0; // no AGE here by that name;

  // if _just_ the name is specified, return the number of the highest harmonic
  if (lua_gettop(L) == 1) {
    k = thisDoc->agelist[i].nn;
    if (k == 0)
      lua_pushnumber(L, 0);
    else
      lua_pushnumber(L, thisDoc->agelist[i].nh[k - 1]);
    return 1;
  }

  // otherwise, give all the magnitudes of the harmonic;
  if (thisDoc->agelist[i].nn == 0)
    return 0; // error case

  double n = lua_todouble(L, 2);
  CComplex acc = 0;
  CComplex acs = 0;
  CComplex brs = 0;
  CComplex brc = 0;
  CComplex bts = 0;
  CComplex btc = 0;

  if (n < 0)
    return 0;
  for (k = 0; k < thisDoc->agelist[i].nn; k++) {
    if (thisDoc->agelist[i].nh[k] == n)
      break;
  }
  if (k < thisDoc->agelist[i].nn) {
    if (n == 0) {
      acc = thisDoc->agelist[i].aco;
      acs = 0;
    } else {
      double R = (thisDoc->agelist[i].ri + thisDoc->agelist[i].ro) / 2.;
      acc = -(R / n) * thisDoc->agelist[i].brs[k];
      acs = (R / n) * thisDoc->agelist[i].brc[k];
      brc = thisDoc->agelist[i].brc[k];
      brs = thisDoc->agelist[i].brs[k];
      btc = thisDoc->agelist[i].btc[k];
      bts = thisDoc->agelist[i].bts[k];
    }
  }

  lua_pushnumber(L, acc);
  lua_pushnumber(L, acs);
  lua_pushnumber(L, brc);
  lua_pushnumber(L, brs);
  lua_pushnumber(L, btc);
  lua_pushnumber(L, bts);

  return 6;
}