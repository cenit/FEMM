#if !defined(AFX_FEMMEDOC_H__C8BAD1A9_C080_4CB6_9B9D_1F22718D3551__INCLUDED_)
#define AFX_FEMMEDOC_H__C8BAD1A9_C080_4CB6_9B9D_1F22718D3551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FemmeDoc.h : header file
//
#include "nosebl.h"

#include "lua.h"
#include "luaconsoledlg.h"
#include "luadebug.h"

extern CFemmApp theApp; //<DP>

/////////////////////////////////////////////////////////////////////////////
// CFemmeDoc document

class CFemmeDoc : public CDocument {
  protected:
  CFemmeDoc(); // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CFemmeDoc)

  // Attributes
  public:
  // General problem attributes
  double Frequency;
  double Precision;
  double MinAngle;
  int SmartMesh;
  double Depth;
  int LengthUnits;
  int ACSolver;
  BOOL ProblemType;
  BOOL Coords;
  CString ProblemNote;
  BOOL FirstDraw;
  BOOL NoDraw;
  CString PrevSoln;
  int PrevType;

  // default behaviors
  double d_prec;
  double d_minangle;
  double d_freq;
  double d_depth;
  int d_coord;
  int d_length;
  int d_type;
  int d_solver;

  CString BinDir;

  // lists of nodes, segments, and block labels
  CArray<CNode, CNode&> nodelist;
  CArray<CSegment, CSegment&> linelist;
  CArray<CArcSegment, CArcSegment&> arclist;
  CArray<CBlockLabel, CBlockLabel&> blocklist;

  // lists of nodes, segments, and block labels for undo purposes...
  CArray<CNode, CNode&> undonodelist;
  CArray<CSegment, CSegment&> undolinelist;
  CArray<CArcSegment, CArcSegment&> undoarclist;
  CArray<CBlockLabel, CBlockLabel&> undoblocklist;

  // CArrays containing the mesh information
  CArray<CPoint, CPoint&> meshline;
  CArray<CPoint, CPoint&> greymeshline;
  CArray<CNode, CNode&> meshnode;

  // lists of properties
  CArray<CMaterialProp, CMaterialProp&> blockproplist;
  CArray<CBoundaryProp, CBoundaryProp&> lineproplist;
  CArray<CPointProp, CPointProp&> nodeproplist;
  CArray<CCircuit, CCircuit&> circproplist;
  double extRo, extRi, extZo;

  // Operations
  public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFemmeDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  //}}AFX_VIRTUAL

  // Implementation
  public:
  virtual ~CFemmeDoc();

  void UnselectAll();
  double ShortestDistance(double p, double q, int segm);
  BOOL AddNode(double x, double y, double d);
  BOOL AddSegment(int n0, int n1, double tol = 0);
  BOOL AddSegment(int n0, int n1, CSegment* parsegm, double tol = 0);
  BOOL AddArcSegment(CArcSegment& asegm, double tol = 0);
  BOOL AddBlockLabel(double x, double y, double d);
  BOOL AddNode(CNode& node, double d);
  BOOL AddSegment(CComplex p0, CComplex p1, CSegment& segm, double tol = 0);
  BOOL AddArcSegment(CComplex p0, CComplex p1, CArcSegment& asegm, double tol = 0);
  BOOL AddBlockLabel(CBlockLabel& blabel, double d);
  int ClosestNode(double x, double y);
  int ClosestBlockLabel(double x, double y);
  int ClosestSegment(double x, double y);
  BOOL GetIntersection(int n0, int n1, int segm, double* xi, double* yi);
  int ClosestArcSegment(double x, double y);
  void GetCircle(CArcSegment& asegm, CComplex& c, double& R);
  int GetLineArcIntersection(CSegment& seg, CArcSegment& arc, CComplex* p);
  int GetArcArcIntersection(CArcSegment& arc1, CArcSegment& arc2, CComplex* p);
  double ShortestDistanceFromArc(CComplex p, CArcSegment& arc);
  void RotateMove(CComplex c, double t, int EditAction);
  void TranslateMove(double dx, double dy, int EditAction);
  void ScaleMove(double bx, double by, double sf, int EditAction);
  void MirrorSelected(double x0, double y0, double x1, double y1, int ea);
  void RotateCopy(CComplex c, double t, int ncopies, int EditAction);
  void TranslateCopy(double dx, double dy, int ncopies, int EditAction);
  BOOL DeleteSelectedNodes();
  BOOL DeleteSelectedSegments();
  BOOL DeleteSelectedArcSegments();
  BOOL DeleteSelectedBlockLabels();
  BOOL OpBlkDlg();
  void OpNodeDlg();
  void OpSegDlg();
  void OpArcSegDlg();
  void OpGrpDlg();
  BOOL LoadMesh();
  BOOL OnWritePoly();
  BOOL FunnyOnWritePoly();
  BOOL ReadDXF(CString fname, double DefTol = -1.);
  BOOL WriteDXF(CString fname);
  BOOL OldOnOpenDocument(LPCTSTR lpszPathName);
  BOOL HasPeriodicBC();
  BOOL CanCreateRadius(int n);
  BOOL CreateRadius(int n, double r);
  double LineLength(int i);
  BOOL ScanPreferences();

