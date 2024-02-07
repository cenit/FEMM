// VPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "VPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVPlotDlg dialog

CVPlotDlg::CVPlotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVPlotDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CVPlotDlg)
  m_vectorscalefactor = 0.0;
  //}}AFX_DATA_INIT
}

void CVPlotDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CVPlotDlg)
  DDX_Control(pDX, IDC_VPLOTTYPE, m_vplottype);
  DDX_Text(pDX, IDC_VPLOT_SCALE, m_vectorscalefactor);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_VPLOT_SCALE, m_IDC_vectorscalefactor);
}

BEGIN_MESSAGE_MAP(CVPlotDlg, CDialog)
//{{AFX_MSG_MAP(CVPlotDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVPlotDlg message handlers

void CVPlotDlg::OnOK()
{
  m_plottype = m_vplottype.GetCurSel();

  CDialog::OnOK();
}

BOOL CVPlotDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (ListType == 0) {
    m_vplottype.AddString("<None>");
    m_vplottype.AddString("B");
    m_vplottype.AddString("H");
    if ((m_plottype > 2) || (m_plottype < 0))
      m_plottype = 0;
    m_vplottype.SetCurSel(m_plottype);
  }

  if (ListType == 1) {
    m_vplottype.AddString("<None>");
    m_vplottype.AddString("B_re");
    m_vplottype.AddString("H_re");
    m_vplottype.AddString("B_im");
    m_vplottype.AddString("H_im");
    m_vplottype.AddString("B_re & B_im");
    m_vplottype.AddString("H_re & H_im");
    if ((m_plottype > 6) || (m_plottype < 0))
      m_plottype = 0;
    m_vplottype.SetCurSel(m_plottype);
  }

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}
