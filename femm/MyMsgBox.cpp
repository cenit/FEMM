// MyMsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "MyMsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyMsgBox dialog


CMyMsgBox::CMyMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMyMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyMsgBox)
	m_mymsg = _T("");
	//}}AFX_DATA_INIT
}


void CMyMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyMsgBox)
	DDX_Text(pDX, IDC_MYMSGBOXOUTPUT, m_mymsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyMsgBox, CDialog)
	//{{AFX_MSG_MAP(CMyMsgBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMsgBox message handlers
