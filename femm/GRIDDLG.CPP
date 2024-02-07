// GRIDDLG.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "GRIDDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GRIDDLG dialog

GRIDDLG::GRIDDLG(CWnd* pParent /*=NULL*/)
    : CDialog(GRIDDLG::IDD, pParent)
{
  //{{AFX_DATA_INIT(GRIDDLG)
  m_gridsize = 0.0;
  //}}AFX_DATA_INIT
}

void GRIDDLG::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(GRIDDLG)
  DDX_Control(pDX, IDC_COORDS, m_coords);
  DDX_Text(pDX, IDC_GRIDSIZE, m_gridsize);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_GRIDSIZE, m_IDC_gridsize);
}

BEGIN_MESSAGE_MAP(GRIDDLG, CDialog)
//{{AFX_MSG_MAP(GRIDDLG)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GRIDDLG message handlers

BOOL GRIDDLG::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_coords.SetCurSel(coords);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void GRIDDLG::OnOK()
{
  // TODO: Add extra validation here
  coords = m_coords.GetCurSel();
  CDialog::OnOK();
}
