// CircDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afxtempl.h>
#include "cv_problem.h"
#include "xyplot.h"
#include "cviewdoc.h"
#include "cv_CircDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cvCCircDlg dialog

cvCCircDlg::cvCCircDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cvCCircDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cvCCircDlg)
  //}}AFX_DATA_INIT
}

void cvCCircDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cvCCircDlg)
  DDX_Control(pDX, IDC_CV_CIRCNAME, m_circname);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(cvCCircDlg, CDialog)
//{{AFX_MSG_MAP(cvCCircDlg)
ON_CBN_SELCHANGE(IDC_CV_CIRCNAME, OnSelchangeCircname)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cvCCircDlg message handlers
//////////////////////////////////////////////////////
void cvCCircDlg::OnSelchangeCircname()
{
  if (NumCircuits == 0)
    return;

  int k;
  CString crslt, s;
  CComplex x, V, q;
  char c[80];

  k = m_circname.GetCurSel();
  V = TheDoc->circproplist[k].V;
  q = TheDoc->circproplist[k].q;

  crslt.Format("Voltage = %s V\r\n", V.ToStringAlt(c));

  s.Format("Current = %s A\r\n", q.ToStringAlt(c));
  crslt = crslt + s;

  if (q != 0) {
    x = V / q;
    s.Format("Volt/Curr = %s Ohms\r\n", x.ToStringAlt(c));
    crslt = crslt + s;
  }

  if (V != 0) {
    x = q / V;
    s.Format("Curr/Volt = %s S\r\n", x.ToStringAlt(c));
    crslt = crslt + s;
  }

  SetDlgItemText(IDC_CV_CIRCRESULT, crslt);
}

BOOL cvCCircDlg::OnInitDialog()
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
