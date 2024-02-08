// XYPlotDlg.cpp : implementation file
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

#include "XYPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXYPlotDlg dialog

CXYPlotDlg::CXYPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CXYPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CXYPlotDlg)
  m_ToFile = FALSE;
  m_npoints = 150;
  //}}AFX_DATA_INIT
  ListType = 0;
  XYPlotType = 0;
  FileFormat = 0;
}

void CXYPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CXYPlotDlg)
  DDX_Control(pDX, IDC_FILEFORMAT, m_fileformat);
  DDX_Control(pDX, IDC_XYPLOTTYPE, m_XYPlotType);
  DDX_Check(pDX, IDC_TOFILE, m_ToFile);
  DDX_Text(pDX, IDC_NPOINTS, m_npoints);
  DDV_MinMaxInt(pDX, m_npoints, 10, 100000);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_NPOINTS, m_IDC_npoints);
}

BEGIN_MESSAGE_MAP(CXYPlotDlg, CDialog)
//{{AFX_MSG_MAP(CXYPlotDlg)
ON_BN_CLICKED(IDC_TOFILE, OnToFileClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYPlotDlg message handlers

BOOL CXYPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (ListType == 0) {
    m_XYPlotType.AddString("Potential");
    m_XYPlotType.AddString("|B|      (Magnitude of flux density)");
    m_XYPlotType.AddString("B . n   (Normal flux density)");
    m_XYPlotType.AddString("B . t   (Tangential flux density)");
    m_XYPlotType.AddString("|H|      (Magnitude of field intensity)");
    m_XYPlotType.AddString("H . n   (Normal field intensity)");
    m_XYPlotType.AddString("H . t   (Tangential field intensity)");
    m_XYPlotType.SetCurSel(1);
  }

  if (ListType == 1) {
    m_XYPlotType.AddString("Potential");
    m_XYPlotType.AddString("|B|        (Magnitude of flux density)");
    m_XYPlotType.AddString("B . n      (Normal flux density)");
    m_XYPlotType.AddString("B . t      (Tangential flux density)");
    m_XYPlotType.AddString("|H|        (Magnitude of field intensity)");
    m_XYPlotType.AddString("H . n      (Normal field intensity)");
    m_XYPlotType.AddString("H . t      (Tangential field intensity)");
    m_XYPlotType.AddString("J_eddy     (Eddy current density)");
    m_XYPlotType.AddString("Js+J_eddy  (Source+eddy current density)");
    m_XYPlotType.SetCurSel(1);
  }

  if (ListType == 2) {
    int k;
    for (k = 0; k < pDoc->blockproplist.GetSize(); k++)
      if (pDoc->blockproplist[k].BHpoints > 0)
        m_XYPlotType.AddString(pDoc->blockproplist[k].BlockName);
    m_XYPlotType.SetCurSel(0);
    SetWindowText("Plot BH Curves");
  }

  m_fileformat.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CXYPlotDlg::OnOK()
{
  // TODO: Add extra validation here
  XYPlotType = m_XYPlotType.GetCurSel();
  FileFormat = m_fileformat.GetCurSel();
  CDialog::OnOK();
}

void CXYPlotDlg::OnToFileClicked()
{
  UpdateData();
}
