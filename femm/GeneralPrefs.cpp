// GeneralPrefs.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "GeneralPrefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPrefs dialog


CGeneralPrefs::CGeneralPrefs(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralPrefs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneralPrefs)
	m_def_lua_console = FALSE;
	m_def_xyplot = FALSE;
	m_def_show_output_window = TRUE;
	m_def_smartmesh = TRUE;
	//}}AFX_DATA_INIT

	s_defdoc=0;
}


void CGeneralPrefs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPrefs)
	DDX_Control(pDX, IDC_DEF_DOC, m_defdoc);
	DDX_Check(pDX, IDC_DEF_LUA_CONSOLE, m_def_lua_console);
	DDX_Check(pDX, IDC_DEF_XYPLOT, m_def_xyplot);
	DDX_Check(pDX, IDC_DEF_SHOWOUTWND, m_def_show_output_window);
	DDX_Check(pDX, IDC_DEF_SMARTMESH, m_def_smartmesh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPrefs, CDialog)
	//{{AFX_MSG_MAP(CGeneralPrefs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPrefs message handlers
char* StripKey(char *c);

void CGeneralPrefs::ScanPrefs()
{
	FILE *fp;
	CString fname = ((CFemmApp *)AfxGetApp())->GetExecutablePath() + "femm.cfg";

	fp=fopen(fname,"rt");
	if (fp!=NULL)
	{
		BOOL flag=FALSE;
		char s[1024];
		char q[1024];
		char *v;

		// parse the file
		while (fgets(s,1024,fp)!=NULL)
		{
			sscanf(s,"%s",q);

			if( _strnicmp(q,"<ShowConsole>",13)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_def_lua_console);
			  q[0]=NULL;
			}
				
			if( _strnicmp(q,"<SeparatePlots>",15)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_def_xyplot);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<ShowOutputWindow>",18)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_def_show_output_window);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<SmartMesh>",18)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_def_smartmesh);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<DefaultType>",13)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&s_defdoc);
			  q[0]=NULL;
			}
		}
		fclose(fp);
	}
}

void CGeneralPrefs::WritePrefs()
{	
	FILE *fp;
	CString fname;
	
	UpdateData();
	
	fname=((CFemmApp *)AfxGetApp())->GetExecutablePath()+"femm.cfg";
	s_defdoc=m_defdoc.GetCurSel();

	fp=fopen(fname,"wt");
	if (fp!=NULL)
	{
		fprintf(fp,"<ShowConsole>      = %i\n",m_def_lua_console);
		fprintf(fp,"<SeparatePlots>    = %i\n",m_def_xyplot);
		fprintf(fp,"<ShowOutputWindow> = %i\n",m_def_show_output_window);
		fprintf(fp,"<SmartMesh>        = %i\n",m_def_smartmesh);
		fprintf(fp,"<DefaultType>      = %i\n",s_defdoc);
		fclose(fp);
	}

	// bubble xy plot preference back to the main app
	((CFemmApp *)AfxGetApp())->d_sepplot=m_def_xyplot;
}

BOOL CGeneralPrefs::PreTranslateMessage(MSG* pMsg) 
{
	 // Pressing ENTER should reroute message to parent
	if( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) )
	{
		GetParent()->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
		// Message needs no further processing
		return TRUE;
	}

	// Pressing ESC should reroute message to parent
	if( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		GetParent()->PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
		// Message needs no further processing
		return TRUE;
	}

	// Allow default handler otherwise
	return CDialog::PreTranslateMessage(pMsg);
} 

BOOL CGeneralPrefs::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ScanPrefs();

	m_defdoc.SetCurSel(s_defdoc);

	UpdateData(FALSE);

	return TRUE;  
}

