// OpNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cdrawDoc.h"
#include "cd_OpNodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdCOpNodeDlg dialog

cdCOpNodeDlg::cdCOpNodeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(cdCOpNodeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(cdCOpNodeDlg)
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void cdCOpNodeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cdCOpNodeDlg)
  DDX_Control(pDX, IDC_CD_NODE_COND, m_nodecond);
  DDX_Control(pDX, IDC_CD_ACKNODE, m_acknode);
  DDX_Text(pDX, IDC_CD_PTGRP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CD_PTGRP, m_IDC_ingroup);
}

BEGIN_MESSAGE_MAP(cdCOpNodeDlg, CDialog)
//{{AFX_MSG_MAP(cdCOpNodeDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdCOpNodeDlg message handlers

BOOL cdCOpNodeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;

  int i;

  // TODO: Add extra initialization here
  m_acknode.AddString("<None>");
  for (i = 0; i < nodeproplist.GetSize(); i++)
    m_acknode.AddString(nodeproplist[i].PointName);
  m_acknode.SetCurSel(cursel);

  CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
  m_nodecond.AddString("<None>");
  for (i = 0; i < circproplist.GetSize(); i++)
    m_nodecond.AddString(circproplist[i].CircName);
  m_nodecond.SetCurSel(condsel);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cdCOpNodeDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_acknode.GetCurSel();
  condsel = m_nodecond.GetCurSel();

  CDialog::OnOK();
}
