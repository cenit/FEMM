// femm.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "femm.h"
#include "FemmeDoc.h"
#include "FemmeView.h"
#include "xyplot.h"
#include "problem.h"
#include "FemmviewDoc.h"
#include "FemmviewView.h"
#include "hdrawDoc.h"
#include "hdrawView.h"
#include "hv_problem.h"
#include "hviewDoc.h"
#include "hviewView.h"

#include "cdrawDoc.h"
#include "cdrawView.h"
#include "cv_problem.h"
#include "cviewDoc.h"
#include "cviewView.h"

#include "beladrawDoc.h"
#include "beladrawView.h"
#include "bv_problem.h"
#include "belaviewDoc.h"
#include "belaviewView.h"
#include "femmplotDoc.h"
#include "femmplotView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "promptbox.h"
#include "lua.h"
#include "MyCommandLineInfo.h"
#include "NewDocDlg.h"
#include <afxadv.h>
#include <afxctl.h>
#include <process.h>
#include "MyRecentFileList.h"

#ifdef MATHLINK
#include "mathlink.h"
#endif

extern void lua_baselibopen(lua_State* L);
extern void lua_iolibopen(lua_State* L);
extern void lua_strlibopen(lua_State* L);
extern void lua_mathlibopen(lua_State* L);
extern void lua_dblibopen(lua_State* L);

extern void* pFemmeDoc;
extern void* pFemmviewdoc;
extern void* pBeladrawDoc;
extern void* pBelaviewDoc;
extern void* phdrawDoc;
extern void* phviewDoc;
extern void* pcdrawDoc;
extern void* pcviewDoc;

extern CLuaConsoleDlg* LuaConsole;

extern lua_State* lua;
extern BOOL bLinehook;
extern BOOL lua_byebye;
extern HANDLE hProc;
extern int m_luaWindowStatus;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFemmApp

BEGIN_MESSAGE_MAP(CFemmApp, CWinApp)
//{{AFX_MSG_MAP(CFemmApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_COMMAND(ID_FILE_OPEN_LUA_SCRIPT, OnFileOpenLuaScript)
ON_COMMAND(ID_FILE_NEW, OnFileNew)
ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
// Standard print setup command
ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFemmApp construction

CFemmApp::CFemmApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance

  // defaults
  d_luaconsole = FALSE;
  d_sepplot = FALSE;
  d_defdoc = 0;
  d_SmartMesh = TRUE;
  session_SmartMesh = -1;
  d_bShowOutputWindow = TRUE;
  bFileLink = FALSE;
  bActiveX = FALSE;
  IFile = GetExecutablePath() + "ifile.txt";
  OFile = GetExecutablePath() + "ofile.txt";
  luaShowWindow = NULL;
  CompatibilityMode = 0;
#ifdef MATHLINK
  bMathLink = FALSE;
#endif

  ScanPrefs();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFemmApp object

CFemmApp theApp;

// lua globals
lua_State* lua; // the main lua object
BOOL bLinehook;
BOOL lua_byebye;
CString luafiles[20];
CLuaConsoleDlg* LuaConsole = NULL;
int luafilepointer; // both used during the CMyCommandLineInfo parsing
CFemmApp* pApp;
HANDLE hProc;
int m_luaWindowStatus = SW_SHOW;
CString luascriptname;

void* pFemmeDoc = NULL; // pointer to the Femme    document currently in use
void* pFemmviewdoc = NULL; // pointer to the Femmview document currently in use
void* pBeladrawDoc = NULL; // pointer to the Beladraw document currently in use
void* pBelaviewDoc = NULL; // pointer to the Belaview document currently in use
void* phdrawDoc = NULL; // pointer to the hdraw document currently in use
void* phviewDoc = NULL; // pointer to the hview document currently in use
void* pcdrawDoc = NULL;
void* pcviewDoc = NULL;

#ifdef MATHLINK
// Mathlink;
MLINK alink;
MLENV mathenv;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFemmApp initialization

