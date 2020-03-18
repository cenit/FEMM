// hd_BdryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hdCBdryDlg dialog

class hdCBdryDlg : public CDialog
{
// Construction
public:
	hdCBdryDlg(CWnd* pParent = NULL);   // standard constructor

	int BdryFormat;
	CArray<CString,CString&> namelist;

	CFont symbfont;

// Dialog Data
	//{{AFX_DATA(hdCBdryDlg)
	enum { IDD = IDD_HD_BDRYDLG };
	CComboBox	m_BdryFormat;
	CString	m_BdryName;
	double	m_qs;
	double	m_beta;
	double	m_htc;
	double	m_To2;
	double	m_To1;
	double	m_Tset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hdCBdryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(hdCBdryDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBdryformat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CLuaEdit m_IDC_Tset, m_IDC_To1, m_IDC_QS, m_IDC_BDRYNAME;
	CLuaEdit m_IDC_beta, m_IDC_htc, m_IDC_To2;

};
