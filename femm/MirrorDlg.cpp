// MirrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "MirrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMirrorDlg dialog


CMirrorDlg::CMirrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMirrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMirrorDlg)
	m_pax = 0.0;
	m_pay = 0.0;
	m_pbx = 0.0;
	m_pby = 0.0;
	//}}AFX_DATA_INIT
}


void CMirrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMirrorDlg)
	DDX_Text(pDX, IDC_PAX, m_pax);
	DDX_Text(pDX, IDC_PAY, m_pay);
	DDX_Text(pDX, IDC_PBX, m_pbx);
	DDX_Text(pDX, IDC_PBY, m_pby);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PAX, m_IDC_PAX);
	DDX_Control(pDX, IDC_PAY, m_IDC_PAY);
	DDX_Control(pDX, IDC_PBX, m_IDC_PBX);
	DDX_Control(pDX, IDC_PBY, m_IDC_PBY);
}


BEGIN_MESSAGE_MAP(CMirrorDlg, CDialog)
	//{{AFX_MSG_MAP(CMirrorDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMirrorDlg message handlers
