// DXFImport.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "DXFImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDXFImport dialog

CDXFImport::CDXFImport(CWnd* pParent /*=NULL*/)
    : CDialog(CDXFImport::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDXFImport)
  m_dxftol = 0.0;
  //}}AFX_DATA_INIT
}

void CDXFImport::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDXFImport)
  DDX_Text(pDX, IDC_DXFTOL, m_dxftol);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_DXFTOL, m_IDC_dxftol);
}

BEGIN_MESSAGE_MAP(CDXFImport, CDialog)
//{{AFX_MSG_MAP(CDXFImport)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDXFImport message handlers
