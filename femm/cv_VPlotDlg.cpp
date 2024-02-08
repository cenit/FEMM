// VPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cv_VPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cvCVPlotDlg dialog

cvCVPlotDlg::cvCVPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cvCVPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cvCVPlotDlg)
  m_vectorscalefactor = 0.0;
  //}}AFX_DATA_INIT
}

void cvCVPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cvCVPlotDlg)
  DDX_Control(pDX, IDC_CV_VPLOTTYPE, m_vplottype);
  DDX_Text(pDX, IDC_CV_SCALEFACTOR, m_vectorscalefactor);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CV_SCALEFACTOR, m_IDC_vectorscalefactor);
}

BEGIN_MESSAGE_MAP(cvCVPlotDlg, CDialog)
//{{AFX_MSG_MAP(cvCVPlotDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cvCVPlotDlg message handlers

void cvCVPlotDlg::OnOK()
{
  m_plottype = m_vplottype.GetCurSel();

  CDialog::OnOK();
}

BOOL cvCVPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_vplottype.SetCurSel(m_plottype);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}
