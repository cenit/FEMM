// Pref.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hvCPref dialog

class hvCPref : public CDialog {
  // Construction
  public:
  hvCPref(CWnd* pParent = NULL); // standard constructor
  ~hvCPref();

  COLORREF* clist;
  int m_d_EditAction;
  int m_d_DensityPlot;
  int m_d_VectorPlot;

  void ScanPrefs();
  void WritePrefs();

  // Dialog Data
  //{{AFX_DATA(hvCPref)
  enum { IDD = IDD_HV_PREF };
  CComboBox m_d_vplotlist;
  CComboBox m_d_dplotlist;
  CComboBox m_d_editlist;
  CComboBox m_d_color;
  BOOL m_d_GreyContours;
  BOOL m_d_LegendFlag;
  int m_d_NumContours;
  BOOL m_d_ResetOnReload;
  BOOL m_d_GridFlag;
  BOOL m_d_ShowAr;
  BOOL m_d_PtsFlag;
  BOOL m_d_MeshFlag;
  BOOL m_d_SnapFlag;
  BOOL m_d_Smooth;
  int m_d_LineIntegralPoints;
  int m_d_PlotPoints;
  BOOL m_d_ShowMask;
  BOOL m_d_shownames;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(hvCPref)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(hvCPref)
  afx_msg void OnModBtn();
  virtual BOOL OnInitDialog();
  afx_msg void OnResetColors();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_d_NumContours, m_IDC_d_LineIntegralPoints;
  CLuaEdit m_IDC_d_PlotPoints;
};
