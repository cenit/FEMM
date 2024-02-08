// CircProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "bd_CircProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCCircProp dialog

bdCCircProp::bdCCircProp(CWnd* pParent /*=NULL*/)
    : CDialog(bdCCircProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCCircProp)
  m_circname = _T("");
  m_v = 0.0;
  m_q = 0.0;
  //}}AFX_DATA_INIT
}

void bdCCircProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCCircProp)
  DDX_Text(pDX, IDC_BD_CIRCNAME, m_circname);
  DDX_Text(pDX, IDC_BD_V, m_v);
  DDX_Text(pDX, IDC_BD_Q, m_q);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BD_CIRCNAME, m_idc_circname);
  DDX_Control(pDX, IDC_BD_V, m_idc_v);
  DDX_Control(pDX, IDC_BD_Q, m_idc_q);
}

BEGIN_MESSAGE_MAP(bdCCircProp, CDialog)
//{{AFX_MSG_MAP(bdCCircProp)
ON_BN_CLICKED(IDC_BD_RADIOAMP, OnRadioamp)
ON_BN_CLICKED(IDC_BD_RADIOVOLT, OnRadiovolt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCCircProp message handlers

void bdCCircProp::OnRadioamp()
{
  m_circtype = 0;
  m_idc_q.EnableWindow(TRUE);
  m_idc_v.EnableWindow(FALSE);
}

void bdCCircProp::OnRadiovolt()
{
  m_circtype = 1;
  m_idc_q.EnableWindow(FALSE);
  m_idc_v.EnableWindow(TRUE);
}

BOOL bdCCircProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  SetEm = IDC_BD_RADIOAMP;
  if (m_circtype == 1) {
    SetEm = IDC_BD_RADIOVOLT;
    OnRadiovolt();
  } else
    OnRadioamp();

  CheckRadioButton(
      IDC_BD_RADIOVOLT, // identifier of first radio button in group
      IDC_BD_RADIOAMP, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void bdCCircProp::OnOK()
{
  UpdateData();
  for (int nn = 0; nn < namelist.GetSize(); nn++) {
    if (m_circname == namelist[nn]) {
      MsgBox("The name \"%s\" has already been used.\nSelect a different name for this property.", m_circname);
      return;
    }
  }

  CDialog::OnOK();
}