BOOL CFemmApp::InitInstance()
{
  // Initialize OLE libraries
  if (!AfxOleInit()) {
    MsgBox("OLE initialization failed.  Make sure that the OLE libraries are the correct version.");
    return FALSE;
  }

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
  Enable3dControlsStatic(); // Call this when linking to MFC statically
#endif

  // Change the registry key under which our settings are stored.
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(_T("Gedanken Magnetics\\FEMM\\4"));

  LoadStdProfileSettings(8); // Load standard INI file options (including MRU)

  // Parse the command line to see if launched as OLE server
  if (RunEmbedded() || RunAutomated()) {
    // Register all OLE server (factories) as running.  This enables the
    //  OLE libraries to create objects from other applications.
    COleTemplateServer::RegisterAll();
    m_luaWindowStatus = SW_SHOWMINNOACTIVE;
  } else {
    // When a server application is launched stand-alone, it is a good idea
    //  to update the system registry in case it has been damaged.
    COleObjectFactory::UpdateRegistryAll();
  }

  // 04EF434A-1A91-495A-85AA-C625602B4AF4
  const IID LIBID_ActiveFEMM = { 0x04EF434A, 0x1A91, 0x495A, { 0x85, 0xAA, 0xC6, 0x25, 0x60, 0x2B, 0x4A, 0xF4 } };

  //	if(AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_ActiveFEMM, _T("femm.TLB"))==FALSE) MsgBox("TypeLib not registered!");
  AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_ActiveFEMM, _T("femm.TLB"), NULL);

  // Initialize Lua
  bLinehook = FALSE;
  lua = lua_open(4096);
  lua_baselibopen(lua);
  lua_strlibopen(lua);
  lua_mathlibopen(lua);
  lua_iolibopen(lua);
  // register some common Lua functionality
  lua_register(lua, "_ALERT", lua_ERROR);
  lua_register(lua, "messagebox", lua_messagebox);
  lua_register(lua, "pause", lua_afxpause);
  lua_register(lua, "prompt", lua_promptbox);
  lua_register(lua, "open", luaOpenDocument);
  lua_register(lua, "quit", lua_endapp);
  lua_register(lua, "exit", lua_endapp);

  lua_register(lua, "create", lua_newdocument);
  lua_register(lua, "Complex", lua_Complex);
  lua_register(lua, "newdocument", lua_newdocument);
  lua_register(lua, "new_document", lua_newdocument);

  lua_register(lua, "showpointprops", lua_showpointprops);
  lua_register(lua, "hidepointprops", lua_hidepointprops);
  lua_register(lua, "show_point_props", lua_showpointprops);
  lua_register(lua, "hide_point_props", lua_hidepointprops);

#ifdef MATHLINK
  lua_register(lua, "mlput", lua_to_mathematica);
  lua_register(lua, "mlopen", lua_open_mathlink);
  lua_register(lua, "mlclose", lua_close_mathlink);
