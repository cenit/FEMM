// BlockInt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlockInt dialog

class CBlockInt : public CDialog {
  // Construction
  public:
  CBlockInt(CWnd* pParent = NULL); // standard constructor

  int binttype;

  // Dialog Data
  //{{AFX_DATA(CBlockInt)
  enum { IDD = IDD_BLOCKINT };
  CComboBox m_binttype;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBlockInt)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CBlockInt)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
