// femmviewDoc.h : interface of the CFemmviewDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "lua.h"
#include "luadebug.h"
#include "luaconsoledlg.h"

// #define APPROXGAP

extern CFemmApp theApp; //<DP>

class CFemmviewDoc : public CDocument {

  protected: // create from serialization only
  CFemmviewDoc();
  DECLARE_DYNCREATE(CFemmviewDoc)

  // Attributes
  public:
  // General problem attributes
  double Frequency;
  double Depth;
  int LengthUnits;
  double* LengthConv;
  BOOL ProblemType;
  BOOL Coords;
  CString ProblemNote;
  CString PrevSoln;
  int PrevType;
  BOOL bIncremental;
  BOOL FirstDraw;
  BOOL Smooth;
  BOOL bMultiplyDefinedLabels;
  int WeightingScheme;
  double extRo, extRi, extZo;

  double A_High, A_Low;
  double A_lb, A_ub;

  double PlotBounds[10][2];
  double d_PlotBounds[10][2];
  //	double  Br_High,Br_Low;
  //	double  Bi_High,Bi_Low;
  double B_High, B_Low;
  double H_High;
  //	double  B_lb, B_ub;
  //	double  Jr_High,Jr_Low;
  //	double  Ji_High,Ji_Low;
  //	double  J_High, J_Low;

  // Some default behaviors
  CString BinDir;
  int d_LineIntegralPoints;
  int d_WeightingScheme;
  BOOL d_ShiftH;
  BOOL bHasMask;

  // lists of nodes, segments, and block labels
  CArray<femmviewdata::CNode, femmviewdata::CNode&> nodelist;
  CArray<femmviewdata::CSegment, femmviewdata::CSegment&> linelist;
  CArray<femmviewdata::CBlockLabel, femmviewdata::CBlockLabel&> blocklist;
  CArray<femmviewdata::CArcSegment, femmviewdata::CArcSegment&> arclist;

  // CArrays containing the mesh information
  CArray<femmviewdata::CMeshNode, femmviewdata::CMeshNode&> meshnode;
  CArray<femmviewdata::CElement, femmviewdata::CElement&> meshelem;
  CArray<femmviewdata::CAirGapElement, femmviewdata::CAirGapElement&> agelist;

  // List of elements connected to each node;
  int* NumList;
  int** ConList;

  // lists of properties
  CArray<femmviewdata::CMaterialProp, femmviewdata::CMaterialProp&> blockproplist;
  CArray<femmviewdata::CBoundaryProp, femmviewdata::CBoundaryProp&> lineproplist;
  CArray<femmviewdata::CPointProp, femmviewdata::CPointProp&> nodeproplist;
  CArray<femmviewdata::CCircuit, femmviewdata::CCircuit&> circproplist;

  // list of points in a user-defined contour;
  CArray<CComplex, CComplex&> contour;

  // stuff that PTLOC needs
  CArray<CMeshNode, CMeshNode&>* pmeshnode;
  CArray<CElement, CElement&>* pmeshelem;
  //	TriEdge recenttri;
  //	int samples;
  //	unsigned long randomseed;
  //	TriEdge bdrylinkhead[512];
  //	int numberofbdrylink;

  // member functions
  int InTriangle(double x, double y);
  BOOL InTriangleTest(double x, double y, int i);
  BOOL GetPointValues(double x, double y, CPointVals& u);
  BOOL GetPointValues(double x, double y, int k, CPointVals& u);
  void GetLineValues(CXYPlot& p, int PlotType, int npoints);
  void GetGapValues(CXYPlot& p, int PlotType, int npoints, int myAGE);
  void GetElementB(femmviewdata::CElement& elm);
  void OnReload();
  void FindBoundaryEdges();
  int ClosestNode(double x, double y);
  int ClosestSegment(double x, double y);
  double ShortestDistance(double p, double q, int segm);
  CComplex Ctr(int i);
  double ElmArea(int i);
  double ElmArea(femmviewdata::CElement* elm);
  void GetPointB(double x, double y, CComplex& B1, CComplex& B2,
      femmviewdata::CElement& elm);
  void GetNodalB(CComplex* b1, CComplex* b2, femmviewdata::CElement& elm);
  CComplex BlockIntegral(int inttype);
  void LineIntegral(int inttype, CComplex* z);
  int ClosestArcSegment(double x, double y);
  void GetCircle(femmviewdata::CArcSegment& asegm, CComplex& c, double& R);
  double ShortestDistanceFromArc(CComplex p, femmviewdata::CArcSegment& arc);
  double ShortestDistanceFromSegment(double p, double q, int segm);
  CComplex GetJA(int k, CComplex* J, CComplex* A);
  CComplex PlnInt(double a, CComplex* u, CComplex* v);
  CComplex AxiInt(double a, CComplex* u, CComplex* v, double* r);
  BOOL ScanPreferences();
  void BendContour(double angle, double anglestep);
  BOOL MakeMask();
  CComplex HenrotteVector(int k);
  BOOL IsKosher(int k);
  double AECF(int k);
  void GetFillFactor(int lbl);

