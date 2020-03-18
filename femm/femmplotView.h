// femmplotView.h : interface of the CFemmplotView class
//
/////////////////////////////////////////////////////////////////////////////

class CFemmplotView : public CView
{
protected: // create from serialization only
	CFemmplotView();
	DECLARE_DYNCREATE(CFemmplotView)

// Attributes
public:
	CStatusBar *StatBar;
	CFemmplotDoc* GetDocument();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFemmplotView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
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
	virtual ~CFemmplotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFemmplotView)
	afx_msg void OnEditCopy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopyAsMetafile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in femmplotView.cpp
inline CFemmplotDoc* CFemmplotView::GetDocument()
   { return (CFemmplotDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
