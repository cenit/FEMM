// MakeABCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "MakeABCDlg.h"

// CMakeABCDlg dialog

IMPLEMENT_DYNAMIC(CMakeABCDlg, CDialog)

CMakeABCDlg::CMakeABCDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMakeABCDlg::IDD, pParent)
    , abcn(0)
    , abcr(0)
    , abcx(0)
    , abcy(0)
{
}

CMakeABCDlg::~CMakeABCDlg()
{
}

void CMakeABCDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_ABCN, abcn);
  DDV_MinMaxInt(pDX, abcn, 1, 12);
  DDX_Text(pDX, IDC_ABCR, abcr);
  DDX_Text(pDX, IDC_ABCX, abcx);
  DDX_Text(pDX, IDC_ABCY, abcy);

  DDX_Control(pDX, IDC_ABCN, m_IDC_ABCN);
  DDX_Control(pDX, IDC_ABCR, m_IDC_ABCR);
  DDX_Control(pDX, IDC_ABCX, m_IDC_ABCX);
  DDX_Control(pDX, IDC_ABCY, m_IDC_ABCY);
  DDX_Control(pDX, IDC_COMBO1, m_edgetype);
}

BEGIN_MESSAGE_MAP(CMakeABCDlg, CDialog)
ON_CBN_SELCHANGE(IDC_COMBO1, &CMakeABCDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

// CMakeABCDlg message handlers

void CMakeABCDlg::OnCbnSelchangeCombo1()
{
  // TODO: Add your control notification handler code here
}

BOOL CMakeABCDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_edgetype.AddString("Dirichlet");
  m_edgetype.AddString("Neumann");
  if (n < 0) {
    m_edgetype.SetCurSel(0);
    m_edgetype.EnableWindow(0);
  } else
    m_edgetype.SetCurSel(n);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CMakeABCDlg::OnOK()
{
  n = m_edgetype.GetCurSel();
  CDialog::OnOK();
}