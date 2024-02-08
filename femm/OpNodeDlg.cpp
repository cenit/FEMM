// OpNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "OpNodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpNodeDlg dialog

COpNodeDlg::COpNodeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COpNodeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(COpNodeDlg)
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void COpNodeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COpNodeDlg)
  DDX_Control(pDX, IDC_ACKNODE, m_acknode);
  DDX_Text(pDX, IDC_PTGRP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_PTGRP, m_IDC_ingroup);
}

BEGIN_MESSAGE_MAP(COpNodeDlg, CDialog)
//{{AFX_MSG_MAP(COpNodeDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpNodeDlg message handlers

BOOL COpNodeDlg::OnInitDialog()
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

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void COpNodeDlg::OnOK()
{
  // TODO: Add extra validation here
  cursel = m_acknode.GetCurSel();

  CDialog::OnOK();
}
