// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__50FDBCE8_9046_4FD7_907E_2A9DAEC8F0B6__INCLUDED_)
#define AFX_MAINFRM_H__50FDBCE8_9046_4FD7_907E_2A9DAEC8F0B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MDITabs.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	COutBox			m_dlgBar;
	CToolBar		m_FE_toolBar1;
	CToolBar		m_BD_toolBar1;
	CToolBar		m_HD_toolBar1;
	CToolBar		m_CD_toolBar1;
	CToolBar		m_FV_toolBar1;
	CToolBar		m_BV_toolBar1;
	CToolBar		m_HV_toolBar1;
	CToolBar		m_CV_toolBar1;
	CToolBar		m_leftbar;
	CToolBar		m_wndToolBar1,m_wndToolBar2;
	CFont			m_StatusBarFont;
	CLuaConsoleDlg	m_LuaConsole;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	virtual void OnUpdateFrameTitle(BOOL bAddToTitle); 

// Implementation
public:
	virtual ~CMainFrame();

	void SetBar(int n);
	BOOL ResizeClient(int nWidth, int nHeight, BOOL bRedraw=FALSE);
	static int luaMinimize(lua_State *L);
	static int luaMaximize(lua_State *L);
	static int luaRestore(lua_State *L);
	static int luaResize(lua_State *L);
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	
protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CMDITabs    m_wndMDITabs;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPtProp();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnViewLuaConsole();
	afx_msg void OnClose();
	afx_msg void OnPreferences();
	afx_msg void OnHelpFinder();
	afx_msg void OnHelpLicense();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__50FDBCE8_9046_4FD7_907E_2A9DAEC8F0B6__INCLUDED_)
