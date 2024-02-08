// NodeProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cd_NodeProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdCNodeProp dialog

cdCNodeProp::cdCNodeProp(CWnd* pParent /*=NULL*/)
    : CDialog(cdCNodeProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(cdCNodeProp)
  m_nodename = _T("");
  m_qp = 0;
  m_vp = 0;
  //}}AFX_DATA_INIT
}

void cdCNodeProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cdCNodeProp)
  DDX_Text(pDX, IDC_CD_NODENAME, m_nodename);
  DDX_Text(pDX, IDC_CD_QP, m_qp);
  DDX_Text(pDX, IDC_CD_VP, m_vp);
  ;
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CD_VP, m_IDC_vpr);
  DDX_Control(pDX, IDC_CD_QP, m_IDC_qpr);
  DDX_Control(pDX, IDC_CD_NODENAME, m_IDC_nodename);
}

BEGIN_MESSAGE_MAP(cdCNodeProp, CDialog)
//{{AFX_MSG_MAP(cdCNodeProp)
ON_BN_CLICKED(IDC_CD_SETA, OnSetA)
ON_BN_CLICKED(IDC_CD_SETI, OnSetI)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdCNodeProp message handlers

void cdCNodeProp::OnSetA()
{
  // TODO: Add your control notification handler code here
  m_IDC_qpr.EnableWindow(FALSE);
  m_IDC_vpr.EnableWindow(TRUE);
  SetDlgItemText(IDC_CD_QP, "0");
  m_qp = 0;
}

void cdCNodeProp::OnSetI()
{
  // TODO: Add your control notification handler code here
  m_IDC_vpr.EnableWindow(FALSE);
  m_IDC_qpr.EnableWindow(TRUE);
  SetDlgItemText(IDC_CD_VP, "0");
  m_vp = 0;
}

BOOL cdCNodeProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  if (abs(m_qp) > 0) {
    SetEm = IDC_CD_SETI;
    OnSetI();
  } else {
    SetEm = IDC_CD_SETA;
    OnSetA();
  }

  CheckRadioButton(
      IDC_CD_SETA, // identifier of first radio button in group
      IDC_CD_SETI, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cdCNodeProp::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_nodename == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_nodename);
      return;
    }
  }

  CDialog::OnOK();
}
