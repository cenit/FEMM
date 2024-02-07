// OpBlkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "OpBlkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpBlkDlg dialog

COpBlkDlg::COpBlkDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COpBlkDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(COpBlkDlg)
  m_sidelength = 0.0;
  m_magdir = 0.0;
  m_turns = 1;
  m_ingroup = 0;
  m_isexternal = FALSE;
  m_isdefault = FALSE;
  //}}AFX_DATA_INIT
  ProblemType = 0;
}

void COpBlkDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COpBlkDlg)
  DDX_Control(pDX, IDC_AUTOMESHCHECK, m_automesh);
  DDX_Control(pDX, IDC_CIRCBLK, m_circblk);
  DDX_Control(pDX, IDC_ACKBLK, m_ackblk);
  DDX_Check(pDX, IDC_FE_EXTERNAL, m_isexternal);
  DDX_Check(pDX, IDC_FE_ISDEFAULT, m_isdefault);
  DDX_Text(pDX, IDC_SIDELENGTH, m_sidelength);
  DDX_Text(pDX, IDC_MAGDIR, m_magdir, m_magdirfctn);
  DDX_Text(pDX, IDC_TURNS, m_turns);
  DDX_Text(pDX, IDC_BLKGROUP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_SIDELENGTH, m_IDC_sidelength);
  DDX_Control(pDX, IDC_MAGDIR, m_IDC_magdir);
  DDX_Control(pDX, IDC_TURNS, m_IDC_turns);
  DDX_Control(pDX, IDC_BLKGROUP, m_IDC_ingroup);
}

BEGIN_MESSAGE_MAP(COpBlkDlg, CDialog)
//{{AFX_MSG_MAP(COpBlkDlg)
ON_CBN_SELCHANGE(IDC_ACKBLK, OnSelchangeAckblk)
ON_CBN_SELCHANGE(IDC_CIRCBLK, OnSelchangeCircblk)
ON_BN_CLICKED(IDC_AUTOMESHCHECK, OnAutomeshcheck)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpBlkDlg message handlers

void COpBlkDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ackblk.GetCurSel();
  circsel = m_circblk.GetCurSel();
  CDialog::OnOK();
}

BOOL COpBlkDlg::OnInitDialog()
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

  m_circblk.AddString("<None>");
  for (i = 0; i < circproplist.GetSize(); i++)
    m_circblk.AddString(circproplist[i].CircName);
  m_circblk.SetCurSel(circsel);
  OnSelchangeCircblk();

  if (m_sidelength == 0)
    m_automesh.SetCheck(TRUE);
  else
    m_automesh.SetCheck(FALSE);
  OnAutomeshcheck();

  CWnd* pCheck = GetDlgItem(IDC_FE_EXTERNAL);
  pCheck->EnableWindow(ProblemType);

  if (m_magdirfctn.GetLength() > 0) {
    SetDlgItemText(IDC_MAGDIR, m_magdirfctn);
  } else {
    CString s;
    s.Format("%.17g", m_magdir);
    SetDlgItemText(IDC_MAGDIR, s);
  }

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void COpBlkDlg::OnSelchangeAckblk()
{
  CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
  BOOL bEnable = FALSE;

  cursel = m_ackblk.GetCurSel();
  if (cursel > 1)
    if (blockproplist[cursel - 2].H_c != 0.)
      bEnable = TRUE;

  m_IDC_magdir.EnableWindow(bEnable);
}

void COpBlkDlg::OnSelchangeCircblk()
{
  CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
  BOOL bEnable = FALSE;

  cursel = m_circblk.GetCurSel();
  if (cursel > 0)
    if (circproplist[cursel - 1].CircType != 0)
      bEnable = TRUE;

  m_IDC_turns.EnableWindow(bEnable);

  if (!bEnable) {
    m_turns = 1;
    SetDlgItemText(IDC_TURNS, "1");
  }
}

void COpBlkDlg::OnAutomeshcheck()
{
  // TODO: Add your control notification handler code here
  int k = m_automesh.GetCheck();
  if (k == TRUE)
    k = FALSE;
  else
    k = TRUE;

  SendDlgItemMessage(
      IDC_SIDELENGTH, // identifier of control
      WM_ENABLE, // message to send
      (WPARAM)k, // first message parameter
      (LPARAM)0 // second message parameter
  );
  if (k == FALSE) {
    m_sidelength = 0;
    SetDlgItemText(IDC_SIDELENGTH, "0");
  }
}
