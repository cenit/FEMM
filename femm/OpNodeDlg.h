// OpNodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpNodeDlg dialog

class COpNodeDlg : public CDialog {
  // Construction
  public:
  COpNodeDlg(CWnd* pParent = NULL); // standard constructor
  int cursel;
  CArray<CPointProp, CPointProp&>* pnodeproplist;

  // Dialog Data
  //{{AFX_DATA(COpNodeDlg)
  enum { IDD = IDD_OPNODEDLG };
  CComboBox m_acknode;
  int m_ingroup;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COpNodeDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(COpNodeDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_ingroup;
};
