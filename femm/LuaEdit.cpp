// LuaEdit.cpp: C++ derived edit control parsing with lua

#include "stdafx.h"
#include "lua.h"

// COMMAND IDs used in the context menu
#define APSTUDIO_INVOKED
#undef APSTUDIO_READONLY_SYMBOLS
#include "resource.h"
#define ID_LUA_PARSE _APS_NEXT_COMMAND_VALUE + 1

extern lua_State* lua;

/////////////////////////////////////////////////////////////////////////////
// LuaEdit

BEGIN_MESSAGE_MAP(CLuaEdit, CEdit)
//{{AFX_MSG_MAP(CLuaEdit)
ON_WM_CHAR()
ON_WM_RBUTTONUP()
//}}AFX_MSG_MAP
ON_COMMAND(ID_EDIT_CUT, Cut)
ON_COMMAND(ID_EDIT_COPY, Copy)
ON_COMMAND(ID_EDIT_PASTE, Paste)
ON_COMMAND(ID_EDIT_UNDO, Undo)
ON_COMMAND(ID_EDIT_CLEAR, Clear)
ON_COMMAND(ID_EDIT_SELECT_ALL, SelectAll)
ON_COMMAND(ID_LUA_PARSE, ParseWithLua)
END_MESSAGE_MAP()

void CLuaEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar == 12)
    ParseWithLua();

  // hook that implements evaluation of Lua input via
  // Shift+Enter in the Lua console
  if ((nChar == 13) && (GetKeyState(VK_SHIFT) & 0x8000))
    GetParent()->PostMessage(WM_COMMAND, IDC_EVALUATE);

  else
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CLuaEdit::Undo()
{
  CEdit::Undo();
}

void CLuaEdit::ParseWithLua()
{
  int nStart, nEnd, i, k;
  CString tolua;

  GetSel(nStart, nEnd);
  GetWindowText(tolua);

  if (nStart != nEnd) {
    tolua = "return " + tolua.Mid(nStart, nEnd - nStart) + "\n";
    i = lua_gettop(lua);
    lua_dostring(lua, tolua);
    k = lua_gettop(lua);
    if (i != k) {
      tolua = lua_tostring(lua, -1);
      if (tolua.GetLength() == 0) {
        AfxMessageBox("Input does not evaluate to a numerical value");
      } else
        ReplaceSel(tolua, TRUE);
      lua_pop(lua, 1); // pop result from Lua stack so we don't get an overflow.
    }
  }
}

BOOL CLuaEdit::IsSelection()
{
  int nStart, nEnd;

  GetSel(nStart, nEnd);
  if (nStart != nEnd)
    return TRUE;

  return FALSE;
}

void CLuaEdit::OnRButtonUp(UINT nFlags, CPoint point)
{

  HANDLE hClip = ::GetClipboardData(CF_TEXT);

  CMenu* pMenu = NULL;
  try {
    pMenu = new CMenu;
  } catch (...) {
    pMenu = NULL;
  }
  ASSERT(pMenu);
  VERIFY(pMenu->CreatePopupMenu());

  if (CanUndo())
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_UNDO, "Undo"));
  else
    VERIFY(pMenu->AppendMenu(MF_GRAYED, ID_EDIT_UNDO, "Undo"));

  VERIFY(pMenu->AppendMenu(MF_SEPARATOR, 0, ""));
  if (IsSelection() || hClip)
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_CUT, "Cut"));
  else
    VERIFY(pMenu->AppendMenu(MF_GRAYED, ID_EDIT_CUT, "Cut"));

  if (IsSelection())
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_COPY, "Copy"));
  else
    VERIFY(pMenu->AppendMenu(MF_GRAYED, ID_EDIT_COPY, "Copy"));

  if (hClip)
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_PASTE, "Paste"));
  else
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_PASTE, "Paste"));

  if (IsSelection())
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_CLEAR, "Delete"));
  else
    VERIFY(pMenu->AppendMenu(MF_GRAYED, ID_EDIT_CLEAR, "Delete"));

  VERIFY(pMenu->AppendMenu(MF_SEPARATOR, 0, ""));

  if (IsSelection())
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_LUA_PARSE, "Evaluate"));
  else
    VERIFY(pMenu->AppendMenu(MF_GRAYED, ID_LUA_PARSE, "Evaluate"));

  VERIFY(pMenu->AppendMenu(MF_SEPARATOR, 0, ""));

  int nStart, nEnd, nLength;
  CString strText;
  GetWindowText(strText);
  GetSel(nStart, nEnd);
  nLength = strText.GetLength();
  if ((nLength != 0) && (nLength - nEnd + nStart > 0))
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_SELECT_ALL, "Select All"));
  else
    VERIFY(pMenu->AppendMenu(MF_GRAYED, ID_EDIT_SELECT_ALL, "Select All"));

  if (!hClip)
    ::CloseHandle(hClip);
  ClientToScreen(&point);

  pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
      point.x, point.y,
      this);

  try {
    delete pMenu;
  } catch (...) {
    ASSERT(FALSE);
    TRACE("CLuaEdit::OnRButtonUp failed to destroy menu resource\n");
    pMenu = FALSE;
  }
}

void CLuaEdit::SelectAll()
{
  CString strText;
  GetWindowText(strText);
  SetSel(0, strText.GetLength());
}