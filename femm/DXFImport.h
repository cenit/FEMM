// DXFImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDXFImport dialog

class CDXFImport : public CDialog
{
// Construction
public:
	CDXFImport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDXFImport)
	enum { IDD = IDD_DXFIMPORT };
	double	m_dxftol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDXFImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDXFImport)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CLuaEdit m_IDC_dxftol;

};
