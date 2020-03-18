// CPlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPlotDlg dialog

class CCPlotDlg : public CDialog
{
// Construction
public:
	CCPlotDlg(CWnd* pParent = NULL);   // standard constructor
	double Alb,Aub;

// Dialog Data
	//{{AFX_DATA(CCPlotDlg)
	enum { IDD = IDD_CPLOTDLG };
	int		m_numcontours;
	BOOL	m_showai;
	BOOL	m_showar;
	double	m_ahigh;
	double	m_alow;
	BOOL	m_showmask;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPlotDlg)
	afx_msg void OnDflt1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_numcontours, m_IDC_ahigh, m_IDC_alow;
};
