#if !defined(AFX_OUTBOX_H__77718BBB_F827_4C67_935E_6C6AD331D19C__INCLUDED_)
#define AFX_OUTBOX_H__77718BBB_F827_4C67_935E_6C6AD331D19C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutBox.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// COutBox dialog

class COutBox : public CDialog {
  // Construction
  public:
  COutBox(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(COutBox)
  enum { IDD = IDD_OUTBOX };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COutBox)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(COutBox)
  afx_msg void OnClose();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTBOX_H__77718BBB_F827_4C67_935E_6C6AD331D19C__INCLUDED_)
