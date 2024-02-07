// CircProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "CircProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCircProp dialog

CCircProp::CCircProp(CWnd* pParent /*=NULL*/)
    : CDialog(CCircProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCircProp)
  m_circname = _T("");
  m_totcurrent = 0;
  //}}AFX_DATA_INIT
}

void CCircProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCircProp)
  DDX_Text(pDX, IDC_CIRCNAME, m_circname);
  DDX_Text(pDX, IDC_TOTCURRENT_RE, m_totcurrent);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CIRCNAME, m_idc_circname);
  DDX_Control(pDX, IDC_TOTCURRENT_RE, m_idc_totcurrent_re);
}

BEGIN_MESSAGE_MAP(CCircProp, CDialog)
//{{AFX_MSG_MAP(CCircProp)
ON_BN_CLICKED(IDC_RADIOAMP, OnRadioamp)
ON_BN_CLICKED(IDC_RADIOVOLT, OnRadiovolt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircProp message handlers

void CCircProp::OnRadioamp()
{
  m_circtype = 0;
}

void CCircProp::OnRadiovolt()
{
  m_circtype = 1;
}

BOOL CCircProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  SetEm = IDC_RADIOAMP;
  if (m_circtype == 1) {
    SetEm = IDC_RADIOVOLT;
    OnRadiovolt();
  } else
    OnRadioamp();

  CheckRadioButton(
      IDC_RADIOVOLT, // identifier of first radio button in group
      IDC_RADIOAMP, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CCircProp::OnOK()
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