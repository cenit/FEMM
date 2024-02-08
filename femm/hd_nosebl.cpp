#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "hd_nosebl.h"
/////////////////////////////////////////////////////////////////////////////
// CNode construction

CNode::CNode()
{
  x = 0.;
  y = 0.;
  IsSelected = FALSE;
  InGroup = 0;
  BoundaryMarker = "<None>";
  InConductor = "<None>";
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
  InConductor = "<None>";
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
  InConductor = "<None>";
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
  IsSelected = FALSE;
  BlockType = "<None>";
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
  Kx = 1.;
  Ky = 1; // volumetric heat capacity
  qv = 0.; // thermal conductivity
  Kt = 0.; // volume heat generation
  npts = 0; // number of points in the curve of
            // conductivity versus temperature
}

CMaterialProp::~CMaterialProp()
{
}

CBoundaryProp::CBoundaryProp()
{
  BdryName = "New Boundary";
  BdryFormat = 0; // type of boundary condition we are applying
                  // 0 = constant value of A
                  // 1 = Small skin depth eddy current BC
                  // 2 = Mixed BC
  Tset = Tinf = qs = beta = h = 0;
}

CPointProp::CPointProp()
{
  PointName = "New Point Property";
  T = 0; // nodal voltage
  qp = 0; // point current density
}

CCircuit::CCircuit()
{
  CircName = "New Conductor";
  T = 0;
  q = 0;
  CircType = 0;
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
