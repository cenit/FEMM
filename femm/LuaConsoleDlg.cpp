// LuaConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "lua.h"
#include "LuaConsoleDlg.h"

extern lua_State *lua;
extern BOOL bLinehook;
extern BOOL lua_byebye;

CLuaConsoleDlg *pThis;

// CLuaConsoleDlg dialog

// IMPLEMENT_DYNAMIC(CLuaConsoleDlg, CResizableDialog)
CLuaConsoleDlg::CLuaConsoleDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CLuaConsoleDlg::IDD, pParent)
	, outbuffer(_T(""))
	, inbuffer(_T(""))
{
}

CLuaConsoleDlg::~CLuaConsoleDlg()
{
}

void CLuaConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_LUA_OUTPUT, outbuffer);
	DDX_Text(pDX, IDC_LUA_INPUT, inbuffer);
	DDX_Control(pDX, IDC_LUA_INPUT, inbufferctrl);
	DDX_Control(pDX, IDC_LUA_OUTPUT, ConsoleOutput);
}


BEGIN_MESSAGE_MAP(CLuaConsoleDlg, CResizableDialog)
	ON_BN_CLICKED(IDC_CLEAR_INPUT, OnBnClickedClearInput)
	ON_BN_CLICKED(IDC_CLEAR_OUTPUT, OnBnClickedClearOutput)
	ON_BN_CLICKED(IDC_EVALUATE, OnBnClickedEvaluate)
END_MESSAGE_MAP()


// CLuaConsoleDlg message handlers
BOOL CLuaConsoleDlg::OnInitDialog()
{	
	CResizableDialog::OnInitDialog();

	pThis=this;
    
	// register lua extensions
    lua_register(lua,"print",lua_Print);
	lua_register(lua,"showconsole",lua_showconsole);
    lua_register(lua,"hideconsole",lua_hideconsole);
	lua_register(lua,"show_console",lua_showconsole);
    lua_register(lua,"hide_console",lua_hideconsole);
	lua_register(lua,"clearconsole",lua_clearconsole);
	lua_register(lua,"clear_console",lua_clearconsole);

	// preset layout
	AddAnchor(IDC_EVALUATE, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR_INPUT, BOTTOM_LEFT);
	AddAnchor(IDC_CLEAR_OUTPUT, BOTTOM_LEFT);
	AddAnchor(IDC_LUA_OUTPUT, TOP_LEFT, MIDDLE_RIGHT);
	AddAnchor(IDC_LUA_INPUT, MIDDLE_LEFT,BOTTOM_RIGHT);

	ConsoleOutput.LimitText(64000);
	
	return TRUE;
}

void CLuaConsoleDlg::ToOutput(CString str)
{
	int Length;
	int n = (int) ConsoleOutput.GetLimitText();

	// check to see if string to be printed is bigger than the
	// buffer size of the edit box.  If so, trim the string.
	if (str.GetLength()>n) str.Left(n-1);

	// trim info in edit box, if needed 
	Length = ConsoleOutput.GetWindowTextLength();
	if ((Length + str.GetLength()) >= n)
	{
		int k=20;
		while (ConsoleOutput.LineIndex(k)<str.GetLength()) k+=1;
		ConsoleOutput.SetSel(0,ConsoleOutput.LineIndex(k),TRUE);
		ConsoleOutput.ReplaceSel("");
	}

	ConsoleOutput.SetSel(Length, Length,TRUE);
	ConsoleOutput.ReplaceSel(str);
	ConsoleOutput.LineScroll( ConsoleOutput.GetLineCount()-1);
}

int CLuaConsoleDlg::lua_Print(lua_State *L)
{
	// Snatched this from the Lua source code,
	// replacing writes to stdout with my custom
	// output to the LuaConsole's edit box
	int n = lua_gettop(L);				// number of arguments
	int i;
	lua_getglobal(L, "tostring");
	for (i=1; i<=n; i++) {
		CString s;
		lua_pushvalue(L, -1);			// function to be called 
		lua_pushvalue(L, i);			// value to print 
		lua_rawcall(L, 1, 1);
		s = lua_tostring(L, -1);		// get result 
		if (i>1) pThis->ToOutput("\t");
		else pThis->ToOutput("--> ");
		pThis->ToOutput(s);
		lua_pop(L, 1);					// pop result
	}
	pThis->ToOutput("\r\n");
 
	return 0;
} 

void CLuaConsoleDlg::OnBnClickedClearInput()
{
	if (bLinehook!=FALSE) return;

	UpdateData(TRUE);
	inbuffer="";
	UpdateData(FALSE);
}

void CLuaConsoleDlg::OnBnClickedClearOutput()
{
	if (bLinehook!=FALSE) return;

	ConsoleOutput.SetWindowText("");
}

void CLuaConsoleDlg::OnBnClickedEvaluate()
{
	if (bLinehook!=FALSE) return;
	
	CString LuaCmd;

	UpdateData();
	ToOutput(inbuffer+"\r\n");
	LuaCmd=inbuffer;
	inbuffer="";
	UpdateData(FALSE);
	bLinehook=NormalLua;

	CStatusBar *StatBar=(CStatusBar *)((CFrameWnd *)GetTopLevelFrame())->GetMessageBar();
		StatBar->SetPaneText(0,"EXECUTING LUASCRIPT -- HIT <ESC> TO ABORT",TRUE);
	int lua_error_code=lua_dostring(lua,LuaCmd);
	
	if(lua_error_code!=FALSE){
//		if (lua_error_code==LUA_ERRRUN)
//			AfxMessageBox("Run Error");
//		if (lua_error_code==LUA_ERRSYNTAX)
//			AfxMessageBox("Syntax Error");
		if (lua_error_code==LUA_ERRMEM)
			AfxMessageBox("Lua memory Error");
		if (lua_error_code==LUA_ERRERR)
			AfxMessageBox("User error error");
		if (lua_error_code==LUA_ERRFILE)
			AfxMessageBox("File Error");
		inbuffer=LuaCmd;
		UpdateData(FALSE);
	}

	bLinehook=FALSE;
	inbufferctrl.SetFocus();
	StatBar->SetPaneText(0,"Ready",TRUE);
	
	if(lua_byebye==TRUE){
		ASSERT(AfxGetMainWnd() != NULL);
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
	}


}

int CLuaConsoleDlg::lua_showconsole(lua_State *L)
{	
	pThis->ShowWindow(SW_SHOW);
    return 0;
}


int CLuaConsoleDlg::lua_hideconsole(lua_State *L)
{
    pThis->ShowWindow(SW_HIDE);
	return 0;
}

int CLuaConsoleDlg::lua_clearconsole(lua_State *L)
{
	pThis->ConsoleOutput.SetWindowText("");
	return 0;
}