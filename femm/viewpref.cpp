// Pref.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "ViewPref.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SelColor clist[0]
#define BackColor clist[1]
#define MeshColor clist[2]
#define BlockColor clist[3]
#define LineColor clist[4]
#define GridColor clist[5]
#define NodeColor clist[6]
#define RegionColor clist[7]
#define TextColor clist[8]
#define RealFluxLineColor clist[9]
#define ImagFluxLineColor clist[10]
#define MaskLineColor clist[11]
#define NameColor clist[12]
#define RealVectorColor clist[13]
#define ImagVectorColor clist[14]
#define Color00 clist[15]
#define Color01 clist[16]
#define Color02 clist[17]
#define Color03 clist[18]
#define Color04 clist[19]
#define Color05 clist[20]
#define Color06 clist[21]
#define Color07 clist[22]
#define Color08 clist[23]
#define Color09 clist[24]
#define Color10 clist[25]
#define Color11 clist[26]
#define Color12 clist[27]
#define Color13 clist[28]
#define Color14 clist[29]
#define Color15 clist[30]
#define Color16 clist[31]
#define Color17 clist[32]
#define Color18 clist[33]
#define Color19 clist[34]
#define Grey00 clist[35]
#define Grey01 clist[36]
#define Grey02 clist[37]
#define Grey03 clist[38]
#define Grey04 clist[39]
#define Grey05 clist[40]
#define Grey06 clist[41]
#define Grey07 clist[42]
#define Grey08 clist[43]
#define Grey09 clist[44]
#define Grey10 clist[45]
#define Grey11 clist[46]
#define Grey12 clist[47]
#define Grey13 clist[48]
#define Grey14 clist[49]
#define Grey15 clist[50]
#define Grey16 clist[51]
#define Grey17 clist[52]
#define Grey18 clist[53]
#define Grey19 clist[54]

/////////////////////////////////////////////////////////////////////////////
// CViewPref dialog

CViewPref::CViewPref(CWnd* pParent /*=NULL*/)
    : CDialog(CViewPref::IDD, pParent)
{
  //{{AFX_DATA_INIT(CViewPref)
  m_d_GreyContours = FALSE;
  m_d_LegendFlag = TRUE;
  m_d_NumContours = 19;
  m_d_ResetOnReload = FALSE;
  m_d_GridFlag = FALSE;
  m_d_ShowAi = FALSE;
  m_d_ShowAr = TRUE;
  m_d_PtsFlag = TRUE;
  m_d_MeshFlag = FALSE;
  m_d_SnapFlag = FALSE;
  m_d_Smooth = TRUE;
  m_d_LineIntegralPoints = 400;
  m_d_ShiftH = TRUE;
  m_d_PlotPoints = 1500;
  m_d_ShowMask = FALSE;
  m_d_shownames = FALSE;
  //}}AFX_DATA_INIT

  m_d_DensityPlot = 0;
  m_d_EditAction = 0;
  m_d_VectorPlot = 0;

  clist = (COLORREF*)calloc(64, sizeof(COLORREF));
  int i = 0;
  clist[i++] = dSelColor;
  clist[i++] = dBackColor;
  clist[i++] = dMeshColor;
  clist[i++] = dBlockColor;
  clist[i++] = dLineColor;
  clist[i++] = dGridColor;
  clist[i++] = dNodeColor;
  clist[i++] = dRegionColor;
  clist[i++] = dTextColor;
  clist[i++] = dRealFluxLineColor;
  clist[i++] = dImagFluxLineColor;
  clist[i++] = dMaskLineColor;
  clist[i++] = dNameColor;
  clist[i++] = dRealVectorColor;
  clist[i++] = dImagVectorColor;
  clist[i++] = dColor00;
  clist[i++] = dColor01;
  clist[i++] = dColor02;
  clist[i++] = dColor03;
  clist[i++] = dColor04;
  clist[i++] = dColor05;
  clist[i++] = dColor06;
  clist[i++] = dColor07;
  clist[i++] = dColor08;
  clist[i++] = dColor09;
  clist[i++] = dColor10;
  clist[i++] = dColor11;
  clist[i++] = dColor12;
  clist[i++] = dColor13;
  clist[i++] = dColor14;
  clist[i++] = dColor15;
  clist[i++] = dColor16;
  clist[i++] = dColor17;
  clist[i++] = dColor18;
  clist[i++] = dColor19;
  clist[i++] = dGrey00;
  clist[i++] = dGrey01;
  clist[i++] = dGrey02;
  clist[i++] = dGrey03;
  clist[i++] = dGrey04;
  clist[i++] = dGrey05;
  clist[i++] = dGrey06;
  clist[i++] = dGrey07;
  clist[i++] = dGrey08;
  clist[i++] = dGrey09;
  clist[i++] = dGrey10;
  clist[i++] = dGrey11;
  clist[i++] = dGrey12;
  clist[i++] = dGrey13;
  clist[i++] = dGrey14;
  clist[i++] = dGrey15;
  clist[i++] = dGrey16;
  clist[i++] = dGrey17;
  clist[i++] = dGrey18;
  clist[i++] = dGrey19;
}

