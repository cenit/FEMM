#if !defined(AFX_LIBFOLDERINFO_H__61112D57_5D5C_4920_945C_E87C037B8AC6__INCLUDED_)
#define AFX_LIBFOLDERINFO_H__61112D57_5D5C_4920_945C_E87C037B8AC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LibFolderInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLibFolderInfo dialog

class CLibFolderInfo : public CDialog
{
// Construction
public:
	CLibFolderInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLibFolderInfo)
	enum { IDD = IDD_LIBFOLDERINFO };
	CString	m_foldername;
	CString	m_folderurl;
	CString	m_foldervendor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibFolderInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLibFolderInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBFOLDERINFO_H__61112D57_5D5C_4920_945C_E87C037B8AC6__INCLUDED_)
