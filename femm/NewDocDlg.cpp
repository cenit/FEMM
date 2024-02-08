// NewDocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "NewDocDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewDocDlg dialog

CNewDocDlg::CNewDocDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CNewDocDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CNewDocDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CNewDocDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CNewDocDlg)
  DDX_Control(pDX, IDC_DOCTYPES, m_doctypes);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewDocDlg, CDialog)
//{{AFX_MSG_MAP(CNewDocDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewDocDlg message handlers

BOOL CNewDocDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_doctypes.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewDocDlg::OnOK()
{
  doctype = m_doctypes.GetCurSel();

  CDialog::OnOK();
}
