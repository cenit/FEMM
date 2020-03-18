// GroupNumber.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupNumber dialog

class CGroupNumber : public CDialog
{
// Construction
public:
	CGroupNumber(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupNumber)
	enum { IDD = IDD_GROUPNO };
	int		m_groupnumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupNumber)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupNumber)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:			
	CLuaEdit m_IDC_groupnumber;
};
