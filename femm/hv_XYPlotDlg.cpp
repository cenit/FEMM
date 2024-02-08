// XYPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afx.h>
#include <afxtempl.h>
#include "hv_problem.h"
#include "xyplot.h"
#include "hviewDoc.h"
#include "hviewView.h"
#include "MainFrm.h"

#include "lua.h"

#include "hv_XYPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hvCXYPlotDlg dialog

hvCXYPlotDlg::hvCXYPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hvCXYPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hvCXYPlotDlg)
  m_ToFile = FALSE;
  m_npoints = 150;
  //}}AFX_DATA_INIT
  ListType = 0;
  XYPlotType = 0;
  FileFormat = 0;
}

void hvCXYPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hvCXYPlotDlg)
  DDX_Control(pDX, IDC_HV_FILEFORMAT, m_fileformat);
  DDX_Control(pDX, IDC_HV_XYPLOTTYPE, m_XYPlotType);
  DDX_Check(pDX, IDC_HV_TOFILE, m_ToFile);
  DDX_Text(pDX, IDC_HV_NPOINTS, m_npoints);
  DDV_MinMaxInt(pDX, m_npoints, 10, 100000);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HV_NPOINTS, m_IDC_npoints);
}

BEGIN_MESSAGE_MAP(hvCXYPlotDlg, CDialog)
//{{AFX_MSG_MAP(hvCXYPlotDlg)
ON_BN_CLICKED(IDC_HV_TOFILE, OnToFileClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hvCXYPlotDlg message handlers

BOOL hvCXYPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_XYPlotType.AddString("T     (Temperature)");
  m_XYPlotType.AddString("|F|   (Magnitude of heat flux density)");
  m_XYPlotType.AddString("F . n (Normal heat flux density)");
  m_XYPlotType.AddString("F . t (Tangential heat flux density)");
  m_XYPlotType.AddString("|G|   (Magnitude of field gradient)");
  m_XYPlotType.AddString("G . n (Normal temperature gradient)");
  m_XYPlotType.AddString("G . t (Tangential temperature gradient)");
  m_XYPlotType.SetCurSel(0);

  m_fileformat.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hvCXYPlotDlg::OnOK()
{
  // TODO: Add extra validation here
  XYPlotType = m_XYPlotType.GetCurSel();
  FileFormat = m_fileformat.GetCurSel();
  CDialog::OnOK();
}

void hvCXYPlotDlg::OnToFileClicked()
{
  UpdateData();
}
