#include "bd_nosebl.h"
#include <afx.h>
#include <afxtempl.h>

#include "lua.h"
#include "luaconsoledlg.h"
#include "luadebug.h"

extern void lua_baselibopen(lua_State* L);
extern void lua_iolibopen(lua_State* L);
extern void lua_strlibopen(lua_State* L);
extern void lua_mathlibopen(lua_State* L);
extern void lua_dblibopen(lua_State* L);

extern CFemmApp theApp; //<DP>

// beladrawDoc.h : interface of the CbeladrawDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CbeladrawDoc : public CDocument {
  protected: // create from serialization only
  CbeladrawDoc();
  DECLARE_DYNCREATE(CbeladrawDoc)

  // Attributes
  public:
  // General problem attributes
  double Precision;
  double MinAngle;
  int SmartMesh;
  double Depth;
  int LengthUnits;
  BOOL ProblemType;
  BOOL Coords;
  CString ProblemNote;
  BOOL FirstDraw;
  BOOL NoDraw;

  // default behaviors
  double d_prec;
  double d_minangle;
  double d_depth;
  int d_coord;
  int d_length;
  int d_type;

  CString BinDir;

  // lists of nodes, segments, and block labels
  CArray<beladrawdata::CNode, beladrawdata::CNode&> nodelist;
  CArray<beladrawdata::CSegment, beladrawdata::CSegment&> linelist;
  CArray<beladrawdata::CArcSegment, beladrawdata::CArcSegment&> arclist;
  CArray<beladrawdata::CBlockLabel, beladrawdata::CBlockLabel&> blocklist;

  // lists of nodes, segments, and block labels for undo purposes...
  CArray<beladrawdata::CNode, beladrawdata::CNode&> undonodelist;
  CArray<beladrawdata::CSegment, beladrawdata::CSegment&> undolinelist;
  CArray<beladrawdata::CArcSegment, beladrawdata::CArcSegment&> undoarclist;
  CArray<beladrawdata::CBlockLabel, beladrawdata::CBlockLabel&> undoblocklist;

  // CArrays containing the mesh information
  CArray<CPoint, CPoint&> meshline;
  CArray<CPoint, CPoint&> greymeshline;
  CArray<beladrawdata::CNode, beladrawdata::CNode&> meshnode;

  // lists of properties
  CArray<beladrawdata::CMaterialProp, beladrawdata::CMaterialProp&> blockproplist;
  CArray<beladrawdata::CBoundaryProp, beladrawdata::CBoundaryProp&> lineproplist;
  CArray<beladrawdata::CPointProp, beladrawdata::CPointProp&> nodeproplist;
  CArray<beladrawdata::CCircuit, beladrawdata::CCircuit&> circproplist;
  double extRo, extRi, extZo;

  // Operations
  public:
  void UnselectAll();
  double ShortestDistance(double p, double q, int segm);

  BOOL AddNode(double x, double y, double d);

  BOOL AddSegment(int n0, int n1, double tol = 0);
  BOOL AddSegment(int n0, int n1, beladrawdata::CSegment* parsegm, double tol = 0);
  BOOL AddArcSegment(beladrawdata::CArcSegment& asegm, double tol = 0);
  BOOL AddBlockLabel(double x, double y, double d);
  BOOL AddNode(beladrawdata::CNode& node, double d);
  BOOL AddSegment(CComplex p0, CComplex p1, beladrawdata::CSegment& segm, double tol = 0);
  BOOL AddArcSegment(CComplex p0, CComplex p1, beladrawdata::CArcSegment& asegm, double tol = 0);

  BOOL AddBlockLabel(beladrawdata::CBlockLabel& blabel, double d);
  int ClosestNode(double x, double y);
  int ClosestBlockLabel(double x, double y);
  int ClosestSegment(double x, double y);
  BOOL GetIntersection(int n0, int n1, int segm, double* xi, double* yi);
  int ClosestArcSegment(double x, double y);
  void GetCircle(beladrawdata::CArcSegment& asegm, CComplex& c, double& R);
  int GetLineArcIntersection(beladrawdata::CSegment& seg, beladrawdata::CArcSegment& arc, CComplex* p);
  int GetArcArcIntersection(beladrawdata::CArcSegment& arc1, beladrawdata::CArcSegment& arc2, CComplex* p);
  double ShortestDistanceFromArc(CComplex p, beladrawdata::CArcSegment& arc);
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
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CbeladrawDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  //}}AFX_VIRTUAL

  // Implementation
  public:
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
  static int lua_getboundingbox(lua_State* L);
  static int lua_getprobleminfo(lua_State* L);
  static int lua_selectcircle(lua_State* L);
  static int lua_selectrectangle(lua_State* L);
  virtual ~CbeladrawDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
  // Generated message map functions
  protected:
  //{{AFX_MSG(CbeladrawDoc)
  afx_msg void OnDefineProblem();
  afx_msg void OnEditMatprops();
  afx_msg void OnEditPtprops();
  afx_msg void OnEditSegprops();
  afx_msg void OnEditCircprops();
  afx_msg void OnEditExterior();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
};

/////////////////////////////////////////////////////////////////////////////
char* StripKey(char* c);
char* ParseDbl(char* t, double* f);
char* ParseInt(char* t, int* f);