// BdryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hd_BdryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCBdryDlg dialog

hdCBdryDlg::hdCBdryDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hdCBdryDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCBdryDlg)
  m_BdryName = _T("");
  m_qs = 0.0;
  m_beta = 0.0;
  m_htc = 0.0;
  m_To2 = 0.0;
  m_To1 = 0.0;
  m_Tset = 0.0;
  //}}AFX_DATA_INIT
}

void hdCBdryDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCBdryDlg)
  DDX_Control(pDX, IDC_HD_BDRYFORMAT, m_BdryFormat);
  DDX_Text(pDX, IDC_HD_BDRYNAME, m_BdryName);
  DDX_Text(pDX, IDC_HD_QS, m_qs);
  DDX_Text(pDX, IDC_HD_BETA, m_beta);
  DDX_Text(pDX, IDC_HD_HTC, m_htc);
  DDX_Text(pDX, IDC_HD_TINF2, m_To2);
  DDX_Text(pDX, IDC_HD_TINF1, m_To1);
  DDX_Text(pDX, IDC_HD_TBDRY, m_Tset);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_TBDRY, m_IDC_Tset);
  DDX_Control(pDX, IDC_HD_QS, m_IDC_QS);
  DDX_Control(pDX, IDC_HD_BDRYNAME, m_IDC_BDRYNAME);
  DDX_Control(pDX, IDC_HD_BETA, m_IDC_beta);
  DDX_Control(pDX, IDC_HD_HTC, m_IDC_htc);
  DDX_Control(pDX, IDC_HD_TINF1, m_IDC_To1);
  DDX_Control(pDX, IDC_HD_TINF2, m_IDC_To2);
}

BEGIN_MESSAGE_MAP(hdCBdryDlg, CDialog)
//{{AFX_MSG_MAP(hdCBdryDlg)
ON_CBN_SELCHANGE(IDC_HD_BDRYFORMAT, OnSelchangeBdryformat)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCBdryDlg message handlers

void hdCBdryDlg::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_BdryName == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_BdryName);
      return;
    }
  }

  BdryFormat = m_BdryFormat.GetCurSel();

  CDialog::OnOK();
}

BOOL hdCBdryDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_BdryFormat.SetCurSel(BdryFormat);
  OnSelchangeBdryformat();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hdCBdryDlg::OnSelchangeBdryformat()
{
  switch (m_BdryFormat.GetCurSel()) {
  case 0:

    m_IDC_Tset.EnableWindow(TRUE);
    m_IDC_QS.EnableWindow(FALSE);
    m_IDC_htc.EnableWindow(FALSE);
    m_IDC_To1.EnableWindow(FALSE);
    m_IDC_beta.EnableWindow(FALSE);
    m_IDC_To2.EnableWindow(FALSE);

    break;

  case 1:

    m_IDC_Tset.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(TRUE);
    m_IDC_htc.EnableWindow(FALSE);
    m_IDC_To1.EnableWindow(FALSE);
    m_IDC_beta.EnableWindow(FALSE);
    m_IDC_To2.EnableWindow(FALSE);

    break;

  case 2:

    m_IDC_Tset.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(TRUE);
    m_IDC_htc.EnableWindow(TRUE);
    m_IDC_To1.EnableWindow(TRUE);
    m_IDC_beta.EnableWindow(FALSE);
    m_IDC_To2.EnableWindow(FALSE);

    break;

  case 3:

    m_IDC_Tset.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(TRUE);
    m_IDC_htc.EnableWindow(TRUE);
    m_IDC_To1.EnableWindow(TRUE);
    m_IDC_beta.EnableWindow(TRUE);
    m_IDC_To2.EnableWindow(TRUE);
    break;

  default:

    m_IDC_Tset.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(FALSE);
    m_IDC_htc.EnableWindow(FALSE);
    m_IDC_To1.EnableWindow(FALSE);
    m_IDC_beta.EnableWindow(FALSE);
    m_IDC_To2.EnableWindow(FALSE);

    break;
  }
}
