// BHData.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "BHData.h"
#include "fullmatrix.h"
#include "BHPlot.h"
#include "BHDatafile.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBHData dialog

CBHData::CBHData(CWnd* pParent /*=NULL*/)
    : CDialog(CBHData::IDD, pParent)
{
  //{{AFX_DATA_INIT(CBHData)
  m_Bdata = _T("");
  m_Hdata = _T("");
  m_BHname = _T("");
  //}}AFX_DATA_INIT

  logplot = FALSE;
}

void CBHData::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBHData)
  DDX_Text(pDX, IDC_BDATA, m_Bdata);
  DDX_Text(pDX, IDC_HDATA, m_Hdata);
  DDX_Text(pDX, IDC_BHNAME, m_BHname);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BDATA, m_IDC_BDATA);
  DDX_Control(pDX, IDC_HDATA, m_IDC_HDATA);
}

BEGIN_MESSAGE_MAP(CBHData, CDialog)
//{{AFX_MSG_MAP(CBHData)
ON_BN_CLICKED(IDC_PLOT_BHCURVE, OnPlotBHcurve)
ON_BN_CLICKED(IDC_LOGPLOT_BHCURVE, OnLogPlotBHcurve)
ON_BN_CLICKED(IDC_READ_BHCURVE, OnReadBhcurve)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBHData message handlers

void CBHData::StripBHData()
{
  int k;
  char *buff, *nptr, *endptr;
  double z;

  B.RemoveAll();
  H.RemoveAll();
  BHpoints = 0;

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
    if (B.GetSize() > 0) { // enforce monotonicity
      if (z <= B[B.GetSize() - 1])
        break;
    } else if (z != 0)
      B.Add(0);
    B.Add(z);
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
    if (H.GetSize() > 0) {
      if (z <= H[H.GetSize() - 1])
        break;
    } else if (z != 0)
      H.Add(0);
    H.Add(z);
  }

  BHpoints = (int)B.GetSize();
  if ((int)H.GetSize() < BHpoints)
    BHpoints = (int)H.GetSize();

  free(buff);
}

void CBHData::GetSlopes()
{
  if (BHpoints == 0)
    return; // catch trivial case;

  CFullMatrix Z;
  int i;
  BOOL CurveOK = FALSE;
  BOOL Smooth = FALSE;
  double l1, l2;
  double *bn, *hn;

  Z.Create(BHpoints);
  slope = (double*)calloc(BHpoints, sizeof(double));
  bn = (double*)calloc(BHpoints, sizeof(double));
  hn = (double*)calloc(BHpoints, sizeof(double));

  while (CurveOK != TRUE) {
    // make sure that the space for computing slopes is cleared out
    Z.Wipe();

    // impose natural BC on the `left'
    l1 = B[1] - B[0];
    Z.M[0][0] = 4. / l1;
    Z.M[0][1] = 2. / l1;
    Z.b[0] = 6. * (H[1] - H[0]) / (l1 * l1);

    // impose natural BC on the `right'
    int n = BHpoints;
    l1 = B[n - 1] - B[n - 2];
    Z.M[n - 1][n - 1] = 4. / l1;
    Z.M[n - 1][n - 2] = 2. / l1;
    Z.b[n - 1] = 6. * (H[n - 1] - H[n - 2]) / (l1 * l1);

    for (i = 1; i < BHpoints - 1; i++) {
      l1 = B[i] - B[i - 1];
      l2 = B[i + 1] - B[i];

      Z.M[i][i - 1] = 2. / l1;
      Z.M[i][i] = 4. * (l1 + l2) / (l1 * l2);
      Z.M[i][i + 1] = 2. / l2;

      Z.b[i] = 6. * (H[i] - H[i - 1]) / (l1 * l1) + 6. * (H[i + 1] - H[i]) / (l2 * l2);
    }

    Z.GaussSolve();

    for (i = 0; i < BHpoints; i++)
      slope[i] = Z.b[i];

    // now, test to see if there are any "bad" segments in there.
    for (i = 1, CurveOK = TRUE; i < BHpoints; i++) {
      double L, c0, c1, c2, d0, d1, u0, u1, X0, X1;
      d0 = slope[i - 1];
      d1 = slope[i];
      u0 = H[i - 1];
      u1 = H[i];
      L = B[i] - B[i - 1];

      c0 = d0;
      c1 = -(2. * (2. * d0 * L + d1 * L + 3. * u0 - 3. * u1)) / (L * L);
      c2 = (3. * (d0 * L + d1 * L + 2. * u0 - 2. * u1)) / (L * L * L);
      X0 = -1.;
      X1 = -1.;

      u0 = c1 * c1 - 4. * c0 * c2;
      // check out degenerate cases
      if (c2 == 0) {
        if (c1 != 0)
          X0 = -c0 / c1;
      } else if (u0 > 0) {
        u0 = sqrt(u0);
        X0 = -(c1 + u0) / (2. * c2);
        X1 = (-c1 + u0) / (2. * c2);
      }

      // now, see if we've struck gold!
      if (((X0 >= 0.) && (X0 <= L)) || ((X1 >= 0.) && (X1 <= L)))
        CurveOK = FALSE;
    }

    if (CurveOK != TRUE) // remedial action
    {
      // Smooth out input points
      // to get rid of rapid transitions;
      // Uses a 3-point moving average
      for (i = 1; i < BHpoints - 1; i++) {
        bn[i] = (B[i - 1] + B[i] + B[i + 1]) / 3.;
        hn[i] = (H[i - 1] + H[i] + H[i + 1]) / 3.;
      }
      for (i = 1; i < BHpoints - 1; i++) {
        H[i] = hn[i];
        B[i] = bn[i];
      }
      Smooth++;
    }
  }
  free(bn);
  free(hn);

  // alert user if smoothing was required
  // if(Smooth!=FALSE) MsgBox("Data was smoothed %i times\nto obtain a single-valued BH curve fit",Smooth);

  return;
}

