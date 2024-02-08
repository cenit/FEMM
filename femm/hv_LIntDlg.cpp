// LIntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hv_LIntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hvCLIntDlg dialog

hvCLIntDlg::hvCLIntDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hvCLIntDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hvCLIntDlg)
  //}}AFX_DATA_INIT
}

void hvCLIntDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hvCLIntDlg)
  DDX_Control(pDX, IDC_HV_LINTTYPE, m_inttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(hvCLIntDlg, CDialog)
//{{AFX_MSG_MAP(hvCLIntDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hvCLIntDlg message handlers

BOOL hvCLIntDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_inttype.SetCurSel(0);
  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hvCLIntDlg::OnOK()
{
  // TODO: Add extra validation here
  linttype = m_inttype.GetCurSel();

  CDialog::OnOK();
}
