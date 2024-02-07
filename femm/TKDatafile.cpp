// TKDatafile.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "TKDatafile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTKDatafile dialog

CTKDatafile::CTKDatafile(CWnd* pParent /*=NULL*/)
    : CDialog(CTKDatafile::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTKDatafile)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CTKDatafile::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTKDatafile)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTKDatafile, CDialog)
//{{AFX_MSG_MAP(CTKDatafile)
ON_BN_CLICKED(IDC_TFIRST, OnTfirst)
ON_BN_CLICKED(IDC_KFIRST, OnKfirst)
ON_BN_CLICKED(IDC_KELV, OnKelvin)
ON_BN_CLICKED(IDC_CELC, OnCelcius)
ON_BN_CLICKED(IDC_FAHR, OnFahrenheit)
ON_BN_CLICKED(IDC_RANK, OnRankine)
ON_BN_CLICKED(IDC_WMK, OnWMK)
ON_BN_CLICKED(IDC_BTU, OnBTU)
ON_BN_CLICKED(IDC_KCAL, OnKCAL)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTKDatafile message handlers

BOOL CTKDatafile::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  TKOrder = 0;
  TUnits = 0;
  KUnits = 0;

  CheckRadioButton(
      IDC_TFIRST, // identifier of first radio button in group
      IDC_KFIRST, // identifier of last radio button in group
      IDC_TFIRST // identifier of radio button to select
  );

  CheckRadioButton(
      IDC_KELV, // identifier of first radio button in group
      IDC_RANK, // identifier of last radio button in group
      IDC_KELV // identifier of radio button to select
  );

  CheckRadioButton(
      IDC_WMK, // identifier of first radio button in group
      IDC_KCAL, // identifier of last radio button in group
      IDC_WMK // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CTKDatafile::OnTfirst()
{
  // TODO: Add your control notification handler code here
  TKOrder = 0;
}

void CTKDatafile::OnKfirst()
{
  TKOrder = 1;
}

void CTKDatafile::OnKelvin()
{
  TUnits = 0;
}

void CTKDatafile::OnCelcius()
{
  TUnits = 1;
}

void CTKDatafile::OnFahrenheit()
{
  TUnits = 2;
}

void CTKDatafile::OnRankine()
{
  TUnits = 3;
}

void CTKDatafile::OnWMK()
{
  KUnits = 0;
}

void CTKDatafile::OnBTU()
{
  KUnits = 1;
}

void CTKDatafile::OnKCAL()
{
  KUnits = 2;
}
