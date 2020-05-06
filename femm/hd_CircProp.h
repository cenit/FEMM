// hd_CircProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hdCCircProp dialog

class hdCCircProp : public CDialog {
  // Construction
  public:
  hdCCircProp(CWnd* pParent = NULL); // standard constructor

  int m_circtype;
  CFont bfont;
  CArray<CString, CString&> namelist;

  // Dialog Data
  //{{AFX_DATA(hdCCircProp)
  enum { IDD = IDD_HD_CIRCPROP };
  CString m_circname;
  double m_v;
  double m_q;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(hdCCircProp)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(hdCCircProp)
  afx_msg void OnRadioamp();
  afx_msg void OnRadiovolt();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  public:
  CLuaEdit m_idc_circname;
  CLuaEdit m_idc_v;
  CLuaEdit m_idc_q;
};
