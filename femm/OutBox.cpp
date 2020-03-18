// OutBox.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "OutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFemmApp theApp;
/////////////////////////////////////////////////////////////////////////////
// COutBox dialog


COutBox::COutBox(CWnd* pParent /*=NULL*/)
	: CDialog(COutBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutBox, CDialog)
	//{{AFX_MSG_MAP(COutBox)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutBox message handlers

void COutBox::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	theApp.bShowOutputWindow=FALSE;
	CDialog::OnClose();
}


