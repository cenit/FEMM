// MatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "MatDlg.h"
#include "BHData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatDlg dialog

CMatDlg::CMatDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMatDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CMatDlg)
  m_Cduct = 0.0;
  m_BlockName = _T("");
  m_H_c = 0.0;
  m_Jsrc = 0;
  m_Lam_d = 0.0;
  m_mu_x = 0.0;
  m_mu_y = 0.0;
  m_lam_fill = 0.0;
  m_mu1label = _T("");
  m_mu2label = _T("");
  m_Theta_hn = 0.0;
  m_Theta_hx = 0.0;
  m_Theta_hy = 0.0;
  m_NStrands = 0;
  m_WireD = 0.0;
  m_h1label = _T("");
  m_h2label = _T("");
  //}}AFX_DATA_INIT
}

void CMatDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMatDlg)
  DDX_Control(pDX, IDC_STATIC_SYMBOL6, m_static6);
  DDX_Control(pDX, IDC_STATIC_SYMBOL5, m_static5);
  DDX_Control(pDX, IDC_STATIC_SYMBOL4, m_static4);
  DDX_Control(pDX, IDC_STATIC_SYMBOL3, m_static3);
  DDX_Control(pDX, IDC_STATIC_SYMBOL2, m_static2);
  DDX_Control(pDX, IDC_STATIC_SYMBOL1, m_static1);
  DDX_Control(pDX, IDC_BHEDIT, m_IDC_bhedit);
  DDX_Control(pDX, IDC_NLCOMBO, m_nlcombo);
  DDX_Control(pDX, IDC_LAM_DIR, m_lam_dir);
  DDX_Text(pDX, IDC_CDUCT, m_Cduct);
  DDX_Text(pDX, IDC_BLOCKNAME, m_BlockName);
  DDX_Text(pDX, IDC_H_C, m_H_c);
  DDX_Text(pDX, IDC_JR, m_Jsrc);
  DDX_Text(pDX, IDC_LAM_D, m_Lam_d);
  DDX_Text(pDX, IDC_MU_X, m_mu_x);
  DDX_Text(pDX, IDC_MU_Y, m_mu_y);
  DDX_Text(pDX, IDC_LAM_FILL, m_lam_fill);
  DDV_MinMaxDouble(pDX, m_lam_fill, 0., 1.);
  DDX_Text(pDX, IDC_MU1LABEL, m_mu1label);
  DDX_Text(pDX, IDC_MU2LABEL, m_mu2label);
  DDX_Text(pDX, IDC_THETA_H, m_Theta_hn);
  DDV_MinMaxDouble(pDX, m_Theta_hn, 0., 90.);
  DDX_Text(pDX, IDC_THETA_HX, m_Theta_hx);
  DDV_MinMaxDouble(pDX, m_Theta_hx, 0., 90.);
  DDX_Text(pDX, IDC_THETA_HY, m_Theta_hy);
  DDV_MinMaxDouble(pDX, m_Theta_hy, 0., 90.);
  DDX_Text(pDX, IDC_H1LABEL, m_h1label);
  DDX_Text(pDX, IDC_H2LABEL, m_h2label);
  DDX_Text(pDX, IDC_NSTRANDS, m_NStrands);
  DDX_Text(pDX, IDC_WIRED, m_WireD);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CDUCT, m_IDC_Cduct);
  DDX_Control(pDX, IDC_BLOCKNAME, m_IDC_BlockName);
  DDX_Control(pDX, IDC_H_C, m_IDC_H_c);
  DDX_Control(pDX, IDC_JR, m_IDC_Jr);
  DDX_Control(pDX, IDC_LAM_D, m_IDC_Lam_d);
  DDX_Control(pDX, IDC_MU_X, m_IDC_mu_x);
  DDX_Control(pDX, IDC_MU_Y, m_IDC_mu_y);
  DDX_Control(pDX, IDC_THETA_H, m_IDC_Theta_h);
  DDX_Control(pDX, IDC_LAM_FILL, m_IDC_lam_fill);
  DDX_Control(pDX, IDC_THETA_HX, m_IDC_Theta_hx);
  DDX_Control(pDX, IDC_THETA_HY, m_IDC_Theta_hy);
  DDX_Control(pDX, IDC_NSTRANDS, m_IDC_NStrands);
  DDX_Control(pDX, IDC_WIRED, m_IDC_WireD);
}

