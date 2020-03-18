// GRIDDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GRIDDLG dialog

class GRIDDLG : public CDialog
{
// Construction
public:
	GRIDDLG(CWnd* pParent = NULL);   // standard constructor

	BOOL coords;

// Dialog Data
	//{{AFX_DATA(GRIDDLG)
	enum { IDD = IDD_GRIDDLG };
	CComboBox	m_coords;
	double	m_gridsize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GRIDDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GRIDDLG)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_gridsize;
};
