// OpBlkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpBlkDlg dialog

class COpBlkDlg : public CDialog {
  // Construction
  public:
  COpBlkDlg(CWnd* pParent = NULL); // standard constructor
  int cursel, circsel;
  int ProblemType;
  CString m_magdirfctn;
  CArray<CMaterialProp, CMaterialProp&>* pblockproplist;
  CArray<CCircuit, CCircuit&>* pcircproplist;

  // Dialog Data
  //{{AFX_DATA(COpBlkDlg)
  enum { IDD = IDD_OPBLKDLG };
  CButton m_automesh;
  CComboBox m_circblk;
  CComboBox m_ackblk;
  BOOL m_isexternal;
  BOOL m_isdefault;
  double m_sidelength;
  double m_magdir;
  int m_turns;
  int m_ingroup;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COpBlkDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(COpBlkDlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnSelchangeAckblk();
  afx_msg void OnSelchangeCircblk();
  afx_msg void OnAutomeshcheck();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_sidelength;
  CLuaEdit m_IDC_magdir;
  CLuaEdit m_IDC_turns;
  CLuaEdit m_IDC_ingroup;
};
