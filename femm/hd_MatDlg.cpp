// MatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hd_MatDlg.h"
#include "KCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCMatDlg dialog

hdCMatDlg::hdCMatDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hdCMatDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCMatDlg)
  m_BlockName = _T("");
  m_Kx = 1.0;
  m_Ky = 1.0;
  m_qv = 0.0;
  m_Kt = 3;
  m_mu1label = _T("");
  m_mu2label = _T("");
  //}}AFX_DATA_INIT

  m_nlflag = FALSE;
  m_npts = 0;
}

void hdCMatDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCMatDlg)
  DDX_Control(pDX, IDC_EDITCURVE, m_editcurve);
  DDX_Control(pDX, IDC_NLCOMBO, m_nlcurve);
  DDX_Text(pDX, IDC_HD_BLOCKNAME, m_BlockName);
  DDX_Text(pDX, IDC_HD_EX, m_Kx);
  DDX_Text(pDX, IDC_HD_EY, m_Ky);
  DDX_Text(pDX, IDC_HD_KT, m_Kt);
  DDX_Text(pDX, IDC_HD_QV, m_qv);
  DDX_Text(pDX, IDC_HD_MU1LABEL, m_mu1label);
  DDX_Text(pDX, IDC_HD_MU2LABEL, m_mu2label);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_BLOCKNAME, m_IDC_BlockName);
  DDX_Control(pDX, IDC_HD_EX, m_IDC_EX);
  DDX_Control(pDX, IDC_HD_EY, m_IDC_EY);
  DDX_Control(pDX, IDC_HD_QV, m_IDC_QV);
  DDX_Control(pDX, IDC_HD_KT, m_IDC_KT);
}

BEGIN_MESSAGE_MAP(hdCMatDlg, CDialog)
//{{AFX_MSG_MAP(hdCMatDlg)
ON_BN_CLICKED(IDC_EDITCURVE, OnEditCurve)
ON_CBN_SELCHANGE(IDC_NLCOMBO, OnSelchangeNlcombo)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCMatDlg message handlers

BOOL hdCMatDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  symbfont.CreateFont(0, 0, 0, 0, FW_BOLD, TRUE, 0, 0,
      SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Symbol");

  if (m_npts == 0)
    m_nlcurve.SetCurSel(0);
  else
    m_nlcurve.SetCurSel(1);

  OnSelchangeNlcombo();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hdCMatDlg::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_BlockName == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_BlockName);
      return;
    }
  }

  if ((m_nlflag == TRUE) && (m_npts > 0)) {
    if (m_npts == 1)
      m_nlflag = FALSE;
    m_Kx = Im(m_Kn[0]);
    m_Ky = m_Kx;
  }

  if (m_nlflag == FALSE)
    m_npts = 0;

  UpdateData(FALSE);

  CDialog::OnOK();
}

void hdCMatDlg::OnSelchangeNlcombo()
{
  m_nlflag = m_nlcurve.GetCurSel();

  if (m_nlflag == TRUE) {
    m_IDC_EX.EnableWindow(FALSE);
    m_IDC_EY.EnableWindow(FALSE);
    m_editcurve.EnableWindow(TRUE);
  } else {
    m_IDC_EX.EnableWindow(TRUE);
    m_IDC_EY.EnableWindow(TRUE);
    m_editcurve.EnableWindow(FALSE);
  }
}

void hdCMatDlg::OnEditCurve()
{
  CTKData dlg;

  int k;

  dlg.T.RemoveAll();
  dlg.K.RemoveAll();
  dlg.npts = m_npts;

  for (k = 0; k < m_npts; k++) {
    dlg.T.Add(Re(m_Kn[k]));
    dlg.K.Add(Im(m_Kn[k]));
  }

  if (dlg.DoModal() == IDOK) {
    dlg.StripTKData();
    m_npts = dlg.npts;
    if (m_npts > 128)
      m_npts = 128;
    for (k = 0; k < m_npts; k++) {
      m_Kn[k] = dlg.T[k] + I * dlg.K[k];
    }
  }
}
