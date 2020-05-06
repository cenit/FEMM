// LIntDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cvCLIntDlg dialog

class cvCLIntDlg : public CDialog {
  // Construction
  public:
  cvCLIntDlg(CWnd* pParent = NULL); // standard constructor

  int linttype;
  double Frequency;

  // Dialog Data
  //{{AFX_DATA(cvCLIntDlg)
  enum { IDD = IDD_CV_LINEINT };
  CComboBox m_inttype;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(cvCLIntDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(cvCLIntDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
