// OpSegDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpSegDlg dialog

class COpSegDlg : public CDialog {
  // Construction
  public:
  COpSegDlg(CWnd* pParent = NULL); // standard constructor
  int cursel;
  CArray<CBoundaryProp, CBoundaryProp&>* plineproplist;

  // Dialog Data
  //{{AFX_DATA(COpSegDlg)
  enum { IDD = IDD_OPSEGDLG };
  CComboBox m_ackseg;
  double m_linemeshsize;
  BOOL m_automesh;
  BOOL m_hide;
  int m_ingroup;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COpSegDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(COpSegDlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnAutomesh();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_ingroup, m_IDC_linemeshsize;
};
