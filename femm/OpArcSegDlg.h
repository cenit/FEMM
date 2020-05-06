// OpArcSegDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpArcSegDlg dialog

class COpArcSegDlg : public CDialog {
  // Construction
  public:
  COpArcSegDlg(CWnd* pParent = NULL); // standard constructor
  int cursel;
  CArray<CBoundaryProp, CBoundaryProp&>* plineproplist;

  // Dialog Data
  //{{AFX_DATA(COpArcSegDlg)
  enum { IDD = IDD_OPARCSEGDLG };
  CComboBox m_ArcSegBdry;
  double m_MaxSeg;
  BOOL m_hide;
  int m_ingroup;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COpArcSegDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(COpArcSegDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_ingroup, m_IDC_MaxSeg;
};
