// GapIntegral.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "problem.h"
#include "XYPlot.h"
#include "femmviewdoc.h"
#include "GapIntegral.h"

// GapIntegral dialog

IMPLEMENT_DYNAMIC(GapIntegral, CDialog)

GapIntegral::GapIntegral(CWnd* pParent /*=NULL*/)
    : CDialog(GapIntegral::IDD, pParent)
{
}

GapIntegral::~GapIntegral()
{
}

void GapIntegral::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_AGENAME, m_AGEname);
  DDX_Control(pDX, IDC_GAPINTTYPE, m_IntType);
}

BEGIN_MESSAGE_MAP(GapIntegral, CDialog)
//{{AFX_MSG_MAP(GapIntegral)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// GapIntegral message handlers

BOOL GapIntegral::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  CArray<CAirGapElement, CAirGapElement&>& agelist = *pagelist;
  int i;

  // TODO: Add extra initialization here
  for (i = 0; i < agelist.GetSize(); i++)
    m_AGEname.AddString(agelist[i].BdryName);
  m_AGEname.SetCurSel(0);

  if (bIncremental == FALSE) {
    m_IntType.AddString("Torque");
    m_IntType.AddString("Force");
    m_IntType.AddString("Energy");
  } else {
    m_IntType.AddString("Incremental Torque");
    m_IntType.AddString("Incremental Force");
  }
  m_IntType.SetCurSel(0);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void GapIntegral::OnOK()
{
  // TODO: Add extra validation here
  CArray<CAirGapElement, CAirGapElement&>& agelist = *pagelist;
  BdryName = agelist[m_AGEname.GetCurSel()].BdryName;

  if (bIncremental == FALSE) {
    myIntType = m_IntType.GetCurSel();
  } else {
    myIntType = m_IntType.GetCurSel() + 3;
  }

  CDialog::OnOK();
}