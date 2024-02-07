// CircDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afxtempl.h>
#include "problem.h"
#include "xyplot.h"
#include "femmviewdoc.h"
#include "CircDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCircDlg dialog

CCircDlg::CCircDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CCircDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCircDlg)
  //}}AFX_DATA_INIT
}

void CCircDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCircDlg)
  DDX_Control(pDX, IDC_FV_CIRCNAME, m_circname);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCircDlg, CDialog)
//{{AFX_MSG_MAP(CCircDlg)
ON_CBN_SELCHANGE(IDC_FV_CIRCNAME, OnSelchangeCircname)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircDlg message handlers

void CCircDlg::OnSelchangeCircname()
{
  if (NumCircuits == 0)
    return;

  int k;
  CString crslt;

  k = m_circname.GetCurSel();

  CString s;
  char c[256];
  CComplex amps, flux, volts;

  amps = TheDoc->circproplist[k].Amps;
  crslt.Format("Total current = %s Amps\r\n", amps.ToStringAlt(c));

  volts = TheDoc->GetVoltageDrop(k);
  s.Format("Voltage Drop = %s Volts\r\n", volts.ToStringAlt(c));
  crslt += s;

  flux = TheDoc->GetFluxLinkage(k);
  s.Format("Flux Linkage = %s Webers\r\n", flux.ToStringAlt(c));
  crslt += s;

  if (amps != 0) {
    s.Format("Flux/Current = %s Henries\r\n",
        (flux / amps).ToStringAlt(c));
    crslt += s;
    s.Format("Voltage/Current = %s Ohms\r\n",
        (volts / amps).ToStringAlt(c));
    crslt += s;
  }

  if (TheDoc->Frequency == 0) {
    s.Format("Power = %g Watts\r\n", Re(amps * volts));
    crslt += s;
  } else {
    s.Format("Real Power = %g Watts\r\n", Re(volts * conj(amps)) / 2.);
    crslt += s;
    s.Format("Reactive Power = %g VAr\r\n", Im(volts * conj(amps)) / 2.);
    crslt += s;
    s.Format("Apparent Power = %g VA\r\n", abs(volts) * abs(amps) / 2.);
    crslt += s;
  }

  SetDlgItemText(IDC_CIRCRESULT, crslt);
}

BOOL CCircDlg::OnInitDialog()
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
