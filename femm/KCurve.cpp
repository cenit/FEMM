// TKData.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "KCurve.h"
#include "BHPlot.h"
#include "TKDatafile.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTKData dialog

CTKData::CTKData(CWnd* pParent /*=NULL*/)
    : CDialog(CTKData::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTKData)
  m_Bdata = _T("");
  m_Hdata = _T("");
  //}}AFX_DATA_INIT

  logplot = FALSE;
}

void CTKData::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTKData)
  DDX_Text(pDX, IDC_BDATA, m_Bdata);
  DDX_Text(pDX, IDC_HDATA, m_Hdata);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BDATA, m_IDC_BDATA);
  DDX_Control(pDX, IDC_HDATA, m_IDC_HDATA);
}

BEGIN_MESSAGE_MAP(CTKData, CDialog)
//{{AFX_MSG_MAP(CTKData)
ON_BN_CLICKED(IDC_PLOT_BHCURVE, OnPlotBHcurve)
ON_BN_CLICKED(IDC_LOGPLOT_BHCURVE, OnLogPlotBHcurve)
ON_BN_CLICKED(IDC_READ_BHCURVE, OnReadBhcurve)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTKData message handlers

void CTKData::StripTKData()
{
  int k;
  char *buff, *nptr, *endptr;
  double z;

  T.RemoveAll();
  K.RemoveAll();
  npts = 0;

  if ((m_Bdata.GetLength() == 0) || (m_Hdata.GetLength() == 0))
    return;

  k = m_Bdata.GetLength() * 2;
  buff = (char*)calloc(k, sizeof(char));
  strcpy(buff, m_Bdata);
  nptr = buff;
  while (sscanf(nptr, "%lf", &z) != EOF) {
    z = strtod(nptr, &endptr);
    if (nptr == endptr)
      nptr++; // catch special case
    else
      nptr = endptr;
    T.Add(z);
  }
  free(buff);

  k = m_Hdata.GetLength() * 2;
  buff = (char*)calloc(k, sizeof(char));
  strcpy(buff, m_Hdata);
  nptr = buff;
  while (sscanf(nptr, "%lf", &z) != EOF) {
    z = strtod(nptr, &endptr);
    if (nptr == endptr)
      nptr++;
    else
      nptr = endptr;
    K.Add(z);
  }

  npts = (int)T.GetSize();
  if ((int)K.GetSize() < npts)
    npts = (int)K.GetSize();

  SortCurve();

  free(buff);
}

void CTKData::OnLogPlotBHcurve()
{
  logplot = TRUE;
  OnPlotBHcurve();
}

void CTKData::SortCurve()
{
  int j;
  double x;
  BOOL bDone = FALSE;

  if (npts < 2)
    return;

  while (bDone == FALSE) {
    for (j = 1, bDone = TRUE; j < npts; j++) {
      if (T[j] < T[j - 1]) {
        bDone = FALSE;
        x = T[j];
        T[j] = T[j - 1];
        T[j - 1] = x;
        x = K[j];
        K[j] = K[j - 1];
        K[j - 1] = x;
      }
    }
  }
}

double CTKData::GetK(double t)
{
  int i, j;

  if (npts == 0)
    return 0;
  if (npts == 1)
    return K[0];
  if (t <= T[0])
    return K[0];
  if (t >= T[npts - 1])
    return K[npts - 1];

  for (i = 0, j = 1; j < npts; i++, j++) {
    if ((t >= T[i]) && (t <= T[j])) {
      return (K[i] + (K[j] - K[i]) * (t - T[i]) / (T[j] - T[i]));
    }
  }

  return 0;
}

