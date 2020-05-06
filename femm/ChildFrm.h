// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__5DA42274_DC99_4A3F_A918_C850B3925734__INCLUDED_)
#define AFX_CHILDFRM_H__5DA42274_DC99_4A3F_A918_C850B3925734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChildFrame : public CMDIChildWnd {
  DECLARE_DYNCREATE(CChildFrame)
  public:
  CChildFrame();
  BOOL ResizeClient(int nWidth, int nHeight, BOOL bRedraw = FALSE);

  // Attributes
  public:
  // Operations
  public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CChildFrame)
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void ActivateFrame(int nCmdShow);
  //}}AFX_VIRTUAL

  // Implementation
  public:
  virtual ~CChildFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  protected:
  //{{AFX_MSG(CChildFrame)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__5DA42274_DC99_4A3F_A918_C850B3925734__INCLUDED_)
