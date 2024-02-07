// BdryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "BdryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBdryDlg dialog

CBdryDlg::CBdryDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBdryDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CBdryDlg)
  m_A0 = 0.0;
  m_A1 = 0.0;
  m_A2 = 0.0;
  m_BdryName = _T("");
  m_c0 = 0.0;
  m_c1 = 0.0;
  m_Mu = 0.0;
  m_Phi = 0.0;
  m_Sig = 0.0;
  m_innerangle = 0.0;
  m_outerangle = 0.0;
  //}}AFX_DATA_INIT
}

void CBdryDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBdryDlg)
  DDX_Control(pDX, IDC_STATIC_SYMB3, m_static3);
  DDX_Control(pDX, IDC_STATIC_SYMB2, m_static2);
  DDX_Control(pDX, IDC_STATIC_SYMB1, m_static1);
  DDX_Control(pDX, IDC_BDRYFORMAT, m_BdryFormat);
  DDX_Text(pDX, IDC_A0, m_A0);
  DDX_Text(pDX, IDC_A1, m_A1);
  DDX_Text(pDX, IDC_A2, m_A2);
  DDX_Text(pDX, IDC_BDRYNAME, m_BdryName);
  DDX_Text(pDX, IDC_C0, m_c0);
  DDX_Text(pDX, IDC_C1, m_c1);
  DDX_Text(pDX, IDC_MU, m_Mu);
  DDX_Text(pDX, IDC_PHI, m_Phi);
  DDX_Text(pDX, IDC_SIGMA, m_Sig);
  DDX_Text(pDX, IDC_INNERANGLE, m_innerangle);
  DDX_Text(pDX, IDC_OUTERANGLE, m_outerangle);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_A0, m_IDC_A0);
  DDX_Control(pDX, IDC_A1, m_IDC_A1);
  DDX_Control(pDX, IDC_A2, m_IDC_A2);
  DDX_Control(pDX, IDC_BDRYNAME, m_IDC_BDRYNAME);
  DDX_Control(pDX, IDC_C0, m_IDC_c0);
  DDX_Control(pDX, IDC_C1, m_IDC_c1);
  DDX_Control(pDX, IDC_MU, m_IDC_Mu);
  DDX_Control(pDX, IDC_PHI, m_IDC_Phi);
  DDX_Control(pDX, IDC_SIGMA, m_IDC_Sig);
  DDX_Control(pDX, IDC_INNERANGLE, m_IDC_INNERANGLE);
  DDX_Control(pDX, IDC_OUTERANGLE, m_IDC_OUTERANGLE);
}

BEGIN_MESSAGE_MAP(CBdryDlg, CDialog)
//{{AFX_MSG_MAP(CBdryDlg)
ON_CBN_SELCHANGE(IDC_BDRYFORMAT, OnSelchangeBdryformat)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBdryDlg message handlers

void CBdryDlg::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_BdryName == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_BdryName);
      return;
    }
  }

  BdryFormat = m_BdryFormat.GetCurSel();

  CDialog::OnOK();
}

BOOL CBdryDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_BdryFormat.SetCurSel(BdryFormat);
  OnSelchangeBdryformat();

  symbfont.CreateFont(0, 0, 0, 0, FW_BOLD, TRUE, 0, 0, SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Symbol");
  m_static1.SetFont(&symbfont);
  m_static2.SetFont(&symbfont);
  m_static3.SetFont(&symbfont);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CBdryDlg::OnSelchangeBdryformat()
{
  // 0 Prescribed A;
  // 1 Small skin depth;
  // 2 Mixed;
  // 3 Strategic Dual Image;
  // 4 Periodic;
  // 5 Anti-periodic;
  // 6 Periodic Air Gap;
  // 7 Anti-periodic Air Gap;

  switch (m_BdryFormat.GetCurSel()) {
  case 0:

    m_IDC_A0.EnableWindow(TRUE);
    m_IDC_A1.EnableWindow(TRUE);
    m_IDC_A2.EnableWindow(TRUE);
    m_IDC_Phi.EnableWindow(TRUE);

    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    m_IDC_Mu.EnableWindow(FALSE);
    m_IDC_Sig.EnableWindow(FALSE);

    m_IDC_INNERANGLE.EnableWindow(FALSE);
    m_IDC_OUTERANGLE.EnableWindow(FALSE);

    break;

  case 1:

    m_IDC_A0.EnableWindow(FALSE);
    m_IDC_A1.EnableWindow(FALSE);
    m_IDC_A2.EnableWindow(FALSE);
    m_IDC_Phi.EnableWindow(FALSE);

    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    m_IDC_Mu.EnableWindow(TRUE);
    m_IDC_Sig.EnableWindow(TRUE);

    m_IDC_INNERANGLE.EnableWindow(FALSE);
    m_IDC_OUTERANGLE.EnableWindow(FALSE);

    break;

  case 2:

    m_IDC_A0.EnableWindow(FALSE);
    m_IDC_A1.EnableWindow(FALSE);
    m_IDC_A2.EnableWindow(FALSE);
    m_IDC_Phi.EnableWindow(FALSE);

    m_IDC_c0.EnableWindow(TRUE);
    m_IDC_c1.EnableWindow(TRUE);

    m_IDC_Mu.EnableWindow(FALSE);
    m_IDC_Sig.EnableWindow(FALSE);

    m_IDC_INNERANGLE.EnableWindow(FALSE);
    m_IDC_OUTERANGLE.EnableWindow(FALSE);

    break;

  case 6:
  case 7:

    m_IDC_A0.EnableWindow(FALSE);
    m_IDC_A1.EnableWindow(FALSE);
    m_IDC_A2.EnableWindow(FALSE);
    m_IDC_Phi.EnableWindow(FALSE);

    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    m_IDC_Mu.EnableWindow(FALSE);
    m_IDC_Sig.EnableWindow(FALSE);

    m_IDC_INNERANGLE.EnableWindow(TRUE);
    m_IDC_OUTERANGLE.EnableWindow(TRUE);

    break;

  default:

    m_IDC_A0.EnableWindow(FALSE);
    m_IDC_A1.EnableWindow(FALSE);
    m_IDC_A2.EnableWindow(FALSE);
    m_IDC_Phi.EnableWindow(FALSE);

    m_IDC_c0.EnableWindow(FALSE);
    m_IDC_c1.EnableWindow(FALSE);

    m_IDC_Mu.EnableWindow(FALSE);
    m_IDC_Sig.EnableWindow(FALSE);

    m_IDC_INNERANGLE.EnableWindow(FALSE);
    m_IDC_OUTERANGLE.EnableWindow(FALSE);

    break;
  }
}
