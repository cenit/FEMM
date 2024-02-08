// OpArcSegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "beladrawDoc.h"
#include "bd_OpArcSegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCOpArcSegDlg dialog

bdCOpArcSegDlg::bdCOpArcSegDlg(CWnd* pParent /*=NULL*/)
    : CDialog(bdCOpArcSegDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCOpArcSegDlg)
  m_MaxSeg = 0.0;
  m_hide = FALSE;
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void bdCOpArcSegDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCOpArcSegDlg)
  DDX_Control(pDX, IDC_BD_ARCSEG_COND, m_arcsegcond);
  DDX_Control(pDX, IDC_BD_ARCSEGBDRY2, m_ArcSegBdry);
  DDX_Text(pDX, IDC_BD_MAXSEG2, m_MaxSeg);
  DDX_Check(pDX, IDC_BD_ARCHIDE, m_hide);
  DDX_Text(pDX, IDC_BD_ARCGRP, m_ingroup);
  DDV_MinMaxDouble(pDX, m_MaxSeg, 1.e-002, 10.);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_ARCGRP, m_IDC_ingroup);
  DDX_Control(pDX, IDC_BD_MAXSEG2, m_IDC_MaxSeg);
}

BEGIN_MESSAGE_MAP(bdCOpArcSegDlg, CDialog)
//{{AFX_MSG_MAP(bdCOpArcSegDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCOpArcSegDlg message handlers

BOOL bdCOpArcSegDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
  int i;

  // TODO: Add extra initialization here
  m_ArcSegBdry.AddString("<None>");
  for (i = 0; i < lineproplist.GetSize(); i++)
    m_ArcSegBdry.AddString(lineproplist[i].BdryName);
  m_ArcSegBdry.SetCurSel(cursel);

  CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
  m_arcsegcond.AddString("<None>");
  for (i = 0; i < circproplist.GetSize(); i++)
    m_arcsegcond.AddString(circproplist[i].CircName);
  m_arcsegcond.SetCurSel(condsel);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCOpArcSegDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ArcSegBdry.GetCurSel();
  condsel = m_arcsegcond.GetCurSel();

  CDialog::OnOK();
}
