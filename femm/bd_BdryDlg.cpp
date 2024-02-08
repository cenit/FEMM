// BdryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bd_BdryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCBdryDlg dialog

bdCBdryDlg::bdCBdryDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bdCBdryDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCBdryDlg)
  m_BdryName = _T("");
  m_c0 = 0.0;
  m_c1 = 0.0;
  m_v = 0.0;
  m_qs = 0.0;
  //}}AFX_DATA_INIT
}

void bdCBdryDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCBdryDlg)
  DDX_Control(pDX, IDC_BD_BDRYFORMAT, m_BdryFormat);
  DDX_Text(pDX, IDC_BD_BDRYNAME, m_BdryName);
  DDX_Text(pDX, IDC_BD_C0, m_c0);
  DDX_Text(pDX, IDC_BD_C1, m_c1);
  DDX_Text(pDX, IDC_BD_VBDRY, m_v);
  DDX_Text(pDX, IDC_BD_QS, m_qs);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_VBDRY, m_IDC_VBDRY);
  DDX_Control(pDX, IDC_BD_QS, m_IDC_QS);
  DDX_Control(pDX, IDC_BD_BDRYNAME, m_IDC_BDRYNAME);
  DDX_Control(pDX, IDC_BD_C0, m_IDC_c0);
  DDX_Control(pDX, IDC_BD_C1, m_IDC_c1);
}

BEGIN_MESSAGE_MAP(bdCBdryDlg, CDialog)
//{{AFX_MSG_MAP(bdCBdryDlg)
ON_CBN_SELCHANGE(IDC_BD_BDRYFORMAT, OnSelchangeBdryformat)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCBdryDlg message handlers

void bdCBdryDlg::OnOK()
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

BOOL bdCBdryDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_BdryFormat.SetCurSel(BdryFormat);
  OnSelchangeBdryformat();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCBdryDlg::OnSelchangeBdryformat()
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
