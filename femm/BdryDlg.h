// BdryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBdryDlg dialog

class CBdryDlg : public CDialog
{
// Construction
public:
	CBdryDlg(CWnd* pParent = NULL);   // standard constructor

	int BdryFormat;
	CArray<CString,CString&> namelist;

	CFont symbfont;

// Dialog Data
	//{{AFX_DATA(CBdryDlg)
	enum { IDD = IDD_BDRYDLG };
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CComboBox	m_BdryFormat;
	double	m_A0;
	double	m_A1;
	double	m_A2;
	CString	m_BdryName;
	CComplex	m_c0;
	CComplex	m_c1;
	double	m_Mu;
	double	m_Phi;
	double	m_Sig;
	double m_innerangle;
	double m_outerangle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBdryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBdryDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBdryformat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CLuaEdit m_IDC_A0, m_IDC_A1, m_IDC_A2, m_IDC_BDRYNAME;
	CLuaEdit m_IDC_c0, m_IDC_c1, m_IDC_Mu, m_IDC_Phi, m_IDC_Sig;
	CLuaEdit m_IDC_INNERANGLE, m_IDC_OUTERANGLE;

};
