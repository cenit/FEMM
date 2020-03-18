// bd_Pref.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bdCPref dialog

class bdCPref : public CDialog
{
// Construction
public:

	bdCPref(CWnd* pParent = NULL);   // standard constructor
	~bdCPref();

	COLORREF *clist;
	int s_action;
	int s_coord;
	int s_length;
	int s_type;

	void ScanPrefs();
	void WritePrefs();

// Dialog Data
	//{{AFX_DATA(bdCPref)
	enum { IDD = IDD_BD_PREF };
	CComboBox	m_d_color;
	CComboBox	m_d_type;
	CComboBox	m_d_length;
	CComboBox	m_d_coord;
	CComboBox	m_d_action;
	double	m_d_gridsize;
	double	m_d_pixels;
	double	m_d_prec;
	double  m_d_minangle;
	double	m_d_depth;
	BOOL	m_d_showgrid;
	BOOL	m_d_snapgrid;
	BOOL	m_d_showorigin;
	BOOL	m_d_shownames;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bdCPref)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bdCPref)
	afx_msg void OnModifyButton();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRestoreColors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CLuaEdit m_IDC_d_gridsize, m_IDC_d_depth;
	CLuaEdit m_IDC_d_pixels, m_IDC_d_prec, m_IDC_d_minangle;

};
