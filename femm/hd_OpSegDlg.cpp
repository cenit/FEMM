// OpSegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hdrawDoc.h"
#include "hd_OpSegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCOpSegDlg dialog

hdCOpSegDlg::hdCOpSegDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hdCOpSegDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCOpSegDlg)
  m_linemeshsize = 0.0;
  m_automesh = FALSE;
  m_hide = FALSE;
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void hdCOpSegDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCOpSegDlg)
  DDX_Control(pDX, IDC_HD_SEG_COND, m_segcond);
  DDX_Control(pDX, IDC_HD_ACKSEG, m_ackseg);
  DDX_Text(pDX, IDC_HD_LINEMESHSIZE, m_linemeshsize);
  DDX_Check(pDX, IDC_HD_AUTOMESH, m_automesh);
  DDX_Check(pDX, IDC_HD_SEGHIDE, m_hide);
  DDX_Text(pDX, IDC_HD_SEGGRP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_SEGGRP, m_IDC_ingroup);
  DDX_Control(pDX, IDC_HD_LINEMESHSIZE, m_IDC_linemeshsize);
}

BEGIN_MESSAGE_MAP(hdCOpSegDlg, CDialog)
//{{AFX_MSG_MAP(hdCOpSegDlg)
ON_BN_CLICKED(IDC_HD_AUTOMESH, OnAutomesh)
ON_CBN_SELCHANGE(IDC_HD_ACKSEG, OnSelchangeAckseg)
ON_CBN_SELCHANGE(IDC_HD_SEG_COND, OnSelchangeSegCond)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCOpSegDlg message handlers

void hdCOpSegDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ackseg.GetCurSel();
  condsel = m_segcond.GetCurSel();

  CDialog::OnOK();
}

BOOL hdCOpSegDlg::OnInitDialog()
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

void hdCOpSegDlg::OnAutomesh()
{
  UpdateData();

  BOOL bEnable = TRUE;

  if (m_automesh == TRUE)
    bEnable = FALSE;

  SendDlgItemMessage(
      IDC_HD_LINEMESHSIZE, // identifier of control
      WM_ENABLE, // message to send
      (WPARAM)bEnable, // first message parameter
      (LPARAM)0 // second message parameter
  );
  if (m_automesh == TRUE) {
    m_linemeshsize = 0;
    SetDlgItemText(IDC_HD_LINEMESHSIZE, "0");
  }
}

void hdCOpSegDlg::OnSelchangeAckseg()
{
  UpdateData();
  m_segcond.SetCurSel(0);
  UpdateData(FALSE);
}

void hdCOpSegDlg::OnSelchangeSegCond()
{
  UpdateData();
  m_ackseg.SetCurSel(0);
  UpdateData(FALSE);
}
