// BlockInt.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hv_BlockInt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hvCBlockInt dialog

hvCBlockInt::hvCBlockInt(CWnd* pParent /*=NULL*/)
    : CDialog(hvCBlockInt::IDD, pParent)
{
  //{{AFX_DATA_INIT(hvCBlockInt)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void hvCBlockInt::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hvCBlockInt)
  DDX_Control(pDX, IDC_HV_BINTTYPE, m_binttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(hvCBlockInt, CDialog)
//{{AFX_MSG_MAP(hvCBlockInt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hvCBlockInt message handlers

BOOL hvCBlockInt::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  binttype = 0;
  m_binttype.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hvCBlockInt::OnOK()
{
  // TODO: Add extra validation here
  binttype = m_binttype.GetCurSel();

  CDialog::OnOK();
}
