// ArcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArcDlg dialog

#include "afxwin.h"

class CArcDlg : public CDialog
{
// Construction
public:
	CArcDlg(CWnd* pParent = NULL);   // standard constructor
	int cursel;
	CArray<CString,CString&> namelist;

// Dialog Data
	//{{AFX_DATA(CArcDlg)
	enum { IDD = IDD_ARCDLG };
	CComboBox	m_ArcSegBdry;
	double	m_ArcAngle;
	double	m_MaxSeg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArcDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit mIDC_ARCANGLE, m_IDC_MAXSEG;
};
