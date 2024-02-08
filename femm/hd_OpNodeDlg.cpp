// OpNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hdrawDoc.h"
#include "hd_OpNodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCOpNodeDlg dialog

hdCOpNodeDlg::hdCOpNodeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(hdCOpNodeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCOpNodeDlg)
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void hdCOpNodeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCOpNodeDlg)
  DDX_Control(pDX, IDC_HD_NODE_COND, m_nodecond);
  DDX_Control(pDX, IDC_HD_ACKNODE, m_acknode);
  DDX_Text(pDX, IDC_HD_PTGRP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_PTGRP, m_IDC_ingroup);
}

BEGIN_MESSAGE_MAP(hdCOpNodeDlg, CDialog)
//{{AFX_MSG_MAP(hdCOpNodeDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCOpNodeDlg message handlers

BOOL hdCOpNodeDlg::OnInitDialog()
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

void hdCOpNodeDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_acknode.GetCurSel();
  condsel = m_nodecond.GetCurSel();

  CDialog::OnOK();
}
