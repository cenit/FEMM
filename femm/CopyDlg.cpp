// CopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "CopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg dialog

CCopyDlg::CCopyDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CCopyDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCopyDlg)
  m_aboutx = 0.0;
  m_abouty = 0.0;
  m_deltax = 0.0;
  m_deltay = 0.0;
  m_shiftangle = 0.0;
  m_ncopies = 0;
  //}}AFX_DATA_INIT
}

void CCopyDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCopyDlg)
  DDX_Text(pDX, IDC_ABOUTX, m_aboutx);
  DDX_Text(pDX, IDC_ABOUTY, m_abouty);
  DDX_Text(pDX, IDC_DELTAX, m_deltax);
  DDX_Text(pDX, IDC_DELTAY, m_deltay);
  DDX_Text(pDX, IDC_SHIFTANGLE, m_shiftangle);
  DDX_Text(pDX, IDC_NCOPIES, m_ncopies);
  DDV_MinMaxInt(pDX, m_ncopies, 0, 100);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_ABOUTX, m_IDC_aboutx);
  DDX_Control(pDX, IDC_ABOUTY, m_IDC_abouty);
  DDX_Control(pDX, IDC_DELTAX, m_IDC_deltax);
  DDX_Control(pDX, IDC_DELTAY, m_IDC_deltay);
  DDX_Control(pDX, IDC_SHIFTANGLE, m_IDC_shiftangle);
  DDX_Control(pDX, IDC_NCOPIES, m_IDC_ncopies);
}

BEGIN_MESSAGE_MAP(CCopyDlg, CDialog)
//{{AFX_MSG_MAP(CCopyDlg)
ON_BN_CLICKED(IDC_ROTATE, OnRotate)
ON_BN_CLICKED(IDC_TRANSLATE, OnTranslate)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg message handlers

BOOL CCopyDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (IsMove == TRUE) {
    SetWindowText("Move");
    SendDlgItemMessage(IDC_NCOPIES, WM_ENABLE, FALSE, 0);
  } else
    SetWindowText("Copy");

  CheckRadioButton(
      IDC_ROTATE, // identifier of first radio button in group
      IDC_TRANSLATE, // identifier of last radio button in group
      IDC_TRANSLATE // identifier of radio button to select
  );

  OnTranslate();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CCopyDlg::OnOK()
{
  // TODO: Add extra validation here

  CDialog::OnOK();
}

void CCopyDlg::OnRotate()
{
  // TODO: Add your control notification handler code here
  BtnState = 0;
  SendDlgItemMessage(IDC_SHIFTANGLE, WM_ENABLE, TRUE, 0);
  SendDlgItemMessage(IDC_ABOUTX, WM_ENABLE, TRUE, 0);
  SendDlgItemMessage(IDC_ABOUTY, WM_ENABLE, TRUE, 0);
  SendDlgItemMessage(IDC_DELTAX, WM_ENABLE, FALSE, 0);
  SendDlgItemMessage(IDC_DELTAY, WM_ENABLE, FALSE, 0);
}

void CCopyDlg::OnTranslate()
{
  // TODO: Add your control notification handler code here
  BtnState = 1;
  SendDlgItemMessage(IDC_SHIFTANGLE, WM_ENABLE, FALSE, 0);
  SendDlgItemMessage(IDC_ABOUTX, WM_ENABLE, FALSE, 0);
  SendDlgItemMessage(IDC_ABOUTY, WM_ENABLE, FALSE, 0);
  SendDlgItemMessage(IDC_DELTAX, WM_ENABLE, TRUE, 0);
  SendDlgItemMessage(IDC_DELTAY, WM_ENABLE, TRUE, 0);
}
