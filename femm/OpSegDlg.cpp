// OpSegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "OpSegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpSegDlg dialog

COpSegDlg::COpSegDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COpSegDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(COpSegDlg)
  m_linemeshsize = 0.0;
  m_automesh = FALSE;
  m_hide = FALSE;
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void COpSegDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COpSegDlg)
  DDX_Control(pDX, IDC_ACKSEG, m_ackseg);
  DDX_Text(pDX, IDC_SEGGRP, m_ingroup);
  DDX_Text(pDX, IDC_LINEMESHSIZE, m_linemeshsize);
  DDX_Check(pDX, IDC_AUTOMESH, m_automesh);
  DDX_Check(pDX, IDC_SEGHIDE, m_hide);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_SEGGRP, m_IDC_ingroup);
  DDX_Control(pDX, IDC_LINEMESHSIZE, m_IDC_linemeshsize);
}

BEGIN_MESSAGE_MAP(COpSegDlg, CDialog)
//{{AFX_MSG_MAP(COpSegDlg)
ON_BN_CLICKED(IDC_AUTOMESH, OnAutomesh)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpSegDlg message handlers

void COpSegDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ackseg.GetCurSel();

  CDialog::OnOK();
}

BOOL COpSegDlg::OnInitDialog()
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
  OnAutomesh();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void COpSegDlg::OnAutomesh()
{
  UpdateData();

  BOOL bEnable = TRUE;

  if (m_automesh == TRUE)
    bEnable = FALSE;

  SendDlgItemMessage(
      IDC_LINEMESHSIZE, // identifier of control
      WM_ENABLE, // message to send
      (WPARAM)bEnable, // first message parameter
      (LPARAM)0 // second message parameter
  );
  if (m_automesh == TRUE) {
    m_linemeshsize = 0;
    SetDlgItemText(IDC_LINEMESHSIZE, "0");
  }
}
