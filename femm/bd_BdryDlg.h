// bd_BdryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bdCBdryDlg dialog

class bdCBdryDlg : public CDialog {
  // Construction
  public:
  bdCBdryDlg(CWnd* pParent = NULL); // standard constructor

  int BdryFormat;
  CArray<CString, CString&> namelist;

  CFont symbfont;

  // Dialog Data
  //{{AFX_DATA(bdCBdryDlg)
  enum { IDD = IDD_BD_BDRYDLG };
  CComboBox m_BdryFormat;
  CString m_BdryName;
  double m_c0;
  double m_c1;
  double m_v;
  double m_qs;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(bdCBdryDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(bdCBdryDlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnSelchangeBdryformat();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  public:
  CLuaEdit m_IDC_VBDRY, m_IDC_QS, m_IDC_BDRYNAME;
  CLuaEdit m_IDC_c0, m_IDC_c1;
};
