// OpArcSegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hdrawDoc.h"
#include "hd_OpArcSegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCOpArcSegDlg dialog

hdCOpArcSegDlg::hdCOpArcSegDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hdCOpArcSegDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCOpArcSegDlg)
  m_MaxSeg = 0.0;
  m_hide = FALSE;
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void hdCOpArcSegDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCOpArcSegDlg)
  DDX_Control(pDX, IDC_HD_ARCSEG_COND, m_arcsegcond);
  DDX_Control(pDX, IDC_HD_ARCSEGBDRY2, m_ArcSegBdry);
  DDX_Text(pDX, IDC_HD_MAXSEG2, m_MaxSeg);
  DDX_Check(pDX, IDC_HD_ARCHIDE, m_hide);
  DDX_Text(pDX, IDC_HD_ARCGRP, m_ingroup);
  DDV_MinMaxDouble(pDX, m_MaxSeg, 1.e-002, 10.);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_ARCGRP, m_IDC_ingroup);
  DDX_Control(pDX, IDC_HD_MAXSEG2, m_IDC_MaxSeg);
}

BEGIN_MESSAGE_MAP(hdCOpArcSegDlg, CDialog)
//{{AFX_MSG_MAP(hdCOpArcSegDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCOpArcSegDlg message handlers

BOOL hdCOpArcSegDlg::OnInitDialog()
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

void hdCOpArcSegDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ArcSegBdry.GetCurSel();
  condsel = m_arcsegcond.GetCurSel();

  CDialog::OnOK();
}
