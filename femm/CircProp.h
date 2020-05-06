// CircProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCircProp dialog

class CCircProp : public CDialog {
  // Construction
  public:
  CCircProp(CWnd* pParent = NULL); // standard constructor

  int m_circtype;
  CArray<CString, CString&> namelist;

  // Dialog Data
  //{{AFX_DATA(CCircProp)
  enum { IDD = IDD_CIRCPROP };
  CString m_circname;
  CComplex m_totcurrent;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCircProp)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CCircProp)
  afx_msg void OnRadioamp();
  afx_msg void OnRadiovolt();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  public:
  CLuaEdit m_idc_circname, m_idc_totcurrent_re;
};
