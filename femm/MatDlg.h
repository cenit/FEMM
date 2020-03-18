// MatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMatDlg dialog

class CMatDlg : public CDialog
{
// Construction
public:
	CMatDlg(CWnd* pParent = NULL);   // standard constructor
	CString BData,HData;
	int LamType;
	BOOL m_nlflag;
	CArray<CString,CString&> namelist;

	CFont symbfont;
	CFont bfont;
	void IsWire();
// Dialog Data
	//{{AFX_DATA(CMatDlg)
	enum { IDD = IDD_MATDLG };
	CStatic	m_static6;
	CStatic	m_static5;
	CStatic	m_static4;
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CButton	m_IDC_bhedit;
	CComboBox	m_nlcombo;
	CComboBox	m_lam_dir;
	double	m_Cduct;
	CString	m_BlockName;
	double	m_H_c;
	CComplex	m_Jsrc;
	double	m_Lam_d;
	double	m_mu_x;
	double	m_mu_y;
	double	m_lam_fill;
	CString	m_mu1label;
	CString	m_mu2label;
	double	m_Theta_hn;
	double	m_Theta_hx;
	double	m_Theta_hy;
	double  m_WireD;
	int		m_NStrands;
	CString	m_h1label;
	CString	m_h2label;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBhedit();
	afx_msg void OnSelchangeLamDir();
	afx_msg void OnSelchangeNlcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit  m_IDC_Cduct, m_IDC_BlockName, m_IDC_H_c;
	CLuaEdit m_IDC_Jr, m_IDC_Lam_d;
	CLuaEdit m_IDC_mu_x, m_IDC_mu_y, m_IDC_Theta_h, m_IDC_lam_fill;
	CLuaEdit m_IDC_Theta_hx, m_IDC_Theta_hy;
	CLuaEdit m_IDC_NStrands, m_IDC_WireD;
};
