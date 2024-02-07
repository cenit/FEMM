// GapPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afx.h>
#include <afxtempl.h>
#include "problem.h"
#include "xyplot.h"
#include "femmviewDoc.h"
#include "femmviewView.h"
#include "MainFrm.h"
#include "lua.h"

#include "GapPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGapPlotDlg dialog

CGapPlotDlg::CGapPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CGapPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CGapPlotDlg)
  m_ToFile = FALSE;
  m_npoints = 150;
  m_age = 0;
  //}}AFX_DATA_INIT
  XYPlotType = 0;
  FileFormat = 0;
}

void CGapPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CGapPlotDlg)
  DDX_Control(pDX, IDC_FILEFORMAT, m_fileformat);
  DDX_Control(pDX, IDC_XYPLOTTYPE, m_XYPlotType);
  DDX_Control(pDX, IDC_BDRYNAME, m_BdryName);
  DDX_Check(pDX, IDC_TOFILE, m_ToFile);
  DDX_Text(pDX, IDC_NPOINTS, m_npoints);
  DDV_MinMaxInt(pDX, m_npoints, 10, 100000);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_NPOINTS, m_IDC_npoints);
}

BEGIN_MESSAGE_MAP(CGapPlotDlg, CDialog)
//{{AFX_MSG_MAP(CGapPlotDlg)
ON_BN_CLICKED(IDC_TOFILE, OnToFileClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGapPlotDlg message handlers

BOOL CGapPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CArray<CAirGapElement, CAirGapElement&>& agelist = *pagelist;
  for (int k = 0; k < agelist.GetSize(); k++)
    m_BdryName.AddString(agelist[k].BdryName);
  m_BdryName.SetCurSel(0);

  m_XYPlotType.AddString("Potential");
  m_XYPlotType.AddString("|B|      (Magnitude of flux density)");
  m_XYPlotType.AddString("B . n   (Normal flux density)");
  m_XYPlotType.AddString("B . t   (Tangential flux density)");
  m_XYPlotType.AddString("|H|      (Magnitude of field intensity)");
  m_XYPlotType.AddString("H . n   (Normal field intensity)");
  m_XYPlotType.AddString("H . t   (Tangential field intensity)");
  m_XYPlotType.SetCurSel(1);

  m_fileformat.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CGapPlotDlg::OnOK()
{
  // TODO: Add extra validation here
  XYPlotType = m_XYPlotType.GetCurSel();
  FileFormat = m_fileformat.GetCurSel();
  m_age = m_BdryName.GetCurSel();
  CDialog::OnOK();
}

void CGapPlotDlg::OnToFileClicked()
{
  UpdateData();
}
