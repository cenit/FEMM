// OpNodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdCOpNodeDlg dialog

class cdCOpNodeDlg : public CDialog {
  // Construction
  public:
  cdCOpNodeDlg(CWnd* pParent = NULL); // standard constructor
  int cursel;
  int condsel;
  CArray<CPointProp, CPointProp&>* pnodeproplist;
  CArray<CCircuit, CCircuit&>* pcircproplist;

  // Dialog Data
  //{{AFX_DATA(cdCOpNodeDlg)
  enum { IDD = IDD_CD_OPNODEDLG };
  CComboBox m_nodecond;
  CComboBox m_acknode;
  int m_ingroup;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(cdCOpNodeDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(cdCOpNodeDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_ingroup;
};
