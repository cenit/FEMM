// CopyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg dialog

class CCopyDlg : public CDialog {
  // Construction
  public:
  CCopyDlg(CWnd* pParent = NULL); // standard constructor
  int BtnState;
  BOOL IsMove;

  // Dialog Data
  //{{AFX_DATA(CCopyDlg)
  enum { IDD = IDD_COPYDLG };
  double m_aboutx;
  double m_abouty;
  double m_deltax;
  double m_deltay;
  double m_shiftangle;
  int m_ncopies;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCopyDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CCopyDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnRotate();
  afx_msg void OnTranslate();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_aboutx, m_IDC_abouty, m_IDC_deltax;
  CLuaEdit m_IDC_deltay, m_IDC_shiftangle, m_IDC_ncopies;
};
