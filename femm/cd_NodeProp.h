// cd_NodeProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdCNodeProp dialog

class cdCNodeProp : public CDialog
{
// Construction
public:
	cdCNodeProp(CWnd* pParent = NULL);   // standard constructor
	
	CArray<CString,CString&> namelist;

// Dialog Data
	//{{AFX_DATA(cdCNodeProp)
	enum { IDD = IDD_CD_NODEPROP };
	CString	m_nodename;
	CComplex	m_qp;
	CComplex	m_vp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cdCNodeProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cdCNodeProp)
	afx_msg void OnSetA();
	afx_msg void OnSetI();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_vpr, m_IDC_qpr, m_IDC_nodename;
};