BEGIN_MESSAGE_MAP(CMatDlg, CDialog)
//{{AFX_MSG_MAP(CMatDlg)
ON_BN_CLICKED(IDC_BHEDIT, OnBhedit)
ON_CBN_SELCHANGE(IDC_LAM_DIR, OnSelchangeLamDir)
ON_CBN_SELCHANGE(IDC_NLCOMBO, OnSelchangeNlcombo)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatDlg message handlers

BOOL CMatDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (LamType != 0)
    LamType++;
  else {
    if ((m_Lam_d != 0) || (m_lam_fill != 1))
      LamType = 1;
  }
  m_lam_dir.SetCurSel(LamType);
  OnSelchangeLamDir();

  m_nlcombo.SetCurSel(m_nlflag);
  OnSelchangeNlcombo();

  symbfont.CreateFont(0, 0, 0, 0, FW_BOLD, TRUE, 0, 0,
      SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Symbol");
  m_static1.SetFont(&symbfont);
  m_static2.SetFont(&symbfont);
  m_static3.SetFont(&symbfont);
  m_static4.SetFont(&symbfont);
  m_static5.SetFont(&symbfont);
  m_static6.SetFont(&symbfont);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CMatDlg::OnOK()
{
  if (!UpdateData())
    return;

  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_BlockName == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_BlockName);
      return;
    }
  }

  LamType = m_lam_dir.GetCurSel();
  LamType--;
  if (LamType < 0)
    LamType = 0;

  if (!UpdateData(TRUE))
    return;

  if (m_lam_dir.GetCurSel() == 0) {
    // if "not laminated" is selected, put in the
    // default values for lam thickness and fill factor.
    m_Lam_d = 0;
    m_lam_fill = 1;
  } else
    m_H_c = 0;

  if (m_lam_dir.GetCurSel() > 3) {
    IsWire();
    if (m_WireD <= 0) {
      // catch case where zero wire diameter
      // is erroneously specified
      m_WireD = 0;
      m_lam_dir.SetCurSel(0);
      LamType = 0;
    }
  }

  UpdateData(FALSE);
  CDialog::OnOK();
}

void CMatDlg::OnBhedit()
{
  // TODO: Add your control notification handler code here
  CBHData dlg;

  if (m_nlflag == FALSE) {
    m_nlflag = TRUE;
    m_nlcombo.SetCurSel(m_nlflag);
    OnSelchangeNlcombo();
  }

  UpdateData();
  dlg.m_Bdata = BData;
  dlg.m_Hdata = HData;
  dlg.m_BHname = m_BlockName;
  if (dlg.DoModal() == IDOK) {
    BData = dlg.m_Bdata;
    HData = dlg.m_Hdata;
    return;
  }

  return;
}

