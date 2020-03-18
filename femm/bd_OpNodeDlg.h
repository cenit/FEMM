// OpNodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bdCOpNodeDlg dialog

class bdCOpNodeDlg : public CDialog
{
// Construction
public:
	bdCOpNodeDlg(CWnd* pParent = NULL);   // standard constructor
	int cursel;
	int condsel;
	CArray<CPointProp,CPointProp&> *pnodeproplist;
	CArray<CCircuit,CCircuit&> *pcircproplist;

// Dialog Data
	//{{AFX_DATA(bdCOpNodeDlg)
	enum { IDD = IDD_BD_OPNODEDLG };
	CComboBox	m_nodecond;
	CComboBox	m_acknode;
	int		m_ingroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bdCOpNodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bdCOpNodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_ingroup;
};
