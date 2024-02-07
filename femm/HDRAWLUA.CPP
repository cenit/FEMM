// hdrawDoc.cpp : implementation of the ChdrawDoc class
//

#include "stdafx.h"
#include "femm.h"
#include "hdrawDoc.h"
#include "hdrawView.h"
#include "promptbox.h"
#include "MainFrm.h"
#include "ChildFrm.h"

extern BOOL lua_byebye;
extern int m_luaWindowStatus;
extern void* phdrawDoc;
extern lua_State* lua;
extern BOOL bLinehook;
extern int m_luaWindowStatus;

#define CatchNullDocument()                      \
  ;                                              \
  if (phdrawDoc == NULL) {                       \
    CString msg = "No heat flow input in focus"; \
    lua_error(L, msg.GetBuffer(1));              \
    return 0;                                    \
  }

//---------------------Lua Extensions------------------------

void ChdrawDoc::initalise_lua()
{

  // register lua extensions
  lua_register(lua, "hi_setfocus", lua_switchfocus);
  lua_register(lua, "hi_saveas", luaSaveDocument);
  lua_register(lua, "hi_createmesh", lua_create_mesh);
  lua_register(lua, "hi_smartmesh", lua_smartmesh);
  lua_register(lua, "hi_showmesh", lua_show_mesh);
  lua_register(lua, "hi_purgemesh", lua_purge_mesh);
  lua_register(lua, "hi_probdef", lua_prob_def);
  lua_register(lua, "hi_analyse", lua_analyze);
  lua_register(lua, "hi_loadsolution", lua_runpost);
  lua_register(lua, "hi_addnode", lua_addnode);
  lua_register(lua, "hi_addblocklabel", lua_addlabel);
  lua_register(lua, "hi_addsegment", lua_addline);
  lua_register(lua, "hi_addarc", lua_addarc);
  lua_register(lua, "hi_selectnode", lua_selectnode);
  lua_register(lua, "hi_selectlabel", lua_selectlabel);
  lua_register(lua, "hi_selectsegment", lua_selectsegment);
  lua_register(lua, "hi_selectarcsegment", lua_selectarcsegment);
  lua_register(lua, "hi_clearselected", lua_clearselected);
  lua_register(lua, "hi_setnodeprop", lua_setnodeprop);
  lua_register(lua, "hi_setblockprop", lua_setblockprop);
  lua_register(lua, "hi_setsegmentprop", lua_setsegmentprop);
  lua_register(lua, "hi_setarcsegmentprop", lua_setarcsegmentprop);
  lua_register(lua, "hi_deleteselected", lua_deleteselected);
  lua_register(lua, "hi_deleteselectednodes", lua_deleteselectednodes);
  lua_register(lua, "hi_deleteselectedlabels", lua_deleteselectedlabels);
  lua_register(lua, "hi_deleteselectedsegments", lua_deleteselectedsegments);
  lua_register(lua, "hi_deleteselectedarcsegments", lua_deleteselectedarcsegments);
  lua_register(lua, "hi_zoomnatural", lua_zoomnatural);
  lua_register(lua, "hi_zoomout", lua_zoomout);
  lua_register(lua, "hi_zoomin", lua_zoomin);
  lua_register(lua, "hi_zoom", lua_zoom);
  lua_register(lua, "hi_addmaterial", lua_addmatprop);
  lua_register(lua, "hi_addpointprop", lua_addpointprop);
  lua_register(lua, "hi_addconductorprop", lua_addcircuitprop);
  lua_register(lua, "hi_addboundprop", lua_addboundprop);
  lua_register(lua, "hi_modifymaterial", lua_modmatprop);
  lua_register(lua, "hi_modifyboundprop", lua_modboundprop);
  lua_register(lua, "hi_modifypointprop", lua_modpointprop);
  lua_register(lua, "hi_modifyconductorprop", lua_modcircprop);
  lua_register(lua, "hi_deletematerial", lua_delmatprop);
  lua_register(lua, "hi_deleteboundprop", lua_delboundprop);
  lua_register(lua, "hi_deleteconductor", lua_delcircuitprop);
  lua_register(lua, "hi_deletepointprop", lua_delpointprop);
  lua_register(lua, "hi_moverotate", lua_move_rotate);
  lua_register(lua, "hi_movetranslate", lua_move_translate);
  lua_register(lua, "hi_copyrotate", lua_copy_rotate);
  lua_register(lua, "hi_copytranslate", lua_copy_translate);
  lua_register(lua, "hi_mirror", lua_mirror);
  lua_register(lua, "hi_scale", lua_scale);
  lua_register(lua, "hi_seteditmode", lua_seteditmode);
  lua_register(lua, "hi_selectgroup", lua_selectgroup);
  lua_register(lua, "hi_newdocument", lua_newdocument);
  lua_register(lua, "hi_savebitmap", lua_savebitmap);
  lua_register(lua, "hi_savemetafile", lua_saveWMF);
  lua_register(lua, "hi_close", lua_exitpre);
  lua_register(lua, "hi_refreshview", lua_updatewindow);
  lua_register(lua, "hi_shownames", lua_shownames);
  lua_register(lua, "hi_showgrid", lua_showgrid);
  lua_register(lua, "hi_hidegrid", lua_hidegrid);
  lua_register(lua, "hi_gridsnap", lua_gridsnap);
  lua_register(lua, "hi_setgrid", lua_setgrid);
  lua_register(lua, "hi_readdxf", lua_readdxf);
  lua_register(lua, "hi_savedxf", lua_savedxf);
  lua_register(lua, "hi_defineouterspace", lua_defineouterspace);
  lua_register(lua, "hi_attachouterspace", lua_attachouterspace);
  lua_register(lua, "hi_detachouterspace", lua_detachouterspace);
  lua_register(lua, "hi_attachdefault", lua_attachdefault);
  lua_register(lua, "hi_detachdefault", lua_detachdefault);
  lua_register(lua, "hi_attach_default", lua_attachdefault);
  lua_register(lua, "hi_detach_default", lua_detachdefault);
  lua_register(lua, "hi_resize", luaResize);
  lua_register(lua, "hi_minimize", luaMinimize);
  lua_register(lua, "hi_gettitle", lua_gettitle);
  lua_register(lua, "hi_get_title", lua_gettitle);
  lua_register(lua, "hi_maximize", luaMaximize);
  lua_register(lua, "hi_restore", luaRestore);
  lua_register(lua, "hi_createradius", lua_createradius);
  lua_register(lua, "hi_addtkpoint", lua_addtkpoint);
  lua_register(lua, "hi_cleartkpoints", lua_cleartkpoints);
  lua_register(lua, "hi_setgroup", lua_setgroup);
  lua_register(lua, "hi_set_group", lua_setgroup);
  lua_register(lua, "hi_getmaterial", lua_getmaterial);
  lua_register(lua, "hi_get_material", lua_getmaterial);
  lua_register(lua, "hi_getprobleminfo", lua_getprobleminfo);
  lua_register(lua, "hi_getboundingbox", lua_getboundingbox);
  lua_register(lua, "hi_selectcircle", lua_selectcircle);
  lua_register(lua, "hi_selectrectangle", lua_selectrectangle);
  lua_register(lua, "hi_setcomment", lua_comment);

  // compatibility lua function names
  lua_register(lua, "hi_select_rectangle", lua_selectrectangle);
  lua_register(lua, "hi_select_circle", lua_selectcircle);
  lua_register(lua, "hi_clear_tk_points", lua_cleartkpoints);
  lua_register(lua, "hi_add_tk_point", lua_addtkpoint);
  lua_register(lua, "hi_define_outer_space", lua_defineouterspace);
  lua_register(lua, "hi_attach_outer_space", lua_attachouterspace);
  lua_register(lua, "hi_detach_outer_space", lua_detachouterspace);
  lua_register(lua, "hi_set_focus", lua_switchfocus);
  lua_register(lua, "hi_save_as", luaSaveDocument);
  lua_register(lua, "hi_create_mesh", lua_create_mesh);
  lua_register(lua, "hi_show_mesh", lua_show_mesh);
  lua_register(lua, "hi_purge_mesh", lua_purge_mesh);
  lua_register(lua, "hi_prob_def", lua_prob_def);
  lua_register(lua, "hi_analyze", lua_analyze);
  lua_register(lua, "hi_load_solution", lua_runpost);
  lua_register(lua, "hi_add_node", lua_addnode);
  lua_register(lua, "hi_add_block_label", lua_addlabel);
  lua_register(lua, "hi_add_segment", lua_addline);
  lua_register(lua, "hi_add_arc", lua_addarc);
  lua_register(lua, "hi_select_node", lua_selectnode);
  lua_register(lua, "hi_select_label", lua_selectlabel);
  lua_register(lua, "hi_select_segment", lua_selectsegment);
  lua_register(lua, "hi_select_arcsegment", lua_selectarcsegment);
  lua_register(lua, "hi_clear_selected", lua_clearselected);
  lua_register(lua, "hi_set_node_prop", lua_setnodeprop);
  lua_register(lua, "hi_set_block_prop", lua_setblockprop);
  lua_register(lua, "hi_set_segment_prop", lua_setsegmentprop);
  lua_register(lua, "hi_set_arcsegment_prop", lua_setarcsegmentprop);
  lua_register(lua, "hi_delete_selected", lua_deleteselected);
  lua_register(lua, "hi_delete_selected_nodes", lua_deleteselectednodes);
  lua_register(lua, "hi_delete_selected_labels", lua_deleteselectedlabels);
  lua_register(lua, "hi_delete_selected_segments", lua_deleteselectedsegments);
  lua_register(lua, "hi_delete_selected_arcsegments", lua_deleteselectedarcsegments);
  lua_register(lua, "hi_zoom_natural", lua_zoomnatural);
  lua_register(lua, "hi_zoom_out", lua_zoomout);
  lua_register(lua, "hi_zoom_in", lua_zoomin);
  lua_register(lua, "hi_add_material", lua_addmatprop);
  lua_register(lua, "hi_add_point_prop", lua_addpointprop);
  lua_register(lua, "hi_add_conductor_prop", lua_addcircuitprop);
  lua_register(lua, "hi_add_bound_prop", lua_addboundprop);
  lua_register(lua, "hi_modify_material", lua_modmatprop);
  lua_register(lua, "hi_modify_bound_prop", lua_modboundprop);
  lua_register(lua, "hi_modify_point_prop", lua_modpointprop);
  lua_register(lua, "hi_modify_conductor_prop", lua_modcircprop);
  lua_register(lua, "hi_delete_material", lua_delmatprop);
  lua_register(lua, "hi_delete_bound_prop", lua_delboundprop);
  lua_register(lua, "hi_delete_conductor", lua_delcircuitprop);
  lua_register(lua, "hi_delete_point_prop", lua_delpointprop);
  lua_register(lua, "hi_move_rotate", lua_move_rotate);
  lua_register(lua, "hi_move_translate", lua_move_translate);
  lua_register(lua, "hi_copy_rotate", lua_copy_rotate);
  lua_register(lua, "hi_copy_translate", lua_copy_translate);
  lua_register(lua, "hi_set_edit_mode", lua_seteditmode);
  lua_register(lua, "hi_select_group", lua_selectgroup);
  lua_register(lua, "hi_new_document", lua_newdocument);
  lua_register(lua, "hi_save_bitmap", lua_savebitmap);
  lua_register(lua, "hi_save_metafile", lua_saveWMF);
  lua_register(lua, "hi_refresh_view", lua_updatewindow);
  lua_register(lua, "hi_show_grid", lua_showgrid);
  lua_register(lua, "hi_hide_grid", lua_hidegrid);
  lua_register(lua, "hi_grid_snap", lua_gridsnap);
  lua_register(lua, "hi_set_grid", lua_setgrid);
  lua_register(lua, "hi_show_names", lua_shownames);
  lua_register(lua, "hi_read_dxf", lua_readdxf);
  lua_register(lua, "hi_save_dxf", lua_savedxf);
  lua_register(lua, "hi_create_radius", lua_createradius);

  phdrawDoc = (ChdrawDoc*)this;
}