CViewPref::~CViewPref()
{
  free(clist);
}

void CViewPref::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CViewPref)
  DDX_Control(pDX, IDC_VECTORPLOT, m_d_vplotlist);
  DDX_Control(pDX, IDC_WST, m_d_wstlist);
  DDX_Control(pDX, IDC_DENSITYPLOT, m_d_dplotlist);
  DDX_Control(pDX, IDC_EDITACTION, m_d_editlist);
  DDX_Control(pDX, IDC_COLORS, m_d_color);
  DDX_Check(pDX, IDC_GREY, m_d_GreyContours);
  DDX_Check(pDX, IDC_LEGEND, m_d_LegendFlag);
  DDX_Text(pDX, IDC_NCONT, m_d_NumContours);
  DDV_MinMaxInt(pDX, m_d_NumContours, 4, 999);
  DDX_Check(pDX, IDC_ONRELOAD, m_d_ResetOnReload);
  DDX_Check(pDX, IDC_SHOWGRID, m_d_GridFlag);
  DDX_Check(pDX, IDC_SHOWIMAGINARY, m_d_ShowAi);
  DDX_Check(pDX, IDC_SHOWREAL, m_d_ShowAr);
  DDX_Check(pDX, IDC_SHOWPTS, m_d_PtsFlag);
  DDX_Check(pDX, IDC_SHOWMESH, m_d_MeshFlag);
  DDX_Check(pDX, IDC_SNAPGRID, m_d_SnapFlag);
  DDX_Check(pDX, IDC_SMOOTHING, m_d_Smooth);
  DDX_Check(pDX, IDC_SHIFTH, m_d_ShiftH);
  DDX_Text(pDX, IDC_LIPOINTS, m_d_LineIntegralPoints);
  DDV_MinMaxInt(pDX, m_d_LineIntegralPoints, 10, 100000);
  DDX_Text(pDX, IDC_PLOTPOINTS, m_d_PlotPoints);
  DDV_MinMaxInt(pDX, m_d_PlotPoints, 10, 100000);
  DDX_Check(pDX, IDC_SHOWMASK, m_d_ShowMask);
  DDX_Check(pDX, IDC_SHOWNAMEZ, m_d_shownames);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_NCONT, m_IDC_d_NumContours);
  DDX_Control(pDX, IDC_LIPOINTS, m_IDC_d_LineIntegralPoints);
  DDX_Control(pDX, IDC_PLOTPOINTS, m_IDC_d_PlotPoints);
}

BEGIN_MESSAGE_MAP(CViewPref, CDialog)
//{{AFX_MSG_MAP(CViewPref)
ON_BN_CLICKED(IDC_FV_MODBTN, OnModBtn)
ON_BN_CLICKED(IDC_FV_RESET, OnResetColors)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPref message handlers

