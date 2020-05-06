// cd_PtProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdCPtProp dialog

class cdCPtProp : public CDialog {
  // Construction
  public:
  cdCPtProp(CWnd* pParent = NULL); // standard constructor

  // variables CPrProp needs;
  int PropType;
  int ProblemType;
  CArray<CPointProp, CPointProp&>* pnodeproplist;
  CArray<CBoundaryProp, CBoundaryProp&>* plineproplist;
  CArray<CMaterialProp, CMaterialProp&>* pblockproplist;
  CArray<CCircuit, CCircuit&>* pcircproplist;

  // Dialog Data
  //{{AFX_DATA(cdCPtProp)
  enum { IDD = IDD_PTPROP };
  CComboBox m_namelist;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(cdCPtProp)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(cdCPtProp)
  afx_msg void OnAddProp();
  afx_msg void OnDelProp();
  afx_msg void OnModProp();
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
