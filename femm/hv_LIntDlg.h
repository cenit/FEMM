// LIntDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hvCLIntDlg dialog

class hvCLIntDlg : public CDialog {
  // Construction
  public:
  hvCLIntDlg(CWnd* pParent = NULL); // standard constructor

  int linttype;

  // Dialog Data
  //{{AFX_DATA(hvCLIntDlg)
  enum { IDD = IDD_HV_LINEINT };
  CComboBox m_inttype;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(hvCLIntDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(hvCLIntDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