void CViewPref::OnModBtn()
{
  CColorDialog dlg;
  COLORREF cclist[16];
  int i, k;

  UpdateData();
  i = m_d_color.GetCurSel();
  for (k = 0; k < 16; k++)
    cclist[k] = RGB(190, 190, 190);

  if (i < 11)
    for (k = 0; k < 11; k++)
      cclist[k] = clist[k];
  if ((i > 10) && (i < 23))
    for (k = 11; k < 23; k++)
      cclist[k - 11] = clist[k];
  if (i > 22)
    for (k = 23; k < 35; k++)
      cclist[k - 23] = clist[k];

  dlg.m_cc.lpCustColors = cclist;
  dlg.m_cc.rgbResult = clist[i];
  dlg.m_cc.Flags = dlg.m_cc.Flags | CC_FULLOPEN | CC_RGBINIT;

  if (dlg.DoModal() == IDOK) {
    clist[i] = dlg.GetColor();
  }
}

BOOL CViewPref::OnInitDialog()
{
  CDialog::OnInitDialog();

  ScanPrefs();

  m_d_color.SetCurSel(0);
  m_d_dplotlist.SetCurSel(m_d_DensityPlot);
  m_d_editlist.SetCurSel(m_d_EditAction);
  m_d_vplotlist.SetCurSel(m_d_VectorPlot);
  m_d_wstlist.SetCurSel(m_d_WeightingScheme);

  UpdateData(FALSE);

  return TRUE;
}

void CViewPref::OnResetColors()
{
  if (AfxMessageBox("Reset all color preferences?", MB_OKCANCEL) == IDOK) {
    int i = 0;
    clist[i++] = dSelColor;
    clist[i++] = dBackColor;
    clist[i++] = dMeshColor;
    clist[i++] = dBlockColor;
    clist[i++] = dLineColor;
    clist[i++] = dGridColor;
    clist[i++] = dNodeColor;
    clist[i++] = dRegionColor;
    clist[i++] = dTextColor;
    clist[i++] = dRealFluxLineColor;
    clist[i++] = dImagFluxLineColor;
    clist[i++] = dMaskLineColor;
    clist[i++] = dNameColor;
    clist[i++] = dRealVectorColor;
    clist[i++] = dImagVectorColor;
    clist[i++] = dColor00;
    clist[i++] = dColor01;
    clist[i++] = dColor02;
    clist[i++] = dColor03;
    clist[i++] = dColor04;
    clist[i++] = dColor05;
    clist[i++] = dColor06;
    clist[i++] = dColor07;
    clist[i++] = dColor08;
    clist[i++] = dColor09;
    clist[i++] = dColor10;
    clist[i++] = dColor11;
    clist[i++] = dColor12;
    clist[i++] = dColor13;
    clist[i++] = dColor14;
    clist[i++] = dColor15;
    clist[i++] = dColor16;
    clist[i++] = dColor17;
    clist[i++] = dColor18;
    clist[i++] = dColor19;
    clist[i++] = dGrey00;
    clist[i++] = dGrey01;
    clist[i++] = dGrey02;
    clist[i++] = dGrey03;
    clist[i++] = dGrey04;
    clist[i++] = dGrey05;
    clist[i++] = dGrey06;
    clist[i++] = dGrey07;
    clist[i++] = dGrey08;
    clist[i++] = dGrey09;
    clist[i++] = dGrey10;
    clist[i++] = dGrey11;
    clist[i++] = dGrey12;
    clist[i++] = dGrey13;
    clist[i++] = dGrey14;
    clist[i++] = dGrey15;
    clist[i++] = dGrey16;
    clist[i++] = dGrey17;
    clist[i++] = dGrey18;
    clist[i++] = dGrey19;
  }
}

BOOL CViewPref::PreTranslateMessage(MSG* pMsg)
{
  // Pressing ENTER should reroute message to parent
  if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
    GetParent()->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
    // Message needs no further processing
    return TRUE;
  }

  // Pressing ESC should reroute message to parent
  if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE)) {
    GetParent()->PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
    // Message needs no further processing
    return TRUE;
  }

  // Allow default handler otherwise
  return CDialog::PreTranslateMessage(pMsg);
}

char* StripKey(char* c);

