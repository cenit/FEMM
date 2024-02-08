// LIntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cv_LIntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cvCLIntDlg dialog

cvCLIntDlg::cvCLIntDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cvCLIntDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cvCLIntDlg)
  //}}AFX_DATA_INIT
}

void cvCLIntDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cvCLIntDlg)
  DDX_Control(pDX, IDC_CV_LINTTYPE, m_inttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(cvCLIntDlg, CDialog)
//{{AFX_MSG_MAP(cvCLIntDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cvCLIntDlg message handlers

BOOL cvCLIntDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (Frequency != 0) {
    m_inttype.AddString("Force via Stress Tensor");
    m_inttype.AddString("Torque via Stress Tensor");
  }

  m_inttype.SetCurSel(0);
  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cvCLIntDlg::OnOK()
{
  // TODO: Add extra validation here
  linttype = m_inttype.GetCurSel();

  CDialog::OnOK();
}