int ChdrawDoc::lua_switchfocus(lua_State* L)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  ChdrawDoc* pDoc;
  CString DocTitle, ThisTitle;
  int n;

  // get title of desired document from Lua
  n = lua_gettop(L);
  if (n == 0)
    return NULL;
  DocTitle.Format("%s", lua_tostring(L, n));
  DocTitle.MakeLower();
  if (DocTitle.Right(4) == ".feh")
    DocTitle = DocTitle.Left(DocTitle.GetLength() - 4);

  // get pointer to document template for FemmeDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 3; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (ChdrawDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetTitle();
    ThisTitle.MakeLower();
    if (ThisTitle.Right(4) == ".feh")
      ThisTitle = ThisTitle.Left(ThisTitle.GetLength() - 4);
    if (ThisTitle == DocTitle) {
      phdrawDoc = pDoc;
      return NULL;
    }
  }

  // if we can't find it, throw an error message
  CString msg;
  msg.Format("No document that matches %s", DocTitle);
  lua_error(L, msg.GetBuffer(1));
  return NULL;
}

int ChdrawDoc::lua_exitpre(lua_State* L)
{
  CatchNullDocument();
  ((ChdrawDoc*)phdrawDoc)->OnCloseDocument();

  return 0;
}

int ChdrawDoc::luaSaveDocument(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  int n;
  n = lua_gettop(L);

  CString temp;
  temp.Format("%s", lua_tostring(L, n));
  thisDoc->OnSaveDocument(temp);
  thisDoc->SetPathName(temp);

  return 0;
}

int ChdrawDoc::lua_readdxf(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n;
  CString DocTitle;
  double DefTol;
  BOOL result;

  // get title of desired document from Lua
  n = lua_gettop(L);
  if (n == 0)
    return FALSE;
  DocTitle.Format("%s", lua_tostring(L, 1));
  if (n == 2)
    DefTol = lua_todouble(L, 2);
  else
    DefTol = 0;

  result = thisDoc->ReadDXF(DocTitle, DefTol);
  if (result)
    theView->InvalidateRect(NULL);

  return result;
}

int ChdrawDoc::lua_savedxf(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n;
  CString DocTitle;
  BOOL result;

  // get title of desired document from Lua
  n = lua_gettop(L);
  if (n == 0)
    return FALSE;
  DocTitle.Format("%s", lua_tostring(L, 1));

  result = thisDoc->WriteDXF(DocTitle);
  if (result)
    theView->InvalidateRect(NULL);

  return result;
}

int ChdrawDoc::lua_comment(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  int n;
  n = lua_gettop(L);

  if (n > 0) {
    CString myComment;
    myComment.Format("%s", lua_tostring(L, 1));
    thisDoc->ProblemNote = myComment;
  }

  return 0;
}