void CViewPref::ScanPrefs()
{
  FILE* fp;
  CString fname = ((CFemmApp*)AfxGetApp())->GetExecutablePath() + "femmview.cfg";

  fp = fopen(fname, "rt");
  if (fp != NULL) {
    BOOL flag = FALSE;
    char s[1024];
    char q[1024];
    char* v;
    int cr, cg, cb;

    // parse the file
    while (fgets(s, 1024, fp) != NULL) {
      sscanf(s, "%s", q);

      if (_strnicmp(q, "<LineIntegralPoints>", 20) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_LineIntegralPoints);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShiftH>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_ShiftH);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<SelColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        SelColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MeshColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        MeshColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<BlockColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        BlockColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<NodeColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        NodeColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<NameColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        NameColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<BkgndColor>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        BackColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<TextColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        TextColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<FluxRColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RealFluxLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<FluxIColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        ImagFluxLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MaskColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        MaskLineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorRColor>", 14) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RealVectorColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorIColor>", 14) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        ImagVectorColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<LineColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        LineColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<RegionColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        RegionColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GridColor>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        GridColor = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color00>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color00 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color01>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color01 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color02>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color02 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color03>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color03 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color04>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color04 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color05>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color05 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color06>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color06 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color07>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color07 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color08>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color08 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color09>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color09 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color10>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color10 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color11>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color11 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color12>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color12 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color13>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color13 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color14>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color14 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color15>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color15 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color16>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color16 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color17>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color17 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color18>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color18 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Color19>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Color19 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey00>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey00 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey01>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey01 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey02>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey02 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey03>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey03 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey04>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey04 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey05>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey05 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey06>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey06 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey07>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey07 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey08>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey08 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey09>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey09 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey10>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey10 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey11>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey11 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey12>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey12 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey13>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey13 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey14>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey14 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey15>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey15 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey16>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey16 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey17>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey17 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey18>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey18 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Grey19>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i	%i	%i", &cr, &cg, &cb);
        Grey19 = RGB(cr, cg, cb);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<EditAction>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_EditAction);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<DensityPlot>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_DensityPlot);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<VectorPlot>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_VectorPlot);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GridFlag>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_GridFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<SnapFlag>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_SnapFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MeshFlag>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_MeshFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<LegendFlag>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_LegendFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<NumContours>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_NumContours);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowAr>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_ShowAr);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowAi>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_ShowAi);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShowMask>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_ShowMask);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<GreyContours>", 14) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_GreyContours);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<PtsFlag>", 9) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_PtsFlag);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Smooth>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_Smooth);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<PlotPoints>", 12) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_PlotPoints);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<WeightingScheme>", 17) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_WeightingScheme);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ResetOnReload>", 15) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &m_d_ResetOnReload);
        q[0] = NULL;
      }
    }
    fclose(fp);
  }
}

void WriteColor(char* cname, COLORREF c, FILE* fp);

