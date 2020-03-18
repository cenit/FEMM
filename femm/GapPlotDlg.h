#pragma once

/////////////////////////////////////////////////////////////////////////////
// CGapPlotDlg dialog

class CGapPlotDlg : public CDialog
{
// Construction
public:
	CGapPlotDlg(CWnd* pParent = NULL);   // standard constructor
	int XYPlotType;
	int FileFormat;
	CFemmviewDoc *pDoc;
	CArray<CAirGapElement,CAirGapElement&> *pagelist;

// Dialog Data
	//{{AFX_DATA(CGapPlotDlg)
	enum { IDD = IDD_GAPPLOTDLG };
	CComboBox	m_fileformat;
	CComboBox	m_XYPlotType;
	CComboBox	m_BdryName;
	BOOL	m_ToFile;
	int		m_npoints;
	int		m_age;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGapPlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGapPlotDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToFileClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CLuaEdit m_IDC_npoints;
};