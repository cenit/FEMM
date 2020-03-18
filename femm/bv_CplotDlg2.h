// bv_CplotDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bvCCPlotDlg2 dialog

class bvCCPlotDlg2 : public CDialog
{
// Construction
public:
	bvCCPlotDlg2(CWnd* pParent = NULL);   // standard constructor
	double Alb,Aub;
	
// Dialog Data
	//{{AFX_DATA(bvCCPlotDlg2)
	enum { IDD = IDD_BV_CPLOTDLG2 };
	int		m_numcontours;
	BOOL	m_showa;
	double	m_ahigh;
	double	m_alow;
	BOOL	m_showmask;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bvCCPlotDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bvCCPlotDlg2)
	afx_msg void OnDflt1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_numcontours, m_IDC_ahigh, m_IDC_alow;
};
