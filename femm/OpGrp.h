// OpGrp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpGrp dialog

class COpGrp : public CDialog {
  // Construction
  public:
  COpGrp(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(COpGrp)
  enum { IDD = IDD_OPGRPDLG };
  int m_ingroup;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COpGrp)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(COpGrp)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_ingroup;
};
