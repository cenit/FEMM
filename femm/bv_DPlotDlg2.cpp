// DPlotDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bv_DPlotDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bvCDPlotDlg2 dialog

bvCDPlotDlg2::bvCDPlotDlg2(CWnd* pParent /*=NULL*/)
    : CDialog(bvCDPlotDlg2::IDD, pParent)
{
  //{{AFX_DATA_INIT(bvCDPlotDlg2)
  m_showit = FALSE;
  m_showlegend = FALSE;
  m_lb2 = 0.0;
  m_ub2 = 0.0;
  m_gscale = FALSE;
  //}}AFX_DATA_INIT
}

void bvCDPlotDlg2::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bvCDPlotDlg2)
  DDX_Control(pDX, IDC_BV_DPLOTTYPE, m_dplottype);
  DDX_Check(pDX, IDC_BV_SHOWIT, m_showit);
  DDX_Check(pDX, IDC_BV_SHOW_LEG2, m_showlegend);
  DDX_Text(pDX, IDC_BV_LB2, m_lb2);
  DDX_Text(pDX, IDC_BV_UB2, m_ub2);
  DDX_Check(pDX, IDC_BV_GSCALE2, m_gscale);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BV_LB2, m_IDC_lb2);
  DDX_Control(pDX, IDC_BV_UB2, m_IDC_ub2);
}

BEGIN_MESSAGE_MAP(bvCDPlotDlg2, CDialog)
//{{AFX_MSG_MAP(bvCDPlotDlg2)
ON_BN_CLICKED(IDC_BV_RESBTN2, OnResbtn2)
ON_CBN_SELCHANGE(IDC_BV_DPLOTTYPE, OnSelchangeDplottype)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bvCDPlotDlg2 message handlers

void bvCDPlotDlg2::OnResbtn2()
{
  // TODO: Add your control notification handler code here
  UpdateData();
  m_ub2 = d_PlotBounds[DensityPlot][1];
  m_lb2 = d_PlotBounds[DensityPlot][0];
  UpdateData(FALSE);
}

BOOL bvCDPlotDlg2::OnInitDialog()
{
  CDialog::OnInitDialog();

  DensityPlot--;
  if (DensityPlot < 0)
    DensityPlot = 0;
  m_dplottype.SetCurSel(DensityPlot);
  m_ub2 = PlotBounds[DensityPlot][1];
  m_lb2 = PlotBounds[DensityPlot][0];
  UpdateData(FALSE);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bvCDPlotDlg2::OnOK()
{
  UpdateData();
  PlotBounds[DensityPlot][1] = m_ub2;
  PlotBounds[DensityPlot][0] = m_lb2;
  DensityPlot++;
  CDialog::OnOK();
}

void bvCDPlotDlg2::OnSelchangeDplottype()
{
  UpdateData();
  PlotBounds[DensityPlot][1] = m_ub2;
  PlotBounds[DensityPlot][0] = m_lb2;
  DensityPlot = m_dplottype.GetCurSel();
  m_ub2 = PlotBounds[DensityPlot][1];
  m_lb2 = PlotBounds[DensityPlot][0];
  UpdateData(FALSE);
}
