#include "stdafx.h"
#include "bhplot.h"

CBHPlot::CBHPlot()
{
  NumRows = 0;
  NumCols = 0;
  NumPts = 0;
  M = NULL;
  lbls = NULL;
  Pts = NULL;
}

CBHPlot::~CBHPlot()
{
  if (NumRows == 0)
    return;

  int i;
  for (i = 0; i < NumRows; i++)
    free(M[i]);
  for (i = 0; i < NumCols; i++)
    free(lbls[i]);
  free(M);
  free(lbls);
  NumRows = 0;
  NumCols = 0;
  if (NumPts != 0) {
    free(Pts);
    NumPts = 0;
  }
}

BOOL CBHPlot::Create(int rows, int cols)
{
  int i;
  BOOL flag = TRUE;

  NumRows = rows;
  NumCols = cols;
  M = (double**)calloc(rows, sizeof(double*));
  lbls = (char**)calloc(cols, sizeof(char*));
  if ((M == NULL) || (lbls == NULL))
    flag = FALSE;
  for (i = 0; i < rows; i++) {
    M[i] = (double*)calloc(cols, sizeof(double));
    if (M[i] == NULL)
      flag = FALSE;
  }
  for (i = 0; i < cols; i++) {
    lbls[i] = (char*)calloc(80, sizeof(char));
    if (lbls[i] == 0)
      flag = FALSE;
  }

  if (flag == FALSE) {
    NumRows = 0;
    NumCols = 0;
    for (i = 0; i < rows; i++)
      free(M[i]);
    for (i = 0; i < cols; i++)
      free(lbls[i]);
    free(M);
    free(lbls);
  }

  return flag;
}

