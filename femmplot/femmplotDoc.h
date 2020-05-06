// femmplotDoc.h : interface of the CFemmplotDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CFemmplotDoc : public CDocument {
  protected: // create from serialization only
  CFemmplotDoc();
  DECLARE_DYNCREATE(CFemmplotDoc)

  // Attributes
  public:
  HENHMETAFILE hMetaPlot;
  double bb_top, bb_bottom, bb_left, bb_right;
  BOOL HasBoundingBox;

  // Operations
  public:
  BOOL CFemmplotDoc::GrabMetafile();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFemmplotDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  //}}AFX_VIRTUAL

  // Implementation
  public:
  virtual ~CFemmplotDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
  // Generated message map functions
  protected:
  //{{AFX_MSG(CFemmplotDoc)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
