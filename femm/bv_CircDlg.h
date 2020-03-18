// CircDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bvCCircDlg dialog

class bvCCircDlg : public CDialog
{
// Construction
public:
	bvCCircDlg(CWnd* pParent = NULL);   // standard constructor

	CbelaviewDoc *TheDoc;
	int NumCircuits;

// Dialog Data
	//{{AFX_DATA(bvCCircDlg)
	enum { IDD = IDD_BV_CIRCPROPS };
	CComboBox	m_circname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bvCCircDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bvCCircDlg)
	afx_msg void OnSelchangeCircname();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
