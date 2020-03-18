// KbdZoom.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "KbdZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKbdZoom dialog


CKbdZoom::CKbdZoom(CWnd* pParent /*=NULL*/)
	: CDialog(CKbdZoom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKbdZoom)
	m_scr_bottom = 0.0;
	m_scr_left = 0.0;
	m_scr_right = 0.0;
	m_scr_top = 0.0;
	//}}AFX_DATA_INIT
}


void CKbdZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKbdZoom)
	DDX_Text(pDX, IDC_SCRBOTTOM, m_scr_bottom);
	DDX_Text(pDX, IDC_SCRLEFT, m_scr_left);
	DDX_Text(pDX, IDC_SCRRIGHT, m_scr_right);
	DDX_Text(pDX, IDC_SCRTOP, m_scr_top);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SCRBOTTOM, m_IDC_scr_bottom);
	DDX_Control(pDX, IDC_SCRLEFT, m_IDC_scr_left);
	DDX_Control(pDX, IDC_SCRRIGHT, m_IDC_scr_right);
	DDX_Control(pDX, IDC_SCRTOP, m_IDC_scr_top);
}


BEGIN_MESSAGE_MAP(CKbdZoom, CDialog)
	//{{AFX_MSG_MAP(CKbdZoom)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKbdZoom message handlers
