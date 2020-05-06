// XYPlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hvCXYPlotDlg dialog

class hvCXYPlotDlg : public CDialog {
  // Construction
  public:
  hvCXYPlotDlg(CWnd* pParent = NULL); // standard constructor
  int ListType;
  int XYPlotType;
  int FileFormat;
  ChviewDoc* pDoc;

  // Dialog Data
  //{{AFX_DATA(hvCXYPlotDlg)
  enum { IDD = IDD_HV_XYPLOTDLG };
  CComboBox m_fileformat;
  CComboBox m_XYPlotType;
  BOOL m_ToFile;
  int m_npoints;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(hvCXYPlotDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(hvCXYPlotDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnToFileClicked();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_npoints;
};
