// GridMod.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridMod dialog

class CGridMod : public CDialog
{
// Construction
public:
	CGridMod(CWnd* pParent = NULL);   // standard constructor
	int coords;

// Dialog Data
	//{{AFX_DATA(CGridMod)
	enum { IDD = IDD_GRIDMOD };
	CComboBox	m_coords;
	double	m_gridsize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridMod)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridMod)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_gridsize;
};
