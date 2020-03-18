// BlockInt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hvCBlockInt dialog

class hvCBlockInt : public CDialog
{
// Construction
public:
	hvCBlockInt(CWnd* pParent = NULL);   // standard constructor

	int binttype;

// Dialog Data
	//{{AFX_DATA(hvCBlockInt)
	enum { IDD = IDD_HV_BLOCKINT };
	CComboBox	m_binttype;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hvCBlockInt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(hvCBlockInt)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
