// ArcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "ArcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArcDlg dialog

CArcDlg::CArcDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CArcDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CArcDlg)
  m_ArcAngle = 0.0;
  m_MaxSeg = 0.0;
  //}}AFX_DATA_INIT
}

void CArcDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CArcDlg)
  DDX_Control(pDX, IDC_ARCSEGBDRY, m_ArcSegBdry);
  DDX_Text(pDX, IDC_ARCANGLE, m_ArcAngle);
  DDV_MinMaxDouble(pDX, m_ArcAngle, 1., 180.);
  DDX_Text(pDX, IDC_MAXSEG, m_MaxSeg);
  DDV_MinMaxDouble(pDX, m_MaxSeg, 1.e-002, 10.);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_ARCANGLE, mIDC_ARCANGLE);
  DDX_Control(pDX, IDC_MAXSEG, m_IDC_MAXSEG);
}

BEGIN_MESSAGE_MAP(CArcDlg, CDialog)
//{{AFX_MSG_MAP(CArcDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArcDlg message handlers

BOOL CArcDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  int i;

  m_ArcSegBdry.AddString("<None>");

  for (i = 0; i < namelist.GetSize(); i++)
    m_ArcSegBdry.AddString(namelist[i]);

  m_ArcSegBdry.SetCurSel(cursel);
  // TODO: Add extra initialization here

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CArcDlg::OnOK()
{
  cursel = m_ArcSegBdry.GetCurSel();

  CDialog::OnOK();
}
