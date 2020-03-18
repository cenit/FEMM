// KbdZoom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKbdZoom dialog

class CKbdZoom : public CDialog
{
// Construction
public:
	CKbdZoom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKbdZoom)
	enum { IDD = IDD_KBDZOOM };
	double	m_scr_bottom;
	double	m_scr_left;
	double	m_scr_right;
	double	m_scr_top;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKbdZoom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKbdZoom)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_scr_bottom, m_IDC_scr_left;
	CLuaEdit m_IDC_scr_right, m_IDC_scr_top;
};
