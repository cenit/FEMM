// NodeProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hd_NodeProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCNodeProp dialog

hdCNodeProp::hdCNodeProp(CWnd* pParent /*=NULL*/)
    : CDialog(hdCNodeProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCNodeProp)
  m_nodename = _T("");
  m_qp = 0.0;
  m_vp = 0.0;
  //}}AFX_DATA_INIT
}

void hdCNodeProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCNodeProp)
  DDX_Text(pDX, IDC_HD_NODENAME, m_nodename);
  DDX_Text(pDX, IDC_HD_QP, m_qp);
  DDX_Text(pDX, IDC_HD_VP, m_vp);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_HD_VP, m_IDC_vp);
  DDX_Control(pDX, IDC_HD_QP, m_IDC_qp);
  DDX_Control(pDX, IDC_HD_NODENAME, m_IDC_nodename);
}

BEGIN_MESSAGE_MAP(hdCNodeProp, CDialog)
//{{AFX_MSG_MAP(hdCNodeProp)
ON_BN_CLICKED(IDC_HD_SETA, OnSetA)
ON_BN_CLICKED(IDC_HD_SETI, OnSetI)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCNodeProp message handlers

void hdCNodeProp::OnSetA()
{
  // TODO: Add your control notification handler code here
  m_IDC_qp.EnableWindow(FALSE);
  m_IDC_vp.EnableWindow(TRUE);
  SetDlgItemText(IDC_HD_QP, "0");
  m_qp = 0;
}

void hdCNodeProp::OnSetI()
{
  // TODO: Add your control notification handler code here
  m_IDC_vp.EnableWindow(FALSE);
  m_IDC_qp.EnableWindow(TRUE);
  SetDlgItemText(IDC_HD_VP, "0");
  m_vp = 0;
}

BOOL hdCNodeProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  if (m_qp != 0) {
    SetEm = IDC_HD_SETI;

    m_IDC_vp.EnableWindow(FALSE);
    m_IDC_qp.EnableWindow(TRUE);
  } else {
    SetEm = IDC_HD_SETA;
    m_IDC_qp.EnableWindow(FALSE);
    m_IDC_vp.EnableWindow(TRUE);
  }

  CheckRadioButton(
      IDC_HD_SETA, // identifier of first radio button in group
      IDC_HD_SETI, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void hdCNodeProp::OnOK()
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
