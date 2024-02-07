// DPlotDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hv_DPlotDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hvCDPlotDlg2 dialog

hvCDPlotDlg2::hvCDPlotDlg2(CWnd* pParent /*=NULL*/)
    : CDialog(hvCDPlotDlg2::IDD, pParent)
{
  //{{AFX_DATA_INIT(hvCDPlotDlg2)
  m_showit = FALSE;
  m_showlegend = FALSE;
  m_lb2 = 0.0;
  m_ub2 = 0.0;
  m_gscale = FALSE;
  //}}AFX_DATA_INIT
}

void hvCDPlotDlg2::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hvCDPlotDlg2)
  DDX_Control(pDX, IDC_HV_DPLOTTYPE, m_dplottype);
  DDX_Check(pDX, IDC_HV_SHOWIT, m_showit);
  DDX_Check(pDX, IDC_HV_SHOW_LEG2, m_showlegend);
  DDX_Text(pDX, IDC_HV_LB2, m_lb2);
  DDX_Text(pDX, IDC_HV_UB2, m_ub2);
  DDX_Check(pDX, IDC_HV_GSCALE2, m_gscale);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HV_LB2, m_IDC_lb2);
  DDX_Control(pDX, IDC_HV_UB2, m_IDC_ub2);
}

BEGIN_MESSAGE_MAP(hvCDPlotDlg2, CDialog)
//{{AFX_MSG_MAP(hvCDPlotDlg2)
ON_BN_CLICKED(IDC_HV_RESBTN2, OnResbtn2)
ON_CBN_SELCHANGE(IDC_HV_DPLOTTYPE, OnSelchangeDplottype)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hvCDPlotDlg2 message handlers

void hvCDPlotDlg2::OnResbtn2()
{
  // TODO: Add your control notification handler code here
  UpdateData();
  m_ub2 = d_PlotBounds[DensityPlot][1];
  m_lb2 = d_PlotBounds[DensityPlot][0];
  UpdateData(FALSE);
}

BOOL hvCDPlotDlg2::OnInitDialog()
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

void hvCDPlotDlg2::OnOK()
{
  UpdateData();
  PlotBounds[DensityPlot][1] = m_ub2;
  PlotBounds[DensityPlot][0] = m_lb2;
  DensityPlot++;
  CDialog::OnOK();
}

void hvCDPlotDlg2::OnSelchangeDplottype()
{
  UpdateData();
  PlotBounds[DensityPlot][1] = m_ub2;
  PlotBounds[DensityPlot][0] = m_lb2;
  DensityPlot = m_dplottype.GetCurSel();
  m_ub2 = PlotBounds[DensityPlot][1];
  m_lb2 = PlotBounds[DensityPlot][0];
  UpdateData(FALSE);
}
