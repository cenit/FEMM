// BHDatafile.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "BHDatafile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBHDatafile dialog

CBHDatafile::CBHDatafile(CWnd* pParent /*=NULL*/)
    : CDialog(CBHDatafile::IDD, pParent)
{
  //{{AFX_DATA_INIT(CBHDatafile)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CBHDatafile::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBHDatafile)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBHDatafile, CDialog)
//{{AFX_MSG_MAP(CBHDatafile)
ON_BN_CLICKED(IDC_AMPM, OnAmpm)
ON_BN_CLICKED(IDC_BFIRST, OnBfirst)
ON_BN_CLICKED(IDC_GAUSS, OnGauss)
ON_BN_CLICKED(IDC_KAMPM, OnKampm)
ON_BN_CLICKED(IDC_KGAUSS, OnKgauss)
ON_BN_CLICKED(IDC_KOERSTED, OnKoersted)
ON_BN_CLICKED(IDC_OERSTED, OnOersted)
ON_BN_CLICKED(IDC_TESLA, OnTesla)
ON_BN_CLICKED(IDC_HFIRST, OnHfirst)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBHDatafile message handlers

BOOL CBHDatafile::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  BHOrder = 0;
  BUnits = 0;
  HUnits = 0;

  CheckRadioButton(
      IDC_BFIRST, // identifier of first radio button in group
      IDC_HFIRST, // identifier of last radio button in group
      IDC_BFIRST // identifier of radio button to select
  );

  CheckRadioButton(
      IDC_TESLA, // identifier of first radio button in group
      IDC_KGAUSS, // identifier of last radio button in group
      IDC_TESLA // identifier of radio button to select
  );

  CheckRadioButton(
      IDC_AMPM, // identifier of first radio button in group
      IDC_KOERSTED, // identifier of last radio button in group
      IDC_AMPM // identifier of radio button to select
  );

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CBHDatafile::OnAmpm()
{
  // TODO: Add your control notification handler code here
  HUnits = 0;
}

void CBHDatafile::OnBfirst()
{
  // TODO: Add your control notification handler code here
  BHOrder = 0;
}

void CBHDatafile::OnGauss()
{
  // TODO: Add your control notification handler code here
  BUnits = 1;
}

void CBHDatafile::OnKampm()
{
  // TODO: Add your control notification handler code here
  HUnits = 1;
}

void CBHDatafile::OnKgauss()
{
  // TODO: Add your control notification handler code here
  BUnits = 2;
}

void CBHDatafile::OnKoersted()
{
  // TODO: Add your control notification handler code here
  HUnits = 3;
}

void CBHDatafile::OnOersted()
{
  // TODO: Add your control notification handler code here
  HUnits = 2;
}

void CBHDatafile::OnTesla()
{
  // TODO: Add your control notification handler code here
  BUnits = 0;
}

void CBHDatafile::OnHfirst()
{
  // TODO: Add your control notification handler code here
  BHOrder = 1;
}
