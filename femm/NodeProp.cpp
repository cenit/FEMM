// NodeProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "NodeProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeProp dialog

CNodeProp::CNodeProp(CWnd* pParent /*=NULL*/)
    : CDialog(CNodeProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(CNodeProp)
  m_ap = 0;
  m_jp = 0;
  m_nodename = _T("");
  //}}AFX_DATA_INIT
}

void CNodeProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CNodeProp)
  DDX_Text(pDX, IDC_A_RE, m_ap);
  DDX_Text(pDX, IDC_J_RE, m_jp);
  DDX_Text(pDX, IDC_NODENAME, m_nodename);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_A_RE, m_IDC_a_re);
  DDX_Control(pDX, IDC_J_RE, m_IDC_j_re);
  DDX_Control(pDX, IDC_NODENAME, m_IDC_nodename);
}

BEGIN_MESSAGE_MAP(CNodeProp, CDialog)
//{{AFX_MSG_MAP(CNodeProp)
ON_BN_CLICKED(IDC_SETA, OnSetA)
ON_BN_CLICKED(IDC_SETI, OnSetI)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeProp message handlers

void CNodeProp::OnSetA()
{
  // TODO: Add your control notification handler code here
  m_IDC_j_re.EnableWindow(FALSE);
  m_IDC_a_re.EnableWindow(TRUE);
  SetDlgItemText(IDC_J_RE, "0");
  m_jp = 0;
}

void CNodeProp::OnSetI()
{
  // TODO: Add your control notification handler code here
  m_IDC_a_re.EnableWindow(FALSE);
  m_IDC_j_re.EnableWindow(TRUE);
  SetDlgItemText(IDC_A_RE, "0");
  m_ap = 0;
}

BOOL CNodeProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  if (abs(m_jp) > 0) {
    SetEm = IDC_SETI;
    m_IDC_a_re.EnableWindow(FALSE);
    m_IDC_j_re.EnableWindow(TRUE);
  } else {
    SetEm = IDC_SETA;
    m_IDC_j_re.EnableWindow(FALSE);
    m_IDC_a_re.EnableWindow(TRUE);
  }

  CheckRadioButton(
      IDC_SETA, // identifier of first radio button in group
      IDC_SETI, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeProp::OnOK()
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