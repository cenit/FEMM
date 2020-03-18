#if !defined(AFX_NEWDOCDLG_H__986FD914_D49F_418D_9D0F_56936AB0744B__INCLUDED_)
#define AFX_NEWDOCDLG_H__986FD914_D49F_418D_9D0F_56936AB0744B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewDocDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewDocDlg dialog

class CNewDocDlg : public CDialog
{
// Construction
public:
	CNewDocDlg(CWnd* pParent = NULL);   // standard constructor
	int doctype;

// Dialog Data
	//{{AFX_DATA(CNewDocDlg)
	enum { IDD = IDD_NEWDOC };
	CComboBox	m_doctypes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewDocDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewDocDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWDOCDLG_H__986FD914_D49F_418D_9D0F_56936AB0744B__INCLUDED_)
