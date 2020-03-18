#pragma once
#include "afxwin.h"


// GapIntegral dialog

class GapIntegral : public CDialog
{
	DECLARE_DYNAMIC(GapIntegral)

public:
	GapIntegral(CWnd* pParent = NULL);   // standard constructor
	virtual ~GapIntegral();


	CArray<CAirGapElement,CAirGapElement&> *pagelist;
	double Frequency;
	int bIncremental;
	CString BdryName;
	int myIntType;

// Dialog Data
	enum { IDD = IDD_GAPINTEGRAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(GapIntegral)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_AGEname;
	CComboBox m_IntType;
};
