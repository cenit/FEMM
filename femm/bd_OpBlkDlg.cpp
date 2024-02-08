// OpBlkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "beladrawDoc.h"
#include "bd_OpBlkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCOpBlkDlg dialog

bdCOpBlkDlg::bdCOpBlkDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bdCOpBlkDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCOpBlkDlg)
  m_sidelength = 0.0;
  m_ingroup = 0;
  m_isexternal = FALSE;
  m_isdefault = FALSE;
  //}}AFX_DATA_INIT
  ProblemType = 0;
}

void bdCOpBlkDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCOpBlkDlg)
  DDX_Control(pDX, IDC_BD_AUTOMESHCHECK, m_automesh);
  DDX_Control(pDX, IDC_BD_ACKBLK, m_ackblk);
  DDX_Check(pDX, IDC_BD_EXTERNAL, m_isexternal);
  DDX_Check(pDX, IDC_BD_ISDEFAULT, m_isdefault);
  DDX_Text(pDX, IDC_BD_SIDELENGTH, m_sidelength);
  DDX_Text(pDX, IDC_BD_BLKGROUP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_SIDELENGTH, m_IDC_sidelength);
  DDX_Control(pDX, IDC_BD_BLKGROUP, m_IDC_ingroup);
}

BEGIN_MESSAGE_MAP(bdCOpBlkDlg, CDialog)
//{{AFX_MSG_MAP(bdCOpBlkDlg)
ON_CBN_SELCHANGE(IDC_BD_ACKBLK, OnSelchangeAckblk)
ON_BN_CLICKED(IDC_BD_AUTOMESHCHECK, OnAutomeshcheck)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCOpBlkDlg message handlers

void bdCOpBlkDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ackblk.GetCurSel();
  CDialog::OnOK();
}

BOOL bdCOpBlkDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
  CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;

  int i;

  // TODO: Add extra initialization here
  m_ackblk.AddString("<None>");
  m_ackblk.AddString("<No Mesh>");
  for (i = 0; i < blockproplist.GetSize(); i++)
    m_ackblk.AddString(blockproplist[i].BlockName);
  m_ackblk.SetCurSel(cursel);
  OnSelchangeAckblk();

  if (m_sidelength == 0)
    m_automesh.SetCheck(TRUE);
  else
    m_automesh.SetCheck(FALSE);
  OnAutomeshcheck();

  CWnd* pCheck = GetDlgItem(IDC_BD_EXTERNAL);
  pCheck->EnableWindow(ProblemType);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCOpBlkDlg::OnSelchangeAckblk()
{
  CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
  BOOL bEnable = FALSE;

  cursel = m_ackblk.GetCurSel();
}

void bdCOpBlkDlg::OnAutomeshcheck()
{
  // TODO: Add your control notification handler code here
  int k = m_automesh.GetCheck();
  if (k == TRUE)
    k = FALSE;
  else
    k = TRUE;

  SendDlgItemMessage(
      IDC_BD_SIDELENGTH, // identifier of control
      WM_ENABLE, // message to send
      (WPARAM)k, // first message parameter
      (LPARAM)0 // second message parameter
  );
  if (k == FALSE) {
    m_sidelength = 0;
    SetDlgItemText(IDC_BD_SIDELENGTH, "0");
  }
}
