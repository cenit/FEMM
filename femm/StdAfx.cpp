// stdafx.cpp : source file that includes just the standard includes
//	femm.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "lua.h"
#include "luadebug.h"
#include "femm.h"

extern BOOL bLinehook;
extern lua_State* lua;
extern CFemmApp theApp;

int MsgBox(CString s)
{
  if (theApp.bActiveX) {
    theApp.MatlabLoveNote = s;
    return IDOK;
  } else
    return AfxMessageBox(s);
}

int MsgBox(PSTR sz, ...)
{
  CString ach;
  va_list args;
  va_start(args, sz);
  ach.FormatV(sz, args);
  if (theApp.bActiveX) {
    theApp.MatlabLoveNote = ach;
    return IDOK;
  } else
    return AfxMessageBox(ach);
}

double sq(double x)
{
  return x * x;
}

CString ToString(CComplex x)
{
  return ToString(x, 8);
}

CString ToString(CComplex x, int n)
{
  char c[80];
  CString s;

  s = x.ToString(c);
  return s;
}
