// CircDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cvCCircDlg dialog

class cvCCircDlg : public CDialog {
  // Construction
  public:
  cvCCircDlg(CWnd* pParent = NULL); // standard constructor

  CcviewDoc* TheDoc;
  int NumCircuits;

  // Dialog Data
  //{{AFX_DATA(cvCCircDlg)
  enum { IDD = IDD_CV_CIRCPROPS };
  CComboBox m_circname;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(cvCCircDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(cvCCircDlg)
  afx_msg void OnSelchangeCircname();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
