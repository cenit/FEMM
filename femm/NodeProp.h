// NodeProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNodeProp dialog

class CNodeProp : public CDialog {
  // Construction
  public:
  CNodeProp(CWnd* pParent = NULL); // standard constructor

  CFont bfont;
  CArray<CString, CString&> namelist;

  // Dialog Data
  //{{AFX_DATA(CNodeProp)
  enum { IDD = IDD_NODEPROP };
  CComplex m_ap;
  CComplex m_jp;
  CString m_nodename;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CNodeProp)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CNodeProp)
  afx_msg void OnSetA();
  afx_msg void OnSetI();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_a_re, m_IDC_j_re, m_IDC_nodename;
};
