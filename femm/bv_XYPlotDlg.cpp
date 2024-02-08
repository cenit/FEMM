// XYPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afx.h>
#include <afxtempl.h>
#include "bv_problem.h"
#include "xyplot.h"
#include "belaviewDoc.h"
#include "belaviewView.h"
#include "MainFrm.h"

#include "lua.h"

#include "bv_XYPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bvCXYPlotDlg dialog

bvCXYPlotDlg::bvCXYPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bvCXYPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bvCXYPlotDlg)
  m_ToFile = FALSE;
  m_npoints = 150;
  //}}AFX_DATA_INIT
  ListType = 0;
  XYPlotType = 0;
  FileFormat = 0;
}

void bvCXYPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bvCXYPlotDlg)
  DDX_Control(pDX, IDC_BV_FILEFORMAT, m_fileformat);
  DDX_Control(pDX, IDC_BV_XYPLOTTYPE, m_XYPlotType);
  DDX_Check(pDX, IDC_BV_TOFILE, m_ToFile);
  DDX_Text(pDX, IDC_BV_NPOINTS, m_npoints);
  DDV_MinMaxInt(pDX, m_npoints, 10, 100000);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BV_NPOINTS, m_IDC_npoints);
}

BEGIN_MESSAGE_MAP(bvCXYPlotDlg, CDialog)
//{{AFX_MSG_MAP(bvCXYPlotDlg)
ON_BN_CLICKED(IDC_BV_TOFILE, OnToFileClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bvCXYPlotDlg message handlers

BOOL bvCXYPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_XYPlotType.AddString("V     (Voltage)");
  m_XYPlotType.AddString("|D|   (Magnitude of flux density)");
  m_XYPlotType.AddString("D . n (Normal flux density)");
  m_XYPlotType.AddString("D . t (Tangential flux density)");
  m_XYPlotType.AddString("|E|   (Magnitude of field intensity)");
  m_XYPlotType.AddString("E . n (Normal field intensity)");
  m_XYPlotType.AddString("E . t (Tangential field intensity)");
  m_XYPlotType.SetCurSel(0);

  m_fileformat.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bvCXYPlotDlg::OnOK()
{
  // TODO: Add extra validation here
  XYPlotType = m_XYPlotType.GetCurSel();
  FileFormat = m_fileformat.GetCurSel();
  CDialog::OnOK();
}

void bvCXYPlotDlg::OnToFileClicked()
{
  UpdateData();
}