void CMatDlg::OnSelchangeLamDir()
{
  switch (m_lam_dir.GetCurSel()) {
  case 1:
    m_IDC_Lam_d.EnableWindow(TRUE);
    m_IDC_lam_fill.EnableWindow(TRUE);
    m_IDC_NStrands.EnableWindow(FALSE);
    m_IDC_WireD.EnableWindow(FALSE);
    m_IDC_H_c.EnableWindow(FALSE);
    m_H_c = 0;
    UpdateData(FALSE);
    break;

  case 2:
    m_IDC_Lam_d.EnableWindow(TRUE);
    m_IDC_lam_fill.EnableWindow(TRUE);
    m_IDC_NStrands.EnableWindow(FALSE);
    m_IDC_WireD.EnableWindow(FALSE);
    m_IDC_H_c.EnableWindow(FALSE);
    m_H_c = 0;
    UpdateData(FALSE);
    break;

  case 3:
    m_IDC_Lam_d.EnableWindow(TRUE);
    m_IDC_lam_fill.EnableWindow(TRUE);
    m_IDC_NStrands.EnableWindow(FALSE);
    m_IDC_WireD.EnableWindow(FALSE);
    m_IDC_H_c.EnableWindow(FALSE);
    m_H_c = 0;
    UpdateData(FALSE);
    break;

  case 4: // round magnet wire
  case 7: // square magnet wire
    m_IDC_NStrands.SetWindowText("1");
    m_NStrands = 1;
    m_IDC_NStrands.EnableWindow(FALSE);
    m_IDC_WireD.EnableWindow(TRUE);
    IsWire();
    break;

  case 5: // plain stranded
  case 6: // litz
  case 8: // 10% CCA
  case 9: // 15% CCA
    m_IDC_NStrands.EnableWindow(TRUE);
    m_IDC_WireD.EnableWindow(TRUE);
    IsWire();
    break;

  default:
    m_IDC_Lam_d.EnableWindow(FALSE);
    m_IDC_lam_fill.EnableWindow(FALSE);
    m_IDC_NStrands.EnableWindow(FALSE);
    m_IDC_WireD.EnableWindow(FALSE);
    m_IDC_H_c.EnableWindow(TRUE);
    break;
  }
}

void CMatDlg::OnSelchangeNlcombo()
{
  m_nlflag = m_nlcombo.GetCurSel();
  UpdateData(TRUE);
  if (m_nlflag == TRUE) {
    m_IDC_Theta_hx.EnableWindow(FALSE);
    m_IDC_Theta_hy.EnableWindow(FALSE);
    m_IDC_mu_x.EnableWindow(FALSE);
    m_IDC_mu_y.EnableWindow(FALSE);

    m_IDC_bhedit.EnableWindow(TRUE);
    m_IDC_Theta_h.EnableWindow(TRUE);

    if (m_lam_dir.GetCurSel() > 3) {
      m_lam_dir.SetCurSel(0);
      OnSelchangeLamDir();
    }
  } else {
    m_IDC_Theta_hx.EnableWindow(TRUE);
    m_IDC_Theta_hy.EnableWindow(TRUE);
    m_IDC_mu_x.EnableWindow(TRUE);
    m_IDC_mu_y.EnableWindow(TRUE);

    m_IDC_bhedit.EnableWindow(FALSE);
    m_IDC_Theta_h.EnableWindow(FALSE);
  }
  UpdateData(FALSE);
}

void CMatDlg::IsWire()
{
  // if the material is specified to be a wire,
  // make sure the permeability and other properties
  // are consistent with a wire.
  UpdateData(TRUE);
  m_mu_x = 1;
  m_mu_y = 1;
  m_H_c = 0;
  m_Jsrc = 0;
  m_Lam_d = 0;
  m_lam_fill = 1;
  m_Theta_hn = 0;
  m_Theta_hx = 0;
  m_Theta_hy = 0.0;
  BData.Empty();
  HData.Empty();
  m_nlflag = FALSE;
  m_nlcombo.SetCurSel(m_nlflag);
  m_IDC_Lam_d.EnableWindow(FALSE);
  m_IDC_lam_fill.EnableWindow(FALSE);
  m_IDC_Theta_hx.EnableWindow(TRUE);
  m_IDC_Theta_hy.EnableWindow(TRUE);
  m_IDC_mu_x.EnableWindow(TRUE);
  m_IDC_mu_y.EnableWindow(TRUE);
  m_IDC_H_c.EnableWindow(FALSE);
  m_IDC_bhedit.EnableWindow(FALSE);
  m_IDC_Theta_h.EnableWindow(FALSE);
  UpdateData(FALSE);
}
