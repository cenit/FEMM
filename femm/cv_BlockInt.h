// BlockInt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cvCBlockInt dialog

class cvCBlockInt : public CDialog {
  // Construction
  public:
  cvCBlockInt(CWnd* pParent = NULL); // standard constructor

  int binttype;
  double Frequency;

  // Dialog Data
  //{{AFX_DATA(cvCBlockInt)
  enum { IDD = IDD_CV_BLOCKINT };
  CComboBox m_binttype;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(cvCBlockInt)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(cvCBlockInt)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
