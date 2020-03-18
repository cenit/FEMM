// DPlotDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// hvCDPlotDlg2 dialog

class hvCDPlotDlg2 : public CDialog
{
// Construction
public:
	hvCDPlotDlg2(CWnd* pParent = NULL);   // standard constructor
	double Bm_l, Bm_h;
	double d_PlotBounds[4][2];
	double PlotBounds[4][2];
	int DensityPlot;

// Dialog Data
	//{{AFX_DATA(hvCDPlotDlg2)
	enum { IDD = IDD_HV_DPLOTDLG2 };
	CComboBox	m_dplottype;
	BOOL	m_showit;
	BOOL	m_showlegend;
	double	m_lb2;
	double	m_ub2;
	BOOL	m_gscale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hvCDPlotDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(hvCDPlotDlg2)
	afx_msg void OnResbtn2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeDplottype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_lb2, m_IDC_ub2;
};