  CComplex GetStrandedVoltageDrop(int lbl);
  CComplex GetVoltageDrop(int circnum);
  CComplex GetFluxLinkage(int circnum);
  CComplex GetStrandedLinkage(int lbl);
  CComplex GetSolidAxisymmetricLinkage(int lbl);
  CComplex GetParallelLinkage(int numcirc);
  CComplex GetParallelLinkageAlt(int numcirc);
  void GetMu(CComplex b1, CComplex b2, CComplex& mu1, CComplex& mu2, int i);
  void GetMu(double b1, double b2, double& mu1, double& mu2, int i);
  void GetMagnetization(int n, CComplex& M1, CComplex& M2);
  void GetH(double b1, double b2, double& h1, double& h2, int k);
  void GetH(CComplex b1, CComplex b2, CComplex& h1, CComplex& h2, int k);

  // Operations
  public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFemmviewDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  //}}AFX_VIRTUAL

  // Implementation
  public:
  // lua extensions
  bool luafired;
  void initalise_lua();
  static int lua_dumpheader(lua_State* L);
  static int lua_getpointvals(lua_State* L);
  static int lua_exitpost(lua_State* L);
  static int lua_addcontour(lua_State* L);
  static int lua_clearcontour(lua_State* L);
  static int lua_lineintegral(lua_State* L);
  static int lua_selectblock(lua_State* L);
  static int lua_groupselectblock(lua_State* L);
  static int lua_blockintegral(lua_State* L);
  static int lua_clearblock(lua_State* L);
  static int lua_zoomout(lua_State* L);
  static int lua_zoomin(lua_State* L);
  static int lua_zoomnatural(lua_State* L);
  static int lua_hidemesh(lua_State* L);
  static int lua_showmesh(lua_State* L);
  static int lua_showgrid(lua_State* L);
  static int lua_hidegrid(lua_State* L);
  static int lua_showdensity(lua_State* L);
  static int lua_hidedensity(lua_State* L);
  static int lua_hidecountour(lua_State* L);
  static int lua_showcountour(lua_State* L);
  static int lua_zoom(lua_State* L);
  static int lua_smoothing(lua_State* L);
  static int lua_hidepoints(lua_State* L);
  static int lua_showpoints(lua_State* L);
  static int lua_gridsnap(lua_State* L);
  static int lua_setgrid(lua_State* L);
  static int lua_getprobleminfo(lua_State* L);
  static int lua_savebitmap(lua_State* L);
  PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
  void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
  static int lua_getcircuitprops(lua_State* L);
  static int lua_saveWMF(lua_State* L);
  static int lua_refreshview(lua_State* L);
  static int lua_selectline(lua_State* L);
  static int lua_seteditmode(lua_State* L);
  static int lua_bendcontour(lua_State* L);
  static int lua_makeplot(lua_State* L);
  static int lua_shownames(lua_State* L);
  static int lua_vectorplot(lua_State* L);
  static int lua_gradient(lua_State* L);
  static int lua_reload(lua_State* L);
  static int lua_switchfocus(lua_State* L);
  static int luaResize(lua_State* L);
  static int luaMinimize(lua_State* L);
  static int luaMaximize(lua_State* L);
  static int luaRestore(lua_State* L);
  static int lua_gettitle(lua_State* L);
  static int lua_weighting(lua_State* L);

  // commands to access low-level information through Lua
  static int lua_numnodes(lua_State* L);
  static int lua_numelements(lua_State* L);
  static int lua_getnode(lua_State* L);
  static int lua_getelement(lua_State* L);

  // functions used for compatibility with older versions
  static int old_lua_lineintegral(lua_State* L);
  static int old_lua_getpointvals(lua_State* L);
  static int old_lua_blockintegral(lua_State* L);
  static int old_lua_getcircuitprops(lua_State* L);

  // functions used to query air gap elements
  static int lua_getAGEflux(lua_State* L);
  static int lua_gapintegral(lua_State* L);
  static int lua_getgapa(lua_State* L);
  static int lua_getgapharmonics(lua_State* L);

  virtual ~CFemmviewDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
  // Generated message map functions
  protected:
  //{{AFX_MSG(CFemmviewDoc)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  public:
};

/////////////////////////////////////////////////////////////////////////////
char* StripKey(char* c);
