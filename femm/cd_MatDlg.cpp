// MatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cd_MatDlg.h"
#include "KCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdCMatDlg dialog

cdCMatDlg::cdCMatDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cdCMatDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cdCMatDlg)
  m_BlockName = _T("");
  m_ex = 1.0;
  m_ey = 1.0;
  m_ltx = 0.0;
  m_lty = 0.0;
  m_ox = 0.0;
  m_oy = 0.0;
  m_mu1label = _T("x");
  m_mu2label = _T("y");
  m_mu1label2 = _T("x");
  m_mu2label2 = _T("y");
  m_mu1label3 = _T("x-dir");
  m_mu2label3 = _T("y-dir");
  //}}AFX_DATA_INIT
}

void cdCMatDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cdCMatDlg)
  DDX_Control(pDX, IDC_STATIC_SYMBOL4, m_static4);
  DDX_Control(pDX, IDC_STATIC_SYMBOL3, m_static3);
  DDX_Control(pDX, IDC_STATIC_SYMBOL7, m_static7);
  DDX_Control(pDX, IDC_STATIC_SYMBOL8, m_static8);
  DDX_Text(pDX, IDC_CD_BLOCKNAME, m_BlockName);
  DDX_Text(pDX, IDC_CD_EX, m_ex);
  DDX_Text(pDX, IDC_CD_EY, m_ey);
  DDX_Text(pDX, IDC_CD_LTX, m_ltx);
  DDX_Text(pDX, IDC_CD_LTY, m_lty);
  DDX_Text(pDX, IDC_CD_OX, m_ox);
  DDX_Text(pDX, IDC_CD_OY, m_oy);
  DDX_Text(pDX, IDC_CD_MU1LABEL, m_mu1label);
  DDX_Text(pDX, IDC_CD_MU2LABEL, m_mu2label);
  DDX_Text(pDX, IDC_CD_MU1LABEL2, m_mu1label2);
  DDX_Text(pDX, IDC_CD_MU2LABEL2, m_mu2label2);
  DDX_Text(pDX, IDC_CD_MU1LABEL3, m_mu1label3);
  DDX_Text(pDX, IDC_CD_MU2LABEL3, m_mu2label3);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CD_BLOCKNAME, m_IDC_BlockName);
  DDX_Control(pDX, IDC_CD_EX, m_IDC_EX);
  DDX_Control(pDX, IDC_CD_EY, m_IDC_EY);
  DDX_Control(pDX, IDC_CD_OX, m_IDC_OX);
  DDX_Control(pDX, IDC_CD_OY, m_IDC_OY);
  DDX_Control(pDX, IDC_CD_LTX, m_IDC_LTX);
  DDX_Control(pDX, IDC_CD_LTY, m_IDC_LTY);
}

BEGIN_MESSAGE_MAP(cdCMatDlg, CDialog)
//{{AFX_MSG_MAP(cdCMatDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdCMatDlg message handlers

BOOL cdCMatDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  symbfont.CreateFont(0, 0, 0, 0, FW_BOLD, TRUE, 0, 0,
      SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Symbol");

  m_static3.SetFont(&symbfont);
  m_static4.SetFont(&symbfont);
  m_static7.SetFont(&symbfont);
  m_static8.SetFont(&symbfont);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cdCMatDlg::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_BlockName == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_BlockName);
      return;
    }
  }

  UpdateData(FALSE);

  CDialog::OnOK();
}
