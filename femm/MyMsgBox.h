// MyMsgBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMsgBox dialog

class CMyMsgBox : public CDialog
{
// Construction
public:
	CMyMsgBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyMsgBox)
	enum { IDD = IDD_MYMSGBOX };
	CString	m_mymsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMsgBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyMsgBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
