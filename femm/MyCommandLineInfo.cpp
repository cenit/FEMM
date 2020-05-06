// MyCommandLineInfo.cpp: implementation of the CMyCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "femm.h"
#include "MyCommandLineInfo.h"
#include "afxwin.h"
#include "lua.h"

extern lua_State* lua;
extern int m_luaWindowStatus;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CString luascriptname;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCommandLineInfo::CMyCommandLineInfo()
{
  //	m_luaWindowStatus=SW_SHOW; // default unless otherwise
}

CMyCommandLineInfo::~CMyCommandLineInfo()
{
}

void CMyCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
  CString theparam;
  theparam.Format("%s", lpszParam);
  theparam.MakeLower();

  if (theparam.Left(11) == "lua-script=" && bFlag == 1)
    luascriptname = theparam.Mid(11);

  if (theparam == "windowhide" && bFlag == 1)
    m_luaWindowStatus = SW_SHOWMINNOACTIVE;
  if (theparam == "hidewindow" && bFlag == 1)
    m_luaWindowStatus = SW_SHOWMINNOACTIVE;

  if (theparam == "filelink")
    ((CFemmApp*)AfxGetApp())->bFileLink = TRUE;

  if (theparam.Left(8) == "lua-var=" && bFlag == 1) {
    CString varname;
    CString vardata;
    for (int pos = 8; pos < theparam.GetLength(); pos++) {
      if (theparam.Mid(pos, 1) == "=") {
        varname = theparam.Mid(8, pos - 8);
        vardata = theparam.Mid(pos + 1, theparam.GetLength() - pos + 1);
        if (lua != NULL) {
          lua_pushstring(lua, vardata);
          lua_setglobal(lua, varname);
        }
        pos = theparam.GetLength() + 1;
      }
    }
  }

  CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
