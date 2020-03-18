// hsolvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ChsolvDlg dialog

class ChsolvDlg : public CDialog
{
// Construction
public:
	ChsolvDlg(CWnd* pParent = NULL);	// standard constructor
	CString ComLine;

// Dialog Data
	//{{AFX_DATA(ChsolvDlg)
	enum { IDD = IDD_HSOLV_DIALOG };
	CProgressCtrl	m_prog2;
	CProgressCtrl	m_prog1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChsolvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(ChsolvDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