#endif

  lua_register(lua, "flput", lua_to_filelink);
  lua_register(lua, "setcompatibilitymode", lua_compatibilitymode);
  lua_register(lua, "setcurrentdirectory", lua_setcurrentdirectory);
  lua_register(lua, "chdir", lua_setcurrentdirectory);
  lua_register(lua, "smartmesh", lua_smartmesh);
  lua_register(lua, "makeplot", lua_makeplot);
  lua_setlinehook(lua, line_hook);

  pApp = this;
  NumViews = 0;

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.

  CMultiDocTemplate* pFemmeDocTemplate;
  pFemmeDocTemplate = new CMultiDocTemplate(
      IDR_FEMMETYPE,
      RUNTIME_CLASS(CFemmeDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CFemmeView));
  AddDocTemplate(pFemmeDocTemplate);

  CMultiDocTemplate* pbeladrawDocTemplate;
  pbeladrawDocTemplate = new CMultiDocTemplate(
      IDR_BELADRAWTYPE,
      RUNTIME_CLASS(CbeladrawDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CbeladrawView));
  AddDocTemplate(pbeladrawDocTemplate);

  CMultiDocTemplate* phdrawDocTemplate;
  phdrawDocTemplate = new CMultiDocTemplate(
      IDR_HDRAWTYPE,
      RUNTIME_CLASS(ChdrawDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(ChdrawView));
  AddDocTemplate(phdrawDocTemplate);

  CMultiDocTemplate* pcdrawDocTemplate;
  pcdrawDocTemplate = new CMultiDocTemplate(
      IDR_CDRAWTYPE,
      RUNTIME_CLASS(CcdrawDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CcdrawView));
  AddDocTemplate(pcdrawDocTemplate);

  CMultiDocTemplate* pFemmviewDocTemplate;
  pFemmviewDocTemplate = new CMultiDocTemplate(
      IDR_FEMMVIEWTYPE,
      RUNTIME_CLASS(CFemmviewDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CFemmviewView));
  AddDocTemplate(pFemmviewDocTemplate);

  CMultiDocTemplate* pBelaviewDocTemplate;
  pBelaviewDocTemplate = new CMultiDocTemplate(
      IDR_BELAVIEWTYPE,
      RUNTIME_CLASS(CbelaviewDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CbelaviewView));
  AddDocTemplate(pBelaviewDocTemplate);

  CMultiDocTemplate* phviewDocTemplate;
  phviewDocTemplate = new CMultiDocTemplate(
      IDR_HVIEWTYPE,
      RUNTIME_CLASS(ChviewDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(ChviewView));
  AddDocTemplate(phviewDocTemplate);

  CMultiDocTemplate* pcviewDocTemplate;
  pcviewDocTemplate = new CMultiDocTemplate(
      IDR_CVIEWTYPE,
      RUNTIME_CLASS(CcviewDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CcviewView));
  AddDocTemplate(pcviewDocTemplate);

  CMultiDocTemplate* pFemmplotDocTemplate;
  pFemmplotDocTemplate = new CMultiDocTemplate(
      IDR_FEMMPLOTTYPE,
      RUNTIME_CLASS(CFemmplotDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CFemmplotView));
  AddDocTemplate(pFemmplotDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;

  if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    return FALSE;
  m_pMainWnd = pMainFrame;

  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  // Enable DDE Execute open
  EnableShellOpen();
  RegisterShellFileTypes(TRUE);

#ifdef MATHLINK
  if (__argc > 1)
    if (strcmp("-mathlink", __argv[1]) == 0)
      bMathLink = TRUE;

  if (!bMathLink)
#endif
  {
    // Parse command line for standard shell commands, DDE, file open
    CMyCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // don't display annoying "new document" dialog automatically
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
      cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

      switch (d_defdoc) {
      case 1:
        OnFileNew();
        break;
      case 2:
        CreateNewDocument(0);
        break;
      case 3:
        CreateNewDocument(1);
        break;
      case 4:
        CreateNewDocument(2);
        break;
      case 5:
        CreateNewDocument(3);
        break;
      default:
        break;
      }

      // load up most recently used file if nothing else on tap
      //		if ((luascriptname.IsEmpty()) && (m_pRecentFileList->GetSize()>0))
      //			m_pMainWnd->PostMessage(WM_COMMAND, ID_FILE_MRU_FILE1);
    }

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
      return FALSE;
  }

  // Kludge to force femm to come up minimized in Wine when commanded to
  HKEY h_key_registry = NULL;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Wine"), 0, KEY_READ, &h_key_registry) == ERROR_SUCCESS)
    if (m_luaWindowStatus == SW_SHOWMINNOACTIVE)
      theApp.GetMainWnd()->ShowWindow(SW_SHOWNOACTIVATE);

  // lua extension: if we are running in quiet mode, hide window
  this->m_nCmdShow = m_luaWindowStatus;

  // The main window has been initialized, so show and update it.
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

#ifdef MATHLINK
  // If -mathlink has been specified on the command line, open the link;
  if (bMathLink) {
    mathenv = MLInitialize(0);
    alink = MLOpen(__argc, __argv);
    if (alink == NULL) {
      MsgBox("problem opening a MathLink link\n");
      bMathLink = FALSE;
    } else
      MLActivate(alink);
  }
#endif

  // Load up an external lua script that does canned initialization.
  // Users can modify this file to get their own custom initialization functionality.
  CString initlua = GetExecutablePath() + "init.lua";
  FILE* fp;
  if ((fp = fopen(initlua, "rt")) != NULL) {
    fclose(fp);
    if (int lua_error_code = lua_dofile(lua, initlua) != 0) {
      if (lua_error_code == LUA_ERRRUN)
        AfxMessageBox("Run Error in init.lua");
      if (lua_error_code == LUA_ERRMEM)
        AfxMessageBox("Lua memory Error in init.lua");
      if (lua_error_code == LUA_ERRERR)
        AfxMessageBox("User error error in init.lua");
      if (lua_error_code == LUA_ERRFILE)
        AfxMessageBox("File Error in init.lua");
    }
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog {
  public:
  CAboutDlg();

  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  CString m_compiledate;
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  //{{AFX_MSG(CAboutDlg)
  // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg()
    : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  m_compiledate = __DATE__;
//	m_compiledate = "11Apr2012";
//}}AFX_DATA_INIT
#ifdef MATHLINK
#ifdef _DEBUG
  m_compiledate += " (Mathematica Debug)";
#else
  m_compiledate += " (Mathematica Build)";
#endif
#endif

#ifdef _WIN64
  m_compiledate += " (x64)";
#endif
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  DDX_Text(pDX, IDC_COMPILEDATE, m_compiledate);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFemmApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFemmApp message handlers

int CFemmApp::ExitInstance()
{
  lua_close(lua);

#ifdef MATHLINK
  if (bMathLink) {
    MLClose(alink);
    MLDeinitialize(mathenv);
    bMathLink = FALSE;
  }
#endif

  return CWinApp::ExitInstance();
}

void CFemmApp::OnFileOpenLuaScript()
{
  static char BASED_CODE szFilter[] = "Lua Script Files (*.lua)|*.lua|";
  char ext[] = ".lua";
  CFileDialog MyDlg(TRUE, ext, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
  if (MyDlg.DoModal() == IDOK) {
    luascriptname = MyDlg.GetPathName();
    CString FolderPath = luascriptname.Left(MyDlg.m_ofn.nFileOffset);
    SetCurrentDirectory(FolderPath);
  }
}

BOOL CFemmApp::OnIdle(LONG lCount)
{
  static time_t timestamp = 0;
  static char inbuff[4096];

  // if a lua script was specified on the command line, run it.
  // Invoking the lua script here, rather than in InitInstance,
  // makes sure that everything is initialized correctly before
  // the lua script starts to run.
  if (luascriptname.GetLength() != 0) {
    if (m_luaWindowStatus == SW_SHOW)
      bLinehook = NormalLua;
    else
      bLinehook = HiddenLua;
    CString doscript = luascriptname;
    luascriptname.Empty();
    int lua_error_code;
    lua_byebye = FALSE;
    CStatusBar* StatBar = (CStatusBar*)((CMainFrame*)m_pMainWnd)->GetMessageBar();
    StatBar->SetPaneText(0, "EXECUTING LUASCRIPT -- HIT <ESC> TO ABORT", TRUE);
    if (lua_error_code = lua_dofile(lua, doscript) != 0) {
      //			if (lua_error_code==LUA_ERRRUN)
      //				AfxMessageBox("Run Error");
      if (lua_error_code == LUA_ERRMEM)
        AfxMessageBox("Lua memory Error");
      if (lua_error_code == LUA_ERRERR)
        AfxMessageBox("User error error");
      if (lua_error_code == LUA_ERRFILE)
        AfxMessageBox("File Error");
    }
    StatBar->SetPaneText(0, "Ready", TRUE);
    if (lua_byebye == TRUE) {
      ASSERT(AfxGetMainWnd() != NULL);
      AfxGetMainWnd()->PostMessage(WM_CLOSE);
    }
    bLinehook = FALSE;
  }

#ifdef MATHLINK

  // Sniff for any Mathlink message if Mathlink is connected
  if (bMathLink) {
    MLFlush(alink);
    if (MLReady(alink)) {
      char* mathstring;
      CString luacmd;
      if (MLGetString(alink, (const char**)&mathstring)) {
        luacmd = mathstring;
        MLDisownString(alink, mathstring);

        if (m_luaWindowStatus == SW_SHOW)
          bLinehook = NormalLua;
        else
          bLinehook = HiddenLua;
        int lua_error_code;
        lua_byebye = FALSE;
        CStatusBar* StatBar = (CStatusBar*)((CMainFrame*)m_pMainWnd)->GetMessageBar();
        //				StatBar->SetPaneText(0,"EXECUTING LUASCRIPT -- HIT <ESC> TO ABORT",TRUE);
        if (lua_error_code = lua_dostring(lua, luacmd) != 0) {
          //					if (lua_error_code==LUA_ERRRUN)
          //						AfxMessageBox("Run Error");
          if (lua_error_code == LUA_ERRMEM)
            AfxMessageBox("Lua memory Error");
          if (lua_error_code == LUA_ERRERR)
            AfxMessageBox("User error error");
          if (lua_error_code == LUA_ERRFILE)
            AfxMessageBox("File Error");
        }
        //				StatBar->SetPaneText(0,"Ready",TRUE);
        if (lua_byebye == TRUE) {
          ASSERT(AfxGetMainWnd() != NULL);
          AfxGetMainWnd()->PostMessage(WM_CLOSE);
        }
        bLinehook = FALSE;

        time(&timestamp);
      } else {
        MsgBox("MathLink::Closing connection");
        MLClose(alink);
        MLDeinitialize(mathenv);
        bMathLink = FALSE;
      }
    }

    // have to return something other than zero
    // so that the program will keep checking
    // to see if there is a new mathlink message;
    // A kludge is to call for a 1ms sleep.  Otherwise,
    // the program hogs all the CPU time checking for
    // new messages, slowing down the entire system.
    if (time(NULL) != timestamp)
      Sleep(1);
    CWinApp::OnIdle(0);
    return TRUE;
  }
#endif

  // Sniff for kludge IPC messages sent via temporary files
  if (bFileLink) {
    FILE* fp;
    char* s;

    if ((fp = fopen(IFile, "rt")) != NULL) {
      s = fgets(inbuff, 4096, fp);
      fclose(fp);

      if ((s != NULL) && (strlen(inbuff) != 0)) {
        DeleteFile(IFile);
        if (m_luaWindowStatus == SW_SHOW)
          bLinehook = NormalLua;
        else
          bLinehook = HiddenLua;
        int lua_error_code;
        lua_byebye = FALSE;
        CStatusBar* StatBar = (CStatusBar*)((CMainFrame*)m_pMainWnd)->GetMessageBar();
        //				StatBar->SetPaneText(0,"EXECUTING LUASCRIPT -- HIT <ESC> TO ABORT",TRUE);
        if (lua_error_code = lua_dostring(lua, inbuff) != 0) {
          //					if (lua_error_code==LUA_ERRRUN)
          //						AfxMessageBox("Run Error");
          if (lua_error_code == LUA_ERRMEM)
            AfxMessageBox("Lua memory Error");
          if (lua_error_code == LUA_ERRERR)
            AfxMessageBox("User error error");
          if (lua_error_code == LUA_ERRFILE)
            AfxMessageBox("File Error");
        }
        //				StatBar->SetPaneText(0,"Ready",TRUE);
        if (lua_byebye == TRUE) {
          ASSERT(AfxGetMainWnd() != NULL);
          AfxGetMainWnd()->PostMessage(WM_CLOSE);
        }
        bLinehook = FALSE;

        time(&timestamp);
      }
    }

    // have to return something other than zero
    // so that the program will keep checking
    // to see if there is a new mathlink message;
    // A kludge is to call for a 1ms sleep.  Otherwise,
    // the program hogs all the CPU time checking for
    // new messages, slowing down the entire system.
    if (time(NULL) != timestamp)
      Sleep(1);
    CWinApp::OnIdle(0);
    return TRUE;
  }

  return CWinApp::OnIdle(lCount);
}

void CFemmApp::line_hook(lua_State* L, lua_Debug* ar)
{
  MSG msg;

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if ((msg.message == WM_KEYDOWN) && (msg.wParam == VK_ESCAPE)) {
      bLinehook = FALSE;

      // If there is a running process (e.g. the solver), kill it
      if (hProc != NULL)
        TerminateProcess(hProc, NULL);
      hProc = NULL;

      // bust out of script
      lua_error(L, NULL);
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

int CFemmApp::lua_endapp(lua_State* L)
{
  lua_byebye = TRUE;
  return 0;
}

int CFemmApp::luaOpenDocument(lua_State* L)
{
  CString temp;

  int n = lua_gettop(L);
  if (n == 0)
    return 0;
  temp.Format("%s", lua_tostring(L, 1));
  if (n > 1) {
    switch ((int)lua_todouble(L, 2)) {
    case 1:
      theApp.luaShowWindow = SW_MINIMIZE;
      break;
    case 2:
      theApp.luaShowWindow = SW_RESTORE;
      break;
    case 3:
      theApp.luaShowWindow = SW_MAXIMIZE;
      break;
    default:
      theApp.luaShowWindow = NULL;
    }
  }
  theApp.OpenDocumentFile(temp);
  return 0;
}

int CFemmApp::lua_promptbox(lua_State* L)
{
  CPromptBox dlg;

  if (lua_gettop(L) > 0)
    dlg.mytitle = lua_tostring(L, 1);
  dlg.DoModal();
  lua_pushstring(L, dlg.instring);

  return TRUE;
}

int CFemmApp::lua_afxpause(lua_State* L)
{
  AfxMessageBox("LUA pause");

  return 0;
}

int CFemmApp::lua_messagebox(lua_State* L)
{

  CString errmsg;
  errmsg.Format("%s", lua_tostring(L, 1));

  AfxMessageBox(errmsg, MB_ICONINFORMATION);

  return 0;
}

int CFemmApp::lua_ERROR(lua_State* L)
{

  CString errmsg;
  errmsg.Format("%s", lua_tostring(L, 1));
  theApp.LuaErrmsg = errmsg;

  // Something went wrong in lua execution
  // Windows doesn't have stdout so lets afxmessagebox it
  if (theApp.bFileLink) {
    FILE* fp;
    fp = fopen(theApp.OFile, "wt");
    if (fp != NULL) {
      errmsg.Replace("error:", "");
      errmsg.Replace("\"", "\\\"");
      errmsg.Replace("\n", "\\n");
      fprintf(fp, "error(\"FEMM returns:\\n%s\\n\")", (const char*)errmsg);
      fclose(fp);
    }
  } else if (!theApp.bActiveX)
    AfxMessageBox(errmsg);

  return 0;
}

int CFemmApp::lua_newdocument(lua_State* L)
{
  if (lua_gettop(L) > 0)
    theApp.CreateNewDocument((int)lua_todouble(L, 1));
  return 0;
}

void CFemmApp::CreateNewDocument(int n)
{
  // First, catch a special case where the new document
  // is an XY plot and we want to open it in another window
  // all together
  if ((n == 7) && (d_sepplot)) {
    char CommandLine[MAX_PATH];
    sprintf(CommandLine, "%sfemmplot.exe", (const char*)((CFemmApp*)AfxGetApp())->GetExecutablePath());
    STARTUPINFO StartupInfo2 = { 0 };
    PROCESS_INFORMATION ProcessInfo2;
    StartupInfo2.cb = sizeof(STARTUPINFO);
    CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo2, &ProcessInfo2);
    CloseHandle(ProcessInfo2.hProcess);
    CloseHandle(ProcessInfo2.hThread);
    return;
  }

  POSITION tpos;
  CDocTemplate* pTemp;
  int k = 0;

  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (k = 0; k <= n; k++)
    if (tpos != NULL)
      pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  if (pTemp != NULL)
    pTemp->OpenDocumentFile(NULL);
}

BOOL CFemmApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;

  return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CString CFemmApp::GetExecutablePath()
{
  char szPath[MAX_PATH];
  char szDrive[64];
  char szDir[MAX_PATH];

  GetModuleFileName(NULL, szPath, MAX_PATH);
  _splitpath(szPath, szDrive, szDir, NULL, NULL);

  return ((CString)szDrive) + szDir;
}

int CFemmApp::HowManyDocs()
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  int k = 0;

  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  while (tpos != NULL) {
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
    dpos = pTemp->GetFirstDocPosition();
    while (dpos != NULL) {
      pTemp->GetNextDoc(dpos);
      k++;
    }
  }

  return k;
}

void CFemmApp::ForceViewPrefReload()
{
  // this is a kludge to get all of the views
  // to reload their color mappings from disk.

  POSITION tpos, dpos, pos;
  CDocTemplate* pTemp;

  tpos = AfxGetApp()->GetFirstDocTemplatePosition();

  // femme
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    CFemmeDoc* pDoc;
    CFemmeView* pView;
    pDoc = (CFemmeDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (CFemmeView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // beladraw
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    CbeladrawDoc* pDoc;
    CbeladrawView* pView;
    pDoc = (CbeladrawDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (CbeladrawView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // hdraw
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    ChdrawDoc* pDoc;
    ChdrawView* pView;
    pDoc = (ChdrawDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (ChdrawView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // cdraw
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    CcdrawDoc* pDoc;
    CcdrawView* pView;
    pDoc = (CcdrawDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (CcdrawView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // femmview
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    CFemmviewDoc* pDoc;
    CFemmviewView* pView;
    pDoc = (CFemmviewDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (CFemmviewView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // belaview
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    CbelaviewDoc* pDoc;
    CbelaviewView* pView;
    pDoc = (CbelaviewDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (CbelaviewView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // hview
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    ChviewDoc* pDoc;
    ChviewView* pView;
    pDoc = (ChviewDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (ChviewView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // cview
  pTemp = AfxGetApp()->GetNextDocTemplate(tpos);
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    CcviewDoc* pDoc;
    CcviewView* pView;
    pDoc = (CcviewDoc*)pTemp->GetNextDoc(dpos);
    pos = pDoc->GetFirstViewPosition();
    pView = (CcviewView*)pDoc->GetNextView(pos);
    pView->ScanPreferences();
    pDoc->ScanPreferences();
    pView->InvalidateRect(NULL);
  }

  // general prefs
  ScanPrefs();
  bShowOutputWindow = d_bShowOutputWindow;
}

void CFemmApp::OnFileNew()
{
  CNewDocDlg dlg;

  if (dlg.DoModal() == IDOK)
    CreateNewDocument(dlg.doctype);
}

char* StripKey(char* c);

void CFemmApp::ScanPrefs()
{
  FILE* fp;
  CString fname = ((CFemmApp*)AfxGetApp())->GetExecutablePath() + "femm.cfg";

  fp = fopen(fname, "rt");
  if (fp != NULL) {
    BOOL flag = FALSE;
    char s[1024];
    char q[1024];
    char* v;

    // parse the file
    while (fgets(s, 1024, fp) != NULL) {
      sscanf(s, "%s", q);

      if (_strnicmp(q, "<ShowConsole>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_luaconsole);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<SeparatePlots>", 15) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_sepplot);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowOutputWindow>", 18) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_bShowOutputWindow);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<SmartMesh>", 18) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_SmartMesh);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<DefaultType>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_defdoc);
        q[0] = NULL;
      }
    }
    fclose(fp);
  }
}

int CFemmApp::lua_showpointprops(lua_State* L)
{
  CMainFrame* pfrm;
  pfrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
  pfrm->m_dlgBar.ShowWindow(SW_SHOWNOACTIVATE);

  return 0;
}

int CFemmApp::lua_hidepointprops(lua_State* L)
{
  CMainFrame* pfrm;
  pfrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
  pfrm->m_dlgBar.ShowWindow(SW_HIDE);

  return 0;
}

// override of LoadStdProfileSettings so that I can get
// the suffix in the MRU files list.

AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
AFX_STATIC_DATA const TCHAR _afxPreviewSection[] = _T("Settings");
AFX_STATIC_DATA const TCHAR _afxPreviewEntry[] = _T("PreviewPages");

void CFemmApp::LoadStdProfileSettings(UINT nMaxMRU)
{
  ASSERT_VALID(this);
  ASSERT(m_pRecentFileList == NULL);

  if (nMaxMRU != 0) {
    // create file MRU since nMaxMRU not zero
    // using MyCRecentFileList override
    m_pRecentFileList = new MyCRecentFileList(0, _afxFileSection, _afxFileEntry, nMaxMRU);
    m_pRecentFileList->ReadList();
  }
  // 0 by default means not set
  m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
}

#ifdef MATHLINK

int CFemmApp::lua_to_mathematica(lua_State* L)
{
  if (((CFemmApp*)AfxGetApp())->bMathLink == FALSE)
    return 0;

  int n = lua_gettop(L);
  MLPutFunction(alink, "List", 1);

  for (int k = 1; k <= n; k++) {
    MLPutFunction(alink, "Sequence", 2);
    if (lua_type(L, k) == LUA_TNUMBER) {
      CComplex z = lua_tonumber(L, k);
      if (Im(z) == 0)
        MLPutReal(alink, Re(z));
      else {
        MLPutFunction(alink, "Complex", 2);
        MLPutReal(alink, Re(z));
        MLPutReal(alink, Im(z));
      }
    } else {
      CString s = lua_tostring(L, k);
      MLPutString(alink, s);
    }
  }
  MLPutFunction(alink, "Sequence", 0);
  MLEndPacket(alink);

  return 0;
}

int CFemmApp::lua_open_mathlink(lua_State* L)
{
  if (((CFemmApp*)AfxGetApp())->bMathLink == FALSE) {
    mathenv = MLInitialize(0);
    alink = MLOpen(__argc, __argv);
    if (alink == NULL)
      MsgBox("problem opening a MathLink link\n");
    else
      ((CFemmApp*)AfxGetApp())->bMathLink = TRUE;
  }

  return 0;
}

int CFemmApp::lua_close_mathlink(lua_State* L)
{
  if (((CFemmApp*)AfxGetApp())->bMathLink == TRUE) {
    MLClose(alink);
    MLDeinitialize(mathenv);
    ((CFemmApp*)AfxGetApp())->bMathLink = FALSE;
  }

  return 0;
}

#endif

BOOL CFemmApp::MOCheckIfAlreadyOpen(CString p)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  CFemmviewDoc* pDoc;
  CString DocTitle, ThisTitle, ThisTitleAlt;

  DocTitle = p;
  DocTitle.MakeLower();

  // get pointer to document template for FemmviewDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 5; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (CFemmviewDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetPathName();
    ThisTitle.MakeLower();
    ThisTitleAlt = "\"" + ThisTitle + "\"";
    if ((ThisTitle == DocTitle) || (ThisTitleAlt == DocTitle)) {
      lua_pushstring(lua, pDoc->GetTitle());
      pDoc->lua_switchfocus(lua);
      pDoc->lua_reload((lua_State*)NULL);
      //			Alternate idea here is:
      //			pDoc->OnReload();
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CFemmApp::EOCheckIfAlreadyOpen(CString p)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  CbelaviewDoc* pDoc;
  CString DocTitle, ThisTitle, ThisTitleAlt;

  DocTitle = p;
  DocTitle.MakeLower();

  // get pointer to document template for FemmviewDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 6; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (CbelaviewDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetPathName();
    ThisTitle.MakeLower();
    ThisTitleAlt = "\"" + ThisTitle + "\"";
    if ((ThisTitle == DocTitle) || (ThisTitleAlt == DocTitle)) {
      lua_pushstring(lua, pDoc->GetTitle());
      pDoc->lua_switchfocus(lua);
      pDoc->lua_reload((lua_State*)NULL);
      //			Alternate idea here is:
      //			pDoc->OnReload();
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CFemmApp::HOCheckIfAlreadyOpen(CString p)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  ChviewDoc* pDoc;
  CString DocTitle, ThisTitle, ThisTitleAlt;

  DocTitle = p;
  DocTitle.MakeLower();

  // get pointer to document template for FemmviewDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 7; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (ChviewDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetPathName();
    ThisTitle.MakeLower();
    ThisTitleAlt = "\"" + ThisTitle + "\"";
    if ((ThisTitle == DocTitle) || (ThisTitleAlt == DocTitle)) {
      lua_pushstring(lua, pDoc->GetTitle());
      pDoc->lua_switchfocus(lua);
      pDoc->lua_reload((lua_State*)NULL);
      //			Alternate idea here is:
      //			pDoc->OnReload();
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CFemmApp::COCheckIfAlreadyOpen(CString p)
{
  POSITION tpos, dpos;
  CDocTemplate* pTemp;
  CcviewDoc* pDoc;
  CString DocTitle, ThisTitle, ThisTitleAlt;

  DocTitle = p;
  DocTitle.MakeLower();

  // get pointer to document template for FemmviewDoc
  tpos = AfxGetApp()->GetFirstDocTemplatePosition();
  for (int i = 0; i < 8; i++)
    pTemp = AfxGetApp()->GetNextDocTemplate(tpos);

  // leaf through documents to get a pointer to the
  // document with the right title
  dpos = pTemp->GetFirstDocPosition();
  while (dpos != NULL) {
    pDoc = (CcviewDoc*)pTemp->GetNextDoc(dpos);
    ThisTitle = pDoc->GetPathName();
    ThisTitle.MakeLower();
    ThisTitleAlt = "\"" + ThisTitle + "\"";
    if ((ThisTitle == DocTitle) || (ThisTitleAlt == DocTitle)) {
      lua_pushstring(lua, pDoc->GetTitle());
      pDoc->lua_switchfocus(lua);
      pDoc->lua_reload((lua_State*)NULL);
      //			Alternate idea here is:
      //			pDoc->OnReload();
      return TRUE;
    }
  }
  return FALSE;
}

int CFemmApp::lua_to_filelink(lua_State* L)
{
  FILE* fp;
  CString LuaResult, s;

  int n = lua_gettop(L);

  do {
    fp = fopen(((CFemmApp*)AfxGetApp())->OFile, "wt");
  } while (fp == NULL);

  if (n == 0)
    LuaResult = "[]\n";
  else {
    LuaResult = "[";
    for (int k = 1; k <= n; k++) {
      s = lua_tostring(L, k);
      if (k != n)
        LuaResult = LuaResult + s + ",";
      else
        LuaResult = LuaResult + s + "]\n";
    }
  }
  fprintf(fp, "%s", (const char*)LuaResult);
  fclose(fp);

  return 0;
}

void CFemmApp::OnFileOpen()
{
  CWinApp::OnFileOpen(); // Use just this line to recoup the default behavior;
}

int CFemmApp::lua_compatibilitymode(lua_State* L)
{
  if (lua_gettop(L) != 0)
    ((CFemmApp*)AfxGetApp())->CompatibilityMode = (int)lua_todouble(L, 1);

  return 0;
}

int CFemmApp::lua_setcurrentdirectory(lua_State* L)
{
  if (lua_gettop(L) != 0) {
    CString temp;
    temp.Format("%s", lua_tostring(L, 1));
    SetCurrentDirectory(temp);
  }

  return 0;
}

int CFemmApp::lua_smartmesh(lua_State* L)
{
  // crowbar smartmesh off for entire program for the entire session
  if (lua_gettop(L) != 0)
    ((CFemmApp*)AfxGetApp())->session_SmartMesh = (int)lua_todouble(L, 1);

  return 0;
}

int CFemmApp::lua_Complex(lua_State* L)
{
  int n;

  CComplex y;

  n = lua_gettop(L);
  if (n == 2)
    y = lua_tonumber(L, 1) + I * lua_tonumber(L, 2);
  else if (n == 1)
    y = lua_tonumber(L, 1);
  else
    y = 0;

  lua_pushnumber(L, y);

  return 1;
}

int CFemmApp::lua_makeplot(lua_State* L)
{
  CXYPlot xyplot;
  int k;
  int nTables = 0;
  int nStrings = 0;
  int n = lua_gettop(L);

  for (k = 1; k <= n; k++) {
    if (lua_istable(L, k)) {
      nTables++;
    }
    if (lua_isstring(L, k)) {
      nStrings++;
    }
  }

  MsgBox("Tables = %i\nStrings = %i", nTables, nStrings);

  return 0;
}