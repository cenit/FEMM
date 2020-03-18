// cv_CplotDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cvCCPlotDlg2 dialog

class cvCCPlotDlg2 : public CDialog
{
// Construction
public:
	cvCCPlotDlg2(CWnd* pParent = NULL);   // standard constructor
	double Alb,Aub;
	
// Dialog Data
	//{{AFX_DATA(cvCCPlotDlg2)
	enum { IDD = IDD_CV_CPLOTDLG2 };
	int		m_numcontours;
	BOOL	m_showa;
	BOOL    m_showa2;
	double	m_ahigh;
	double	m_alow;
	BOOL	m_showmask;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cvCCPlotDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cvCCPlotDlg2)
	afx_msg void OnDflt1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_numcontours, m_IDC_ahigh, m_IDC_alow;
};
