// hd_NodeProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hdCNodeProp dialog

class hdCNodeProp : public CDialog
{
// Construction
public:
	hdCNodeProp(CWnd* pParent = NULL);   // standard constructor
	
	CArray<CString,CString&> namelist;
	CFont bfont;

// Dialog Data
	//{{AFX_DATA(hdCNodeProp)
	enum { IDD = IDD_HD_NODEPROP };
	CString	m_nodename;
	double	m_qp;
	double	m_vp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hdCNodeProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(hdCNodeProp)
	afx_msg void OnSetA();
	afx_msg void OnSetI();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_vp, m_IDC_qp, m_IDC_nodename;
};
