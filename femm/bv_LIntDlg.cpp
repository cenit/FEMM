// LIntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bv_LIntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bvCLIntDlg dialog

bvCLIntDlg::bvCLIntDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bvCLIntDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bvCLIntDlg)
  //}}AFX_DATA_INIT
}

void bvCLIntDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bvCLIntDlg)
  DDX_Control(pDX, IDC_BV_LINTTYPE, m_inttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(bvCLIntDlg, CDialog)
//{{AFX_MSG_MAP(bvCLIntDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bvCLIntDlg message handlers

BOOL bvCLIntDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_inttype.SetCurSel(0);
  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bvCLIntDlg::OnOK()
{
  // TODO: Add extra validation here
  linttype = m_inttype.GetCurSel();

  CDialog::OnOK();
}
