// GroupNumber.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "GroupNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupNumber dialog


CGroupNumber::CGroupNumber(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupNumber::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupNumber)
	m_groupnumber = 0;
	//}}AFX_DATA_INIT
}


void CGroupNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupNumber)
	DDX_Text(pDX, IDC_GROUPNUMBER, m_groupnumber);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GROUPNUMBER, m_IDC_groupnumber);
}


BEGIN_MESSAGE_MAP(CGroupNumber, CDialog)
	//{{AFX_MSG_MAP(CGroupNumber)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupNumber message handlers
