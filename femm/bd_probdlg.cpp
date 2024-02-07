// probdlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bd_probdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// probdlg dialog

bdCProbDlg::bdCProbDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bdCProbDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCProbDlg)
  m_problem_note = _T("");
  m_precision = 1.e-8;
  m_depth = 1.0;
  m_minangle = DEFAULT_MINIMUM_ANGLE;
  //}}AFX_DATA_INIT
}

void bdCProbDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCProbDlg)
  DDX_Control(pDX, IDC_BD_LENGTH_UNITS, m_length_units);
  DDX_Control(pDX, IDC_BSMART, m_bsmart);
  DDX_Control(pDX, IDC_BD_PROBTYPE, m_probtype);
  DDX_Text(pDX, IDC_BD_PROBNOTE, m_problem_note);
  DDX_Text(pDX, IDC_BD_PRC, m_precision);
  DDV_MinMaxDouble(pDX, m_precision, 1.e-016, 1.e-008);
  DDX_Text(pDX, IDC_BD_MINANG, m_minangle);
  DDV_MinMaxDouble(pDX, m_minangle, 1., MINANGLE_MAX);
  DDX_Text(pDX, IDC_BD_EDIT_DEPTH, m_depth);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_EDIT_DEPTH, m_IDC_depth);
  DDX_Control(pDX, IDC_BD_PROBNOTE, m_IDC_problem_note);
  DDX_Control(pDX, IDC_BD_PRC, m_IDC_precision);
  DDX_Control(pDX, IDC_BD_MINANG, m_IDC_minangle);
}

BEGIN_MESSAGE_MAP(bdCProbDlg, CDialog)
//{{AFX_MSG_MAP(bdCProbDlg)
ON_CBN_SELCHANGE(IDC_BD_LENGTH_UNITS, OnSelchangeLengthUnits)
ON_CBN_SELCHANGE(IDC_BD_PROBTYPE, OnSelchangeProbtype)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// probdlg message handlers

BOOL bdCProbDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_probtype.SetCurSel(probtype);
  if (probtype)
    m_IDC_depth.EnableWindow(FALSE);
  else
    m_IDC_depth.EnableWindow(TRUE);

  m_length_units.SetCurSel(lengthunits);
  m_bsmart.SetCurSel(bsmart);
  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCProbDlg::OnOK()
{
  if (UpdateData() == FALSE)
    return;
  probtype = m_probtype.GetCurSel();
  if (probtype)
    m_depth = 1;
  UpdateData(FALSE);
  lengthunits = m_length_units.GetCurSel();
  bsmart = m_bsmart.GetCurSel();
  CDialog::OnOK();
}

void bdCProbDlg::OnSelchangeLengthUnits()
{
}

void bdCProbDlg::OnSelchangeProbtype()
{
  if (m_probtype.GetCurSel())
    m_IDC_depth.EnableWindow(FALSE);
  else
    m_IDC_depth.EnableWindow(TRUE);
}
