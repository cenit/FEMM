// BlockInt.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bv_BlockInt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bvCBlockInt dialog

bvCBlockInt::bvCBlockInt(CWnd* pParent /*=NULL*/)
    : CDialog(bvCBlockInt::IDD, pParent)
{
  //{{AFX_DATA_INIT(bvCBlockInt)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void bvCBlockInt::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bvCBlockInt)
  DDX_Control(pDX, IDC_BV_BINTTYPE, m_binttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(bvCBlockInt, CDialog)
//{{AFX_MSG_MAP(bvCBlockInt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bvCBlockInt message handlers

BOOL bvCBlockInt::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  binttype = 0;
  m_binttype.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bvCBlockInt::OnOK()
{
  // TODO: Add extra validation here
  binttype = m_binttype.GetCurSel();

  CDialog::OnOK();
}