void CTKData::OnPlotBHcurve()
{
  CBHPlot xyplot;
  int i;
  double t, dt;
  BOOL logscale = logplot;
  logplot = FALSE;

  UpdateData();
  StripTKData();

  if (npts < 2) {
    MsgBox("Must have at least 2 pairs of data points");
    return;
  }

  // copy raw B-H data for plotting in comparison to interpolated curve
  xyplot.NumPts = npts;
  xyplot.Pts = (CComplex*)calloc(npts, sizeof(CComplex));
  for (i = 0; i < npts; i++)
    xyplot.Pts[i] = T[i] + I * K[i];

  // get path to femmplot that we will need to display
  // the plot of the B-H curve...
  CString comline = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  // Actually evaluate all the points on the line...
  if (xyplot.Create(101, 2) == FALSE)
    return;

  dt = (T[npts - 1] - T[0]) / 100.;

  for (i = 0, t = T[0]; i <= 100; i++, t += dt) {
    xyplot.M[i][0] = t;
    xyplot.M[i][1] = GetK(t);
  }

  sprintf(xyplot.lbls[0], "T, K");
  sprintf(xyplot.lbls[1], "k, W/(m*K)");

  // Create the plot, send it to the clipboard, spawn viewer...
  CMetaFileDC Meta;
  Meta.CreateEnhanced(NULL, NULL, NULL, NULL);
  xyplot.MakePlot(&Meta, logscale);
  HENHMETAFILE hMeta = Meta.CloseEnhanced();
  if (hMeta == NULL)
    MsgBox("No Handle...");
  if (OpenClipboard() == FALSE)
    MsgBox("Cannot access the Clipboard");
  else {
    EmptyClipboard();
    if (SetClipboardData(CF_ENHMETAFILE, hMeta) == NULL) {
      MsgBox("Couldn't SetClipboardData");
    }
    CloseClipboard();

    // fire up plot viewer;
    //		((CFemmApp *)AfxGetApp())->CreateNewDocument(8);

    // in this case call the external program rather than
    // displaying in the window.  Since dialog is running modal,
    // it obscures the plotted BH curve--this is an annoying kludge.
    char CommandLine[MAX_PATH];
    sprintf(CommandLine, "%sfemmplot.exe", (const char*)((CFemmApp*)AfxGetApp())->GetExecutablePath());
    STARTUPINFO StartupInfo2 = { 0 };
    PROCESS_INFORMATION ProcessInfo2;
    StartupInfo2.cb = sizeof(STARTUPINFO);
    CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo2, &ProcessInfo2);
    CloseHandle(ProcessInfo2.hProcess);
    CloseHandle(ProcessInfo2.hThread);
  }
}

void CTKData::OnReadBhcurve()
{
  // TODO: Add your control notification handler code here

  CFileDialog* fname_dia;
  CString infile;

  fname_dia = new CFileDialog(
      TRUE,
      "dat | * ",
      infile,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "Two column text data file (*.dat) | *.dat; *.DAT | All Files (*.*) | *.*||",
      NULL);

  if (fname_dia->DoModal() == IDCANCEL) {
    delete[] fname_dia;
    return;
  }
  infile = fname_dia->GetPathName();
  delete[] fname_dia;

  CTKDatafile dlg;
  if (dlg.DoModal() == IDCANCEL)
    return;

  // read in data from text file;
  m_Bdata.Empty();
  m_Hdata.Empty();

  char s[1024];
  double t, k;

  FILE* fp = fopen(infile, "rt");
  if (fp == NULL) {
    MsgBox("problem opening data file");
    return;
  }
  while (fgets(s, 1024, fp) != NULL) {
    if (dlg.TKOrder == 0)
      sscanf(s, "%lf %lf", &t, &k);
    else
      sscanf(s, "%lf %lf", &k, &t);

    switch (dlg.TUnits) {
    case 1: // Celcius to Kelvin
      t += 273.15;
      break;
    case 2: // Fahrenheit to Kelvin
      t = 5. * (t + 459.67) / 9.;
      break;
    case 3: // Rankine to Kelvin
      t = 5. * t / 9.;
      break;
    default: // Kelvin
      t *= 1.;
      break;
    }

    switch (dlg.KUnits) {
    case 1: // Btu/(ft*hr*F) to W/(m*K)
      k *= 1.73073466637139;
      break;
    case 2: // kcal/(m*hr*K) to W/(m*K)
      k *= 1.163;
      break;
    default: // W/(m*K)
      k *= 1.;
      break;
    }

    sprintf(s, "%.15g\r\n", t);
    m_Bdata += s;
    sprintf(s, "%.15g\r\n", k);
    m_Hdata += s;
  }

  SetDlgItemText(IDC_BDATA, m_Bdata);
  SetDlgItemText(IDC_HDATA, m_Hdata);

  fclose(fp);
}

BOOL CTKData::OnInitDialog()
{
  CDialog::OnInitDialog();

  // build string representation of the data out of numerical data
  int i;
  CString c;

  m_Bdata.Empty();
  m_Hdata.Empty();

  for (i = 0; i < npts; i++) {
    c.Format("%.15g\r\n", T[i]);
    m_Bdata += c;
    c.Format("%.15g\r\n", K[i]);
    m_Hdata += c;
  }

  UpdateData(FALSE);

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CTKData::OnOK()
{
  StripTKData();

  CDialog::OnOK();
}