int ChdrawDoc::lua_prob_def(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  CString units, type;
  int n;
  n = lua_gettop(L);

  // error traps
  int LengthUnits = -1;
  int ProblemType = -1;
  double Precision;
  double MinAngle = -1;
  CString PrevSoln = "";
  double dT = 0;

  units.Format("%s", lua_tostring(L, 1));
  type.Format("%s", lua_tostring(L, 2));
  Precision = lua_todouble(L, 3);
  if (n > 3)
    thisDoc->Depth = lua_todouble(L, 4);
  if (n > 4)
    MinAngle = lua_todouble(L, 5);
  if (units == "inches")
    LengthUnits = 0;
  if (units == "millimeters")
    LengthUnits = 1;
  if (units == "centimeters")
    LengthUnits = 2;
  if (units == "meters")
    LengthUnits = 3;
  if (units == "mills")
    LengthUnits = 4;
  if (units == "mils")
    LengthUnits = 4;
  if (units == "micrometers")
    LengthUnits = 5;
  if (type == "planar")
    ProblemType = 0;
  if (type == "axi")
    ProblemType = 1;
  if (n > 5)
    PrevSoln.Format("%s", lua_tostring(L, 6));
  if (n > 6)
    dT = lua_todouble(L, 7);

  // error detection and nice variable handling

  if (LengthUnits == -1) {
    CString msg;
    msg.Format("Unknown length unit %s", units);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  } else {
    thisDoc->LengthUnits = LengthUnits;
  }

  if (ProblemType == -1) {
    CString msg;
    msg.Format("Unknown problem type %s", type);
    lua_error(L, msg.GetBuffer(1));
    return 0;

  } else {
    thisDoc->ProblemType = ProblemType;
  }

  // Changed to correct the precision rather than throwing a cryptic error
  if ((Precision == 0) || (Precision > 1.e-8))
    Precision = 1.e-8;
  if (Precision < 1.e-16)
    Precision = 1.e-16;
  thisDoc->Precision = Precision;

  if ((MinAngle <= MINANGLE_MAX) && (MinAngle >= 1.)) {
    thisDoc->MinAngle = MinAngle;
  }

  if (thisDoc->Depth <= 0)
    thisDoc->Depth = 1;

  // Test to see if the previous solution and specified time step are consistent
  if (PrevSoln.GetLength() == 0)
    dT = 0;
  if (dT == 0)
    PrevSoln = "";

  thisDoc->PrevSoln = PrevSoln;
  thisDoc->dT = dT;

  //	CString temp;
  //	temp.Format("%s",lua_tostring(L,n));

  return 0;
}

int ChdrawDoc::lua_create_mesh(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  theView->lnuMakeMesh();

  return 1;
}

int ChdrawDoc::lua_smartmesh(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  if (lua_gettop(L) != 0) {
    int k = (int)lua_todouble(L, 1);
    if (k != 0)
      k = 1;
    thisDoc->SmartMesh = k;
  }

  return 0;
}

int ChdrawDoc::lua_show_mesh(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  theView->lnu_show_mesh();

  return 0;
}

int ChdrawDoc::lua_purge_mesh(lua_State* L)
{
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  theView->lnu_purge_mesh();

  return 0;
}

int ChdrawDoc::lua_analyze(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n = lua_gettop(L);
  int bHide = 0;
  if ((n == 0) && (theApp.GetMainWnd()->GetStyle() & WS_MINIMIZE))
    bHide = 1;
  if (n > 0)
    bHide = (int)lua_todouble(L, 1);
  if (bHide != 0)
    bHide = 1;

  theView->lnu_analyze(bHide);

  return 0;
}

