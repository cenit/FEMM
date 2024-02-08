// OpSegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "beladrawDoc.h"
#include "bd_OpSegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCOpSegDlg dialog

bdCOpSegDlg::bdCOpSegDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bdCOpSegDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCOpSegDlg)
  m_linemeshsize = 0.0;
  m_automesh = FALSE;
  m_hide = FALSE;
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void bdCOpSegDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCOpSegDlg)
  DDX_Control(pDX, IDC_BD_SEG_COND, m_segcond);
  DDX_Control(pDX, IDC_BD_ACKSEG, m_ackseg);
  DDX_Text(pDX, IDC_BD_LINEMESHSIZE, m_linemeshsize);
  DDX_Check(pDX, IDC_BD_AUTOMESH, m_automesh);
  DDX_Check(pDX, IDC_BD_SEGHIDE, m_hide);
  DDX_Text(pDX, IDC_BD_SEGGRP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_SEGGRP, m_IDC_ingroup);
  DDX_Control(pDX, IDC_BD_LINEMESHSIZE, m_IDC_linemeshsize);
}

BEGIN_MESSAGE_MAP(bdCOpSegDlg, CDialog)
//{{AFX_MSG_MAP(bdCOpSegDlg)
ON_BN_CLICKED(IDC_BD_AUTOMESH, OnAutomesh)
ON_CBN_SELCHANGE(IDC_BD_ACKSEG, OnSelchangeAckseg)
ON_CBN_SELCHANGE(IDC_BD_SEG_COND, OnSelchangeSegCond)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCOpSegDlg message handlers

void bdCOpSegDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ackseg.GetCurSel();
  condsel = m_segcond.GetCurSel();

  CDialog::OnOK();
}

BOOL bdCOpSegDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
  int i;

  // TODO: Add extra initialization here
  m_ackseg.AddString("<None>");
  for (i = 0; i < lineproplist.GetSize(); i++)
    m_ackseg.AddString(lineproplist[i].BdryName);
  m_ackseg.SetCurSel(cursel);

  CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
  m_segcond.AddString("<None>");
  for (i = 0; i < circproplist.GetSize(); i++)
    m_segcond.AddString(circproplist[i].CircName);
  m_segcond.SetCurSel(condsel);

  OnAutomesh();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCOpSegDlg::OnAutomesh()
{
  UpdateData();

  BOOL bEnable = TRUE;

  if (m_automesh == TRUE)
    bEnable = FALSE;

  SendDlgItemMessage(
      IDC_BD_LINEMESHSIZE, // identifier of control
      WM_ENABLE, // message to send
      (WPARAM)bEnable, // first message parameter
      (LPARAM)0 // second message parameter
  );
  if (m_automesh == TRUE) {
    m_linemeshsize = 0;
    SetDlgItemText(IDC_BD_LINEMESHSIZE, "0");
  }
}

void bdCOpSegDlg::OnSelchangeAckseg()
{
  UpdateData();
  m_segcond.SetCurSel(0);
  UpdateData(FALSE);
}

void bdCOpSegDlg::OnSelchangeSegCond()
{
  UpdateData();
  m_ackseg.SetCurSel(0);
  UpdateData(FALSE);
}
