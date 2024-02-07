// BlockInt.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cv_BlockInt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cvCBlockInt dialog

cvCBlockInt::cvCBlockInt(CWnd* pParent /*=NULL*/)
    : CDialog(cvCBlockInt::IDD, pParent)
{
  //{{AFX_DATA_INIT(cvCBlockInt)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void cvCBlockInt::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cvCBlockInt)
  DDX_Control(pDX, IDC_CV_BINTTYPE, m_binttype);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(cvCBlockInt, CDialog)
//{{AFX_MSG_MAP(cvCBlockInt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cvCBlockInt message handlers

BOOL cvCBlockInt::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (Frequency != 0) {
    m_binttype.AddString("Force via Stress Tensor");
    m_binttype.AddString("Torque via Stress Tensor");
  }

  binttype = 0;
  m_binttype.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cvCBlockInt::OnOK()
{
  // TODO: Add extra validation here
  binttype = m_binttype.GetCurSel();

  CDialog::OnOK();
}
