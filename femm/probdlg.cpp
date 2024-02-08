// probdlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "probdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// probdlg dialog

probdlg::probdlg(CWnd* pParent /*=NULL*/)
    : CDialog(probdlg::IDD, pParent)
    , m_prevsoln(_T(""))
{
  //{{AFX_DATA_INIT(probdlg)
  m_frequency = 0.0;
  m_problem_note = _T("");
  m_precision = 1.e-8;
  m_depth = 1.0;
  m_minangle = DEFAULT_MINIMUM_ANGLE;
  m_prevsoln = _T("");
  //}}AFX_DATA_INIT
}

void probdlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(probdlg)
  DDX_Control(pDX, IDC_SOLVER, m_solver);
  DDX_Control(pDX, IDC_LENGTH_UNITS, m_length_units);
  DDX_Control(pDX, IDC_BSMART, m_bsmart);
  DDX_Control(pDX, IDC_PROBTYPE, m_probtype);
  DDX_Control(pDX, IDC_PREVTYPE, m_prevtype);
  DDX_Text(pDX, IDC_EDIT_FREQ, m_frequency);
  DDX_Text(pDX, IDC_PROBNOTE, m_problem_note);
  DDX_Text(pDX, IDC_PRC, m_precision);
  DDV_MinMaxDouble(pDX, m_precision, 1.e-016, 1.e-008);
  DDX_Text(pDX, IDC_MINANG, m_minangle);
  DDV_MinMaxDouble(pDX, m_minangle, 1., MINANGLE_MAX);
  DDX_Text(pDX, IDC_EDIT_DEPTH, m_depth);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_EDIT_FREQ, m_IDC_frequency);
  DDX_Control(pDX, IDC_EDIT_DEPTH, m_IDC_depth);
  DDX_Control(pDX, IDC_PROBNOTE, m_IDC_problem_note);
  DDX_Control(pDX, IDC_PRC, m_IDC_precision);
  DDX_Control(pDX, IDC_MINANG, m_IDC_minangle);
  DDX_Text(pDX, IDC_PREVSOLN, m_prevsoln);
}

BEGIN_MESSAGE_MAP(probdlg, CDialog)
//{{AFX_MSG_MAP(probdlg)
ON_CBN_SELCHANGE(IDC_LENGTH_UNITS, OnSelchangeLengthUnits)
ON_CBN_SELCHANGE(IDC_PROBTYPE, OnSelchangeProbtype)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// probdlg message handlers

BOOL probdlg::OnInitDialog()
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
  m_solver.SetCurSel(solver);
  m_prevtype.SetCurSel(prevtype);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void probdlg::OnOK()
{
  if (UpdateData() == FALSE)
    return;
  probtype = m_probtype.GetCurSel();
  if (probtype)
    m_depth = 1;
  m_frequency = fabs(m_frequency);
  UpdateData(FALSE);
  lengthunits = m_length_units.GetCurSel();
  bsmart = m_bsmart.GetCurSel();
  solver = m_solver.GetCurSel();
  prevtype = m_prevtype.GetCurSel();
  CDialog::OnOK();
}

void probdlg::OnSelchangeLengthUnits()
{
}

void probdlg::OnSelchangeProbtype()
{
  if (m_probtype.GetCurSel())
    m_IDC_depth.EnableWindow(FALSE);
  else
    m_IDC_depth.EnableWindow(TRUE);
}
