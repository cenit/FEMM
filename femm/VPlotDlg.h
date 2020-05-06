#if !defined(AFX_VPLOTDLG_H__2CA08017_4A4D_457C_B3B3_A2F29FB62CAE__INCLUDED_)
#define AFX_VPLOTDLG_H__2CA08017_4A4D_457C_B3B3_A2F29FB62CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VPlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVPlotDlg dialog

class CVPlotDlg : public CDialog {
  // Construction
  public:
  CVPlotDlg(CWnd* pParent = NULL); // standard constructor

  int m_plottype;
  int ListType;
  CLuaEdit m_IDC_vectorscalefactor;

  // Dialog Data
  //{{AFX_DATA(CVPlotDlg)
  enum { IDD = IDD_VPLOTDLG };
  CComboBox m_vplottype;
  double m_vectorscalefactor;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CVPlotDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CVPlotDlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VPLOTDLG_H__2CA08017_4A4D_457C_B3B3_A2F29FB62CAE__INCLUDED_)
