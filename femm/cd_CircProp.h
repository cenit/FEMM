// cd_CircProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdCCircProp dialog

class cdCCircProp : public CDialog
{
// Construction
public:
	cdCCircProp(CWnd* pParent = NULL);   // standard constructor

	int m_circtype;
	CArray<CString,CString&> namelist;

// Dialog Data
	//{{AFX_DATA(cdCCircProp)
	enum { IDD = IDD_CD_CIRCPROP };
	CString	m_circname;
	CComplex  m_vc;
	CComplex  m_qc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cdCCircProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cdCCircProp)
	afx_msg void OnRadioamp();
	afx_msg void OnRadiovolt();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CLuaEdit m_idc_circname, m_idc_vc, m_idc_qc;
};
