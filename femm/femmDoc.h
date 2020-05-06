// femmDoc.h : interface of the CFemmDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMMDOC_H__8889277C_B515_4580_AEAA_E5095D7D94C9__INCLUDED_)
#define AFX_FEMMDOC_H__8889277C_B515_4580_AEAA_E5095D7D94C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFemmDoc : public CDocument {
  protected: // create from serialization only
  CFemmDoc();
  DECLARE_DYNCREATE(CFemmDoc)

  // Attributes
  public:
  // Operations
  public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFemmDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  //}}AFX_VIRTUAL

  // Implementation
  public:
  virtual ~CFemmDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
  // Generated message map functions
  protected:
  //{{AFX_MSG(CFemmDoc)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEMMDOC_H__8889277C_B515_4580_AEAA_E5095D7D94C9__INCLUDED_)
