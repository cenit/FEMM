// XYPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afx.h>
#include <afxtempl.h>
#include "cv_problem.h"
#include "xyplot.h"
#include "cviewDoc.h"
#include "cviewView.h"
#include "MainFrm.h"

#include "lua.h"

#include "cv_XYPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cvCXYPlotDlg dialog

cvCXYPlotDlg::cvCXYPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cvCXYPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cvCXYPlotDlg)
  m_ToFile = FALSE;
  m_npoints = 150;
  //}}AFX_DATA_INIT
  ListType = 0;
  XYPlotType = 0;
  FileFormat = 0;
}

void cvCXYPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cvCXYPlotDlg)
  DDX_Control(pDX, IDC_CV_FILEFORMAT, m_fileformat);
  DDX_Control(pDX, IDC_CV_XYPLOTTYPE, m_XYPlotType);
  DDX_Check(pDX, IDC_CV_TOFILE, m_ToFile);
  DDX_Text(pDX, IDC_CV_NPOINTS, m_npoints);
  DDV_MinMaxInt(pDX, m_npoints, 10, 100000);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CV_NPOINTS, m_IDC_npoints);
}

BEGIN_MESSAGE_MAP(cvCXYPlotDlg, CDialog)
//{{AFX_MSG_MAP(cvCXYPlotDlg)
ON_BN_CLICKED(IDC_CV_TOFILE, OnToFileClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cvCXYPlotDlg message handlers

BOOL cvCXYPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_XYPlotType.AddString("V     (Voltage)");
  m_XYPlotType.AddString("|J|   (Magnitude of current density)");
  m_XYPlotType.AddString("J . n (Normal current density)");
  m_XYPlotType.AddString("J . t (Tangential current density)");
  m_XYPlotType.AddString("|E|   (Magnitude of electric field intensity)");
  m_XYPlotType.AddString("E . n (Normal electric field intensity)");
  m_XYPlotType.AddString("E . t (Tangential electric field intensity)");
  m_XYPlotType.AddString("|Jc|   (Magnitude of conduction current density)");
  m_XYPlotType.AddString("Jc . n (Normal conduction current density)");
  m_XYPlotType.AddString("Jc . t (Tangential conduction current density)");
  m_XYPlotType.AddString("|Jd|   (Magnitude of displacement current density)");
  m_XYPlotType.AddString("Jd . n (Normal displacement current density)");
  m_XYPlotType.AddString("Jd . t (Tangential displacement current density)");
  m_XYPlotType.SetCurSel(0);

  m_fileformat.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cvCXYPlotDlg::OnOK()
{
  // TODO: Add extra validation here
  XYPlotType = m_XYPlotType.GetCurSel();
  FileFormat = m_fileformat.GetCurSel();
  CDialog::OnOK();
}

void cvCXYPlotDlg::OnToFileClicked()
{
  UpdateData();
}
