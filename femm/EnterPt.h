// EnterPt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnterPt dialog

class CEnterPt : public CDialog {
  // Construction
  public:
  CEnterPt(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CEnterPt)
  enum { IDD = IDD_ENTERPT };
  double m_coord1;
  double m_coord2;
  CString m_label1;
  CString m_label2;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CEnterPt)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CEnterPt)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_coord1, m_IDC_coord2;
};
