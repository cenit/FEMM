// Pref.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPref dialog

class CPref : public CDialog {
  // Construction
  public:
  CPref(CWnd* pParent = NULL); // standard constructor
  ~CPref();
  COLORREF* clist;
  int s_action;
  int s_coord;
  int s_length;
  int s_type;
  int s_solver;

  void ScanPrefs();
  void WritePrefs();

  // Dialog Data
  //{{AFX_DATA(CPref)
  enum { IDD = IDD_EDITPREF };
  CComboBox m_d_solver;
  CComboBox m_d_color;
  CComboBox m_d_type;
  CComboBox m_d_length;
  CComboBox m_d_coord;
  CComboBox m_d_action;
  double m_d_freq;
  double m_d_gridsize;
  double m_d_pixels;
  double m_d_depth;
  double m_d_prec;
  double m_d_minangle;
  BOOL m_d_showgrid;
  BOOL m_d_snapgrid;
  BOOL m_d_showorigin;
  BOOL m_d_shownames;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPref)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CPref)
  afx_msg void OnModifyButton();
  virtual BOOL OnInitDialog();
  afx_msg void OnRestoreColors();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  public:
  CLuaEdit m_IDC_d_freq, m_IDC_d_gridsize;
  CLuaEdit m_IDC_d_pixels, m_IDC_d_prec;
  CLuaEdit m_IDC_d_depth;
  CLuaEdit m_IDC_d_minangle;
};
