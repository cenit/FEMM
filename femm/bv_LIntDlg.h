// LIntDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bvCLIntDlg dialog

class bvCLIntDlg : public CDialog
{
// Construction
public:
	bvCLIntDlg(CWnd* pParent = NULL);   // standard constructor

	int linttype;

// Dialog Data
	//{{AFX_DATA(bvCLIntDlg)
	enum { IDD = IDD_BV_LINEINT };
	CComboBox	m_inttype;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bvCLIntDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bvCLIntDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
