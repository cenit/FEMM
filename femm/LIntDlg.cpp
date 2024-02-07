// LIntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "LIntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLIntDlg dialog

CLIntDlg::CLIntDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CLIntDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CLIntDlg)
  //}}AFX_DATA_INIT
}

void CLIntDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CLIntDlg)
  DDX_Control(pDX, IDC_LINTTYPE, m_inttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLIntDlg, CDialog)
//{{AFX_MSG_MAP(CLIntDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLIntDlg message handlers

BOOL CLIntDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_inttype.SetCurSel(0);
  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CLIntDlg::OnOK()
{
  // TODO: Add extra validation here
  linttype = m_inttype.GetCurSel();

  CDialog::OnOK();
}
