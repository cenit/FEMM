// BHDatafile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBHDatafile dialog

class CBHDatafile : public CDialog
{
// Construction
public:
	CBHDatafile(CWnd* pParent = NULL);   // standard constructor
	int BHOrder,BUnits,HUnits;

// Dialog Data
	//{{AFX_DATA(CBHDatafile)
	enum { IDD = IDD_BHDATAFILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBHDatafile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBHDatafile)
	virtual BOOL OnInitDialog();
	afx_msg void OnAmpm();
	afx_msg void OnBfirst();
	afx_msg void OnGauss();
	afx_msg void OnKampm();
	afx_msg void OnKgauss();
	afx_msg void OnKoersted();
	afx_msg void OnOersted();
	afx_msg void OnTesla();
	afx_msg void OnHfirst();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
