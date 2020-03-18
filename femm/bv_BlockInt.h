// BlockInt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bvCBlockInt dialog

class bvCBlockInt : public CDialog
{
// Construction
public:
	bvCBlockInt(CWnd* pParent = NULL);   // standard constructor

	int binttype;

// Dialog Data
	//{{AFX_DATA(bvCBlockInt)
	enum { IDD = IDD_BV_BLOCKINT };
	CComboBox	m_binttype;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bvCBlockInt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bvCBlockInt)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
