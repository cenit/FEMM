// femmviewView.h : interface of the CFemmviewView class
//
/////////////////////////////////////////////////////////////////////////////

class CFemmviewView : public CView {
  protected: // create from serialization only
  CFemmviewView();
  DECLARE_DYNCREATE(CFemmviewView)

  // Attributes
  public:
  CFemmviewDoc* GetDocument();

  void ScreenToDwg(int xs, int ys, double* xd, double* yd, RECT* r);
  BOOL DwgToScreen(double xd, double yd, int* xs, int* ys, RECT* r);
  void EnterPoint();

  int EditAction; // tells if operating on points, lines, or blox
  double mx, my; // location of the mouse in model coordinates
  double ox, oy, mag; // location of lower left corner & scaling factor
  CStatusBar* StatBar; // pointer to the main window's status bar
  double GridSize; // size of each block in the grid
  BOOL GridFlag; // Flag telling whether or not to show grid
  BOOL SnapFlag;
  BOOL MeshFlag;
  BOOL LegendFlag;
  BOOL ShowNames;
  BOOL GreyContours;
  BOOL PtsFlag;
  BOOL Coords; // False=Cartesian; True=Polar
  int ZoomWndFlag;
  int DrawSelected;
  int VectorPlot;
  double VectorScaleFactor;
  double wzx, wzy;

  //  default behaviors
  int d_EditAction;
  BOOL d_GridFlag;
  BOOL d_SnapFlag;
  BOOL d_MeshFlag;
  BOOL d_LegendFlag;
  BOOL d_GreyContours;
  int d_NumContours;
  BOOL d_ShowAr;
  BOOL d_ShowAi;
  BOOL d_ShowMask;
  int d_DensityPlot;
  BOOL d_PtsFlag;
  BOOL d_ResetOnReload;
  BOOL d_Smooth;
  BOOL d_ShowNames;
  BOOL d_PlotPoints;
  int d_VectorPlot;

  COutBox* Dspl;
  CString BinDir; // pathname for triangle.exe

  int NumContours;
  BOOL ShowAr, ShowAi, ShowMask, bOnDraw;
  int DensityPlot;
  CString OutputWindowText;

  CArray<CPixel, CPixel&> LinePix;
  CArray<CZPix, CZPix&> ZoomPix;

  // Greyscale Density Plot Colormap
  COLORREF Grey00;
  COLORREF Grey01;
  COLORREF Grey02;
  COLORREF Grey03;
  COLORREF Grey04;
  COLORREF Grey05;
  COLORREF Grey06;
  COLORREF Grey07;
  COLORREF Grey08;
  COLORREF Grey09;
  COLORREF Grey10;
  COLORREF Grey11;
  COLORREF Grey12;
  COLORREF Grey13;
  COLORREF Grey14;
  COLORREF Grey15;
  COLORREF Grey16;
  COLORREF Grey17;
  COLORREF Grey18;
  COLORREF Grey19;

  // Density Plot Colormap
  COLORREF Color00;
  COLORREF Color01;
  COLORREF Color02;
  COLORREF Color03;
  COLORREF Color04;
  COLORREF Color05;
  COLORREF Color06;
  COLORREF Color07;
  COLORREF Color08;
  COLORREF Color09;
  COLORREF Color10;
  COLORREF Color11;
  COLORREF Color12;
  COLORREF Color13;
  COLORREF Color14;
  COLORREF Color15;
  COLORREF Color16;
  COLORREF Color17;
  COLORREF Color18;
  COLORREF Color19;

  // Other colors
  COLORREF SelColor;
  COLORREF MeshColor;
  COLORREF BlockColor;
  COLORREF LineColor;
  COLORREF RegionColor;
  COLORREF GridColor;
  COLORREF BackColor;
  COLORREF TextColor;
  COLORREF NodeColor;
  COLORREF RealFluxLineColor;
  COLORREF ImagFluxLineColor;
  COLORREF MaskLineColor;
  COLORREF NameColor;
  COLORREF RealVectorColor;
  COLORREF ImagVectorColor;

  // Operations
  public:
  void DoContours(CDC* pDC, int* p, int side, int Aflag);
  void PlotFluxDensity(CDC* pDC, int elmnum, int flag);
  void DisplayPointProperties(double px, double py);
  void DrawUserContour(BOOL flag);
  void EraseUserContour(BOOL flag);
  void SpecialLine(CDC* pDC, int x0, int y0, int x1, int y1, BOOL flag);
  void PlotSelectedElm(CDC* pDC, CElement& elm);
  void RedrawView();
  BOOL IsMinimized();
  BOOL ScanPreferences();
  BOOL WritePreferences();
  void CheckIt();
  void LuaViewInfo();

  void MyMoveTo(CDC* pDC, int x, int y);
  void MyLineTo(CDC* pDC, int x, int y);

  BOOL Pump();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFemmviewView)
  public:
  virtual void OnDraw(CDC* pDC); // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void OnInitialUpdate();
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

  protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
  //}}AFX_VIRTUAL

  // Implementation
  public:
  virtual ~CFemmviewView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
  // Generated message map functions
  protected:
  //{{AFX_MSG(CFemmviewView)
  afx_msg void OnZoomIn();
  afx_msg void OnSnapGrid();
  afx_msg void OnShowMesh();
  afx_msg void OnShowGrid();
  afx_msg void OnSetGrid();
  afx_msg void OnCplot();
  afx_msg void OnDplot();
  afx_msg void OnZoomNatural();
  afx_msg void OnZoomOut();
  afx_msg void OnZoomWindow();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnEditCopy();
  afx_msg void OnReload();
  afx_msg void OnMenuArea();
  afx_msg void OnMenuContour();
  afx_msg void OnMenuPoint();
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnMenuPlot();
  afx_msg void OnMenuIntegrate();
  afx_msg void OnMenushowpts();
  afx_msg void OnKbdZoom();
  afx_msg void OnSmooth();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnPanDown();
  afx_msg void OnPanLeft();
  afx_msg void OnPanRight();
  afx_msg void OnPanUp();
  afx_msg void OnViewCircprops();
  afx_msg void OnEditCopyAsMetafile();
  afx_msg void OnViewBHcurves();
  afx_msg void OnViewInfo();
  afx_msg void OnViewShownames();
  afx_msg void OnVplot();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG // debug version in femmviewView.cpp
inline CFemmviewDoc* CFemmviewView::GetDocument()
{
  return (CFemmviewDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////
