// hsolvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hsolv.h"
#include "hsolvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern ChsolvApp theApp;

/////////////////////////////////////////////////////////////////////////////
// ChsolvDlg dialog

ChsolvDlg::ChsolvDlg(CWnd* pParent /*=NULL*/)
    : CDialog(ChsolvDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(ChsolvDlg)
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ChsolvDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ChsolvDlg)
  DDX_Control(pDX, IDC_PROGRESS2, m_prog2);
  DDX_Control(pDX, IDC_PROGRESS1, m_prog1);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ChsolvDlg, CDialog)
//{{AFX_MSG_MAP(ChsolvDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChsolvDlg message handlers

BOOL ChsolvDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE); // Set big icon
  SetIcon(m_hIcon, FALSE); // Set small icon

  // TODO: Add extra initialization here

  // Kludge to force the solver to come up minimized in Wine, since wine doesn't honor SW_SHOWMINNOACTIVE startup parameters
  HKEY h_key_registry = NULL;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Wine"), 0, KEY_READ, &h_key_registry) == ERROR_SUCCESS) {
    if (ComLine.Find("bLinehook") > 0) {
      theApp.GetMainWnd()->ShowWindow(SW_SHOWNOACTIVATE);
      theApp.GetMainWnd()->ShowWindow(SW_SHOWMINNOACTIVE);
    }
  }

  return FALSE; // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ChsolvDlg::OnPaint()
{
  if (IsIconic()) {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ChsolvDlg::OnQueryDragIcon()
{
  return (HCURSOR)m_hIcon;
}

void ChsolvDlg::OnOK()
{
}
