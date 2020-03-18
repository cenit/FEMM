#pragma once
#include "afxwin.h"


// CPromptBox dialog

class CPromptBox : public CDialog
{
	DECLARE_DYNAMIC(CPromptBox)

public:
	CPromptBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPromptBox();
	double ParseDouble();

// Dialog Data
	enum { IDD = IDD_PROMPTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString instring;
	CString mytitle;
	CLuaEdit lua_instring;
	virtual BOOL OnInitDialog();
};
