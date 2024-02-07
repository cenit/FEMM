// probdlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hd_probdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// probdlg dialog

hdCProbDlg::hdCProbDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hdCProbDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCProbDlg)
  m_problem_note = _T("");
  m_precision = 1.e-8;
  m_minangle = DEFAULT_MINIMUM_ANGLE;
  m_depth = 1.0;
  m_dt = 0.0;
  m_prevsoln = _T("");
  //}}AFX_DATA_INIT
}

void hdCProbDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCProbDlg)
  DDX_Control(pDX, IDC_HD_LENGTH_UNITS, m_length_units);
  DDX_Control(pDX, IDC_BSMART, m_bsmart);
  DDX_Control(pDX, IDC_HD_PROBTYPE, m_probtype);
  DDX_Text(pDX, IDC_HD_PROBNOTE, m_problem_note);
  DDX_Text(pDX, IDC_HD_PRC, m_precision);
  DDV_MinMaxDouble(pDX, m_precision, 1.e-016, 1.e-008);
  DDX_Text(pDX, IDC_HD_MINANG, m_minangle);
  DDV_MinMaxDouble(pDX, m_minangle, 1., MINANGLE_MAX);
  DDX_Text(pDX, IDC_HD_EDIT_DEPTH, m_depth);
  DDX_Text(pDX, IDC_HD_DT, m_dt);
  DDX_Text(pDX, IDC_HD_PREVSOLN, m_prevsoln);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_EDIT_DEPTH, m_IDC_depth);
  DDX_Control(pDX, IDC_HD_PROBNOTE, m_IDC_problem_note);
  DDX_Control(pDX, IDC_HD_PREVSOLN, m_IDC_prevsoln);
  DDX_Control(pDX, IDC_HD_DT, m_IDC_dt);
  DDX_Control(pDX, IDC_HD_PRC, m_IDC_precision);
  DDX_Control(pDX, IDC_HD_MINANG, m_IDC_minangle);
}

BEGIN_MESSAGE_MAP(hdCProbDlg, CDialog)
//{{AFX_MSG_MAP(hdCProbDlg)
ON_CBN_SELCHANGE(IDC_HD_LENGTH_UNITS, OnSelchangeLengthUnits)
ON_CBN_SELCHANGE(IDC_HD_PROBTYPE, OnSelchangeProbtype)
ON_EN_CHANGE(IDC_HD_PREVSOLN, OnChangeHdPrevsoln)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// probdlg message handlers

BOOL hdCProbDlg::OnInitDialog()
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

  if (m_prevsoln.GetLength() == 0) {
    m_IDC_dt.EnableWindow(FALSE);
    m_dt = 0;
  } else
    m_IDC_dt.EnableWindow(TRUE);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hdCProbDlg::OnOK()
{
  if (UpdateData() == FALSE)
    return;
  probtype = m_probtype.GetCurSel();
  if (probtype)
    m_depth = 1;
  if (m_prevsoln.GetLength() == 0)
    m_dt = 0;
  UpdateData(FALSE);
  lengthunits = m_length_units.GetCurSel();
  bsmart = m_bsmart.GetCurSel();
  CDialog::OnOK();
}

void hdCProbDlg::OnSelchangeLengthUnits()
{
}

void hdCProbDlg::OnSelchangeProbtype()
{
  if (m_probtype.GetCurSel())
    m_IDC_depth.EnableWindow(FALSE);
  else
    m_IDC_depth.EnableWindow(TRUE);
}

void hdCProbDlg::OnChangeHdPrevsoln()
{
  // TODO: If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  UpdateData();
  if (m_prevsoln.GetLength() == 0) {
    m_IDC_dt.EnableWindow(FALSE);
    m_dt = 0;
    UpdateData(FALSE);
  } else
    m_IDC_dt.EnableWindow(TRUE);
}
