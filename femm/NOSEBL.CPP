#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "nosebl.h"
/////////////////////////////////////////////////////////////////////////////
// CNode construction

CNode::CNode()
{
  x = 0.;
  y = 0.;
  IsSelected = FALSE;
  InGroup = 0;
  BoundaryMarker = "<None>";
}

double CNode::GetDistance(double xo, double yo)
{
  return sqrt((x - xo) * (x - xo) + (y - yo) * (y - yo));
}

CComplex CNode::CC()
{
  return CComplex(x, y);
}

void CNode::ToggleSelect()
{
  if (IsSelected == TRUE)
    IsSelected = FALSE;
  else
    IsSelected = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSegment construction

CSegment::CSegment()
{
  n0 = 0;
  n1 = 0;
  IsSelected = FALSE;
  Hidden = FALSE;
  MaxSideLength = -1;
  BoundaryMarker = "<None>";
  InGroup = 0;
}

void CSegment::ToggleSelect()
{
  if (IsSelected == TRUE)
    IsSelected = FALSE;
  else
    IsSelected = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CArcSegment construction

CArcSegment::CArcSegment()
{
  n0 = 0;
  n1 = 0;
  IsSelected = FALSE;
  Hidden = FALSE;
  ArcLength = 90.;
  MaxSideLength = 10.;
  mySideLength = 1;
  BoundaryMarker = "<None>";
  InGroup = 0;
  NormalDirection = TRUE;
}

void CArcSegment::ToggleSelect()
{
  if (IsSelected == TRUE)
    IsSelected = FALSE;
  else
    IsSelected = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNode construction

CBlockLabel::CBlockLabel()
{
  x = 0.;
  y = 0.;
  MaxArea = 0.;
  MagDir = 0.;
  Turns = 1;
  IsSelected = FALSE;
  BlockType = "<None>";
  InCircuit = "<None>";
  InGroup = 0;
  IsExternal = FALSE;
  IsDefault = FALSE;
}

void CBlockLabel::ToggleSelect()
{
  if (IsSelected == TRUE)
    IsSelected = FALSE;
  else
    IsSelected = TRUE;
}

double CBlockLabel::GetDistance(double xo, double yo)
{
  return sqrt((x - xo) * (x - xo) + (y - yo) * (y - yo));
}

CMaterialProp::CMaterialProp()
{
  BlockName = "New Material";
  mu_x = 1.;
  mu_y = 1.; // permeabilities, relative
  H_c = 0.; // magnetization, A/m
  Jsrc = 0; // applied current density, MA/m^2
  Cduct = 0.; // conductivity of the material, MS/m
  Lam_d = 0.; // lamination thickness, mm
  Theta_hn = 0.; // hysteresis angle, degrees
  Theta_hx = 0.; // hysteresis angle, degrees
  Theta_hy = 0.; // hysteresis angle, degrees
  Theta_m = 0.; // magnetization direction, degrees;
  LamFill = 1.; // lamination fill factor;
  LamType = 0; // type of lamination;
  WireD = 0; // strand diameter, mm
  NStrands = 0; // number of strands per wire

  BHpoints = 0;
}

CMaterialProp::~CMaterialProp()
{
  if (BHpoints > 0)
    free(BHdata);
}

void CMaterialProp::StripBHData(CString& b, CString& h)
{
  int i, k;
  char *buff, *nptr, *endptr;
  double z;
  CArray<double, double> B;
  CArray<double, double> H;

  if (BHpoints > 0)
    free(BHdata);
  B.RemoveAll();
  H.RemoveAll();

  k = b.GetLength() + 10;
  buff = (char*)calloc(k, sizeof(char));
  strcpy(buff, b);
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

  k = h.GetLength() + 10;
  buff = (char*)calloc(k, sizeof(char));
  strcpy(buff, h);
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

  k = (int)B.GetSize();
  if ((int)H.GetSize() < k)
    k = (int)H.GetSize();

  if (k > 1) {
    BHpoints = k;
    {
      BHdata = (CComplex*)calloc(k, sizeof(CComplex));
      for (i = 0; i < k; i++)
        BHdata[i].Set(B[i], H[i]);
    }
  } else
    BHpoints = 0;
  free(buff);

  return;
}

void CMaterialProp::BHDataToCString(CString& b, CString& h)
{
  int i;
  char c[80];

  b.Empty();
  h.Empty();

  for (i = 0; i < BHpoints; i++) {
    sprintf(c, "%.15g\r\n", BHdata[i].re);
    b += c;
    sprintf(c, "%.15g\r\n", BHdata[i].im);
    h += c;
  }

  b.AnsiToOem();
  h.AnsiToOem();
}

CBoundaryProp::CBoundaryProp()
{
  BdryName = "New Boundary";
  BdryFormat = 0; // type of boundary condition we are applying
                  // 0 = constant value of A
                  // 1 = Small skin depth eddy current BC
                  // 2 = Mixed BC

  A0 = 0.;
  A1 = 0.;
  A2 = 0.;
  phi = 0.; // set value of A for BdryFormat=0;

  Mu = 0.;
  Sig = 0.; // material properties necessary to apply
            // eddy current BC

  c0 = 0.;
  c1 = 0.; // coefficients for mixed BC

  InnerAngle = 0; // information for air gap element definition
  OuterAngle = 0;
}

CPointProp::CPointProp()
{
  PointName = "New Point Property";
  Jp = 0; // applied point current, A
  Ap = 0; // prescribed nodal value;
}

CCircuit::CCircuit()
{
  CircName = "New Circuit";
  Amps = 0;
  CircType = 1;
};

CPeriodicBoundary::CPeriodicBoundary()
{
  BdryName = "";
  BdryFormat = 0;
  nseg = 0;
  narc = 0;
  seg[0] = 0;
  seg[1] = 0;
}

CAirGapElement::CAirGapElement()
{
  BdryName = "";
  BdryFormat = 0;

  totalArcElements = 0;
  totalArcLength = 0;
  ri = 0;
  ro = 0;
  agc = 0;
  InnerAngle = 0;
  OuterAngle = 0;
  node = NULL;
}

CAirGapElement::~CAirGapElement()
{
  if (node != NULL)
    free(node);
}

CCommonPoint::CCommonPoint()
{
  x = y = t = 0;
}

void CCommonPoint::Order()
{
  int z;

  if (x > y) {
    z = y;
    y = x;
    x = z;
  }
}

CQuadPoint::CQuadPoint()
{
  n0 = n1 = -1;
  w0 = w1 = 0;
}