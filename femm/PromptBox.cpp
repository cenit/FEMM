// PromptBox.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "PromptBox.h"
#include "lua.h"

extern lua_State *lua;

// CPromptBox dialog

IMPLEMENT_DYNAMIC(CPromptBox, CDialog)
CPromptBox::CPromptBox(CWnd* pParent /*=NULL*/)
	: CDialog(CPromptBox::IDD, pParent)
	, instring(_T(""))
{
}

CPromptBox::~CPromptBox()
{
}

double CPromptBox::ParseDouble()
{
	// parse the contents of the edit box and return a double;

	CString tolua;
	int i,k;
	double x=0;

	tolua="return " + instring;
	i=lua_gettop(lua);
	lua_dostring(lua,tolua);
	k=lua_gettop(lua);
	if (i!=k){
		tolua=lua_tostring(lua,-1);
		if (tolua.GetLength()==0) AfxMessageBox("Input does not evaluate to a numerical value");
		else x=lua_todouble(lua,-1);
	}		
	return x;
}

void CPromptBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, instring);
	DDX_Control(pDX, IDC_EDIT1, lua_instring);
}


BEGIN_MESSAGE_MAP(CPromptBox, CDialog)
END_MESSAGE_MAP()


// CPromptBox message handlers

BOOL CPromptBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(mytitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
