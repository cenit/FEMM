// OpSegDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bdCOpSegDlg dialog

class bdCOpSegDlg : public CDialog
{
// Construction
public:
	bdCOpSegDlg(CWnd* pParent = NULL);   // standard constructor
	int cursel;
	int condsel;
	CArray<CBoundaryProp,CBoundaryProp&> *plineproplist;
	CArray<CCircuit,CCircuit&> *pcircproplist;

// Dialog Data
	//{{AFX_DATA(bdCOpSegDlg)
	enum { IDD = IDD_BD_OPSEGDLG };
	CComboBox	m_segcond;
	CComboBox	m_ackseg;
	double	m_linemeshsize;
	BOOL	m_automesh;
	BOOL	m_hide;
	int		m_ingroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bdCOpSegDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bdCOpSegDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAutomesh();
	afx_msg void OnSelchangeAckseg();
	afx_msg void OnSelchangeSegCond();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_ingroup, m_IDC_linemeshsize;
};
