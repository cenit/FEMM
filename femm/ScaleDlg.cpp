// ScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "ScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog

CScaleDlg::CScaleDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CScaleDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CScaleDlg)
  m_basex = 0.0;
  m_basey = 0.0;
  m_scalefactor = 0.0;
  //}}AFX_DATA_INIT
}

void CScaleDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CScaleDlg)
  DDX_Text(pDX, IDC_BASEX, m_basex);
  DDX_Text(pDX, IDC_BASEY, m_basey);
  DDX_Text(pDX, IDC_SCALEFACTOR, m_scalefactor);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BASEX, m_IDC_basex);
  DDX_Control(pDX, IDC_BASEY, m_IDC_basey);
  DDX_Control(pDX, IDC_SCALEFACTOR, m_IDC_scalefactor);
}

BEGIN_MESSAGE_MAP(CScaleDlg, CDialog)
//{{AFX_MSG_MAP(CScaleDlg)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg message handlers
