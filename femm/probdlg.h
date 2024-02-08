// probdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// probdlg dialog

class probdlg : public CDialog {
  // Construction
  public:
  probdlg(CWnd* pParent = NULL); // standard constructor

  BOOL probtype; // variable denoting problem type
                 // so list entry can be correctly set
  int lengthunits; // contains units of length;
  int bsmart;
  int prevtype; // contains previous solution type for incremental/frozen perm problems
  int solver; // contains the solver type to be used in AC problems.

  // Dialog Data
  //{{AFX_DATA(probdlg)
  enum { IDD = IDD_PROBDLG };
  CComboBox m_solver;
  CComboBox m_length_units;
  CComboBox m_bsmart;
  CComboBox m_probtype;
  CComboBox m_prevtype;
  double m_frequency;
  CString m_problem_note;
  double m_precision;
  double m_minangle;
  double m_depth;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(probdlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(probdlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnSelchangeLengthUnits();
  afx_msg void OnSelchangeProbtype();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_frequency, m_IDC_problem_note, m_IDC_precision, m_IDC_depth, m_IDC_minangle;
  CString m_prevsoln;
};
