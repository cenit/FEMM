// cd_MatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdCMatDlg dialog

class cdCMatDlg : public CDialog
{
// Construction
public:
	cdCMatDlg(CWnd* pParent = NULL);   // standard constructor
	CComplex m_Kn[128];
	int m_npts, m_nlflag;
	CArray<CString,CString&> namelist;

	CFont symbfont;
	CFont bfont;

// Dialog Data
	//{{AFX_DATA(cdCMatDlg)
	enum { IDD = IDD_CD_MATDLG };
	CString	m_BlockName;
	double	m_ox;
	double	m_oy;
	double	m_ex;
	double	m_ey;
	double	m_ltx;
	double	m_lty;
	CString	m_mu1label;
	CString	m_mu2label;
	CString	m_mu1label2;
	CString	m_mu2label2;
	CString	m_mu1label3;
	CString	m_mu2label3;
	CStatic	m_static4;
	CStatic	m_static3;
	CStatic	m_static7;
	CStatic	m_static8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cdCMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cdCMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit  m_IDC_EX, m_IDC_EY, m_IDC_BlockName, m_IDC_OX, m_IDC_OY, m_IDC_LTX, m_IDC_LTY;

};
