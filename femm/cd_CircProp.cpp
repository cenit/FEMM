// CircProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "cd_CircProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdCCircProp dialog

cdCCircProp::cdCCircProp(CWnd* pParent /*=NULL*/)
    : CDialog(cdCCircProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(cdCCircProp)
  m_circname = _T("");
  m_vc = 0;
  m_qc = 0;
  //}}AFX_DATA_INIT
}

void cdCCircProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(cdCCircProp)
  DDX_Text(pDX, IDC_CD_CIRCNAME, m_circname);
  DDX_Text(pDX, IDC_CD_V, m_vc);
  DDX_Text(pDX, IDC_CD_Q, m_qc);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CD_CIRCNAME, m_idc_circname);
  DDX_Control(pDX, IDC_CD_V, m_idc_vc);
  DDX_Control(pDX, IDC_CD_Q, m_idc_qc);
}

BEGIN_MESSAGE_MAP(cdCCircProp, CDialog)
//{{AFX_MSG_MAP(cdCCircProp)
ON_BN_CLICKED(IDC_CD_RADIOAMP, OnRadioamp)
ON_BN_CLICKED(IDC_CD_RADIOVOLT, OnRadiovolt)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdCCircProp message handlers

void cdCCircProp::OnRadioamp()
{
  m_circtype = 0;
  m_idc_qc.EnableWindow(TRUE);
  m_idc_vc.EnableWindow(FALSE);
}

void cdCCircProp::OnRadiovolt()
{
  m_circtype = 1;
  m_idc_qc.EnableWindow(FALSE);
  m_idc_vc.EnableWindow(TRUE);
}

BOOL cdCCircProp::OnInitDialog()
{
  CDialog::OnInitDialog();

  int SetEm;

  SetEm = IDC_CD_RADIOAMP;
  if (m_circtype == 1) {
    SetEm = IDC_CD_RADIOVOLT;
    OnRadiovolt();
  } else
    OnRadioamp();

  CheckRadioButton(
      IDC_CD_RADIOVOLT, // identifier of first radio button in group
      IDC_CD_RADIOAMP, // identifier of last radio button in group
      SetEm // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void cdCCircProp::OnOK()
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
