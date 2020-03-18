// hd_MatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hdCMatDlg dialog

class hdCMatDlg : public CDialog
{
// Construction
public:
	hdCMatDlg(CWnd* pParent = NULL);   // standard constructor
	CComplex m_Kn[128];
	int m_npts, m_nlflag;
	CArray<CString,CString&> namelist;

	CFont symbfont;
	CFont bfont;

// Dialog Data
	//{{AFX_DATA(hdCMatDlg)
	enum { IDD = IDD_HD_MATDLG };
	CButton	m_editcurve;
	CComboBox	m_nlcurve;
	CString	m_BlockName;
	double	m_Kx;
	double	m_Ky;
	double	m_Kt;
	double	m_qv;
	CString	m_mu1label;
	CString	m_mu2label;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hdCMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(hdCMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditCurve();
	afx_msg void OnSelchangeNlcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit  m_IDC_EX, m_IDC_EY, m_IDC_BlockName, m_IDC_QV, m_IDC_KT;

};
