#if !defined(AFX_EXTERIORPROPS_H__1854BB86_0B6A_4F9B_9407_F5D2B109EF17__INCLUDED_)
#define AFX_EXTERIORPROPS_H__1854BB86_0B6A_4F9B_9407_F5D2B109EF17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExteriorProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExteriorProps dialog

class CExteriorProps : public CDialog {
  // Construction
  public:
  CExteriorProps(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CExteriorProps)
  enum { IDD = IDD_EXTERIORPROPS };
  double m_Ri;
  double m_Ro;
  double m_Zo;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CExteriorProps)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CExteriorProps)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  public:
  CLuaEdit m_IDC_RO, m_IDC_RI, m_IDC_ZO;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERIORPROPS_H__1854BB86_0B6A_4F9B_9407_F5D2B109EF17__INCLUDED_)
