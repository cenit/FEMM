// fknDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFknDlg dialog

class CFknDlg : public CDialog {
  // Construction
  public:
  CFknDlg(CWnd* pParent = NULL); // standard constructor
  CString ComLine;

  // Dialog Data
  //{{AFX_DATA(CFknDlg)
  enum { IDD = IDD_FKN_DIALOG };
  CProgressCtrl m_prog2;
  CProgressCtrl m_prog1;
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFknDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  HICON m_hIcon;

  // Generated message map functions
  //{{AFX_MSG(CFknDlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
