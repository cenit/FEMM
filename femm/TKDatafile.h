// TKDatafile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTKDatafile dialog

class CTKDatafile : public CDialog {
  // Construction
  public:
  CTKDatafile(CWnd* pParent = NULL); // standard constructor
  int TKOrder, TUnits, KUnits;

  // Dialog Data
  //{{AFX_DATA(CTKDatafile)
  enum { IDD = IDD_TKDATAFILE };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTKDatafile)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CTKDatafile)
  virtual BOOL OnInitDialog();
  afx_msg void OnTfirst();
  afx_msg void OnKfirst();
  afx_msg void OnKelvin();
  afx_msg void OnCelcius();
  afx_msg void OnFahrenheit();
  afx_msg void OnRankine();
  afx_msg void OnWMK();
  afx_msg void OnBTU();
  afx_msg void OnKCAL();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
