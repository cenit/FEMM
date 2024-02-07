// NodeProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bd_NodeProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCNodeProp dialog

bdCNodeProp::bdCNodeProp(CWnd* pParent /*=NULL*/)
    : CDialog(bdCNodeProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCNodeProp)
  m_nodename = _T("");
  m_qp = 0.0;
  m_vp = 0.0;
  //}}AFX_DATA_INIT
}

void bdCNodeProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCNodeProp)
  DDX_Text(pDX, IDC_BD_NODENAME, m_nodename);
  DDX_Text(pDX, IDC_BD_QP, m_qp);
  DDX_Text(pDX, IDC_BD_VP, m_vp);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_VP, m_IDC_vp);
  DDX_Control(pDX, IDC_BD_QP, m_IDC_qp);
  DDX_Control(pDX, IDC_BD_NODENAME, m_IDC_nodename);
}

BEGIN_MESSAGE_MAP(bdCNodeProp, CDialog)
//{{AFX_MSG_MAP(bdCNodeProp)
ON_BN_CLICKED(IDC_BD_SETA, OnSetA)
ON_BN_CLICKED(IDC_BD_SETI, OnSetI)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCNodeProp message handlers

void bdCNodeProp::OnSetA()
{
  // TODO: Add your control notification handler code here
  m_IDC_qp.EnableWindow(FALSE);
  m_IDC_vp.EnableWindow(TRUE);
  SetDlgItemText(IDC_BD_QP, "0");
  m_qp = 0;
}

void bdCNodeProp::OnSetI()
{
  // TODO: Add your control notification handler code here
  m_IDC_vp.EnableWindow(FALSE);
  m_IDC_qp.EnableWindow(TRUE);
  SetDlgItemText(IDC_BD_VP, "0");
  m_vp = 0;
}

BOOL bdCNodeProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  if (m_qp != 0) {
    SetEm = IDC_BD_SETI;

    m_IDC_vp.EnableWindow(FALSE);
    m_IDC_qp.EnableWindow(TRUE);
  } else {
    SetEm = IDC_BD_SETA;
    m_IDC_qp.EnableWindow(FALSE);
    m_IDC_vp.EnableWindow(TRUE);
  }

  CheckRadioButton(
      IDC_BD_SETA, // identifier of first radio button in group
      IDC_BD_SETI, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCNodeProp::OnOK()
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