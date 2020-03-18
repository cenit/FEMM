// XYPlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXYPlotDlg dialog

class CXYPlotDlg : public CDialog
{
// Construction
public:
	CXYPlotDlg(CWnd* pParent = NULL);   // standard constructor
	int ListType;
	int XYPlotType;
	int FileFormat;
	CFemmviewDoc *pDoc;

// Dialog Data
	//{{AFX_DATA(CXYPlotDlg)
	enum { IDD = IDD_XYPLOTDLG };
	CComboBox	m_fileformat;
	CComboBox	m_XYPlotType;
	BOOL	m_ToFile;
	int		m_npoints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXYPlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXYPlotDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToFileClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_npoints;
};
