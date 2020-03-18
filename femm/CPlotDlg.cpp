// CPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "CPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPlotDlg dialog


CCPlotDlg::CCPlotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPlotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPlotDlg)
	m_numcontours = 0;
	m_showai = FALSE;
	m_showar = FALSE;
	m_ahigh = 0.0;
	m_alow = 0.0;
	m_showmask = FALSE;
	//}}AFX_DATA_INIT
}


void CCPlotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPlotDlg)
	DDX_Text(pDX, IDC_NUMCONTOURS, m_numcontours);
	DDV_MinMaxInt(pDX, m_numcontours, 4, 999);
	DDX_Check(pDX, IDC_SHOW_A_IM, m_showai);
	DDX_Check(pDX, IDC_SHOW_A_RE, m_showar);
	DDX_Text(pDX, IDC_AHIGH, m_ahigh);
	DDX_Text(pDX, IDC_ALOW, m_alow);
	DDX_Check(pDX, IDC_SHOW_MASK1, m_showmask);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NUMCONTOURS, m_IDC_numcontours);
	DDX_Control(pDX, IDC_AHIGH, m_IDC_ahigh);
	DDX_Control(pDX, IDC_ALOW, m_IDC_alow);
}


BEGIN_MESSAGE_MAP(CCPlotDlg, CDialog)
	//{{AFX_MSG_MAP(CCPlotDlg)
	ON_BN_CLICKED(IDC_DFLT1, OnDflt1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPlotDlg message handlers

void CCPlotDlg::OnDflt1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_alow=Alb;
	m_ahigh=Aub;
	m_numcontours=19;
	UpdateData(FALSE);
}
