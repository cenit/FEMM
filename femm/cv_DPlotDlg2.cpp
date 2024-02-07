// DPlotDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cv_DPlotDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cvCDPlotDlg2 dialog

cvCDPlotDlg2::cvCDPlotDlg2(CWnd* pParent /*=NULL*/)
    : CDialog(cvCDPlotDlg2::IDD, pParent)
{
  //{{AFX_DATA_INIT(cvCDPlotDlg2)
  m_showit = FALSE;
  m_showlegend = FALSE;
  m_lb2 = 0.0;
  m_ub2 = 0.0;
  m_gscale = FALSE;
  //}}AFX_DATA_INIT
}

void cvCDPlotDlg2::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cvCDPlotDlg2)
  DDX_Control(pDX, IDC_CV_DPLOTTYPE, m_dplottype);
  DDX_Check(pDX, IDC_CV_SHOWIT, m_showit);
  DDX_Check(pDX, IDC_CV_SHOW_LEG2, m_showlegend);
  DDX_Text(pDX, IDC_CV_LB2, m_lb2);
  DDX_Text(pDX, IDC_CV_UB2, m_ub2);
  DDX_Check(pDX, IDC_CV_GSCALE2, m_gscale);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CV_LB2, m_IDC_lb2);
  DDX_Control(pDX, IDC_CV_UB2, m_IDC_ub2);
}

BEGIN_MESSAGE_MAP(cvCDPlotDlg2, CDialog)
//{{AFX_MSG_MAP(cvCDPlotDlg2)
ON_BN_CLICKED(IDC_CV_RESBTN2, OnResbtn2)
ON_CBN_SELCHANGE(IDC_CV_DPLOTTYPE, OnSelchangeDplottype)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cvCDPlotDlg2 message handlers

void cvCDPlotDlg2::OnResbtn2()
{
  // TODO: Add your control notification handler code here
  UpdateData();
  m_ub2 = d_PlotBounds[DensityPlot][1];
  m_lb2 = d_PlotBounds[DensityPlot][0];
  UpdateData(FALSE);
}

BOOL cvCDPlotDlg2::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (listtype == 0) {
    m_dplottype.AddString("|Voltage| (V)");
    m_dplottype.AddString("Re(Voltage) (V)");
    m_dplottype.AddString("Im(Voltage) (V)");
    m_dplottype.AddString("|J| (A/m^2)");
    m_dplottype.AddString("Re(J) (A/m^2)");
    m_dplottype.AddString("Im(J) (A/m^2)");
    m_dplottype.AddString("|E| (V/m)");
    m_dplottype.AddString("Re(E) (V/m)");
    m_dplottype.AddString("Im(E) (V/m)");
  }

  if (listtype == 1) {
    m_dplottype.AddString("Flux Density (T)");
    m_dplottype.AddString("Field Intensity (A/m)");
    m_dplottype.AddString("Current Density (MA/m^2)");
    m_dplottype.AddString("log10(|Flux Density|) Log(T)");
  }

  if (listtype == 2) {
    m_dplottype.AddString("|Flux Density| (T)");
    m_dplottype.AddString("Re(Flux Density) (T)");
    m_dplottype.AddString("Im(Flux Density) (T)");
    m_dplottype.AddString("|Field Intensity| (A/m)");
    m_dplottype.AddString("Re(Field Intensity) (A/m)");
    m_dplottype.AddString("Im(Field Intensity) (A/m)");
    m_dplottype.AddString("|Current Density| (MA/m^2)");
    m_dplottype.AddString("Re(Current Density) (MA/m^2)");
    m_dplottype.AddString("Im(Current Density) (MA/m^2)");
    m_dplottype.AddString("log10(|Flux Density|) Log(T)");
  }

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

void cvCDPlotDlg2::OnOK()
{
  UpdateData();
  PlotBounds[DensityPlot][1] = m_ub2;
  PlotBounds[DensityPlot][0] = m_lb2;
  DensityPlot++;
  CDialog::OnOK();
}

void cvCDPlotDlg2::OnSelchangeDplottype()
{
  UpdateData();
  PlotBounds[DensityPlot][1] = m_ub2;
  PlotBounds[DensityPlot][0] = m_lb2;
  DensityPlot = m_dplottype.GetCurSel();
  m_ub2 = PlotBounds[DensityPlot][1];
  m_lb2 = PlotBounds[DensityPlot][0];
  UpdateData(FALSE);
}
