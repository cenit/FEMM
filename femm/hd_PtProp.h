// hd_PtProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hdCPtProp dialog

class hdCPtProp : public CDialog {
  // Construction
  public:
  hdCPtProp(CWnd* pParent = NULL); // standard constructor

  // variables CPrProp needs;
  int PropType;
  int ProblemType;
  CArray<CPointProp, CPointProp&>* pnodeproplist;
  CArray<CBoundaryProp, CBoundaryProp&>* plineproplist;
  CArray<CMaterialProp, CMaterialProp&>* pblockproplist;
  CArray<CCircuit, CCircuit&>* pcircproplist;

  // Dialog Data
  //{{AFX_DATA(hdCPtProp)
  enum { IDD = IDD_PTPROP };
  CComboBox m_namelist;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(hdCPtProp)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(hdCPtProp)
  afx_msg void OnAddProp();
  afx_msg void OnDelProp();
  afx_msg void OnModProp();
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
