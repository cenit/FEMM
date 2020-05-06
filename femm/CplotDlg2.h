// CplotDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCplotDlg2 dialog

class CCplotDlg2 : public CDialog {
  // Construction
  public:
  CCplotDlg2(CWnd* pParent = NULL); // standard constructor
  double Alb, Aub;

  // Dialog Data
  //{{AFX_DATA(CCplotDlg2)
  enum { IDD = IDD_CPLOTDLG2 };
  int m_numcontours;
  BOOL m_showa;
  double m_ahigh;
  double m_alow;
  BOOL m_showmask;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCplotDlg2)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CCplotDlg2)
  afx_msg void OnDflt1();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_numcontours, m_IDC_ahigh, m_IDC_alow;
};
