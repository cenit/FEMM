// OpGrp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "OpGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpGrp dialog

COpGrp::COpGrp(CWnd* pParent /*=NULL*/)
    : CDialog(COpGrp::IDD, pParent)
{
  //{{AFX_DATA_INIT(COpGrp)
  m_ingroup = 0;
  //}}AFX_DATA_INIT
}

void COpGrp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COpGrp)
  DDX_Text(pDX, IDC_INGRP, m_ingroup);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_INGRP, m_IDC_ingroup);
}

BEGIN_MESSAGE_MAP(COpGrp, CDialog)
//{{AFX_MSG_MAP(COpGrp)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpGrp message handlers
