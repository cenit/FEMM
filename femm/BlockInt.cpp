// BlockInt.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "BlockInt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockInt dialog

CBlockInt::CBlockInt(CWnd* pParent /*=NULL*/)
    : CDialog(CBlockInt::IDD, pParent)
{
  //{{AFX_DATA_INIT(CBlockInt)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CBlockInt::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBlockInt)
  DDX_Control(pDX, IDC_BINTTYPE, m_binttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlockInt, CDialog)
//{{AFX_MSG_MAP(CBlockInt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockInt message handlers

BOOL CBlockInt::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  binttype = 0;
  m_binttype.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CBlockInt::OnOK()
{
  // TODO: Add extra validation here
  binttype = m_binttype.GetCurSel();

  CDialog::OnOK();
}