void CViewPref::WritePrefs()
{
  UpdateData();
  m_d_DensityPlot = m_d_dplotlist.GetCurSel();
  m_d_EditAction = m_d_editlist.GetCurSel();
  m_d_VectorPlot = m_d_vplotlist.GetCurSel();
  m_d_WeightingScheme = m_d_wstlist.GetCurSel();

  FILE* fp;
  CString fname = ((CFemmApp*)AfxGetApp())->GetExecutablePath() + "femmview.cfg";

  fp = fopen(fname, "wt");
  if (fp != NULL) {
    WriteColor("SelColor", SelColor, fp);
    WriteColor("BkgndColor", BackColor, fp);
    WriteColor("MeshColor", MeshColor, fp);
    WriteColor("BlockColor", BlockColor, fp);
    WriteColor("LineColor", LineColor, fp);
    WriteColor("GridColor", GridColor, fp);
    WriteColor("NodeColor", NodeColor, fp);
    WriteColor("RegionColor", RegionColor, fp);
    WriteColor("TextColor", TextColor, fp);
    WriteColor("FluxRColor", RealFluxLineColor, fp);
    WriteColor("FluxIColor", ImagFluxLineColor, fp);
    WriteColor("MaskColor", MaskLineColor, fp);
    WriteColor("VectorRColor", RealVectorColor, fp);
    WriteColor("VectorIColor", ImagVectorColor, fp);
    WriteColor("NameColor", NameColor, fp);
    WriteColor("Color00", Color00, fp);
    WriteColor("Color01", Color01, fp);
    WriteColor("Color02", Color02, fp);
    WriteColor("Color03", Color03, fp);
    WriteColor("Color04", Color04, fp);
    WriteColor("Color05", Color05, fp);
    WriteColor("Color06", Color06, fp);
    WriteColor("Color07", Color07, fp);
    WriteColor("Color08", Color08, fp);
    WriteColor("Color09", Color09, fp);
    WriteColor("Color10", Color10, fp);
    WriteColor("Color11", Color11, fp);
    WriteColor("Color12", Color12, fp);
    WriteColor("Color13", Color13, fp);
    WriteColor("Color14", Color14, fp);
    WriteColor("Color15", Color15, fp);
    WriteColor("Color16", Color16, fp);
    WriteColor("Color17", Color17, fp);
    WriteColor("Color18", Color18, fp);
    WriteColor("Color19", Color19, fp);
    WriteColor("Grey00", Grey00, fp);
    WriteColor("Grey01", Grey01, fp);
    WriteColor("Grey02", Grey02, fp);
    WriteColor("Grey03", Grey03, fp);
    WriteColor("Grey04", Grey04, fp);
    WriteColor("Grey05", Grey05, fp);
    WriteColor("Grey06", Grey06, fp);
    WriteColor("Grey07", Grey07, fp);
    WriteColor("Grey08", Grey08, fp);
    WriteColor("Grey09", Grey09, fp);
    WriteColor("Grey10", Grey10, fp);
    WriteColor("Grey11", Grey11, fp);
    WriteColor("Grey12", Grey12, fp);
    WriteColor("Grey13", Grey13, fp);
    WriteColor("Grey14", Grey14, fp);
    WriteColor("Grey15", Grey15, fp);
    WriteColor("Grey16", Grey16, fp);
    WriteColor("Grey17", Grey17, fp);
    WriteColor("Grey18", Grey18, fp);
    WriteColor("Grey19", Grey19, fp);

    fprintf(fp, "<EditAction> = %i\n", m_d_EditAction);
    fprintf(fp, "<DensityPlot> = %i\n", m_d_DensityPlot);
    fprintf(fp, "<VectorPlot> = %i\n", m_d_VectorPlot);
    fprintf(fp, "<GridFlag> = %i\n", m_d_GridFlag);
    fprintf(fp, "<SnapFlag> = %i\n", m_d_SnapFlag);
    fprintf(fp, "<MeshFlag> = %i\n", m_d_MeshFlag);
    fprintf(fp, "<LegendFlag> = %i\n", m_d_LegendFlag);
    fprintf(fp, "<NumContours> = %i\n", m_d_NumContours);
    fprintf(fp, "<ShowAr> = %i\n", m_d_ShowAr);
    fprintf(fp, "<ShowAi> = %i\n", m_d_ShowAi);
    fprintf(fp, "<ShowMask> = %i\n", m_d_ShowMask);
    fprintf(fp, "<GreyContours> = %i\n", m_d_GreyContours);
    fprintf(fp, "<PtsFlag> = %i\n", m_d_PtsFlag);
    fprintf(fp, "<ResetOnReload> = %i\n", m_d_ResetOnReload);
    fprintf(fp, "<Smooth> = %i\n", m_d_Smooth);
    fprintf(fp, "<PlotPoints> = %i\n", m_d_PlotPoints);
    fprintf(fp, "<LineIntegralPoints> = %i\n", m_d_LineIntegralPoints);
    fprintf(fp, "<ShiftH> = %i\n", m_d_ShiftH);
    fprintf(fp, "<WeightingScheme> = %i\n", m_d_WeightingScheme);

    fclose(fp);
  }
}
