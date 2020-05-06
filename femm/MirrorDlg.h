// MirrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMirrorDlg dialog

#include "afxwin.h"
class CMirrorDlg : public CDialog {
  // Construction
  public:
  CMirrorDlg(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CMirrorDlg)
  enum { IDD = IDD_MIRRORDLG };
  double m_pax;
  double m_pay;
  double m_pbx;
  double m_pby;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMirrorDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CMirrorDlg)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_PAX, m_IDC_PAY;
  CLuaEdit m_IDC_PBX, m_IDC_PBY;
};
