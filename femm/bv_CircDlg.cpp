// CircDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afxtempl.h>
#include "bv_problem.h"
#include "xyplot.h"
#include "belaviewdoc.h"
#include "bv_CircDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bvCCircDlg dialog

bvCCircDlg::bvCCircDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bvCCircDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bvCCircDlg)
  //}}AFX_DATA_INIT
}

void bvCCircDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bvCCircDlg)
  DDX_Control(pDX, IDC_BV_CIRCNAME, m_circname);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(bvCCircDlg, CDialog)
//{{AFX_MSG_MAP(bvCCircDlg)
ON_CBN_SELCHANGE(IDC_BV_CIRCNAME, OnSelchangeCircname)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bvCCircDlg message handlers
//////////////////////////////////////////////////////
void bvCCircDlg::OnSelchangeCircname()
{
  if (NumCircuits == 0)
    return;

  int k;
  CString crslt;

  k = m_circname.GetCurSel();
  crslt.Format("Voltage = %g Volts\r\nCharge = %g Coulombs",
      TheDoc->circproplist[k].V, TheDoc->circproplist[k].q);
  SetDlgItemText(IDC_BV_CIRCRESULT, crslt);
}

BOOL bvCCircDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  int i;

  NumCircuits = (int)TheDoc->circproplist.GetSize();
  for (i = 0; i < NumCircuits; i++)
    m_circname.AddString(TheDoc->circproplist[i].CircName);

  if (NumCircuits != 0) {
    m_circname.SetCurSel(0);
    OnSelchangeCircname();
  }

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}
