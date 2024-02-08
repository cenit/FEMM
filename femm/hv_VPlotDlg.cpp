// VPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hv_VPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hvCVPlotDlg dialog

hvCVPlotDlg::hvCVPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hvCVPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hvCVPlotDlg)
  m_vectorscalefactor = 0.0;
  //}}AFX_DATA_INIT
}

void hvCVPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hvCVPlotDlg)
  DDX_Control(pDX, IDC_HV_VPLOTTYPE, m_vplottype);
  DDX_Text(pDX, IDC_HV_SCALEFACTOR, m_vectorscalefactor);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HV_SCALEFACTOR, m_IDC_vectorscalefactor);
}

BEGIN_MESSAGE_MAP(hvCVPlotDlg, CDialog)
//{{AFX_MSG_MAP(hvCVPlotDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hvCVPlotDlg message handlers

void hvCVPlotDlg::OnOK()
{
  m_plottype = m_vplottype.GetCurSel();

  CDialog::OnOK();
}

BOOL hvCVPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_vplottype.SetCurSel(m_plottype);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}
