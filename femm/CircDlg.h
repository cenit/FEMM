// CircDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCircDlg dialog

class CCircDlg : public CDialog
{
// Construction
public:
	CCircDlg(CWnd* pParent = NULL);   // standard constructor

	CFemmviewDoc *TheDoc;
	int NumCircuits;

// Dialog Data
	//{{AFX_DATA(CCircDlg)
	enum { IDD = IDD_CIRCPROPS };
	CComboBox	m_circname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCircDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCircDlg)
	afx_msg void OnSelchangeCircname();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
