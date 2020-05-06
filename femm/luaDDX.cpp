#include "stdafx.h"
#include "lua.h"
#include "femm.h"

#undef DDX_Text

extern void lua_baselibopen(lua_State* L);
extern void lua_iolibopen(lua_State* L);
extern void lua_strlibopen(lua_State* L);
extern void lua_mathlibopen(lua_State* L);
extern void lua_dblibopen(lua_State* L);

extern CFemmApp theApp;
extern lua_State* lua;

// changed 27Nov2005 to work right with complex-valued Lua

void Lua_DDX_Text(CDataExchange* pDX, int nIDC, int& x)
{
  // if initializing things, just call DDX_Text in the usual way...
  if (pDX->m_bSaveAndValidate == FALSE)
    DDX_Text(pDX, nIDC, x);
  // otherwise, side-track things by sending the contents
  // to Lua, processing them, and then stripping off the results;
  else {
    CString tolua;
    int i, k;
    DDX_Text(pDX, nIDC, tolua);
    tolua = "return " + tolua;
    i = lua_gettop(lua);
    lua_dostring(lua, tolua);
    k = lua_gettop(lua);
    if (i != k) {
      tolua = lua_tostring(lua, -1);
      if (tolua.GetLength() == 0) {
        AfxMessageBox("Input does not evaluate to a numerical value");
        pDX->Fail();
      } else
        x = (int)Re(lua_tonumber(lua, -1));
    } else
      pDX->Fail();
  }
}

void Lua_DDX_Text(CDataExchange* pDX, int nIDC, CString& value)
{
  DDX_Text(pDX, nIDC, value);
}

void Lua_DDX_Text(CDataExchange* pDX, int nIDC, double& x)
{
  // if initializing things, just call DDX_Text in the usual way...
  if (pDX->m_bSaveAndValidate == FALSE)
    DDX_Text(pDX, nIDC, x);
  // otherwise, side-track things by sending the contents
  // to Lua, processing them, and then stripping off the results;
  else {
    CString tolua;
    int i, k;
    DDX_Text(pDX, nIDC, tolua);
    tolua = "return " + tolua;
    i = lua_gettop(lua);
    lua_dostring(lua, tolua);
    k = lua_gettop(lua);
    if (i != k) {
      tolua = lua_tostring(lua, -1);
      if (tolua.GetLength() == 0) {
        AfxMessageBox("Input does not evaluate to a numerical value");
        pDX->Fail();
      } else
        x = Re(lua_tonumber(lua, -1));
    } else
      pDX->Fail();
  }
}

void Lua_DDX_Text(CDataExchange* pDX, int nIDC, CComplex& x)
{
  // if initializing things, call the DDX_Text version used to
  // initialize strings.  The string form of the desired complex
  // number is used as the initializer.  We have to do this because
  // there is no native complex form of DDX_Text.
  if (pDX->m_bSaveAndValidate == FALSE) {
    CString tmp;
    tmp.Format("%s", ToString(x, 16));
    DDX_Text(pDX, nIDC, tmp);
  }
  // otherwise, side-track things by sending the contents
  // to Lua, processing them, and then stripping off the results;
  else {
    CString tolua;
    int i, k;
    DDX_Text(pDX, nIDC, tolua);
    tolua = "return " + tolua;
    i = lua_gettop(lua);
    lua_dostring(lua, tolua);
    k = lua_gettop(lua);
    if (i != k) {
      tolua = lua_tostring(lua, -1);
      if (tolua.GetLength() == 0) {
        AfxMessageBox("Input does not evaluate to a numerical value");
        pDX->Fail();
      } else
        x = lua_tonumber(lua, -1);
    } else
      pDX->Fail();
  }
}

// Special form to use for magnet direction, where result could either be
// a hard number or a function to be evaluated later...
void Lua_DDX_Text(CDataExchange* pDX, int nIDC, double& x, CString& s)
{
  // if initializing things, just call DDX_Text in the usual way...
  if (pDX->m_bSaveAndValidate == FALSE)
    DDX_Text(pDX, nIDC, x);
  // otherwise, side-track things by sending the contents
  // to Lua, processing them, and then stripping off the results;
  else {
    CString tolua;
    int i, k;
    DDX_Text(pDX, nIDC, tolua);

    lua_State* LocalLua = lua_open(100);
    lua_baselibopen(LocalLua);
    lua_strlibopen(LocalLua);
    lua_mathlibopen(LocalLua);

    tolua = "return " + tolua;
    i = lua_gettop(LocalLua);
    lua_dostring(LocalLua, tolua);
    k = lua_gettop(LocalLua);
    if (i != k) {
      tolua = lua_tostring(LocalLua, -1);
      if (tolua.GetLength() == 0) {
        DDX_Text(pDX, nIDC, s);
        x = 0;
      } else {
        x = Re(lua_tonumber(LocalLua, -1));
        s.Empty();
      }
    } else {
      DDX_Text(pDX, nIDC, s);
      x = 0;
    }
    lua_close(LocalLua);
  }
}
