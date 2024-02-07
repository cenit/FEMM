// BdryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cd_BdryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdCBdryDlg dialog

cdCBdryDlg::cdCBdryDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cdCBdryDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cdCBdryDlg)
  m_BdryName = _T("");
  m_c0 = 0;
  m_c1 = 0;
  m_vs = 0;
  m_qs = 0;
  //}}AFX_DATA_INIT
}

void cdCBdryDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cdCBdryDlg)
  DDX_Control(pDX, IDC_CD_BDRYFORMAT, m_BdryFormat);
  DDX_Text(pDX, IDC_CD_BDRYNAME, m_BdryName);
  DDX_Text(pDX, IDC_CD_C0, m_c0);
  DDX_Text(pDX, IDC_CD_C1, m_c1);
  DDX_Text(pDX, IDC_CD_VBDRY, m_vs);
  DDX_Text(pDX, IDC_CD_QS, m_qs);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CD_VBDRY, m_IDC_VBDRY);
  DDX_Control(pDX, IDC_CD_QS, m_IDC_QS);
  DDX_Control(pDX, IDC_CD_BDRYNAME, m_IDC_BDRYNAME);
  DDX_Control(pDX, IDC_CD_C0, m_IDC_c0);
  DDX_Control(pDX, IDC_CD_C1, m_IDC_c1);
}

BEGIN_MESSAGE_MAP(cdCBdryDlg, CDialog)
//{{AFX_MSG_MAP(cdCBdryDlg)
ON_CBN_SELCHANGE(IDC_CD_BDRYFORMAT, OnSelchangeBdryformat)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdCBdryDlg message handlers

void cdCBdryDlg::OnOK()
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

BOOL cdCBdryDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_BdryFormat.SetCurSel(BdryFormat);
  OnSelchangeBdryformat();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cdCBdryDlg::OnSelchangeBdryformat()
{
  switch (m_BdryFormat.GetCurSel()) {
  case 0:

    m_IDC_VBDRY.EnableWindow(TRUE);
    m_IDC_QS.EnableWindow(FALSE);
    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    break;

  case 1:

    m_IDC_VBDRY.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(FALSE);
    m_IDC_c0.EnableWindow(TRUE);
    m_IDC_c1.EnableWindow(TRUE);

    break;

  case 2:

    m_IDC_VBDRY.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(TRUE);
    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    break;

  default:

    m_IDC_VBDRY.EnableWindow(FALSE);
    m_IDC_QS.EnableWindow(FALSE);
    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    break;
  }
}
