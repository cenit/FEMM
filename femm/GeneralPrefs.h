#if !defined(AFX_GENERALPREFS_H__9C2D61CC_B458_496C_9A23_BD2CD5D22AC8__INCLUDED_)
#define AFX_GENERALPREFS_H__9C2D61CC_B458_496C_9A23_BD2CD5D22AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPrefs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralPrefs dialog

class CGeneralPrefs : public CDialog
{
// Construction
public:
	CGeneralPrefs(CWnd* pParent = NULL);   // standard constructor
	int s_defdoc;

	void WritePrefs();
	void ScanPrefs();

// Dialog Data
	//{{AFX_DATA(CGeneralPrefs)
	enum { IDD = IDD_GENPREFS };
	CComboBox	m_defdoc;
	BOOL	m_def_lua_console;
	BOOL	m_def_xyplot;
	BOOL	m_def_show_output_window;
	BOOL    m_def_smartmesh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPrefs)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralPrefs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPREFS_H__9C2D61CC_B458_496C_9A23_BD2CD5D22AC8__INCLUDED_)