int ChdrawDoc::lua_runpost(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;
  CString pn = thisDoc->GetPathName();

  if (pn.GetLength() == 0) {
    CString msg;
    msg.Format("No results to display");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  CString rootname = "\"" + pn.Left(pn.ReverseFind('.')) + ".anh\"";
  if (((CFemmApp*)AfxGetApp())->HOCheckIfAlreadyOpen(rootname) == FALSE)
    AfxGetApp()->OpenDocumentFile(rootname);

  return 0;
}

int ChdrawDoc::lua_addnode(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  double x, y, d;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();

  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);

  //	if(thisDoc->Coords)
  //	{
  //		double r,t;
  //		r=mx;
  //		t=my*PI/180;
  //		x=mx*cos(t);
  //		y=mx*sin(t);
  //	}

  if (thisDoc->nodelist.GetSize() < 2)
    d = 1.e-08;
  else {
    CComplex p0, p1, p2;
    p0 = thisDoc->nodelist[0].CC();
    p1 = p0;
    for (int i = 1; i < thisDoc->nodelist.GetSize(); i++) {
      p2 = thisDoc->nodelist[i].CC();
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
  BOOL flag;
  flag = thisDoc->AddNode(x, y, d);

  if (flag == TRUE) {
    theView->MeshUpToDate = FALSE;
    if (theView->MeshFlag == TRUE)
      theView->lnu_show_mesh();
    else
      theView->DrawPSLG();
  }

  return 0;
}

int ChdrawDoc::lua_addlabel(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  double x, y, d;

  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  //	if(thisDoc->Coords)
  //	{
  //		double r,t;
  //		r=mx;
  //		t=my*PI/180;
  //		x=mx*cos(t);
  //		y=mx*sin(t);
  //	}

  if (thisDoc->nodelist.GetSize() < 2)
    d = 1.e-08;
  else {
    CComplex p0, p1, p2;
    p0 = thisDoc->nodelist[0].CC();
    p1 = p0;
    for (int i = 1; i < thisDoc->nodelist.GetSize(); i++) {
      p2 = thisDoc->nodelist[i].CC();
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
  BOOL flag;
  flag = thisDoc->AddBlockLabel(x, y, d);

  if (flag == TRUE) {
    theView->MeshUpToDate = FALSE;
    if (theView->MeshFlag == TRUE)
      theView->lnu_show_mesh();
    else
      theView->DrawPSLG();
  }

  return 0;
}

int ChdrawDoc::lua_addline(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  CPoint mypntstart, mypntend;

  double sx, sy, ex, ey;

  sx = lua_todouble(L, 1);
  sy = lua_todouble(L, 2);

  ex = lua_todouble(L, 3);
  ey = lua_todouble(L, 4);

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();

  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  BOOL flag;

  flag = thisDoc->AddSegment(thisDoc->ClosestNode(sx, sy), thisDoc->ClosestNode(ex, ey));

  if (flag == TRUE) {
    theView->MeshUpToDate = FALSE;
    if (theView->MeshFlag == TRUE)
      theView->lnu_show_mesh();
    else
      theView->DrawPSLG();
  } else
    theView->DrawPSLG();

  return 0;
}

int ChdrawDoc::lua_addarc(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  double sx, sy, ex, ey;
  double angle, maxseg;

  sx = lua_todouble(L, 1);
  sy = lua_todouble(L, 2);
  ex = lua_todouble(L, 3);
  ey = lua_todouble(L, 4);

  angle = lua_todouble(L, 5);
  maxseg = lua_todouble(L, 6);

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();

  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  BOOL flag;

  CArcSegment asegm;
  asegm.n0 = thisDoc->ClosestNode(sx, sy);
  asegm.n1 = thisDoc->ClosestNode(ex, ey);
  thisDoc->nodelist[asegm.n1].ToggleSelect();
  theView->DrawPSLG();

  asegm.MaxSideLength = maxseg;
  asegm.ArcLength = angle;
  asegm.BoundaryMarker = "<None>";

  flag = thisDoc->AddArcSegment(asegm);
  flag = FALSE;
  thisDoc->UnselectAll();
  if (flag == TRUE) {
    theView->MeshUpToDate = FALSE;
    if (theView->MeshFlag == TRUE)
      theView->lnu_show_mesh();
    else
      theView->DrawPSLG();
  } else
    theView->DrawPSLG();

  return 0;
}

int ChdrawDoc::lua_selectnode(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  double mx, my;
  mx = lua_todouble(L, 1);
  my = lua_todouble(L, 2);
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int node;
  if (thisDoc->nodelist.GetSize() == 0)
    return 0;
  node = thisDoc->ClosestNode(mx, my);
  thisDoc->nodelist[node].ToggleSelect();

  lua_pushnumber(L, thisDoc->nodelist[node].x);
  lua_pushnumber(L, thisDoc->nodelist[node].y);

  return 2;
}

int ChdrawDoc::lua_selectlabel(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  double mx, my;
  mx = lua_todouble(L, 1);
  my = lua_todouble(L, 2);
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int node;
  if (thisDoc->blocklist.GetSize() == 0)
    return 0;
  node = thisDoc->ClosestBlockLabel(mx, my);
  thisDoc->blocklist[node].ToggleSelect();

  lua_pushnumber(L, thisDoc->blocklist[node].x);
  lua_pushnumber(L, thisDoc->blocklist[node].y);

  return 2;
}

int ChdrawDoc::lua_selectsegment(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  double mx, my;
  mx = lua_todouble(L, 1);
  my = lua_todouble(L, 2);
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int node;
  if (thisDoc->linelist.GetSize() == 0)
    return 0;
  node = thisDoc->ClosestSegment(mx, my);
  thisDoc->linelist[node].ToggleSelect();

  lua_pushnumber(L, thisDoc->nodelist[thisDoc->linelist[node].n0].x);
  lua_pushnumber(L, thisDoc->nodelist[thisDoc->linelist[node].n0].y);
  lua_pushnumber(L, thisDoc->nodelist[thisDoc->linelist[node].n1].x);
  lua_pushnumber(L, thisDoc->nodelist[thisDoc->linelist[node].n1].y);

  return 4;
}

int ChdrawDoc::lua_selectarcsegment(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  double mx, my;
  mx = lua_todouble(L, 1);
  my = lua_todouble(L, 2);
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int node;
  if (thisDoc->arclist.GetSize() == 0)
    return 0;
  node = thisDoc->ClosestArcSegment(mx, my);
  thisDoc->arclist[node].ToggleSelect();

  lua_pushnumber(L, thisDoc->nodelist[thisDoc->arclist[node].n0].x);
  lua_pushnumber(L, thisDoc->nodelist[thisDoc->arclist[node].n0].y);
  lua_pushnumber(L, thisDoc->nodelist[thisDoc->arclist[node].n1].x);
  lua_pushnumber(L, thisDoc->nodelist[thisDoc->arclist[node].n1].y);

  return 4;
}

int ChdrawDoc::lua_clearselected(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  thisDoc->UnselectAll();
  return 0;
}

int ChdrawDoc::lua_setnodeprop(lua_State* L)
{
  CatchNullDocument();
  CString nodeprop, inconductor;
  int groupno;

  nodeprop.Format("%s", lua_tostring(L, 1));
  groupno = (int)lua_todouble(L, 2);
  inconductor.Format("%s", lua_tostring(L, 3));

  if (groupno < 0) {
    CString msg;
    msg.Format("Invalid group no %d", groupno);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int i;

  // check to see how many (if any) nodes are selected.
  for (i = 0; i < thisDoc->nodelist.GetSize(); i++) {
    if (thisDoc->nodelist[i].IsSelected == TRUE) {
      thisDoc->nodelist[i].InGroup = groupno;
      thisDoc->nodelist[i].BoundaryMarker = nodeprop;
      thisDoc->nodelist[i].InConductor = inconductor;
    }
  }

  return 0;
}

int ChdrawDoc::lua_setblockprop(lua_State* L)
{
  CatchNullDocument();
  CString blocktype;
  BOOL automesh;
  double meshsize;
  int group;

  int n = lua_gettop(L);

  blocktype = "<None>";
  automesh = 1;
  meshsize = 0;
  group = 0;

  if (n > 0)
    blocktype.Format("%s", lua_tostring(L, 1));
  if (n > 1)
    automesh = (int)lua_todouble(L, 2);
  if (n > 2)
    meshsize = lua_todouble(L, 3);
  if (n > 3)
    group = (int)lua_todouble(L, 4);

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int i;

  for (i = 0; i < thisDoc->blocklist.GetSize(); i++) {
    if (thisDoc->blocklist[i].IsSelected == TRUE) {
      thisDoc->blocklist[i].MaxArea = PI * meshsize * meshsize / 4.;
      thisDoc->blocklist[i].BlockType = blocktype;
      thisDoc->blocklist[i].InGroup = group;
      if (automesh == 1)
        thisDoc->blocklist[i].MaxArea = 0;
    }
  }

  return 0;
}

int ChdrawDoc::lua_setsegmentprop(lua_State* L)
{
  CatchNullDocument();
  CString prop, inconductor;
  double elesize;
  int automesh, hide, group;

  prop.Format("%s", lua_tostring(L, 1));
  elesize = lua_todouble(L, 2);
  automesh = (int)lua_todouble(L, 3);
  hide = (int)lua_todouble(L, 4);
  group = (int)lua_todouble(L, 5);
  inconductor.Format("%s", lua_tostring(L, 6));

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  for (int i = 0; i < thisDoc->linelist.GetSize(); i++) {
    if (thisDoc->linelist[i].IsSelected == TRUE) {
      if (automesh == 1)
        thisDoc->linelist[i].MaxSideLength = -1;
      else {
        if (elesize > 0)
          thisDoc->linelist[i].MaxSideLength = elesize;
        else
          elesize = -1;
      }
      thisDoc->linelist[i].BoundaryMarker = prop;
      thisDoc->linelist[i].Hidden = hide;
      thisDoc->linelist[i].InGroup = group;
      thisDoc->linelist[i].InConductor = inconductor;
    }
  }

  return 0;
}

int ChdrawDoc::lua_setarcsegmentprop(lua_State* L)
{
  CatchNullDocument();
  CString boundprop, inconductor;
  double maxsegdeg;
  int hide, group;

  maxsegdeg = lua_todouble(L, 1);
  if (maxsegdeg > 10)
    maxsegdeg = 10;
  if (maxsegdeg <= 0)
    maxsegdeg = 1;
  boundprop.Format("%s", lua_tostring(L, 2));
  hide = (int)lua_todouble(L, 3);
  group = (int)lua_todouble(L, 4);
  inconductor.Format("%s", lua_tostring(L, 5));

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int i;

  for (i = 0; i < thisDoc->arclist.GetSize(); i++) {
    if (thisDoc->arclist[i].IsSelected == TRUE) {
      thisDoc->arclist[i].BoundaryMarker = boundprop;
      thisDoc->arclist[i].MaxSideLength = maxsegdeg;
      thisDoc->arclist[i].Hidden = hide;
      thisDoc->arclist[i].InGroup = group;
      thisDoc->arclist[i].InConductor = inconductor;
    }
  }

  return 0;
}

int ChdrawDoc::lua_deleteselectednodes(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  thisDoc->DeleteSelectedNodes();
  return 0;
}

int ChdrawDoc::lua_deleteselectedlabels(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  thisDoc->DeleteSelectedBlockLabels();
  return 0;
}

int ChdrawDoc::lua_deleteselectedsegments(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  thisDoc->DeleteSelectedSegments();
  return 0;
}

int ChdrawDoc::lua_deleteselectedarcsegments(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  thisDoc->DeleteSelectedArcSegments();
  return 0;
}

int ChdrawDoc::lua_deleteselected(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  thisDoc->DeleteSelectedSegments();
  thisDoc->DeleteSelectedArcSegments();
  thisDoc->DeleteSelectedNodes();
  thisDoc->DeleteSelectedBlockLabels();

  return 0;
}

int ChdrawDoc::lua_newdocument(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  thisDoc->OnNewDocument();
  thisDoc->SetPathName("Untitled", FALSE);
  thisDoc->SetTitle("Untitled");
  return 0;
}

int ChdrawDoc::lua_zoomnatural(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  theView->lua_zoomnatural();

  return 0;
}

int ChdrawDoc::lua_zoomout(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  theView->lua_zoomout();

  return 0;
}

int ChdrawDoc::lua_zoomin(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  theView->lua_zoomin();

  return 0;
}

int ChdrawDoc::lua_zoom(lua_State* L)
{
  CatchNullDocument();

  double x[2], y[2], m[2];

  x[0] = lua_todouble(L, 1);
  y[0] = lua_todouble(L, 2);
  x[1] = lua_todouble(L, 3);
  y[1] = lua_todouble(L, 4);

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

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

int ChdrawDoc::lua_copy_rotate(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n;
  n = lua_gettop(L);
  // IF N=5 WE Have an edit action, if n=4 we dont !

  int EditAction, copies;

  double x, y, angle;
  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);
  angle = lua_todouble(L, 3);
  copies = (int)lua_todouble(L, 4);

  if (n == 5)
    EditAction = (int)lua_todouble(L, 5);
  if (n == 4)
    EditAction = theView->EditAction;

  if (n != 4 && n != 5) {
    CString msg;
    msg.Format("Invalid number of parameters for copy rotate");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  thisDoc->RotateCopy(CComplex(x, y), angle, copies, EditAction);
  thisDoc->meshnode.RemoveAll();
  thisDoc->meshline.RemoveAll();
  thisDoc->greymeshline.RemoveAll();
  theView->MeshFlag = FALSE;
  theView->MeshUpToDate = FALSE;
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_copy_translate(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n;
  n = lua_gettop(L);

  int EditAction, copies;
  double x, y;
  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);
  copies = (int)lua_todouble(L, 3);

  if (n == 4)
    EditAction = (int)lua_todouble(L, 4);
  if (n == 3)
    EditAction = theView->EditAction;

  if (n != 4 && n != 3) {
    CString msg;
    msg.Format("Invalid number of parameters for copy translate");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  thisDoc->UpdateUndo();
  thisDoc->TranslateCopy(x, y, copies, EditAction);
  thisDoc->meshnode.RemoveAll();
  thisDoc->meshline.RemoveAll();
  thisDoc->greymeshline.RemoveAll();
  theView->MeshFlag = FALSE;
  theView->MeshUpToDate = FALSE;
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_move_translate(lua_State* L)
{
  CatchNullDocument();

  double x, y;
  int EditAction;

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n;
  n = lua_gettop(L);

  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);

  if (n == 3)
    EditAction = (int)lua_todouble(L, 3);
  if (n == 2)
    EditAction = theView->EditAction;

  if (n != 3 && n != 2) {
    CString msg;
    msg.Format("Invalid number of parameters for move translate");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  {
    thisDoc->UpdateUndo();
    thisDoc->TranslateMove(x, y, EditAction);
    thisDoc->meshnode.RemoveAll();
    thisDoc->meshline.RemoveAll();
    thisDoc->greymeshline.RemoveAll();
    theView->MeshFlag = FALSE;
    theView->MeshUpToDate = FALSE;
    theView->InvalidateRect(NULL);
  }

  return 0;
}

int ChdrawDoc::lua_move_rotate(lua_State* L)
{
  CatchNullDocument();

  double x, y, shiftangle;
  int EditAction;

  int n;
  n = lua_gettop(L);

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);
  shiftangle = lua_todouble(L, 3);

  if (n == 4)
    EditAction = (int)lua_todouble(L, 4);
  if (n == 3)
    EditAction = theView->EditAction;

  if (n != 4 && n != 3) {
    CString msg;
    msg.Format("Invalid number of parameters for move rotate");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  {
    thisDoc->UpdateUndo();
    thisDoc->RotateMove(CComplex(x, y), shiftangle, EditAction);
    thisDoc->meshnode.RemoveAll();
    thisDoc->meshline.RemoveAll();
    thisDoc->greymeshline.RemoveAll();
    theView->MeshFlag = FALSE;
    theView->MeshUpToDate = FALSE;
    theView->InvalidateRect(NULL);
  }

  return 0;
}

int ChdrawDoc::lua_mirror(lua_State* L)
{
  CatchNullDocument();
  double m_pax, m_pay, m_pbx, m_pby;
  int EditAction;

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n;
  n = lua_gettop(L);

  m_pax = lua_todouble(L, 1);
  m_pay = lua_todouble(L, 2);
  m_pbx = lua_todouble(L, 3);
  m_pby = lua_todouble(L, 4);

  if (n == 5)
    EditAction = (int)lua_todouble(L, 5);
  if (n == 4)
    EditAction = theView->EditAction;

  if (n != 4 && n != 5) {
    CString msg;
    msg.Format("Invalid number of parameters for mirror");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  thisDoc->UpdateUndo();
  thisDoc->MirrorSelected(m_pax, m_pay, m_pbx, m_pby, EditAction);
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_scale(lua_State* L)
{
  CatchNullDocument();
  int EditAction;
  double x, y, scalefactor;

  int n;
  n = lua_gettop(L);

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);
  scalefactor = lua_todouble(L, 3);
  EditAction = (int)lua_todouble(L, 4);

  if (n == 4)
    EditAction = (int)lua_todouble(L, 4);
  if (n == 3)
    EditAction = theView->EditAction;

  if (n != 4 && n != 3) {
    CString msg;
    msg.Format("Invalid number of parameters for scale");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  thisDoc->UpdateUndo();
  thisDoc->ScaleMove(x, y, scalefactor, EditAction);
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_seteditmode(lua_State* L)
{
  CatchNullDocument();
  CString EditAction;

  EditAction.Format("%s", lua_tostring(L, 1));
  EditAction.MakeLower();

  ChdrawDoc* thisDoc;
  ChdrawView* theView;

  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int iEditAction;
  iEditAction = -1;

  if (EditAction == "nodes")
    iEditAction = 0;
  if (EditAction == "segments")
    iEditAction = 1;
  if (EditAction == "blocks")
    iEditAction = 2;
  if (EditAction == "arcsegments")
    iEditAction = 3;
  if (EditAction == "group")
    iEditAction = 4;

  if (iEditAction == -1) {
    CString msg;
    msg.Format("Invalid Edit Action %s", EditAction);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  } else {
    theView->EditAction = iEditAction;
  }

  return 0;
}

int ChdrawDoc::lua_selectgroup(lua_State* L)
{
  CatchNullDocument();
  int group, i;
  group = (int)lua_todouble(L, 1);

  if (group < 0) {
    CString msg;
    msg.Format("Invalid group %d", group);
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  //	thisDoc->UnselectAll();

  // select nodes
  for (i = 0; i < thisDoc->nodelist.GetSize(); i++) {
    if (thisDoc->nodelist[i].InGroup == group)
      thisDoc->nodelist[i].IsSelected = TRUE;
  }

  // select segments
  for (i = 0; i < thisDoc->linelist.GetSize(); i++) {
    if (thisDoc->linelist[i].InGroup == group)
      thisDoc->linelist[i].IsSelected = TRUE;
  }

  // select arc segments
  for (i = 0; i < thisDoc->arclist.GetSize(); i++) {
    if (thisDoc->arclist[i].InGroup == group)
      thisDoc->arclist[i].IsSelected = TRUE;
  }

  // select blocks
  for (i = 0; i < thisDoc->blocklist.GetSize(); i++) {
    if (thisDoc->blocklist[i].InGroup == group)
      thisDoc->blocklist[i].IsSelected = TRUE;
  }

  theView->EditAction = 4; // set to group

  return 0;
}

int ChdrawDoc::lua_addmatprop(lua_State* L)
{
  CatchNullDocument();

  CMaterialProp m;
  int n = lua_gettop(L);

  if (n > 0)
    m.BlockName.Format("%s", lua_tostring(L, 1));
  if (n > 1) {
    m.Kx = lua_todouble(L, 2);
    m.Ky = m.Kx;
  }
  if (n > 2)
    m.Ky = lua_todouble(L, 3);
  if (n > 3)
    m.qv = lua_todouble(L, 4);
  if (n > 4)
    m.Kt = lua_todouble(L, 5);

  ((ChdrawDoc*)phdrawDoc)->blockproplist.Add(m);

  return 0;
}

int ChdrawDoc::lua_modmatprop(lua_State* L)
{
  CatchNullDocument();
  CString BlockName;
  int k, modprop;

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  // find the index of the material to modify;
  if (thisDoc->blockproplist.GetSize() == 0)
    return TRUE;
  BlockName.Format("%s", lua_tostring(L, 1));
  for (k = 0; k < thisDoc->blockproplist.GetSize(); k++)
    if (BlockName == thisDoc->blockproplist[k].BlockName)
      break;

  // get out of here if there's no matching material
  if (k == thisDoc->blockproplist.GetSize())
    return TRUE;

  modprop = (int)lua_todouble(L, 2);

  // now, modify the specified attribute...
  switch (modprop) {
  case 0:
    thisDoc->blockproplist[k].BlockName.Format("%s", lua_tostring(L, 3));
    break;
  case 1:
    thisDoc->blockproplist[k].Kx = lua_todouble(L, 3);
    break;
  case 2:
    thisDoc->blockproplist[k].Ky = lua_todouble(L, 3);
    break;
  case 3:
    thisDoc->blockproplist[k].qv = lua_todouble(L, 3);
    break;
  case 4:
    thisDoc->blockproplist[k].Kt = lua_todouble(L, 3);
    break;
  default:
    break;
  }

  return 0;
}

int ChdrawDoc::lua_addboundprop(lua_State* L)
{
  CatchNullDocument();

  CBoundaryProp m;
  int n = lua_gettop(L);

  if (n > 0)
    m.BdryName.Format("%s", lua_tostring(L, 1));
  if (n > 1)
    m.BdryFormat = (int)lua_todouble(L, 2);
  if (n > 2)
    m.Tset = lua_todouble(L, 3);
  if (n > 3)
    m.qs = lua_todouble(L, 4);
  if (n > 4) {
    m.Tinf = lua_todouble(L, 5);
    m.TinfRad = m.Tinf; // for backward compatibility
  }
  if (n > 5)
    m.h = lua_todouble(L, 6);
  if (n > 6)
    m.beta = lua_todouble(L, 7);
  if (n > 7)
    m.TinfRad = lua_todouble(L, 8);

  ((ChdrawDoc*)phdrawDoc)->lineproplist.Add(m);

  return 0;
}

int ChdrawDoc::lua_modboundprop(lua_State* L)
{
  CatchNullDocument();
  CString BdryName;
  int k, modprop;

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  // find the index of the boundary property to modify;
  if (thisDoc->lineproplist.GetSize() == 0)
    return TRUE;
  BdryName.Format("%s", lua_tostring(L, 1));
  for (k = 0; k < thisDoc->lineproplist.GetSize(); k++)
    if (BdryName == thisDoc->lineproplist[k].BdryName)
      break;

  // get out of here if there's no matching material
  if (k == thisDoc->lineproplist.GetSize()) {
    MsgBox("Boundary \"%s\" not found", BdryName);
    return 0;
  }

  modprop = (int)lua_todouble(L, 2);

  // now, modify the specified attribute...
  switch (modprop) {
  case 0:
    thisDoc->lineproplist[k].BdryName.Format("%s", lua_tostring(L, 3));
    break;
  case 1:
    thisDoc->lineproplist[k].BdryFormat = (int)lua_todouble(L, 3);
    break;
  case 2:
    thisDoc->lineproplist[k].Tset = lua_todouble(L, 3);
    break;
  case 3:
    thisDoc->lineproplist[k].qs = lua_todouble(L, 3);
    break;
  case 4:
    thisDoc->lineproplist[k].Tinf = lua_todouble(L, 3);
    // note--may break backward compatibility for old scripts that used
    // radiation boundary condition because Tinf for radiation
    // now changed as 7th value. No bullet-proofed way to surreptitiously
    // define TinfRad here when Tinf is defined.
    break;
  case 5:
    thisDoc->lineproplist[k].h = lua_todouble(L, 3);
    break;
  case 6:
    thisDoc->lineproplist[k].beta = lua_todouble(L, 3);
    break;
  case 7:
    thisDoc->lineproplist[k].TinfRad = lua_todouble(L, 3);
    break;

  default:
    break;
  }

  return 0;
}

int ChdrawDoc::lua_addpointprop(lua_State* L)
{
  CatchNullDocument();

  CPointProp m;
  int n = lua_gettop(L);

  if (n > 0)
    m.PointName.Format("%s", lua_tostring(L, 1));
  if (n > 1)
    m.T = lua_todouble(L, 2);
  if (n > 2)
    m.qp = lua_todouble(L, 3);

  ((ChdrawDoc*)phdrawDoc)->nodeproplist.Add(m);

  return 0;
}

int ChdrawDoc::lua_modpointprop(lua_State* L)
{
  CatchNullDocument();
  CString PointName;
  int k, modprop;

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  // find the index of the material to modify;
  if (thisDoc->nodeproplist.GetSize() == 0)
    return TRUE;
  PointName.Format("%s", lua_tostring(L, 1));
  for (k = 0; k < thisDoc->nodeproplist.GetSize(); k++)
    if (PointName == thisDoc->nodeproplist[k].PointName)
      break;

  // get out of here if there's no matching material
  if (k == thisDoc->nodeproplist.GetSize())
    return TRUE;

  modprop = (int)lua_todouble(L, 2);

  // now, modify the specified attribute...
  switch (modprop) {
  case 0:
    thisDoc->nodeproplist[k].PointName.Format("%s", lua_tostring(L, 3));
    break;
  case 1:
    thisDoc->nodeproplist[k].T = lua_todouble(L, 3);
    break;
  case 2:
    thisDoc->nodeproplist[k].qp = lua_todouble(L, 3);
    break;
  default:
    break;
  }

  return 0;
}

int ChdrawDoc::lua_addcircuitprop(lua_State* L)
{
  CatchNullDocument();

  CCircuit m;
  int n = lua_gettop(L);

  if (n > 0)
    m.CircName = lua_tostring(L, 1);
  if (n > 1)
    m.T = lua_todouble(L, 2);
  if (n > 2)
    m.q = lua_todouble(L, 3);
  if (n > 3)
    m.CircType = (int)lua_todouble(L, 4);

  // there shouldn't be duplicate conductor names.
  for (int k = 0; k < ((ChdrawDoc*)phdrawDoc)->circproplist.GetSize(); k++) {
    if (m.CircName == ((ChdrawDoc*)phdrawDoc)->circproplist[k].CircName) {
      CString msg;
      msg.Format("Duplicate conductor name (%s) not allowed", m.CircName);
      lua_error(L, msg.GetBuffer(1));
      return NULL;
    }
  }

  ((ChdrawDoc*)phdrawDoc)->circproplist.Add(m);

  return 0;
}

int ChdrawDoc::lua_modcircprop(lua_State* L)
{
  CatchNullDocument();
  CString CircName;
  int k, modprop;

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  // find the index of the material to modify;
  if (thisDoc->circproplist.GetSize() == 0)
    return TRUE;
  CircName.Format("%s", lua_tostring(L, 1));
  for (k = 0; k < thisDoc->circproplist.GetSize(); k++)
    if (CircName == thisDoc->circproplist[k].CircName)
      break;

  // get out of here if there's no matching circuit
  if (k == thisDoc->circproplist.GetSize())
    return TRUE;

  modprop = (int)lua_todouble(L, 2);

  // now, modify the specified attribute...
  switch (modprop) {
  case 0:
    thisDoc->circproplist[k].CircName.Format("%s", lua_tostring(L, 3));
    break;
  case 3:
    thisDoc->circproplist[k].CircType = (int)lua_todouble(L, 3);
    break;
  case 1:
    thisDoc->circproplist[k].T = lua_todouble(L, 3);
    break;
  case 2:
    thisDoc->circproplist[k].q = lua_todouble(L, 3);
    break;
  default:
    break;
  }

  return 0;
}

int ChdrawDoc::lua_delcircuitprop(lua_State* L)
{
  CatchNullDocument();
  CString name;
  name.Format("%s", lua_tostring(L, 1));

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->circproplist.GetSize(); i++) {
    if (thisDoc->circproplist[i].CircName == name)
      thisDoc->circproplist.RemoveAt(i);
  }

  return 0;
}

int ChdrawDoc::lua_delmatprop(lua_State* L)
{
  CatchNullDocument();
  CString name;
  name.Format("%s", lua_tostring(L, 1));

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->blockproplist.GetSize(); i++) {
    if (thisDoc->blockproplist[i].BlockName == name)
      thisDoc->blockproplist.RemoveAt(i);
  }

  return 0;
}

int ChdrawDoc::lua_delboundprop(lua_State* L)
{
  CatchNullDocument();
  CString name;
  name.Format("%s", lua_tostring(L, 1));

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->lineproplist.GetSize(); i++) {
    if (thisDoc->lineproplist[i].BdryName == name)
      thisDoc->lineproplist.RemoveAt(i);
  }

  return 0;
}

int ChdrawDoc::lua_delpointprop(lua_State* L)
{
  CatchNullDocument();
  CString name;
  name.Format("%s", lua_tostring(L, 1));

  ChdrawDoc* thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->nodeproplist.GetSize(); i++) {
    if (thisDoc->nodeproplist[i].PointName == name)
      thisDoc->nodeproplist.RemoveAt(i);
  }

  return 0;
}

int ChdrawDoc::lua_updatewindow(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  CDC tempDC;
  CDC* pDC = theView->GetDC();

  // erase background
  CBrush backBrush(theView->BackColor);
  CBrush* pOldBrush = pDC->SelectObject(&backBrush);
  CRect rect;
  pDC->GetClipBox(&rect);
  pDC->PatBlt(rect.left, rect.top, rect.Width(),
      rect.Height(), PATCOPY);
  pDC->SelectObject(pOldBrush);

  // force redraw
  theView->OnDraw(pDC);

  return 0;
}

int ChdrawDoc::lua_savebitmap(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
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

  PBITMAPINFO pbmi;

  pbmi = thisDoc->CreateBitmapInfoStruct(theView->m_hWnd, HBITMAP(bitmap));
  thisDoc->CreateBMPFile(theView->m_hWnd, filename.GetBuffer(1), pbmi, HBITMAP(bitmap), tempDC.m_hDC);

  return 0;
}

PBITMAPINFO ChdrawDoc::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
  BITMAP bmp;
  PBITMAPINFO pbmi;
  WORD cClrBits;

  // Retrieve the bitmap's color format, width, and height.
  if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) { // errhandler("GetObject", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy1");
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

  if (cClrBits != 24)
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

void ChdrawDoc::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
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
    MsgBox("Critical error on getting bmp info, possible page fault ahoy2");
  //              errhandler("GlobalAlloc", hwnd);

  // Retrieve the color table (RGBQUAD array) and the bits
  // (array of palette indices) from the DIB.
  if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
          DIB_RGB_COLORS)) {
    //    errhandler("GetDIBits", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy3");
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
    MsgBox("Critical error on getting bmp info, possible page fault ahoy4");
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
    MsgBox("Critical error on getting bmp info, possible page fault ahoy5");

    //      errhandler("WriteFile", hwnd);
  }

  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
  if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD),
          (LPDWORD)&dwTmp, (NULL)))
    //        errhandler("WriteFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy6");

  // Copy the array of color indices into the .BMP file.
  dwTotal = cb = pbih->biSizeImage;
  hp = lpBits;
  if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
    //           errhandler("WriteFile", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy7");

  // Close the .BMP file.
  if (!CloseHandle(hf))
    //           errhandler("CloseHandle", hwnd);
    MsgBox("Critical error on getting bmp info, possible page fault ahoy8");

  // Free memory.
  GlobalFree((HGLOBAL)lpBits);
}

int ChdrawDoc::lua_saveWMF(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

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

int ChdrawDoc::lua_hidegrid(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_HD_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->GridFlag = FALSE;
  MMnu->CheckMenuItem(ID_SHOW_GRID, MF_UNCHECKED);
  tc->PressButton(ID_SHOW_GRID, FALSE);
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_showgrid(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_HD_toolBar1;
  CToolBarCtrl* tc = &pToolBar->GetToolBarCtrl();

  theView->GridFlag = TRUE;
  MMnu->CheckMenuItem(ID_SHOW_GRID, MF_CHECKED);
  tc->PressButton(ID_SHOW_GRID, TRUE);
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_gridsnap(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  CString temp;
  temp.Format("%s", lua_tostring(L, 1));
  temp.MakeLower();

  CMainFrame* MFrm;
  MFrm = (CMainFrame*)theView->GetTopLevelFrame();
  CMenu* MMnu = MFrm->GetMenu();
  CToolBar* pToolBar;
  pToolBar = &MFrm->m_HD_toolBar1;
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

int ChdrawDoc::lua_setgrid(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

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
  theView->InvalidateRect(NULL);

  return 0;
}

int ChdrawDoc::lua_shownames(lua_State* L)
{
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int flg = (int)lua_todouble(L, 1);
  if (flg != 0)
    flg = 1;
  theView->ShowNames = flg;
  theView->InvalidateRect(NULL);

  return 0;
}

void ChdrawView::lnuMakeMesh()
{
  OnMakeMesh();
}

void ChdrawView::lnu_purge_mesh()
{
  OnPurgemesh();
}

void ChdrawView::lnu_show_mesh()
{
  OnShowMesh();
}

void ChdrawView::lnu_analyze(int bShow)
{
  if (bShow > 0)
    bLinehook = HiddenLua;
  else
    bLinehook = NormalLua;
  OnMenuAnalyze();
}

void ChdrawView::lua_zoomnatural()
{
  OnZoomNatural();
}

void ChdrawView::lua_zoomout()
{
  OnZoomOut();
}

void ChdrawView::lua_zoomin()
{
  OnZoomIn();
}

int ChdrawDoc::luaResize(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();
  int n = lua_gettop(L);
  if (n != 2)
    return 0;

  int nWidth = (int)lua_todouble(L, 1);
  int nHeight = (int)lua_todouble(L, 2);
  pChildFrm->ResizeClient(nWidth, nHeight, TRUE);

  return 0;
}

int ChdrawDoc::luaMinimize(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_MINIMIZE);

  return 0;
}

int ChdrawDoc::luaMaximize(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_MAXIMIZE);

  return 0;
}

int ChdrawDoc::luaRestore(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);
  CChildFrame* pChildFrm = (CChildFrame*)theView->GetParentFrame();

  pChildFrm->ShowWindow(SW_RESTORE);

  return 0;
}

int ChdrawDoc::lua_defineouterspace(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  int n = lua_gettop(L);
  if (n != 3)
    return 0;

  thisDoc->extZo = fabs(lua_todouble(L, 1));
  thisDoc->extRo = fabs(lua_todouble(L, 2));
  thisDoc->extRi = fabs(lua_todouble(L, 3));

  if ((thisDoc->extRo == 0) || (thisDoc->extRi == 0)) {
    thisDoc->extZo = 0;
    thisDoc->extRo = 0;
    thisDoc->extRi = 0;
  }

  return 0;
}

int ChdrawDoc::lua_attachouterspace(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      thisDoc->blocklist[i].IsExternal = TRUE;

  return 0;
}

int ChdrawDoc::lua_detachouterspace(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      thisDoc->blocklist[i].IsExternal = FALSE;

  return 0;
}

int ChdrawDoc::lua_createradius(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  int n = lua_gettop(L);
  if (n != 3)
    return 0;

  double x, y, r;
  int k;

  x = lua_todouble(L, 1);
  y = lua_todouble(L, 2);
  r = fabs(lua_todouble(L, 3));

  if ((k = thisDoc->ClosestNode(x, y)) < 0)
    return 0; // catch case where no nodes have been drawn yet;

  if (thisDoc->CanCreateRadius(k) == FALSE) {
    CString msg;
    msg.Format("The specified point is not suitable for conversion into a radius");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  if (thisDoc->CreateRadius(k, r)) {
    theView->InvalidateRect(NULL);
    theView->MeshUpToDate = FALSE;
  } else {
    CString msg;
    msg.Format("Could not make a radius of the prescribed dimension");
    lua_error(L, msg.GetBuffer(1));
    return 0;
  }

  return 0;
}

int ChdrawDoc::lua_addtkpoint(lua_State* L)
{
  CatchNullDocument();
  CString BlockName;
  int k;

  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  // find the index of the material to modify;
  if (thisDoc->blockproplist.GetSize() == 0)
    return 0;
  BlockName.Format("%s", lua_tostring(L, 1));
  for (k = 0; k < thisDoc->blockproplist.GetSize(); k++)
    if (BlockName == thisDoc->blockproplist[k].BlockName)
      break;

  // get out of here if there's no matching material
  if (k == thisDoc->blockproplist.GetSize())
    return 0;

  // now, add the tkpoint for the specified material;
  thisDoc->blockproplist[k].Kn[thisDoc->blockproplist[k].npts] = lua_todouble(L, 2) + I * lua_todouble(L, 3);
  thisDoc->blockproplist[k].npts++;

  return 0;
}

int ChdrawDoc::lua_cleartkpoints(lua_State* L)
{
  CatchNullDocument();
  CString BlockName;
  int k;

  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  // find the index of the material to modify;
  if (thisDoc->blockproplist.GetSize() == 0)
    return 0;
  BlockName.Format("%s", lua_tostring(L, 1));
  for (k = 0; k < thisDoc->blockproplist.GetSize(); k++)
    if (BlockName == thisDoc->blockproplist[k].BlockName)
      break;

  // get out of here if there's no matching material
  if (k == thisDoc->blockproplist.GetSize())
    return 0;

  // now, zap the T-k points for the specified material;
  thisDoc->blockproplist[k].npts = 0;

  return 0;
}

int ChdrawDoc::lua_gettitle(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  lua_pushstring(L, thisDoc->GetTitle());

  return 1;
}

int ChdrawDoc::lua_setgroup(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;
  int grp, i;

  int n = lua_gettop(L);

  if (n > 0)
    grp = (int)lua_todouble(L, 1);
  else
    return 0;

  for (i = 0; i < thisDoc->nodelist.GetSize(); i++)
    if (thisDoc->nodelist[i].IsSelected == TRUE)
      thisDoc->nodelist[i].InGroup = grp;

  for (i = 0; i < thisDoc->linelist.GetSize(); i++)
    if (thisDoc->linelist[i].IsSelected == TRUE)
      thisDoc->linelist[i].InGroup = grp;

  for (i = 0; i < thisDoc->arclist.GetSize(); i++)
    if (thisDoc->arclist[i].IsSelected == TRUE)
      thisDoc->arclist[i].InGroup = grp;

  for (i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      thisDoc->blocklist[i].InGroup = grp;

  thisDoc->UnselectAll();

  return 0;
}

int ChdrawDoc::lua_getmaterial(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  CString matname;
  int n = lua_gettop(L);

  if (n > 0)
    matname = lua_tostring(L, 1);
  else
    return 0;

  CString LibName = thisDoc->BinDir + "heatlib.dat";

  FILE* fp;
  int i, j, k;
  char s[1024], q[1024];
  char* v;

  CMaterialProp MProp;

  if ((fp = fopen(LibName, "rt")) == NULL) {
    CString msg;
    msg.Format("Couldn't open %s", LibName);
    lua_error(L, msg.GetBuffer(1));
    return FALSE;
  }

  // parse the file
  while (fgets(s, 1024, fp) != NULL) {
    if (sscanf(s, "%s", q) == EOF)
      q[0] = NULL;

    // Block Properties;
    if (_strnicmp(q, "<beginblock>", 12) == 0) {
      MProp.BlockName = "New Material";
      MProp.Kx = 1.;
      MProp.Ky = 1.;
      MProp.Kt = 3;
      MProp.qv = 0.;
      MProp.npts = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<blockname>", 10) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      MProp.BlockName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<Kx>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Kx);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<Ky>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Ky);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<Kt>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Kt);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qv>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.qv);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<TKPoints>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &MProp.npts);
      if (MProp.npts > 0) {
        for (j = 0; j < MProp.npts; j++) {
          fgets(s, 1024, fp);
          sscanf(s, "%lf	%lf", &MProp.Kn[j].re, &MProp.Kn[j].im);
        }
      }
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endblock>", 9) == 0) {
      if (MProp.BlockName == matname) {
        thisDoc->blockproplist.Add(MProp);
        fclose(fp);
        return 0;
      }
      q[0] = NULL;
    }
  }

  fclose(fp);

  CString msg;
  msg.Format("Couldn't load \"%s\" from the materials library", matname);
  lua_error(L, msg.GetBuffer(1));
  return FALSE;
}

int ChdrawDoc::lua_attachdefault(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0, j = 0; i < thisDoc->blocklist.GetSize(); i++) {
    if ((thisDoc->blocklist[i].IsSelected == TRUE) && (j == 0)) {
      thisDoc->blocklist[i].IsDefault = 2;
      j++;
    } else
      thisDoc->blocklist[i].IsDefault = FALSE;
  }

  return 0;
}

int ChdrawDoc::lua_detachdefault(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  for (int i = 0; i < thisDoc->blocklist.GetSize(); i++)
    if (thisDoc->blocklist[i].IsSelected == TRUE)
      thisDoc->blocklist[i].IsDefault = FALSE;

  return 0;
}

int ChdrawDoc::lua_getprobleminfo(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  lua_pushnumber(L, thisDoc->ProblemType);
  lua_pushnumber(L, thisDoc->Depth);
  switch (thisDoc->LengthUnits) {
  case 1: // mm
    lua_pushnumber(L, 0.001);
    break;
  case 2: // cm
    lua_pushnumber(L, 0.01);
    break;
  case 3: // m
    lua_pushnumber(L, 1.0);
    break;
  case 4: // mils
    lua_pushnumber(L, 2.54e-05);
    break;
  case 5: // um
    lua_pushnumber(L, 1.0e-06);
    break;
  default: // inches
    lua_pushnumber(L, 0.0254);
    break;
  }
  return 3;
}

int ChdrawDoc::lua_getboundingbox(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* thisDoc;
  ChdrawView* theView;
  thisDoc = (ChdrawDoc*)phdrawDoc;

  POSITION pos;
  pos = thisDoc->GetFirstViewPosition();
  theView = (ChdrawView*)thisDoc->GetNextView(pos);

  double x[2], y[2];
  if (theView->GetBoundingBox(x, y)) {
    lua_pushnumber(L, x[0]);
    lua_pushnumber(L, x[1]);
    lua_pushnumber(L, y[0]);
    lua_pushnumber(L, y[1]);
    return 4;
  } else
    return 0;
}

int ChdrawDoc::lua_selectcircle(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* pDoc;
  ChdrawView* theView;

  pDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = pDoc->GetFirstViewPosition();
  theView = (ChdrawView*)pDoc->GetNextView(pos);

  int i, k, n, EditAction;
  double R;
  CComplex c, q;

  n = lua_gettop(L);
  if (n < 3)
    return 0;
  c = lua_tonumber(L, 1) + I * lua_tonumber(L, 2);
  R = lua_todouble(L, 3);
  if (n > 3)
    EditAction = (int)lua_todouble(L, 4);
  else
    EditAction = theView->EditAction;
  if ((EditAction > 4) || (EditAction < 0))
    return 0;

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

  theView->InvalidateRect(NULL);
  return 0;
}

int ChdrawDoc::lua_selectrectangle(lua_State* L)
{
  CatchNullDocument();
  ChdrawDoc* pDoc;
  ChdrawView* theView;

  pDoc = (ChdrawDoc*)phdrawDoc;
  POSITION pos;
  pos = pDoc->GetFirstViewPosition();
  theView = (ChdrawView*)pDoc->GetNextView(pos);

  int i, k, n, EditAction;
  double mx, my, wzx, wzy, x, y, z;

  n = lua_gettop(L);
  if (n < 4)
    return 0;
  mx = lua_todouble(L, 1);
  my = lua_todouble(L, 2);
  wzx = lua_todouble(L, 3);
  wzy = lua_todouble(L, 4);
  if (n > 4)
    EditAction = (int)lua_todouble(L, 5);
  else
    EditAction = theView->EditAction;
  if ((EditAction > 4) || (EditAction < 0))
    return 0;

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

  theView->InvalidateRect(NULL);
  return 0;
}