// LibFolderInfo.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "LibFolderInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibFolderInfo dialog


CLibFolderInfo::CLibFolderInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CLibFolderInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLibFolderInfo)
	m_foldername = _T("");
	m_folderurl = _T("");
	m_foldervendor = _T("");
	//}}AFX_DATA_INIT
}


void CLibFolderInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibFolderInfo)
	DDX_Text(pDX, IDC_FOLDERNAME, m_foldername);
	DDX_Text(pDX, IDC_FOLDERURL, m_folderurl);
	DDX_Text(pDX, IDC_FOLDERVENDOR, m_foldervendor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLibFolderInfo, CDialog)
	//{{AFX_MSG_MAP(CLibFolderInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibFolderInfo message handlers
