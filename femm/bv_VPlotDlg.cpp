// VPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bv_VPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bvCVPlotDlg dialog

bvCVPlotDlg::bvCVPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bvCVPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bvCVPlotDlg)
  m_vectorscalefactor = 0.0;
  //}}AFX_DATA_INIT
}

void bvCVPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bvCVPlotDlg)
  DDX_Control(pDX, IDC_BV_VPLOTTYPE, m_vplottype);
  DDX_Text(pDX, IDC_BV_SCALEFACTOR, m_vectorscalefactor);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BV_SCALEFACTOR, m_IDC_vectorscalefactor);
}

BEGIN_MESSAGE_MAP(bvCVPlotDlg, CDialog)
//{{AFX_MSG_MAP(bvCVPlotDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bvCVPlotDlg message handlers

void bvCVPlotDlg::OnOK()
{
  m_plottype = m_vplottype.GetCurSel();

  CDialog::OnOK();
}

BOOL bvCVPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_vplottype.SetCurSel(m_plottype);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}
