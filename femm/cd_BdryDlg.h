// cd_BdryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdCBdryDlg dialog

class cdCBdryDlg : public CDialog
{
// Construction
public:
	cdCBdryDlg(CWnd* pParent = NULL);   // standard constructor

	int BdryFormat;
	CArray<CString,CString&> namelist;

// Dialog Data
	//{{AFX_DATA(cdCBdryDlg)
	enum { IDD = IDD_CD_BDRYDLG };
	CComboBox	m_BdryFormat;
	CString	m_BdryName;
	CComplex	m_qs;
	CComplex	m_vs;
	CComplex	m_c0;
	CComplex	m_c1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cdCBdryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cdCBdryDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBdryformat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CLuaEdit m_IDC_VBDRY, m_IDC_QS, m_IDC_BDRYNAME;
	CLuaEdit m_IDC_c0, m_IDC_c1;

};
