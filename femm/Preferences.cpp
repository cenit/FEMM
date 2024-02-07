// Preferences.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog

CPreferences::CPreferences(CWnd* pParent /*=NULL*/)
    : CDialog(CPreferences::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPreferences)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CPreferences::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPreferences)
  DDX_Control(pDX, IDC_TAB1, m_tab1);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPreferences, CDialog)
//{{AFX_MSG_MAP(CPreferences)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferences message handlers

BOOL CPreferences::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_tab1.InsertItem(0, _T("Magnetics Input"));
  m_tab1.InsertItem(1, _T("Magnetics Output"));
  m_tab1.InsertItem(2, _T("Electrostatics Input"));
  m_tab1.InsertItem(3, _T("Electrostatics Output"));
  m_tab1.InsertItem(4, _T("Heat Flow Input"));
  m_tab1.InsertItem(5, _T("Heat Flow Output"));
  m_tab1.InsertItem(6, _T("Current Flow Input"));
  m_tab1.InsertItem(7, _T("Current Flow Output"));
  m_tab1.InsertItem(8, _T("General Attributes"));

  m_tab1.Init();

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferences::OnOK()
{
  m_tab1.WritePrefs();

  CDialog::OnOK();
}