/*
void CBHData::GetSlopes()
{
  if (BHpoints==0) return; // catch trivial case;

  int i,bDone;
  double m1,m2;

  slope=(double *)calloc(BHpoints,sizeof(double));

  do{
    for(i=1;i<BHpoints-1;i++)
    {
      // calculate nominal slope of each intermediate knot
      m1=(H[i]-H[i-1])/(B[i]-B[i-1]);
      m2=(H[i+1]-H[i])/(B[i+1]-B[i]);

      // average of the linear interpolation slope on either side of the knot
      slope[i]=(m1+m2)/2.;

      // check a sufficient condition for monotonicity.
      bDone=TRUE;
      if ((slope[i]>3.*m1) || (slope[i]>3.*m2))
      {
        H[i]=(8.*H[i]+H[i-1]+H[i+1])/10.;
        B[i]=(8.*B[i]+B[i-1]+B[i+1])/10.;
        bDone=FALSE;
      }
    }
  }while(!bDone);

  // do endpoints
  slope[0]=(H[1]-H[0])/(B[1]-B[0]);
  slope[i]=(H[i]-H[i-1])/(B[i]-B[i-1]);

  return;
}
*/

double CBHData::GetH(double x)
{
  double b, h, z, z2, l;
  int i;

  b = fabs(x);
  if ((BHpoints == 0) || (b == 0))
    return 0;

  if (b > B[BHpoints - 1])
    return (H[BHpoints - 1] + slope[BHpoints - 1] * (b - B[BHpoints - 1]));

  for (i = 0; i < BHpoints - 1; i++)
    if ((b >= B[i]) && (b <= B[i + 1])) {
      l = (B[i + 1] - B[i]);
      z = (b - B[i]) / l;
      z2 = z * z;
      h = (1. - 3. * z2 + 2. * z2 * z) * H[i] + z * (1. - 2. * z + z2) * l * slope[i] + z2 * (3. - 2. * z) * H[i + 1] + z2 * (z - 1.) * l * slope[i + 1];
      return h;
    }

  return 0;
}

void CBHData::OnLogPlotBHcurve()
{
  logplot = TRUE;
  OnPlotBHcurve();
}

void CBHData::OnPlotBHcurve()
{
  CBHPlot xyplot;
  int i;
  double b, db;
  BOOL logscale = logplot;
  logplot = FALSE;

  slope = NULL;
  UpdateData();
  StripBHData();

  if (BHpoints < 3) {
    MsgBox("Must have at least 3 pairs of data points");
    return;
  }

  // copy raw B-H data for plotting in comparison to
  // splined (and possibly smoothed) curve
  xyplot.NumPts = BHpoints;
  xyplot.Pts = (CComplex*)calloc(BHpoints, sizeof(CComplex));
  for (i = 0; i < BHpoints; i++)
    xyplot.Pts[i] = H[i] + I * B[i];

  GetSlopes();

  // get path to femmplot that we will need to display
  // the plot of the B-H curve...
  CString comline = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  // Actually evaluate all the points on the line...
  if (xyplot.Create(101, 2) == FALSE) {
    free(slope);
    return;
  }

  db = (B[BHpoints - 1] - B[0]) / 100.;

  for (i = 0, b = B[0]; i <= 100; i++, b += db) {
    xyplot.M[i][1] = b;
    xyplot.M[i][0] = GetH(b);
  }

  sprintf(xyplot.lbls[0], "H, Amp/Meter");
  sprintf(xyplot.lbls[1], "B, Tesla");

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

  if (slope != NULL)
    free(slope);
}

void CBHData::OnReadBhcurve()
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

  CBHDatafile dlg;
  if (dlg.DoModal() == IDCANCEL)
    return;

  // read in data from text file;
  m_Bdata.Empty();
  m_Hdata.Empty();

  double bunits[] = { 1., 0.0001, 0.1 };
  double hunits[] = { 1., 1000., 79.5775, 79577.5 };
  char s[1024];
  double b_in, h_in, b_off, h_off;

  b_off = 0;
  h_off = 0;
  FILE* fp = fopen(infile, "rt");
  if (fp == NULL) {
    MsgBox("problem opening data file");
    return;
  }
  do {
    if (fgets(s, 1024, fp) == NULL)
      break;
    if (dlg.BHOrder == 0)
      sscanf(s, "%lf %lf", &b_in, &h_in);
    else
      sscanf(s, "%lf %lf", &h_in, &b_in);
    b_in *= bunits[dlg.BUnits];
    if ((b_off == 0) && (b_in < 0))
      b_off = fabs(b_in);
    b_in += b_off;
    h_in *= hunits[dlg.HUnits];
    if ((h_off == 0) && (h_in < 0))
      h_off = fabs(h_in);
    h_in += h_off;
    sprintf(s, "%.15g\r\n", b_in);
    m_Bdata += s;
    sprintf(s, "%.15g\r\n", h_in);
    m_Hdata += s;
  } while (1 > 0);

  SetDlgItemText(IDC_BDATA, m_Bdata);
  SetDlgItemText(IDC_HDATA, m_Hdata);

  fclose(fp);

  if (h_off != 0) {
    sprintf(s, "Suggested Hc = %.0f A/m", h_off);
    AfxMessageBox(s, MB_ICONINFORMATION);
  }
}
