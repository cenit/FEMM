// CircDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hvCCircDlg dialog

class hvCCircDlg : public CDialog
{
// Construction
public:
	hvCCircDlg(CWnd* pParent = NULL);   // standard constructor

	ChviewDoc *TheDoc;
	int NumCircuits;

// Dialog Data
	//{{AFX_DATA(hvCCircDlg)
	enum { IDD = IDD_HV_CIRCPROPS };
	CComboBox	m_circname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hvCCircDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(hvCCircDlg)
	afx_msg void OnSelchangeCircname();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
