// MyCommandLineInfo.h: interface for the CMyCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "afxwin.h"

#if !defined(AFX_MYCOMMANDLINEINFO_H__9571FF44_5FF5_11D5_98F9_005004B84D6A__INCLUDED_)
#define AFX_MYCOMMANDLINEINFO_H__9571FF44_5FF5_11D5_98F9_005004B84D6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyCommandLineInfo : public CCommandLineInfo  
{
public:
//	int luavarstart;
//	bool parseluastack;
//	CString lastparameter;
//	int parameterno;
//	bool luafilenameready;
	CMyCommandLineInfo();
	virtual ~CMyCommandLineInfo();
	virtual void ParseParam (LPCTSTR lpszParam,BOOL bFlag,BOOL bLast );




};

#endif // !defined(AFX_MYCOMMANDLINEINFO_H__9571FF44_5FF5_11D5_98F9_005004B84D6A__INCLUDED_)
