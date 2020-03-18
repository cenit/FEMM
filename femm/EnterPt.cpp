// EnterPt.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "EnterPt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnterPt dialog


CEnterPt::CEnterPt(CWnd* pParent /*=NULL*/)
	: CDialog(CEnterPt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnterPt)
	m_coord1 = 0.0;
	m_coord2 = 0.0;
	m_label1 = _T("");
	m_label2 = _T("");
	//}}AFX_DATA_INIT
}

void CEnterPt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnterPt)
	DDX_Text(pDX, IDC_COORD1, m_coord1);
	DDX_Text(pDX, IDC_COORD2, m_coord2);
	DDX_Text(pDX, IDC_LABEL1, m_label1);
	DDX_Text(pDX, IDC_LABEL2, m_label2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COORD1, m_IDC_coord1);
	DDX_Control(pDX, IDC_COORD2, m_IDC_coord2);
}


BEGIN_MESSAGE_MAP(CEnterPt, CDialog)
	//{{AFX_MSG_MAP(CEnterPt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnterPt message handlers

