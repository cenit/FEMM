#if !defined(AFX_MASKPROGRESS_H__C617543A_0490_40D7_BF6F_DD1C35A4A4C0__INCLUDED_)
#define AFX_MASKPROGRESS_H__C617543A_0490_40D7_BF6F_DD1C35A4A4C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaskProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaskProgress dialog

class CMaskProgress : public CDialog {
  // Construction
  public:
  CMaskProgress(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CMaskProgress)
  enum { IDD = IDD_MASKPROGRESS };
  CProgressCtrl m_mask_progress_status;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMaskProgress)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CMaskProgress)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASKPROGRESS_H__C617543A_0490_40D7_BF6F_DD1C35A4A4C0__INCLUDED_)
