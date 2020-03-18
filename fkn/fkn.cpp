// fkn.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "fkn.h"
#include "fknDlg.h"
#include <process.h>
#include "lua.h"

extern void lua_baselibopen (lua_State *L);
extern void lua_iolibopen (lua_State *L);
extern void lua_strlibopen (lua_State *L);
extern void lua_mathlibopen (lua_State *L);
extern void lua_dblibopen (lua_State *L);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFknApp

BEGIN_MESSAGE_MAP(CFknApp, CWinApp)
	//{{AFX_MSG_MAP(CFknApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFknApp construction

CFknApp::CFknApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFknApp object

CFknApp theApp;
lua_State *lua; // the main lua object

/////////////////////////////////////////////////////////////////////////////
// CFknApp initialization

BOOL CFknApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFknDlg dlg;
	m_pMainWnd = &dlg;

	// Initialize Lua
	lua=lua_open(4096);
	lua_baselibopen(lua);
	lua_strlibopen(lua);
	lua_mathlibopen(lua);
	lua_iolibopen(lua);

	dlg.ComLine=m_lpCmdLine;
	_beginthread( old_main, 0, (void *) &dlg  );
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	lua_close(lua);	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
