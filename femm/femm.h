// femm.h : main header file for the FEMM application
//

#if !defined(AFX_FEMM_H__9E266B2B_F162_4533_8CBF_6ED85E61B8FA__INCLUDED_)
#define AFX_FEMM_H__9E266B2B_F162_4533_8CBF_6ED85E61B8FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "lua.h"
#include "luadebug.h"

/////////////////////////////////////////////////////////////////////////////
// CFemmApp:
// See femm.cpp for the implementation of this class
//

class CFemmApp : public CWinApp
{
public:
	CFemmApp();

	static int luaOpenDocument(lua_State *L);
	static int lua_ERROR(lua_State *L);
	static int lua_messagebox(lua_State *L);
	static int lua_afxpause(lua_State *L);
	static int lua_promptbox(lua_State *L);
	static int lua_endapp(lua_State *L);
	static int lua_newdocument(lua_State *L);
	static int lua_Complex(lua_State *L);
    static int lua_showpointprops(lua_State *L);
    static int lua_hidepointprops(lua_State *L);
	static void line_hook(lua_State *L, lua_Debug *ar);
	BOOL MOCheckIfAlreadyOpen(CString p);
	BOOL EOCheckIfAlreadyOpen(CString p);
	BOOL HOCheckIfAlreadyOpen(CString p);
	BOOL COCheckIfAlreadyOpen(CString p);

	int NumViews;
	int	luaShowWindow;

#ifdef MATHLINK
	static int lua_to_mathematica(lua_State *L);
	static int lua_open_mathlink(lua_State *L);
	static int lua_close_mathlink(lua_State *L);
	BOOL bMathLink;
#endif

	static int lua_to_filelink(lua_State *L);
	BOOL bFileLink;
	BOOL bActiveX;
	CString LuaErrmsg;
	CString MatlabLoveNote;

	int CompatibilityMode;
	static int CFemmApp::lua_compatibilitymode(lua_State *L);
	static int CFemmApp::lua_setcurrentdirectory(lua_State *L);
	static int CFemmApp::lua_smartmesh(lua_State *L);
	static int CFemmApp::lua_makeplot(lua_State *L);

	CString CFemmApp::GetExecutablePath();
	int HowManyDocs();
	void CreateNewDocument(int n);
	void ScanPrefs();
	void ForceViewPrefReload();

	CString IFile,OFile;

	// general defaults
	int d_luaconsole;
	int d_sepplot;
	int d_defdoc;
	int d_SmartMesh;
	int session_SmartMesh;
	int d_bShowOutputWindow,bShowOutputWindow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFemmApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnIdle(LONG lCount);
	virtual void LoadStdProfileSettings(UINT nMaxMRU);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFemmApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpenLuaScript();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEMM_H__9E266B2B_F162_4533_8CBF_6ED85E61B8FA__INCLUDED_)
