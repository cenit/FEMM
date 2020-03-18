// ExteriorProps.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "ExteriorProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExteriorProps dialog


CExteriorProps::CExteriorProps(CWnd* pParent /*=NULL*/)
	: CDialog(CExteriorProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExteriorProps)
	m_Ri = 0.0;
	m_Ro = 0.0;
	m_Zo = 0.0;
	//}}AFX_DATA_INIT
}


void CExteriorProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExteriorProps)
	DDX_Text(pDX, IDC_RI, m_Ri);
	DDX_Text(pDX, IDC_RO, m_Ro);
	DDX_Text(pDX, IDC_ZO, m_Zo);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RO, m_IDC_RO);
	DDX_Control(pDX, IDC_RI, m_IDC_RI);
	DDX_Control(pDX, IDC_ZO, m_IDC_ZO);
}


BEGIN_MESSAGE_MAP(CExteriorProps, CDialog)
	//{{AFX_MSG_MAP(CExteriorProps)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExteriorProps message handlers
