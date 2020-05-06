// ScaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog

class CScaleDlg : public CDialog {
  // Construction
  public:
  CScaleDlg(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CScaleDlg)
  enum { IDD = IDD_SCALE };
  double m_basex;
  double m_basey;
  double m_scalefactor;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CScaleDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CScaleDlg)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_basex, m_IDC_basey, m_IDC_scalefactor;
};
