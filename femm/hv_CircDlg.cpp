// CircDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afxtempl.h>
#include "hv_problem.h"
#include "xyplot.h"
#include "hviewdoc.h"
#include "hv_CircDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hvCCircDlg dialog

hvCCircDlg::hvCCircDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hvCCircDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hvCCircDlg)
  //}}AFX_DATA_INIT
}

void hvCCircDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hvCCircDlg)
  DDX_Control(pDX, IDC_HV_CIRCNAME, m_circname);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(hvCCircDlg, CDialog)
//{{AFX_MSG_MAP(hvCCircDlg)
ON_CBN_SELCHANGE(IDC_HV_CIRCNAME, OnSelchangeCircname)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hvCCircDlg message handlers
//////////////////////////////////////////////////////
void hvCCircDlg::OnSelchangeCircname()
{
  if (NumCircuits == 0)
    return;

  int k;
  CString crslt;

  k = m_circname.GetCurSel();
  crslt.Format("Temperature = %g K\r\nHeat Flux = %g W",
      TheDoc->circproplist[k].V, TheDoc->circproplist[k].q);
  SetDlgItemText(IDC_HV_CIRCRESULT, crslt);
}

BOOL hvCCircDlg::OnInitDialog()
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
