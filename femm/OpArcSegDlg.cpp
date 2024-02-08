// OpArcSegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "OpArcSegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpArcSegDlg dialog

COpArcSegDlg::COpArcSegDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COpArcSegDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(COpArcSegDlg)
  m_MaxSeg = 0.0;
  m_hide = FALSE;
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void COpArcSegDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COpArcSegDlg)
  DDX_Control(pDX, IDC_ARCSEGBDRY2, m_ArcSegBdry);
  DDX_Check(pDX, IDC_ARCHIDE, m_hide);
  DDX_Text(pDX, IDC_ARCGRP, m_ingroup);
  DDX_Text(pDX, IDC_MAXSEG2, m_MaxSeg);
  DDX_Control(pDX, IDC_ARCGRP, m_IDC_ingroup);
  DDX_Control(pDX, IDC_MAXSEG2, m_IDC_MaxSeg);
  DDV_MinMaxDouble(pDX, m_MaxSeg, 1.e-002, 10.);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpArcSegDlg, CDialog)
//{{AFX_MSG_MAP(COpArcSegDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpArcSegDlg message handlers

BOOL COpArcSegDlg::OnInitDialog()
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

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void COpArcSegDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_ArcSegBdry.GetCurSel();

  CDialog::OnOK();
}