  void UpdateUndo();
  void Undo();
  void EnforcePSLG(); // makes sure that everything is kosher...
  void EnforcePSLG(double tol);
  void FancyEnforcePSLG(double tol);
  BOOL SelectOrphans();
  BOOL dxf_line_hook();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  protected:
  //{{AFX_MSG(CFemmeDoc)
  afx_msg void OnDefineProblem();
  afx_msg void OnEditMatprops();
  afx_msg void OnEditPtprops();
  afx_msg void OnEditSegprops();
  afx_msg void OnEditCircprops();
  afx_msg void OnEditExterior();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  afx_msg void OnViewLuaConsole();

  void initalise_lua();
  static int lua_prob_def(lua_State* L);
  static int luaSaveDocument(lua_State* L);
  static int lua_create_mesh(lua_State* L);
  static int lua_smartmesh(lua_State* L);
  static int lua_purge_mesh(lua_State* L);
  static int lua_show_mesh(lua_State* L);
  static int lua_analyze(lua_State* L);
  static int lua_runpost(lua_State* L);
  static int lua_addnode(lua_State* L);
  static int lua_addlabel(lua_State* L);
  static int lua_addline(lua_State* L);
  static int lua_addarc(lua_State* L);
  static int lua_selectnode(lua_State* L);
  static int lua_selectlabel(lua_State* L);
  static int lua_selectsegment(lua_State* L);
  static int lua_selectarcsegment(lua_State* L);
  static int lua_clearselected(lua_State* L);
  static int lua_setnodeprop(lua_State* L);
  static int lua_setblockprop(lua_State* L);
  static int lua_setsegmentprop(lua_State* L);
  static int lua_setarcsegmentprop(lua_State* L);
  static int lua_deleteselected(lua_State* L);
  static int lua_deleteselectedsegments(lua_State* L);
  static int lua_deleteselectednodes(lua_State* L);
  static int lua_deleteselectedlabels(lua_State* L);
  static int lua_deleteselectedarcsegments(lua_State* L);
  static int lua_zoomout(lua_State* L);
  static int lua_zoomnatural(lua_State* L);
  static int lua_zoomin(lua_State* L);
  static int lua_move_translate(lua_State* L);
  static int lua_move_rotate(lua_State* L);
  static int lua_copy_translate(lua_State* L);
  static int lua_copy_rotate(lua_State* L);
  static int lua_mirror(lua_State* L);
  static int lua_scale(lua_State* L);
  static int lua_addmatprop(lua_State* L);
  static int lua_addpointprop(lua_State* L);
  static int lua_addboundprop(lua_State* L);
  static int lua_addcircuitprop(lua_State* L);
  static int lua_delcircuitprop(lua_State* L);
  static int lua_delpointprop(lua_State* L);
  static int lua_delboundprop(lua_State* L);
  static int lua_delmatprop(lua_State* L);
  static int lua_seteditmode(lua_State* L);
  static int lua_selectgroup(lua_State* L);
  static int lua_zoom(lua_State* L);
  static int lua_newdocument(lua_State* L);
  static int lua_savebitmap(lua_State* L);
  static int lua_modmatprop(lua_State* L);
  static int lua_modboundprop(lua_State* L);
  static int lua_modpointprop(lua_State* L);
  static int lua_modcircprop(lua_State* L);
  static int lua_exitpre(lua_State* L);
  static int lua_addbhpoint(lua_State* L);
  static int lua_clearbhpoints(lua_State* L);
  PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
  void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
  static int lua_saveWMF(lua_State* L);
  static int lua_updatewindow(lua_State* L);
  static int lua_shownames(lua_State* L);
  static int lua_showgrid(lua_State* L);
  static int lua_hidegrid(lua_State* L);
  static int lua_gridsnap(lua_State* L);
  static int lua_setgrid(lua_State* L);
  static int lua_switchfocus(lua_State* L);
  static int lua_readdxf(lua_State* L);
  static int lua_savedxf(lua_State* L);
  static int luaResize(lua_State* L);
  static int luaMinimize(lua_State* L);
  static int luaMaximize(lua_State* L);
  static int luaRestore(lua_State* L);
  static int lua_defineouterspace(lua_State* L);
  static int lua_attachouterspace(lua_State* L);
  static int lua_detachouterspace(lua_State* L);
  static int lua_attachdefault(lua_State* L);
  static int lua_detachdefault(lua_State* L);
  static int lua_createradius(lua_State* L);
  static int lua_gettitle(lua_State* L);
  static int lua_setgroup(lua_State* L);
  static int lua_getmaterial(lua_State* L);
  static int lua_getprobleminfo(lua_State* L);
  static int lua_getboundingbox(lua_State* L);
  static int lua_selectcircle(lua_State* L);
  static int lua_selectrectangle(lua_State* L);
  static int lua_previous(lua_State* L);
  static int lua_comment(lua_State* L);
  static int old_lua_addmatprop(lua_State* L);
  static int old_lua_modmatprop(lua_State* L);
  static int old_lua_addpointprop(lua_State* L);
  static int old_lua_modpointprop(lua_State* L);
  static int old_lua_addcircuitprop(lua_State* L);
  static int old_lua_modcircprop(lua_State* L);
};

char* StripKey(char* c);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEMMEDOC_H__C8BAD1A9_C080_4CB6_9B9D_1F22718D3551__INCLUDED_)
