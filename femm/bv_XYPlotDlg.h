// XYPlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bvCXYPlotDlg dialog

class bvCXYPlotDlg : public CDialog {
  // Construction
  public:
  bvCXYPlotDlg(CWnd* pParent = NULL); // standard constructor
  int ListType;
  int XYPlotType;
  int FileFormat;
  CbelaviewDoc* pDoc;

  // Dialog Data
  //{{AFX_DATA(bvCXYPlotDlg)
  enum { IDD = IDD_BV_XYPLOTDLG };
  CComboBox m_fileformat;
  CComboBox m_XYPlotType;
  BOOL m_ToFile;
  int m_npoints;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(bvCXYPlotDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(bvCXYPlotDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnToFileClicked();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_npoints;
};
