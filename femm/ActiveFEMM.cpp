// ActiveFEMM.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "lua.h"
#include "luadebug.h"
#include "luaconsoledlg.h"
#include "ActiveFEMM.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern lua_State *lua;
extern BOOL bLinehook;
extern BOOL lua_byebye;
extern HANDLE hProc;
extern int m_luaWindowStatus;
extern CFemmApp theApp;

CString LuaResult;

/////////////////////////////////////////////////////////////////////////////
// ActiveFEMM

IMPLEMENT_DYNCREATE(ActiveFEMM, CCmdTarget)

ActiveFEMM::ActiveFEMM()
{
	EnableAutomation();
	EnableTypeLib();

	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();

	lua_register(lua,"actxprint", lua_to_string);
	lua_register(lua,"lua2matlab",lua_to_matlab);
}

ActiveFEMM::~ActiveFEMM()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void ActiveFEMM::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	// We have to close things in a funny way so that FEMM shuts down
	// the way that it expects to. First, the call to AfxOleSetUserCtrl
	// makes it so that the the application won't get closed when the
	// base class version of OnFinalRelease gets called.
	AfxOleSetUserCtrl(TRUE);

	// Then, post a message to the main window requesting a shutdown.
	// This is the way that FEMM likes to shut down. Since the message
	// has been posted rather than sent, it will be acted upon after
	// ActiveFEMM has shut itself down.
	AfxGetMainWnd()->PostMessage(WM_CLOSE);

	// Then, call the base class to shut down ActiveFEMM
	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(ActiveFEMM, CCmdTarget)
	//{{AFX_MSG_MAP(ActiveFEMM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(ActiveFEMM, CCmdTarget)
	//{{AFX_DISPATCH_MAP(ActiveFEMM)
	DISP_FUNCTION(ActiveFEMM, "call2femm", call2femm, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(ActiveFEMM, "mlab2femm", mlab2femm, VT_BSTR, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// Type library ID and version
// {04EF434A-1A91-495A-85AA-C625602B4AF4}
static const GUID _tlid = 
{ 0x04EF434A, 0x1A91, 0x495A, { 0x85, 0xAA, 0xC6, 0x25, 0x60, 0x2B, 0x4A, 0xF4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;
IMPLEMENT_OLETYPELIB(ActiveFEMM, _tlid, _wVerMajor, _wVerMinor)




// Note: we add support for IID_IActiveFEMM to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E08185B4-FEDF-4B1B-A88D-D40C97625060}
static const IID IID_IActiveFEMM =
{ 0xe08185b4, 0xfedf, 0x4b1b, { 0xa8, 0x8d, 0xd4, 0xc, 0x97, 0x62, 0x50, 0x60 } };

BEGIN_INTERFACE_MAP(ActiveFEMM, CCmdTarget)
	INTERFACE_PART(ActiveFEMM, IID_IActiveFEMM, Dispatch)
END_INTERFACE_MAP()

// {0A35D5BD-DCA9-4C39-9512-1D89A1A37047}
IMPLEMENT_OLECREATE2(ActiveFEMM, "femm.ActiveFEMM", 0xa35d5bd, 0xdca9, 0x4c39, 0x95, 0x12, 0x1d, 0x89, 0xa1, 0xa3, 0x70, 0x47)

BOOL ActiveFEMM::GetDispatchIID(IID* pIID)
{
         *pIID = IID_IActiveFEMM;

         return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ActiveFEMM message handlers

BSTR ActiveFEMM::call2femm(LPCTSTR luacmd) 
{
	// executes the line contained in luacmd
	// and returns a string containing the results
	// of the command with the results separated
	// by newline characters.
	CString strToLua;

	theApp.MatlabLoveNote.Empty();
	strToLua=luacmd;
	strToLua="actxprint(" + strToLua +")";
	DoLuaCmd(strToLua);

	// Returns the result of errors that are trapped 
	// via message boxes during a normal UI session
	if(theApp.MatlabLoveNote.GetLength()>0)
	{
		LuaResult.Format("error: %s",theApp.MatlabLoveNote);
	}
	return LuaResult.AllocSysString();
}

BSTR ActiveFEMM::mlab2femm(LPCTSTR luacmd) 
{
	// executes the line contained in luacmd
	// and returns a string containing the results
	// of the command formatted in matlab format.
	// One would expect that all the results are
	// real numbers, in which case we can eval()
	// the result in matlab to get a vector of numbers.
	CString strToLua;

	theApp.MatlabLoveNote.Empty();
	strToLua=luacmd;
	strToLua="lua2matlab(" + strToLua +")";
	DoLuaCmd(strToLua);
	
	// Returns the result of errors that are trapped 
	// via message boxes during a normal UI session
	if(theApp.MatlabLoveNote.GetLength()>0)
	{
		LuaResult.Format("error: %s",theApp.MatlabLoveNote);
	}

	return LuaResult.AllocSysString();
}

void ActiveFEMM::DoLuaCmd(CString strToLua)
{
	LuaResult.Empty();
	if(m_luaWindowStatus==SW_SHOW) bLinehook=NormalLua;
	else bLinehook=HiddenLua;
	theApp.bActiveX=TRUE;
	if (lua_dostring(lua,strToLua)!=0) LuaResult=theApp.LuaErrmsg;
	theApp.bActiveX=FALSE;
	lua_byebye=FALSE;
	bLinehook=FALSE;
}

int ActiveFEMM::lua_to_string(lua_State *L)
{
	CString s;

	int n = lua_gettop(L);	
	LuaResult="";

	for(int k=1;k<=n;k++)
	{
		s=lua_tostring(L,k);
		LuaResult = LuaResult + s +"\n";
	}
	
	return 0;
}

int ActiveFEMM::lua_to_matlab(lua_State *L)
{
	CString s;

	int n = lua_gettop(L);	
	if(n>0){
		LuaResult="[ ";
		for(int k=1;k<=n;k++)
		{
			s=lua_tostring(L,k);
			LuaResult = LuaResult + s + " ";
		}
		LuaResult += "]";
	}
	else LuaResult.Empty();

	return 0;
}
