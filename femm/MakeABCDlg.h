#pragma once
#include "afxwin.h"


// CMakeABCDlg dialog

class CMakeABCDlg : public CDialog
{
	DECLARE_DYNAMIC(CMakeABCDlg)

public:
	CMakeABCDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMakeABCDlg();

// Dialog Data
	enum { IDD = IDD_MAKE_ABC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	int abcn;
	int n;
	double abcr;
	double abcx;
	double abcy;

	CLuaEdit m_IDC_ABCN;
	CLuaEdit m_IDC_ABCR;
	CLuaEdit m_IDC_ABCX;
	CLuaEdit m_IDC_ABCY;
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_edgetype;
};
