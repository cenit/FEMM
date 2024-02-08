// MatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bd_MatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCMatDlg dialog

bdCMatDlg::bdCMatDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bdCMatDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCMatDlg)
  m_BlockName = _T("");
  m_ex = 0.0;
  m_ey = 0.0;
  m_qv = 0.0;
  m_mu1label = _T("");
  m_mu2label = _T("");
  //}}AFX_DATA_INIT
}

void bdCMatDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCMatDlg)
  DDX_Control(pDX, IDC_STATIC_SYMBOL4, m_static4);
  DDX_Control(pDX, IDC_STATIC_SYMBOL3, m_static3);
  DDX_Text(pDX, IDC_BD_BLOCKNAME, m_BlockName);
  DDX_Text(pDX, IDC_BD_EX, m_ex);
  DDX_Text(pDX, IDC_BD_EY, m_ey);
  DDX_Text(pDX, IDC_BD_QV, m_qv);
  DDX_Text(pDX, IDC_BD_MU1LABEL, m_mu1label);
  DDX_Text(pDX, IDC_BD_MU2LABEL, m_mu2label);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_BLOCKNAME, m_IDC_BlockName);
  DDX_Control(pDX, IDC_BD_EX, m_IDC_EX);
  DDX_Control(pDX, IDC_BD_EY, m_IDC_EY);
  DDX_Control(pDX, IDC_BD_QV, m_IDC_QV);
}

BEGIN_MESSAGE_MAP(bdCMatDlg, CDialog)
//{{AFX_MSG_MAP(bdCMatDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCMatDlg message handlers

BOOL bdCMatDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  symbfont.CreateFont(0, 0, 0, 0, FW_BOLD, TRUE, 0, 0,
      SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Symbol");
  m_static3.SetFont(&symbfont);
  m_static4.SetFont(&symbfont);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCMatDlg::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_BlockName == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_BlockName);
      return;
    }
  }

  CDialog::OnOK();
}
