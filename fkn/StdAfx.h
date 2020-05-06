// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#include <afxwin.h> // MFC core and standard components
#include <afxext.h> // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> // MFC support for Windows 95 Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

int MsgBox(CString s);
int MsgBox(PSTR sz, ...);