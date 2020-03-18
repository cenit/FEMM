// XYPlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cvCXYPlotDlg dialog

class cvCXYPlotDlg : public CDialog
{
// Construction
public:
	cvCXYPlotDlg(CWnd* pParent = NULL);   // standard constructor
	int ListType;
	int XYPlotType;
	int FileFormat;
	CcviewDoc *pDoc;

// Dialog Data
	//{{AFX_DATA(cvCXYPlotDlg)
	enum { IDD = IDD_CV_XYPLOTDLG };
	CComboBox	m_fileformat;
	CComboBox	m_XYPlotType;
	BOOL	m_ToFile;
	int		m_npoints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cvCXYPlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cvCXYPlotDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToFileClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_npoints;
};
