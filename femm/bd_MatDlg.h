// bd_MatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bdCMatDlg dialog

class bdCMatDlg : public CDialog {
  // Construction
  public:
  bdCMatDlg(CWnd* pParent = NULL); // standard constructor
  CString BData, HData;
  int LamType;
  BOOL m_nlflag;
  CArray<CString, CString&> namelist;

  CFont symbfont;
  CFont bfont;

  // Dialog Data
  //{{AFX_DATA(bdCMatDlg)
  enum { IDD = IDD_BD_MATDLG };
  CStatic m_static4;
  CStatic m_static3;
  CString m_BlockName;
  double m_ex;
  double m_ey;
  double m_qv;
  CString m_mu1label;
  CString m_mu2label;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(bdCMatDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(bdCMatDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_EX, m_IDC_EY, m_IDC_BlockName, m_IDC_QV;
};
