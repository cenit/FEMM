// MaskProgress.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "MaskProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskProgress dialog


CMaskProgress::CMaskProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CMaskProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaskProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMaskProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaskProgress)
	DDX_Control(pDX, IDC_PROGRESS1, m_mask_progress_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaskProgress, CDialog)
	//{{AFX_MSG_MAP(CMaskProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskProgress message handlers