void CBHPlot::MakePlot(CDC* pDC, BOOL logscale)
{
  double HiX, LoX, HiY, LoY, dx, dy, d;
  static const double Height = 250.;
  static const double Width = 400.;
  static const double OffsetX = 80.;
  static const double OffsetY = 30.;
  char s[80];
  int i, j, k;
  CPen PlotPen;
  CPen* pOldPen;
  static COLORREF PenCols[] = { 0x00000000, 0x00ff0000, 0x0000ff00, 0x000000ff,
    0x00ffff00, 0x00ff00ff, 0x0000ffff };

  if (NumRows == 0)
    return;

  // catch the case where logscale barfs
  if ((Pts[0].re == 0) && (logscale)) {
    for (i = 1; i < NumPts; i++)
      Pts[i - 1] = Pts[i];
    NumPts--;

    for (i = 0, j = 0; i < NumRows; i++)
      if (M[i][0] < Pts[0].re)
        j++;
    NumRows -= j;
    for (i = 0; i < NumRows; i++)
      for (k = 0; k < NumCols; k++)
        M[i][k] = M[i + j][k];
  }

  // if logscale, take the log of H
  if (logscale) {
    for (i = 0; i < NumRows; i++)
      M[i][0] = log10(M[i][0]);
    for (i = 0; i < NumPts; i++)
      Pts[i].re = log10(Pts[i].re);
  }

  LoY = M[0][1];
  HiY = M[0][1];

  for (i = 1; i < NumRows; i++)
    for (j = 1; j < NumCols; j++) {
      if (M[i][j] > HiY)
        HiY = M[i][j];
      if (M[i][j] < LoY)
        LoY = M[i][j];
    }

  if (HiY == LoY) {
    HiY = 1.;
    LoY = -1.;
  }

  dy = pow(10., floor(log10(HiY - LoY)));
  LoY = dy * floor(LoY / dy);
  HiY = dy * ceil(HiY / dy);

  if (logscale) {
    LoX = floor(Re(Pts[0]));
    HiX = ceil(Re(Pts[NumPts - 1]));
    dx = 1.;
  } else {
    HiX = M[NumRows - 1][0];
    LoX = M[0][0]; // LoX=0.;
    dx = pow(10., floor(log10(HiX - LoX)));
  }

  // scale those suckers...
  for (i = 0; i < NumRows; i++) {
    for (j = 1; j < NumCols; j++)
      M[i][j] = Height - (M[i][j] - LoY) * Height / (HiY - LoY) + OffsetY;
    M[i][0] = (M[i][0] - LoX) * Width / (HiX - LoX) + OffsetX;
  }

  for (i = 0; i < NumPts; i++) {
    Pts[i].re = (Pts[i].re - LoX) * Width / (HiX - LoX) + OffsetX;
    Pts[i].im = Height - (Pts[i].im - LoY) * Height / (HiY - LoY) + OffsetY;
  }

  PlotPen.CreatePen(PS_SOLID, 1, PenCols[0]);
  pOldPen = pDC->SelectObject(&PlotPen);
  pDC->MoveTo(0, 0);
  pDC->LineTo(588, 0);
  pDC->LineTo(588, 343);
  pDC->LineTo(0, 343);
  pDC->LineTo(0, 0);

  pDC->MoveTo((int)OffsetX, (int)OffsetY);
  pDC->LineTo((int)OffsetX, (int)(OffsetY + Height));
  pDC->LineTo((int)(OffsetX + Width), (int)(OffsetY + Height));
  pDC->SelectObject(pOldPen);
  PlotPen.DeleteObject();

  CFont fntArial, *pOldFont;
  fntArial.CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  pOldFont = pDC->SelectObject(&fntArial);

  // draw those suckers...
  PlotPen.CreatePen(PS_SOLID, 1, PenCols[1]);
  pOldPen = pDC->SelectObject(&PlotPen);
  for (i = 0; i < NumPts; i++) {
    pDC->MoveTo((int)Pts[i].re - 2, (int)Pts[i].im - 2);
    pDC->LineTo((int)Pts[i].re - 2, (int)Pts[i].im + 2);
    pDC->LineTo((int)Pts[i].re + 2, (int)Pts[i].im + 2);
    pDC->LineTo((int)Pts[i].re + 2, (int)Pts[i].im - 2);
    pDC->LineTo((int)Pts[i].re - 2, (int)Pts[i].im - 2);
  }
  pDC->SelectObject(pOldPen);
  PlotPen.DeleteObject();

  for (i = 1; i < NumCols; i++) {
    PlotPen.CreatePen(PS_SOLID, 1, PenCols[i - 1]);
    pOldPen = pDC->SelectObject(&PlotPen);
    pDC->MoveTo((int)M[0][0], (int)M[0][i]);
    for (j = 0; j < NumRows; j++)
      pDC->LineTo((int)M[j][0], (int)M[j][i]);
    pDC->SetTextColor(PenCols[i - 1]);
    pDC->TextOut((int)(OffsetX + Width + 10.), ((int)OffsetY) + 14 * i, lbls[i], (int)strlen(lbls[i]));
    pDC->SelectObject(pOldPen);
    PlotPen.DeleteObject();
  }
  pDC->SetTextColor(0x00000000);
  pDC->TextOut(200, (int)(OffsetY + Height + 30), lbls[0], (int)strlen(lbls[0]));
  k = (int)((HiY - LoY) / dy + 0.5);
  if (k < 4) {
    k *= 2;
    dy /= 2.;
  }
  if (k == 0)
    k++;
  pDC->SetTextAlign(TA_RIGHT);
  for (i = 0; i <= k; i++) {
    pDC->MoveTo((int)OffsetX, (i * (int)Height) / k + (int)OffsetY);
    pDC->LineTo((int)OffsetX - 5, (i * (int)Height) / k + (int)OffsetY);
    sprintf(s, "%.4g", HiY - ((double)i) * dy);
    pDC->TextOut(((int)OffsetX) - 10, (i * (int)Height) / k + (int)OffsetY - 6, s, (int)strlen(s));
  }
  k = (int)((HiX - LoX) / dx);
  if ((k < 4) && (!logscale)) {
    dx /= 2.;
    k = (int)((HiX - LoX) / dx);
  }
  d = (((double)Width) * dx / (HiX - LoX));
  pDC->SetTextAlign(TA_CENTER);
  for (i = 0; i <= k; i++) {
    pDC->MoveTo((int)(((double)i) * d + OffsetX), (int)(OffsetY + Height));
    pDC->LineTo((int)(((double)i) * d + OffsetX), (int)(OffsetY + Height) + 5);
    if (logscale)
      sprintf(s, "%g", pow((double)10, LoX + ((double)i) * dx));
    else
      sprintf(s, "%.0f", LoX + ((double)i) * dx);
    pDC->TextOut((int)(((double)i) * d + OffsetX), (int)OffsetY + (int)Height + 10, s, (int)strlen(s));
  }
  pDC->SetTextAlign(TA_LEFT);

  pDC->SelectObject(pOldFont);
}

BOOL CBHPlot::ToDisk()
{
  FILE* fp;
  int i, j;
  CString dest_name;

  if ((NumCols == 0) || (NumRows == 0)) {
    MsgBox("No data to write to disk.");
    return FALSE;
  }

  CFileDialog* fname_dia = new CFileDialog(
      FALSE,
      "txt | * ",
      dest_name,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "Text Files (*.txt) | *.txt; *.TXT | All Files (*.*) | *.*||",
      NULL);

  if (fname_dia->DoModal() == IDCANCEL) {
    delete[] fname_dia;
    return FALSE;
  }

  dest_name = fname_dia->GetPathName();

  delete[] fname_dia;

  if ((fp = fopen(dest_name, "wt")) != NULL) {
    for (i = 0; i < NumCols; i++)
      fprintf(fp, "#Column %i: %s\n", i, lbls[i]);
    for (i = 0; i < NumRows; i++) {
      for (j = 0; j < NumCols; j++)
        fprintf(fp, "%e	", M[i][j]);
      fprintf(fp, "\n");
    }
    fclose(fp);
    CString OutMsg = "Output written to " + dest_name;
    AfxMessageBox(OutMsg, MB_ICONINFORMATION);
  } else {
    MsgBox("Couldn't open output file");
    return FALSE;
  }

  return TRUE;
}
