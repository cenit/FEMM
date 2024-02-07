// beladrawDoc.cpp : implementation of the CbeladrawDoc class
//

#include "stdafx.h"
#include "femm.h"
#include "beladrawDoc.h"
#include "beladrawView.h"
#include "bd_probdlg.h"
#include "bd_PtProp.h"
#include "bd_OpBlkDlg.h"
#include "bd_OpNodeDlg.h"
#include "bd_OpSegDlg.h"
#include "bd_OpArcSegDlg.h"
#include "OpGrp.h"
#include "ArcDlg.h"
#include "ExteriorProps.h"

extern void* pBeladrawDoc;
extern lua_State* lua;
extern BOOL bLinehook;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CbeladrawDoc

IMPLEMENT_DYNCREATE(CbeladrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CbeladrawDoc, CDocument)
//{{AFX_MSG_MAP(CbeladrawDoc)
ON_COMMAND(ID_DEFINE_PROBLEM, OnDefineProblem)
ON_COMMAND(ID_EDIT_MATPROPS, OnEditMatprops)
ON_COMMAND(ID_EDIT_PTPROPS, OnEditPtprops)
ON_COMMAND(ID_EDIT_SEGPROPS, OnEditSegprops)
ON_COMMAND(ID_EDIT_CIRCPROPS, OnEditCircprops)
ON_COMMAND(ID_EDIT_EXTERIOR, OnEditExterior)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CbeladrawDoc construction/destruction

CbeladrawDoc::CbeladrawDoc()
{
  // set some default values for starting up rendering
  // things properly
  FirstDraw = FALSE;
  NoDraw = FALSE;

  // set up some default document behaviors
  d_prec = 1.e-08;
  d_minangle = DEFAULT_MINIMUM_ANGLE;
  d_depth = 1;
  d_coord = 0;
  d_length = 0;
  d_type = 0;

  // Figure out what directory the executables
  // are in, so we can call `triangle' if we need to.
  BinDir = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  // read document default behaviors from disk
  ScanPreferences();

  // fire up lua
  initalise_lua();

  // initialize the data in the document
  OnNewDocument();
}

CbeladrawDoc::~CbeladrawDoc()
{
  if (pBeladrawDoc == this)
    pBeladrawDoc = NULL;
}

BOOL CbeladrawDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // clear out all current lines, nodes, and block labels
  nodelist.RemoveAll();
  linelist.RemoveAll();
  arclist.RemoveAll();
  blocklist.RemoveAll();
  undonodelist.RemoveAll();
  undolinelist.RemoveAll();
  undoarclist.RemoveAll();
  undoblocklist.RemoveAll();
  nodeproplist.RemoveAll();
  lineproplist.RemoveAll();
  blockproplist.RemoveAll();
  circproplist.RemoveAll();
  meshnode.RemoveAll();
  meshline.RemoveAll();
  greymeshline.RemoveAll();

  // set problem attributes to generic ones;
  Precision = d_prec;
  MinAngle = d_minangle;
  SmartMesh = theApp.d_SmartMesh;
  Depth = d_depth;
  LengthUnits = d_length;
  ProblemType = d_type;
  Coords = d_coord;
  ProblemNote = "Add comments here.";
  extRo = extRi = extZo = 0;

  // reset view to default attributes
  CbeladrawView* pView;
  POSITION pos;
  pos = GetFirstViewPosition();
  if (pos != NULL) {
    pView = (CbeladrawView*)GetNextView(pos);
    if (pView != NULL)
      pView->OnNewDocument();
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CbeladrawDoc serialization

void CbeladrawDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) {
    // TODO: add storing code here
  } else {
    // TODO: add loading code here
  }
}

/////////////////////////////////////////////////////////////////////////////
// CbeladrawDoc diagnostics

#ifdef _DEBUG
void CbeladrawDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CbeladrawDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CbeladrawDoc commands

void CbeladrawDoc::OnDefineProblem()
{
  bdCProbDlg pDlg;

  // Send present parameter values to the dialog
  // pDlg.m_rji=TheDoc->vi[0];
  pDlg.probtype = ProblemType;
  pDlg.m_problem_note = ProblemNote;
  pDlg.m_precision = Precision;
  pDlg.m_minangle = MinAngle;
  pDlg.bsmart = SmartMesh;
  pDlg.m_depth = Depth;
  pDlg.lengthunits = LengthUnits;

  // Display dialog and collect data
  if (pDlg.DoModal() == IDOK) {
    Precision = pDlg.m_precision;
    MinAngle = pDlg.m_minangle;
    SmartMesh = pDlg.bsmart;
    ProblemNote = pDlg.m_problem_note;
    ProblemType = pDlg.probtype;
    LengthUnits = pDlg.lengthunits;
    Depth = pDlg.m_depth;
  }
}

void CbeladrawDoc::UnselectAll()
{
  int i;

  for (i = 0; i < nodelist.GetSize(); i++)
    nodelist[i].IsSelected = FALSE;
  for (i = 0; i < linelist.GetSize(); i++)
    linelist[i].IsSelected = FALSE;
  for (i = 0; i < blocklist.GetSize(); i++)
    blocklist[i].IsSelected = FALSE;
  for (i = 0; i < arclist.GetSize(); i++)
    arclist[i].IsSelected = FALSE;
}

BOOL CbeladrawDoc::AddNode(double x, double y, double d)
{
  int i, k;
  CNode pt;
  CSegment segm;
  CArcSegment asegm;
  CComplex c, a0, a1, a2;
  double R;

  // test to see if ``too close'' to existing node...
  for (i = 0; i < nodelist.GetSize(); i++)
    if (nodelist[i].GetDistance(x, y) < d)
      return FALSE;

  // can't put a node on top of a block label; do same sort of test.
  for (i = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].GetDistance(x, y) < d)
      return FALSE;

  // if all is OK, add point in to the node list...
  pt.x = x;
  pt.y = y;
  nodelist.Add(pt);

  // test to see if node is on an existing line; if so,
  // break into two lines;
  k = (int)linelist.GetSize();
  for (i = 0; i < k; i++) {
    if (fabs(ShortestDistance(x, y, i)) < d) {
      segm = linelist[i];
      linelist[i].n1 = (int)nodelist.GetSize() - 1;
      segm.n0 = (int)nodelist.GetSize() - 1;
      linelist.Add(segm);
    }
  }

  // test to see if node is on an existing arc; if so,
  // break into two arcs;
  k = (int)arclist.GetSize();
  for (i = 0; i < k; i++) {
    if (ShortestDistanceFromArc(CComplex(x, y), arclist[i]) < d) {
      a0.Set(nodelist[arclist[i].n0].x, nodelist[arclist[i].n0].y);
      a1.Set(nodelist[arclist[i].n1].x, nodelist[arclist[i].n1].y);
      a2.Set(x, y);
      GetCircle(arclist[i], c, R);
      asegm = arclist[i];
      arclist[i].n1 = (int)nodelist.GetSize() - 1;
      arclist[i].ArcLength = arg((a2 - c) / (a0 - c)) * 180. / PI;
      asegm.n0 = (int)nodelist.GetSize() - 1;
      asegm.ArcLength = arg((a1 - c) / (a2 - c)) * 180. / PI;
      arclist.Add(asegm);
    }
  }
  return TRUE;
}

BOOL CbeladrawDoc::AddSegment(int n0, int n1, double tol)
{
  return AddSegment(n0, n1, NULL, tol);
}

BOOL CbeladrawDoc::AddSegment(int n0, int n1, CSegment* parsegm, double tol)
{
  int i, j, k;
  double xi, yi, t;
  CComplex p[2];
  CSegment segm;
  CArray<CComplex, CComplex&> newnodes;

  newnodes.RemoveAll();

  // don't add if line is degenerate
  if (n0 == n1)
    return FALSE;

  // don't add if the line is already in the list;
  for (i = 0; i < linelist.GetSize(); i++) {
    if ((linelist[i].n0 == n0) && (linelist[i].n1 == n1))
      return FALSE;
    if ((linelist[i].n0 == n1) && (linelist[i].n1 == n0))
      return FALSE;
  }

  // add proposed line to the linelist
  segm.BoundaryMarker = "<None>";
  if (parsegm != NULL)
    segm = *parsegm;
  segm.IsSelected = FALSE;
  segm.n0 = n0;
  segm.n1 = n1;

  // check to see if there are intersections with segments
  for (i = 0; i < linelist.GetSize(); i++)
    if (GetIntersection(n0, n1, i, &xi, &yi) == TRUE)
      newnodes.Add(CComplex(xi, yi));

  // check to see if there are intersections with arcs
  for (i = 0; i < arclist.GetSize(); i++) {
    j = GetLineArcIntersection(segm, arclist[i], p);
    if (j > 0)
      for (k = 0; k < j; k++)
        newnodes.Add(p[k]);
  }

  // add nodes at intersections
  if (tol == 0) {
    if (nodelist.GetSize() < 2)
      t = 1.e-08;
    else {
      CComplex p0, p1;
      p0 = nodelist[0].CC();
      p1 = p0;
      for (i = 1; i < nodelist.GetSize(); i++) {
        if (nodelist[i].x < p0.re)
          p0.re = nodelist[i].x;
        if (nodelist[i].x > p1.re)
          p1.re = nodelist[i].x;
        if (nodelist[i].y < p0.im)
          p0.im = nodelist[i].y;
        if (nodelist[i].y > p1.im)
          p1.im = nodelist[i].y;
      }
      t = abs(p1 - p0) * CLOSE_ENOUGH;
    }
  } else
    t = tol;

  for (i = 0; i < newnodes.GetSize(); i++)
    AddNode(newnodes[i].re, newnodes[i].im, t);

  // Add proposed line segment
  linelist.Add(segm);

  // check to see if proposed line passes through other points;
  // if so, delete line and create lines that link intermediate points;
  // does this by recursive use of AddSegment;
  double d, dmin;
  UnselectAll();
  if (tol == 0)
    dmin = abs(nodelist[n1].CC() - nodelist[n0].CC()) * 1.e-05;
  else
    dmin = tol;

  k = (int)linelist.GetSize() - 1;
  for (i = 0; i < nodelist.GetSize(); i++) {
    if ((i != n0) && (i != n1)) {
      d = ShortestDistance(nodelist[i].x, nodelist[i].y, k);
      if (abs(nodelist[i].CC() - nodelist[n0].CC()) < dmin)
        d = 2. * dmin;
      if (abs(nodelist[i].CC() - nodelist[n1].CC()) < dmin)
        d = 2. * dmin;
      if (d < dmin) {
        linelist[k].ToggleSelect();
        DeleteSelectedSegments();
        if (parsegm == NULL) {
          AddSegment(n0, i, dmin);
          AddSegment(i, n1, dmin);
        } else {
          AddSegment(n0, i, &segm, dmin);
          AddSegment(i, n1, &segm, dmin);
        }
        i = (int)nodelist.GetSize();
      }
    }
  }

  return TRUE;
}

void CbeladrawDoc::GetCircle(CArcSegment& arc, CComplex& c, double& R)
{
  CComplex a0, a1, t;
  double d, tta;

  a0.Set(nodelist[arc.n0].x, nodelist[arc.n0].y);
  a1.Set(nodelist[arc.n1].x, nodelist[arc.n1].y);
  d = abs(a1 - a0); // distance between arc endpoints

  // figure out what the radius of the circle is...
  t = (a1 - a0) / d;
  tta = arc.ArcLength * PI / 180.;
  R = d / (2. * sin(tta / 2.));
  c = a0 + (d / 2. + I * sqrt(R * R - d * d / 4.)) * t; // center of the arc segment's circle...
}

int CbeladrawDoc::GetLineArcIntersection(CSegment& seg, CArcSegment& arc, CComplex* p)
{
  CComplex p0, p1, a0, a1, t, v, c;
  double d, l, R, z, tta;
  int i = 0;

  p0.Set(nodelist[seg.n0].x, nodelist[seg.n0].y);
  p1.Set(nodelist[seg.n1].x, nodelist[seg.n1].y);
  a0.Set(nodelist[arc.n0].x, nodelist[arc.n0].y);
  a1.Set(nodelist[arc.n1].x, nodelist[arc.n1].y);
  d = abs(a1 - a0); // distance between arc endpoints

  // figure out what the radius of the circle is...
  t = (a1 - a0) / d;
  tta = arc.ArcLength * PI / 180.;
  R = d / (2. * sin(tta / 2.));
  c = a0 + (d / 2. + I * sqrt(R * R - d * d / 4.)) * t; // center of the arc segment's circle...

  // figure out the distance between line and circle's center;
  d = abs(p1 - p0);
  t = (p1 - p0) / d;
  v = (c - p0) / t;
  if (fabs(Im(v)) > R)
    return 0;
  l = sqrt(R * R - Im(v) * Im(v)); // Im(v) is distance between line and center...

  if ((l / R) < 1.e-05) { // case where line is very close to a tangent;
    p[i] = p0 + Re(v) * t; // make it be a tangent.
    R = Re((p[i] - p0) / t);
    z = arg((p[i] - c) / (a0 - c));
    if ((R > 0) && (R < d) && (z > 0.) && (z < tta))
      i++;
    return i;
  }

  p[i] = p0 + (Re(v) + l) * t; // first possible intersection;
  R = Re((p[i] - p0) / t);
  z = arg((p[i] - c) / (a0 - c));
  if ((R > 0) && (R < d) && (z > 0.) && (z < tta))
    i++;

  p[i] = p0 + (Re(v) - l) * t; // second possible intersection
  R = Re((p[i] - p0) / t);
  z = arg((p[i] - c) / (a0 - c));
  if ((R > 0) && (R < d) && (z > 0.) && (z < tta))
    i++;

  // returns the number of valid intersections found;
  // intersections are returned in the array p[];
  return i;
}

int CbeladrawDoc::GetArcArcIntersection(CArcSegment& arc0, CArcSegment& arc1, CComplex* p)
{
  CComplex a0, a1, t, c0, c1;
  double d, l, R0, R1, z0, z1, c, tta0, tta1;
  int i = 0;

  a0.Set(nodelist[arc0.n0].x, nodelist[arc0.n0].y);
  a1.Set(nodelist[arc1.n0].x, nodelist[arc1.n0].y);

  GetCircle(arc1, c1, R1);
  GetCircle(arc0, c0, R0);

  d = abs(c1 - c0); // distance between centers

  if ((d > R0 + R1) || (d < 1.e-08))
    return 0;
  // directly eliminate case where there can't
  // be any crossings....

  l = sqrt((R0 + R1 - d) * (d + R0 - R1) * (d - R0 + R1) * (d + R0 + R1)) / (2. * d);
  c = 1. + (R0 / d) * (R0 / d) - (R1 / d) * (R1 / d);
  t = (c1 - c0) / d;
  tta0 = arc0.ArcLength * PI / 180;
  tta1 = arc1.ArcLength * PI / 180;

  p[i] = c0 + (c * d / 2. + I * l) * t; // first possible intersection;
  z0 = arg((p[i] - c0) / (a0 - c0));
  z1 = arg((p[i] - c1) / (a1 - c1));
  if ((z0 > 0.) && (z0 < tta0) && (z1 > 0.) && (z1 < tta1))
    i++;

  if (fabs(d - R0 + R1) / (R0 + R1) < 1.e-05) {
    p[i] = c0 + c * d * t / 2.;
    return i;
  }

  p[i] = c0 + (c * d / 2. - I * l) * t; // second possible intersection
  z0 = arg((p[i] - c0) / (a0 - c0));
  z1 = arg((p[i] - c1) / (a1 - c1));
  if ((z0 > 0.) && (z0 < tta0) && (z1 > 0.) && (z1 < tta1))
    i++;

  // returns the number of valid intersections found;
  // intersections are returned in the array p[];
  return i;
}

BOOL CbeladrawDoc::AddArcSegment(CArcSegment& asegm, double tol)
{
  int i, j, k;
  CSegment segm;
  CArcSegment newarc;
  CComplex c, p[2];
  CArray<CComplex, CComplex&> newnodes;
  double R, d, dmin, t;

  newnodes.RemoveAll();

  // don't add if line is degenerate
  if (asegm.n0 == asegm.n1)
    return FALSE;

  // don't add if the arc is already in the list;
  for (i = 0; i < arclist.GetSize(); i++) {
    if ((arclist[i].n0 == asegm.n0) && (arclist[i].n1 == asegm.n1) && (fabs(arclist[i].ArcLength - asegm.ArcLength) < 1.e-02))
      return FALSE;
    // arcs are ``the same'' if start and end points are the same, and if
    // the arc lengths are relatively close (but a lot farther than
    // machine precision...
  }

  // add proposed arc to the linelist
  asegm.IsSelected = FALSE;

  // check to see if there are intersections
  for (i = 0; i < linelist.GetSize(); i++) {
    j = GetLineArcIntersection(linelist[i], asegm, p);
    if (j > 0)
      for (k = 0; k < j; k++)
        newnodes.Add(p[k]);
  }
  for (i = 0; i < arclist.GetSize(); i++) {
    j = GetArcArcIntersection(asegm, arclist[i], p);
    if (j > 0)
      for (k = 0; k < j; k++)
        newnodes.Add(p[k]);
  }

  // add nodes at intersections
  if (tol == 0) {
    if (nodelist.GetSize() < 2)
      t = 1.e-08;
    else {
      CComplex p0, p1;
      p0 = nodelist[0].CC();
      p1 = p0;
      for (i = 1; i < nodelist.GetSize(); i++) {
        if (nodelist[i].x < p0.re)
          p0.re = nodelist[i].x;
        if (nodelist[i].x > p1.re)
          p1.re = nodelist[i].x;
        if (nodelist[i].y < p0.im)
          p0.im = nodelist[i].y;
        if (nodelist[i].y > p1.im)
          p1.im = nodelist[i].y;
      }
      t = abs(p1 - p0) * CLOSE_ENOUGH;
    }
  } else
    t = tol;

  for (i = 0; i < newnodes.GetSize(); i++)
    AddNode(newnodes[i].re, newnodes[i].im, t);

  // add proposed arc segment;
  arclist.Add(asegm);

  // check to see if proposed arc passes through other points;
  // if so, delete arc and create arcs that link intermediate points;
  // does this by recursive use of AddArcSegment;

  UnselectAll();
  GetCircle(asegm, c, R);
  if (tol == 0)
    dmin = fabs(R * PI * asegm.ArcLength / 180.) * 1.e-05;
  else
    dmin = tol;
  k = (int)arclist.GetSize() - 1;

  for (i = 0; i < nodelist.GetSize(); i++) {
    if ((i != asegm.n0) && (i != asegm.n1)) {
      d = ShortestDistanceFromArc(CComplex(nodelist[i].x, nodelist[i].y), arclist[k]);
      if (d < dmin) {
        CComplex a0, a1, a2;
        a0.Set(nodelist[asegm.n0].x, nodelist[asegm.n0].y);
        a1.Set(nodelist[asegm.n1].x, nodelist[asegm.n1].y);
        a2.Set(nodelist[i].x, nodelist[i].y);
        arclist[k].ToggleSelect();
        DeleteSelectedArcSegments();

        newarc = asegm;
        newarc.n1 = i;
        newarc.ArcLength = arg((a2 - c) / (a0 - c)) * 180. / PI;
        AddArcSegment(newarc, dmin);

        newarc = asegm;
        newarc.n0 = i;
        newarc.ArcLength = arg((a1 - c) / (a2 - c)) * 180. / PI;
        AddArcSegment(newarc, dmin);

        i = (int)nodelist.GetSize();
      }
    }
  }

  return TRUE;
}

BOOL CbeladrawDoc::AddBlockLabel(double x, double y, double d)
{
  int i;
  BOOL AddFlag = TRUE;

  // test to see if ``too close'' to existing node...
  for (i = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].GetDistance(x, y) < d)
      AddFlag = FALSE;

  // can't put a block label on top of an existing node...
  for (i = 0; i < nodelist.GetSize(); i++)
    if (nodelist[i].GetDistance(x, y) < d)
      return FALSE;

  // can't put a block label on a line, either...
  for (i = 0; i < linelist.GetSize(); i++)
    if (ShortestDistance(x, y, i) < d)
      return FALSE;

  // if all is OK, add point in to the node list...
  if (AddFlag == TRUE) {
    CBlockLabel pt;
    pt.x = x;
    pt.y = y;
    blocklist.Add(pt);
  }

  return TRUE;
}

int CbeladrawDoc::ClosestNode(double x, double y)
{
  int i, j;
  double d0, d1;

  if (nodelist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = nodelist[0].GetDistance(x, y);
  for (i = 0; i < nodelist.GetSize(); i++) {
    d1 = nodelist[i].GetDistance(x, y);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

int CbeladrawDoc::ClosestBlockLabel(double x, double y)
{
  int i, j;
  double d0, d1;

  if (blocklist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = blocklist[0].GetDistance(x, y);
  for (i = 0; i < blocklist.GetSize(); i++) {
    d1 = blocklist[i].GetDistance(x, y);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

double CbeladrawDoc::ShortestDistanceFromArc(CComplex p, CArcSegment& arc)
{
  double R, d, l, z;
  CComplex a0, a1, c, t;

  a0.Set(nodelist[arc.n0].x, nodelist[arc.n0].y);
  a1.Set(nodelist[arc.n1].x, nodelist[arc.n1].y);
  GetCircle(arc, c, R);
  d = abs(p - c);

  if (d == 0)
    return R;

  t = (p - c) / d;
  l = abs(p - c - R * t);
  z = arg(t / (a0 - c)) * 180 / PI;
  if ((z > 0) && (z < arc.ArcLength))
    return l;

  z = abs(p - a0);
  l = abs(p - a1);
  if (z < l)
    return z;
  return l;
}

double CbeladrawDoc::ShortestDistance(double p, double q, int segm)
{
  double t, x[3], y[3];

  x[0] = nodelist[linelist[segm].n0].x;
  y[0] = nodelist[linelist[segm].n0].y;
  x[1] = nodelist[linelist[segm].n1].x;
  y[1] = nodelist[linelist[segm].n1].y;

  t = ((p - x[0]) * (x[1] - x[0]) + (q - y[0]) * (y[1] - y[0])) / ((x[1] - x[0]) * (x[1] - x[0]) + (y[1] - y[0]) * (y[1] - y[0]));

  if (t > 1.)
    t = 1.;
  if (t < 0.)
    t = 0.;

  x[2] = x[0] + t * (x[1] - x[0]);
  y[2] = y[0] + t * (y[1] - y[0]);

  return sqrt((p - x[2]) * (p - x[2]) + (q - y[2]) * (q - y[2]));
}

int CbeladrawDoc::ClosestSegment(double x, double y)
{
  double d0, d1;
  int i, j;

  if (linelist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = ShortestDistance(x, y, 0);
  for (i = 0; i < linelist.GetSize(); i++) {
    d1 = ShortestDistance(x, y, i);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

int CbeladrawDoc::ClosestArcSegment(double x, double y)
{
  double d0, d1;
  int i, j;

  if (arclist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = ShortestDistanceFromArc(CComplex(x, y), arclist[0]);
  for (i = 0; i < arclist.GetSize(); i++) {
    d1 = ShortestDistanceFromArc(CComplex(x, y), arclist[i]);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

BOOL CbeladrawDoc::GetIntersection(int n0, int n1, int segm, double* xi, double* yi)
// prospective line specified by n0,n1;
// segment specified by segm;
// coordinates of the intersection returned in xi,yi
{
  CComplex p0, p1, q0, q1;
  double ee, x, z;

  // Check to see if the two lines have a common endpoint
  // If they do, there can be no other intersection...
  if (n0 == linelist[segm].n0)
    return FALSE;
  if (n0 == linelist[segm].n1)
    return FALSE;
  if (n1 == linelist[segm].n0)
    return FALSE;
  if (n1 == linelist[segm].n1)
    return FALSE;

  // Get a definition of "real small" based on the lengths
  // of the lines of interest;
  p0 = nodelist[linelist[segm].n0].CC();
  p1 = nodelist[linelist[segm].n1].CC();
  q0 = nodelist[n0].CC();
  q1 = nodelist[n1].CC();
  ee = min(abs(p1 - p0), abs(q1 - q0)) * 1.0e-8;

  // Rotate and scale the prospective line
  q0 = (q0 - p0) / (p1 - p0);
  q1 = (q1 - p0) / (p1 - p0);

  // Check for cases where there is obviously no intersection
  if ((Re(q0) <= 0.) && (Re(q1) <= 0.))
    return FALSE;
  if ((Re(q0) >= 1.) && (Re(q1) >= 1.))
    return FALSE;
  if ((Im(q0) <= 0.) && (Im(q1) <= 0.))
    return FALSE;
  if ((Im(q0) >= 0.) && (Im(q1) >= 0.))
    return FALSE;

  // compute intersection
  z = Im(q0) / Im(q0 - q1);

  // check to see if the line segments intersect at a point sufficiently
  // far from the segment endpoints....
  x = Re((1.0 - z) * q0 + z * q1);
  if ((x < ee) || (x > (1.0 - ee)))
    return FALSE;

  // return resulting intersection point
  p0 = (1.0 - z) * nodelist[n0].CC() + z * nodelist[n1].CC();
  *xi = Re(p0);
  *yi = Im(p0);

  return TRUE;
}

BOOL CbeladrawDoc::DeleteSelectedBlockLabels()
{
  int i = 0;
  BOOL flag = FALSE;

  if (blocklist.GetSize() > 0)
    do {
      if (blocklist[i].IsSelected == TRUE) {
        blocklist.RemoveAt(i, 1);
        flag = TRUE;
      } else
        i++;
    } while (i < blocklist.GetSize());

  blocklist.FreeExtra();
  return flag;
}

BOOL CbeladrawDoc::DeleteSelectedSegments()
{
  int i = 0;
  BOOL flag = FALSE;

  if (linelist.GetSize() > 0)
    do {
      if (linelist[i].IsSelected == TRUE) {
        linelist.RemoveAt(i, 1);
        flag = TRUE;
      } else
        i++;
    } while (i < linelist.GetSize());

  linelist.FreeExtra();

  return flag;
}

BOOL CbeladrawDoc::DeleteSelectedArcSegments()
{
  int i = 0;
  BOOL flag = FALSE;

  if (arclist.GetSize() > 0)
    do {
      if (arclist[i].IsSelected == TRUE) {
        arclist.RemoveAt(i, 1);
        flag = TRUE;
      } else
        i++;
    } while (i < arclist.GetSize());

  arclist.FreeExtra();

  return flag;
}

BOOL CbeladrawDoc::DeleteSelectedNodes()
{
  int i = 0;
  int j;
  BOOL flag = FALSE;

  if (nodelist.GetSize() > 0)
    do {
      if (nodelist[i].IsSelected == TRUE) {
        flag = TRUE;
        // first remove all lines that contain the point;
        for (j = 0; j < linelist.GetSize(); j++)
          if ((linelist[j].n0 == i) || (linelist[j].n1 == i))
            linelist[j].ToggleSelect();
        DeleteSelectedSegments();

        // remove all arcs that contain the point;
        for (j = 0; j < arclist.GetSize(); j++)
          if ((arclist[j].n0 == i) || (arclist[j].n1 == i))
            arclist[j].ToggleSelect();
        DeleteSelectedArcSegments();

        // remove node from the nodelist...
        nodelist.RemoveAt(i, 1);

        // update lines to point to the new node numbering
        for (j = 0; j < linelist.GetSize(); j++) {
          if (linelist[j].n0 > i)
            linelist[j].n0--;
          if (linelist[j].n1 > i)
            linelist[j].n1--;
        }

        // update arcs to point to the new node numbering
        for (j = 0; j < arclist.GetSize(); j++) {
          if (arclist[j].n0 > i)
            arclist[j].n0--;
          if (arclist[j].n1 > i)
            arclist[j].n1--;
        }
      } else
        i++;
    } while (i < nodelist.GetSize());

  nodelist.FreeExtra();

  return flag;
}

void CbeladrawDoc::OnEditMatprops()
{
  // TODO: Add your command handler code here
  bdCPtProp pProp;

  pProp.pblockproplist = &blockproplist;
  pProp.PropType = 2;
  pProp.ProblemType = ProblemType;

  pProp.DoModal();
}

void CbeladrawDoc::OnEditPtprops()
{
  // TODO: Add your command handler code here
  bdCPtProp pProp;

  pProp.pnodeproplist = &nodeproplist;
  pProp.PropType = 0;

  pProp.DoModal();
}

void CbeladrawDoc::OnEditSegprops()
{
  // TODO: Add your command handler code here
  bdCPtProp pProp;

  pProp.plineproplist = &lineproplist;
  pProp.PropType = 1;
  pProp.ProblemType = ProblemType;
  pProp.DoModal();
}

void CbeladrawDoc::OnEditCircprops()
{
  bdCPtProp pProp;

  pProp.pcircproplist = &circproplist;
  pProp.ProblemType = ProblemType;
  pProp.PropType = 3;

  pProp.DoModal();
}

BOOL CbeladrawDoc::OpBlkDlg()
{
  int i, j, k, nselected;
  double a;
  bdCOpBlkDlg zDlg;

  zDlg.ProblemType = ProblemType;

  // check to see how many (if any) blocks are selected.
  for (i = 0, k = 0, nselected = 0; i < blocklist.GetSize(); i++) {
    if (blocklist[i].IsSelected == TRUE) {
      if (nselected == 0) {
        nselected++;
        zDlg.m_ingroup = blocklist[i].InGroup;
      } else if (blocklist[i].BlockType != blocklist[k].BlockType)
        nselected++;
      if (blocklist[i].InGroup != zDlg.m_ingroup)
        zDlg.m_ingroup = 0;
      k = i;
      if (blocklist[i].IsExternal)
        zDlg.m_isexternal = TRUE;
      if (blocklist[i].IsDefault)
        zDlg.m_isdefault = TRUE;
    }
  }
  if (nselected == 0)
    return FALSE;
  if (nselected > 1)
    zDlg.m_isdefault = FALSE;

  // find average block size;
  for (i = 0, j = 0, a = 0.; i < blocklist.GetSize(); i++)
    if (blocklist[i].IsSelected == TRUE)
      if (blocklist[i].MaxArea > a)
        a = blocklist[i].MaxArea;

  zDlg.m_sidelength = floor(2.e07 * sqrt(a / PI) + 0.5) / 1.e07;

  zDlg.pblockproplist = &blockproplist;
  zDlg.pcircproplist = &circproplist;

  if (nselected == 1) {
    if (blocklist[k].BlockType == "<No Mesh>")
      zDlg.cursel = 1;
    else
      for (i = 0, zDlg.cursel = 0; i < blockproplist.GetSize(); i++)
        if (blockproplist[i].BlockName == blocklist[k].BlockType)
          zDlg.cursel = i + 2;
  } else
    zDlg.cursel = 0;

  if (zDlg.DoModal() == IDOK) {
    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].IsSelected == TRUE) {
        blocklist[i].MaxArea = PI * zDlg.m_sidelength * zDlg.m_sidelength / 4.;
        if (zDlg.cursel == 0)
          blocklist[i].BlockType = "<None>";
        else if (zDlg.cursel == 1)
          blocklist[i].BlockType = "<No Mesh>";
        else
          blocklist[i].BlockType = blockproplist[zDlg.cursel - 2].BlockName;
        blocklist[i].InGroup = zDlg.m_ingroup;
        blocklist[i].IsExternal = zDlg.m_isexternal;

        // Should be one and only one label specified as the default;
        if (nselected == 1) {
          if (zDlg.m_isdefault == TRUE) {
            blocklist[i].IsDefault = 2;
            for (j = 0; j < blocklist.GetSize(); j++)
              if (j != i)
                blocklist[j].IsDefault = FALSE;
          } else
            blocklist[i].IsDefault = FALSE;
        }
      }
    }
    return TRUE;
  }

  return FALSE;
}

void CbeladrawDoc::OpNodeDlg()
{
  int i, k, nselected;
  bdCOpNodeDlg zDlg;

  // check to see how many (if any) nodes are selected.
  for (i = 0, k = 0, nselected = 0; i < nodelist.GetSize(); i++) {
    if (nodelist[i].IsSelected == TRUE) {
      if (nselected == 0) {
        nselected++;
        zDlg.m_ingroup = nodelist[i].InGroup;
      } else if (nodelist[i].BoundaryMarker != nodelist[k].BoundaryMarker)
        nselected++;
      if (nodelist[i].InGroup != zDlg.m_ingroup)
        zDlg.m_ingroup = 0;
      k = i;
    }
  }
  if (nselected == 0)
    return;

  zDlg.pnodeproplist = &nodeproplist;
  zDlg.pcircproplist = &circproplist;

  if (nselected == 1) {
    for (i = 0, zDlg.cursel = 0; i < nodeproplist.GetSize(); i++)
      if (nodeproplist[i].PointName == nodelist[k].BoundaryMarker)
        zDlg.cursel = i + 1;

    for (i = 0, zDlg.condsel = 0; i < circproplist.GetSize(); i++)
      if (circproplist[i].CircName == nodelist[k].InConductor)
        zDlg.condsel = i + 1;
  } else {
    zDlg.cursel = 0;
    zDlg.condsel = 0;
  }

  if (zDlg.DoModal() == IDOK) {
    for (i = 0; i < nodelist.GetSize(); i++) {
      if (nodelist[i].IsSelected == TRUE) {
        if (zDlg.cursel == 0)
          nodelist[i].BoundaryMarker = "<None>";
        else
          nodelist[i].BoundaryMarker = nodeproplist[zDlg.cursel - 1].PointName;

        if (zDlg.condsel == 0)
          nodelist[i].InConductor = "<None>";
        else
          nodelist[i].InConductor = circproplist[zDlg.condsel - 1].CircName;

        nodelist[i].InGroup = zDlg.m_ingroup;
      }
    }
  }
}

void CbeladrawDoc::OpSegDlg()
{
  int i, j, k, nselected;
  bdCOpSegDlg zDlg;

  // check to see how many (if any) lines are selected.
  for (i = 0, k = 0, nselected = 0; i < linelist.GetSize(); i++) {
    if (linelist[i].IsSelected == TRUE) {
      if (nselected == 0) {
        nselected++;
        zDlg.m_ingroup = linelist[i].InGroup;
      } else if (linelist[i].BoundaryMarker != linelist[k].BoundaryMarker)
        nselected++;
      if (linelist[i].InGroup != zDlg.m_ingroup)
        zDlg.m_ingroup = 0;
      k = i;
    }
  }
  if (nselected == 0)
    return;

  // find properties block size;
  double ms = 0;
  zDlg.m_hide = FALSE;
  for (i = 0, j = 0; i < linelist.GetSize(); i++)
    if (linelist[i].IsSelected == TRUE) {
      if (linelist[i].MaxSideLength < 0)
        ms = -1;
      if (ms >= 0)
        ms += linelist[k].MaxSideLength;
      j++;
      if (linelist[i].Hidden == TRUE)
        zDlg.m_hide = TRUE;
    }
  ms /= (double)j;

  zDlg.plineproplist = &lineproplist;
  zDlg.pcircproplist = &circproplist;

  if (nselected == 1) {
    for (i = 0, zDlg.cursel = 0; i < lineproplist.GetSize(); i++)
      if (lineproplist[i].BdryName == linelist[k].BoundaryMarker)
        zDlg.cursel = i + 1;

    for (i = 0, zDlg.condsel = 0; i < circproplist.GetSize(); i++)
      if (circproplist[i].CircName == linelist[k].InConductor)
        zDlg.condsel = i + 1;
  } else {
    zDlg.cursel = 0;
    zDlg.condsel = 0;
  }

  if (ms < 0) {
    zDlg.m_automesh = TRUE;
    zDlg.m_linemeshsize = 0;
  } else {
    zDlg.m_automesh = FALSE;
    zDlg.m_linemeshsize = ms;
  }

  if (zDlg.DoModal() == IDOK) {
    for (i = 0; i < linelist.GetSize(); i++) {
      if (linelist[i].IsSelected == TRUE) {

        if (zDlg.m_automesh == TRUE)
          linelist[i].MaxSideLength = -1;
        else {
          if (zDlg.m_linemeshsize > 0)
            linelist[i].MaxSideLength = zDlg.m_linemeshsize;
          else
            zDlg.m_linemeshsize = -1;
        }
        if (zDlg.cursel == 0)
          linelist[i].BoundaryMarker = "<None>";
        else
          linelist[i].BoundaryMarker = lineproplist[zDlg.cursel - 1].BdryName;

        if (zDlg.condsel == 0)
          linelist[i].InConductor = "<None>";
        else
          linelist[i].InConductor = circproplist[zDlg.condsel - 1].CircName;

        linelist[i].Hidden = zDlg.m_hide;
        linelist[i].InGroup = zDlg.m_ingroup;
      }
    }
  }
}

void CbeladrawDoc::OpArcSegDlg()
{
  int i, j, k, nselected;
  bdCOpArcSegDlg zDlg;

  // check to see how many (if any) arcs are selected.

  for (i = 0, k = 0, nselected = 0; i < arclist.GetSize(); i++) {
    if (arclist[i].IsSelected == TRUE) {
      if (nselected == 0) {
        nselected++;
        zDlg.m_ingroup = arclist[i].InGroup;
      } else if (arclist[i].BoundaryMarker != arclist[k].BoundaryMarker)
        nselected++;
      if (arclist[i].InGroup != zDlg.m_ingroup)
        zDlg.m_ingroup = 0;
      k = i;
    }
  }
  if (nselected == 0)
    return;

  // find properties block size;
  double ms = 0;
  zDlg.m_hide = FALSE;
  for (i = 0, j = 0; i < arclist.GetSize(); i++)
    if (arclist[i].IsSelected == TRUE) {
      ms += arclist[k].MaxSideLength;
      j++;
      if (arclist[i].Hidden == TRUE)
        zDlg.m_hide = TRUE;
    }

  ms /= (double)j;

  zDlg.plineproplist = &lineproplist;
  zDlg.pcircproplist = &circproplist;

  if (nselected == 1) {
    zDlg.m_MaxSeg = ms;
    for (i = 0, zDlg.cursel = 0; i < lineproplist.GetSize(); i++)
      if (lineproplist[i].BdryName == arclist[k].BoundaryMarker)
        zDlg.cursel = i + 1;

    for (i = 0, zDlg.condsel = 0; i < circproplist.GetSize(); i++)
      if (circproplist[i].CircName == arclist[k].InConductor)
        zDlg.condsel = i + 1;
  } else {
    zDlg.cursel = 0;
    zDlg.m_MaxSeg = ms;
    zDlg.condsel = 0;
  }

  if (zDlg.DoModal() == IDOK) {
    for (i = 0; i < arclist.GetSize(); i++) {
      if (arclist[i].IsSelected == TRUE) {
        if (zDlg.cursel == 0)
          arclist[i].BoundaryMarker = "<None>";
        else
          arclist[i].BoundaryMarker = lineproplist[zDlg.cursel - 1].BdryName;
        if (zDlg.condsel == 0)
          arclist[i].InConductor = "<None>";
        else
          arclist[i].InConductor = circproplist[zDlg.condsel - 1].CircName;
        arclist[i].MaxSideLength = zDlg.m_MaxSeg;
        arclist[i].Hidden = zDlg.m_hide;
        arclist[i].InGroup = zDlg.m_ingroup;
      }
    }
  }
}

void CbeladrawDoc::OpGrpDlg()
{
  COpGrp dlg;
  BOOL bFlag = FALSE;
  int nsel = 0;
  int i;

  dlg.m_ingroup = 0;

  for (i = 0; i < arclist.GetSize(); i++)
    if (arclist[i].IsSelected == TRUE) {
      nsel++;
      if (dlg.m_ingroup != arclist[i].InGroup) {
        if (bFlag == FALSE) {
          dlg.m_ingroup = arclist[i].InGroup;
          bFlag = TRUE;
        } else
          dlg.m_ingroup = 0;
      }
    }

  for (i = 0; i < linelist.GetSize(); i++)
    if (linelist[i].IsSelected == TRUE) {
      nsel++;
      if (dlg.m_ingroup != linelist[i].InGroup) {
        if (bFlag == FALSE) {
          dlg.m_ingroup = linelist[i].InGroup;
          bFlag = TRUE;
        } else
          dlg.m_ingroup = 0;
      }
    }

  for (i = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].IsSelected == TRUE) {
      nsel++;
      if (dlg.m_ingroup != blocklist[i].InGroup) {
        if (bFlag == FALSE) {
          dlg.m_ingroup = blocklist[i].InGroup;
          bFlag = TRUE;
        } else
          dlg.m_ingroup = 0;
      }
    }

  for (i = 0; i < nodelist.GetSize(); i++)
    if (nodelist[i].IsSelected == TRUE) {
      nsel++;
      if (dlg.m_ingroup != nodelist[i].InGroup) {
        if (bFlag == FALSE) {
          dlg.m_ingroup = nodelist[i].InGroup;
          bFlag = TRUE;
        } else
          dlg.m_ingroup = 0;
      }
    }
  if (nsel == 0)
    return;

  if (dlg.DoModal() == IDOK) {

    for (i = 0; i < nodelist.GetSize(); i++)
      if (nodelist[i].IsSelected == TRUE)
        nodelist[i].InGroup = dlg.m_ingroup;

    for (i = 0; i < linelist.GetSize(); i++)
      if (linelist[i].IsSelected == TRUE)
        linelist[i].InGroup = dlg.m_ingroup;

    for (i = 0; i < arclist.GetSize(); i++)
      if (arclist[i].IsSelected == TRUE)
        arclist[i].InGroup = dlg.m_ingroup;

    for (i = 0; i < blocklist.GetSize(); i++)
      if (blocklist[i].IsSelected == TRUE)
        blocklist[i].InGroup = dlg.m_ingroup;
  }
}

BOOL CbeladrawDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // make sure old document is cleared out...
  OnNewDocument();

  FILE* fp;
  int i, k, t;
  int vers = 0;
  char s[1024], q[1024];
  char* v;
  CPointProp PProp;
  CBoundaryProp BProp;
  CMaterialProp MProp;
  CCircuit CProp;
  CNode node;
  CSegment segm;
  CArcSegment asegm;
  CBlockLabel blk;

  if ((fp = fopen(lpszPathName, "rt")) == NULL) {
    MsgBox("Couldn't read from specified .fee file");
    return FALSE;
  }

  // hook to catch old files where depth wasn't defined:
  Depth = -1;

  // parse the file
  while (fgets(s, 1024, fp) != NULL) {
    if (sscanf(s, "%s", q) == EOF)
      q[0] = NULL;

    //	int _strnicmp( const char *string1, const char *string2, size_t count );

    // Deal with flag for file format version
    if (_strnicmp(q, "[format]", 8) == 0) {
      v = StripKey(s);
      double dblvers;
      sscanf(v, "%lf", &dblvers);
      vers = (int)(10. * dblvers + 0.5);
      if (vers > 33)
        MsgBox("File is from a newer version of FEMM\nThis file may contain attributes not\nsupported by this version of FEMM");
      q[0] = NULL;
    }

    // Depth in the into-the-page direction
    if (_strnicmp(q, "[depth]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &Depth);
      q[0] = NULL;
    }

    // Required solver precision
    if (_strnicmp(q, "[precision]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &Precision);
      q[0] = NULL;
    }

    // Minimum Angle Constraint for finite element mesh
    if (_strnicmp(q, "[minangle]", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MinAngle);
      q[0] = NULL;
    }

    // Smart Mesh flag
    if (_strnicmp(q, "[dosmartmesh]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &SmartMesh);
      q[0] = NULL;
    }

    // Units of length used by the problem
    if (_strnicmp(q, "[lengthunits]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%s", q);
      if (_strnicmp(q, "inches", 6) == 0)
        LengthUnits = 0;
      else if (_strnicmp(q, "millimeters", 11) == 0)
        LengthUnits = 1;
      else if (_strnicmp(q, "centimeters", 1) == 0)
        LengthUnits = 2;
      else if (_strnicmp(q, "mils", 4) == 0)
        LengthUnits = 4;
      else if (_strnicmp(q, "microns", 6) == 0)
        LengthUnits = 5;
      else if (_strnicmp(q, "meters", 6) == 0)
        LengthUnits = 3;
      q[0] = NULL;
    }

    // Problem Type (planar or axisymmetric)
    if (_strnicmp(q, "[problemtype]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%s", q);
      if (_strnicmp(q, "planar", 6) == 0)
        ProblemType = 0;
      if (_strnicmp(q, "axisymmetric", 3) == 0)
        ProblemType = 1;
      q[0] = NULL;
    }

    // Coordinates (cartesian or polar)
    if (_strnicmp(q, "[coordinates]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%s", q);
      if (_strnicmp(q, "cartesian", 4) == 0)
        Coords = 0;
      if (_strnicmp(q, "polar", 5) == 0)
        Coords = 1;
      q[0] = NULL;
    }

    // Comments
    if (_strnicmp(q, "[comment]", 9) == 0) {
      v = StripKey(s);
      // put in carriage returns;
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if ((v[i] == '\\') && (v[i + 1] == 'n')) {
          v[i] = 13;
          v[i + 1] = 10;
        }

      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      ProblemNote = v;
      q[0] = NULL;
    }

    // properties for axisymmetric external region
    if (_strnicmp(q, "[extzo]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &extZo);
      q[0] = NULL;
    }

    if (_strnicmp(q, "[extro]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &extRo);
      q[0] = NULL;
    }

    if (_strnicmp(q, "[extri]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &extRi);
      q[0] = NULL;
    }

    // Point Properties
    if (_strnicmp(q, "<beginpoint>", 11) == 0) {
      PProp.PointName = "New Point Property";
      PProp.V = 0;
      PProp.qp = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<pointname>", 11) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      PProp.PointName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<vp>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.V);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qp>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.qp);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endpoint>", 9) == 0) {
      nodeproplist.Add(PProp);
      q[0] = NULL;
    }

    // Boundary Properties;
    if (_strnicmp(q, "<beginbdry>", 11) == 0) {
      BProp.BdryName = "New Boundary";
      BProp.BdryFormat = 0;
      BProp.V = 0;
      BProp.qs = 0;
      BProp.c0 = 0;
      BProp.c1 = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<bdryname>", 10) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      BProp.BdryName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<bdrytype>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &BProp.BdryFormat);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<vs>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.V);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qs>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.qs);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c0>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c0);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c1>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c1);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endbdry>", 9) == 0) {
      lineproplist.Add(BProp);
      q[0] = NULL;
    }

    // Block Properties;
    if (_strnicmp(q, "<beginblock>", 12) == 0) {
      MProp.BlockName = "New Material";
      MProp.ex = 1.;
      MProp.ey = 1.; // permeabilities, relative
      MProp.qv = 0.;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<blockname>", 10) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      MProp.BlockName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<ex>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.ex);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<ey>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.ey);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qv>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.qv);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endblock>", 9) == 0) {
      blockproplist.Add(MProp);
      q[0] = NULL;
    }

    // Circuit Properties
    if (_strnicmp(q, "<beginconductor>", 16) == 0) {
      CProp.CircName = "New Conductor";
      CProp.V = 0.;
      CProp.q = 0.;
      CProp.CircType = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<conductorname>", 15) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      CProp.CircName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<vc>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &CProp.V);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qc>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &CProp.q);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<conductortype>", 15) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &CProp.CircType);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endconductor>", 14) == 0) {
      circproplist.Add(CProp);
      q[0] = NULL;
    }

    // Points list;
    if (_strnicmp(q, "[numpoints]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);
        v = ParseDbl(s, &node.x);
        v = ParseDbl(v, &node.y);
        v = ParseInt(v, &t);
        if (t == 0)
          node.BoundaryMarker = "";
        else if (t <= (int)nodeproplist.GetSize())
          node.BoundaryMarker = nodeproplist[t - 1].PointName;
        v = ParseInt(v, &node.InGroup);
        v = ParseInt(v, &t);
        if (t == 0)
          node.InConductor = "<None>";
        else if (t <= (int)circproplist.GetSize())
          node.InConductor = circproplist[t - 1].CircName;
        nodelist.Add(node);
      }
      q[0] = NULL;
    }

    // read in segment list
    if (_strnicmp(q, "[numsegments]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);

        // some defaults
        segm.MaxSideLength = 0;
        t = 0;
        segm.Hidden = 0;
        segm.InGroup = 0;

        // scan in data
        v = ParseInt(s, &segm.n0);
        v = ParseInt(v, &segm.n1);
        v = ParseDbl(v, &segm.MaxSideLength);
        v = ParseInt(v, &t);
        if (t == 0)
          segm.BoundaryMarker = "";
        else if (t <= (int)lineproplist.GetSize())
          segm.BoundaryMarker = lineproplist[t - 1].BdryName;
        v = ParseInt(v, &segm.Hidden);
        v = ParseInt(v, &segm.InGroup);
        v = ParseInt(v, &t);
        if (t == 0)
          segm.InConductor = "<None>";
        else if (t <= (int)circproplist.GetSize())
          segm.InConductor = circproplist[t - 1].CircName;

        linelist.Add(segm);
      }
      q[0] = NULL;
    }

    // read in arc segment list
    if (_strnicmp(q, "[numarcsegments]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);

        // some defaults
        asegm.Hidden = 0;
        asegm.MaxSideLength = -1.;
        asegm.InGroup = 0;
        t = 0;

        // scan in data
        v = ParseInt(s, &asegm.n0);
        v = ParseInt(v, &asegm.n1);
        v = ParseDbl(v, &asegm.ArcLength);
        v = ParseDbl(v, &asegm.MaxSideLength);
        v = ParseInt(v, &t);
        if (t == 0)
          asegm.BoundaryMarker = "";
        else if (t <= (int)lineproplist.GetSize())
          asegm.BoundaryMarker = lineproplist[t - 1].BdryName;
        v = ParseInt(v, &asegm.Hidden);
        v = ParseInt(v, &asegm.InGroup);
        v = ParseInt(v, &t);
        if (t == 0)
          asegm.InConductor = "<None>";
        else if (t <= (int)circproplist.GetSize())
          asegm.InConductor = circproplist[t - 1].CircName;
        asegm.mySideLength = asegm.MaxSideLength;
        arclist.Add(asegm);
      }
      q[0] = NULL;
    }

    // read in list of holes;
    if (_strnicmp(q, "[numholes]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0) {
        blk.BlockType = "<No Mesh>";
        blk.MaxArea = 0;
        blk.InGroup = 0;
        for (i = 0; i < k; i++) {
          fgets(s, 1024, fp);
          v = ParseDbl(s, &blk.x);
          v = ParseDbl(v, &blk.y);
          v = ParseInt(v, &blk.InGroup);

          blocklist.Add(blk);
        }
      }
      q[0] = NULL;
    }

    // read in regional attributes
    if (_strnicmp(q, "[numblocklabels]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);

        // some defaults
        t = 0;
        blk.MaxArea = 0.;
        blk.InGroup = 0;
        blk.IsExternal = 0;
        blk.IsDefault = 0;

        // scan in data
        v = ParseDbl(s, &blk.x);
        v = ParseDbl(v, &blk.y);

        v = ParseInt(v, &t);
        if (t == 0)
          blk.BlockType = "<None>";
        else if (t <= (int)blockproplist.GetSize())
          blk.BlockType = blockproplist[t - 1].BlockName;

        v = ParseDbl(v, &blk.MaxArea);
        if (blk.MaxArea < 0)
          blk.MaxArea = 0;
        else
          blk.MaxArea = PI * blk.MaxArea * blk.MaxArea / 4.;

        v = ParseInt(v, &blk.InGroup);
        v = ParseInt(v, &blk.IsExternal);
        blk.IsDefault = blk.IsExternal & 2;
        blk.IsExternal = blk.IsExternal & 1;

        blocklist.Add(blk);
      }
      q[0] = NULL;
    }
  }

  fclose(fp);

  if (Depth == -1) {
    // if this is a v3.2 file we are importing, make the length
    // equal to 1 meter, because 3.2 was all per-meter calculations
    switch (LengthUnits) {
    case 1:
      Depth = 1000.; // mm
      break;
    case 2:
      Depth = 100.; // cm
      break;
    case 3:
      Depth = 1.; // m
      break;
    case 4:
      Depth = 1000. / 0.0254; // mils
      break;
    case 5:
      Depth = 1.e6; // microns
      break;
    default:
      Depth = 1. / 0.0254; // inches
      break;
    }
  }

  FirstDraw = TRUE;

  return TRUE;
}

BOOL CbeladrawDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
  // TODO: Add your specialized code here and/or call the base class
  FILE* fp;
  int i, j, k, t;
  CString s;

  // check to see if we are ready to write a datafile;
  if ((fp = fopen(lpszPathName, "wt")) == NULL) {
    for (k = 0; k < 10; k++) {
      Sleep(10 + k * 20);
      if ((fp = fopen(lpszPathName, "wt")) != NULL)
        break;
    }
    if (k == 10) {
      MsgBox("Couldn't write to specified file.\nPerhaps the file is write-protected?");
      return FALSE;
    }
  }

  fprintf(fp, "[Format]      =  1\n");
  fprintf(fp, "[Precision]   =  %.17g\n", Precision);
  fprintf(fp, "[MinAngle]    =  %.17g\n", MinAngle);
  fprintf(fp, "[DoSmartMesh] =  %i\n", SmartMesh);
  fprintf(fp, "[Depth]       =  %.17g\n", Depth);
  fprintf(fp, "[LengthUnits] =  ");
  switch (LengthUnits) {
  case 1:
    fprintf(fp, "millimeters\n");
    break;
  case 2:
    fprintf(fp, "centimeters\n");
    break;
  case 3:
    fprintf(fp, "meters\n");
    break;
  case 4:
    fprintf(fp, "mils\n");
    break;
  case 5:
    fprintf(fp, "microns\n");
    break;
  default:
    fprintf(fp, "inches\n");
    break;
  }

  if (ProblemType == 0)
    fprintf(fp, "[ProblemType] =  planar\n");
  else {
    fprintf(fp, "[ProblemType] =  axisymmetric\n");
    if ((extRo != 0) && (extRi != 0)) {
      fprintf(fp, "[extZo] = %.17g\n", extZo);
      fprintf(fp, "[extRo] = %.17g\n", extRo);
      fprintf(fp, "[extRi] = %.17g\n", extRi);
    }
  }

  if (Coords == 0)
    fprintf(fp, "[Coordinates] =  cartesian\n");
  else
    fprintf(fp, "[Coordinates] =  polar\n");

  s = ProblemNote;
  for (i = 0; i < ProblemNote.GetLength(); i++) {
    if (s[i] == 13)
      s.SetAt(i, '\\');
    if (s[i] == 10)
      s.SetAt(i, 'n');
  }
  fprintf(fp, "[Comment]     =  \"%s\"\n", (const char*)s);

  // write out materials properties stuff...
  fprintf(fp, "[PointProps]   = %i\n", nodeproplist.GetSize());
  for (i = 0; i < nodeproplist.GetSize(); i++) {
    fprintf(fp, "  <BeginPoint>\n");
    fprintf(fp, "    <PointName> = \"%s\"\n", (const char*)nodeproplist[i].PointName);
    fprintf(fp, "    <Vp> = %.17g\n", nodeproplist[i].V);
    fprintf(fp, "    <qp> = %.17g\n", nodeproplist[i].qp);
    fprintf(fp, "  <EndPoint>\n");
  }

  fprintf(fp, "[BdryProps]   = %i\n", lineproplist.GetSize());
  for (i = 0; i < lineproplist.GetSize(); i++) {
    fprintf(fp, "  <BeginBdry>\n");
    fprintf(fp, "    <BdryName> = \"%s\"\n", (const char*)lineproplist[i].BdryName);
    fprintf(fp, "    <BdryType> = %i\n", lineproplist[i].BdryFormat);
    fprintf(fp, "    <Vs> = %.17g\n", lineproplist[i].V);
    fprintf(fp, "    <qs> = %.17g\n", lineproplist[i].qs);
    fprintf(fp, "    <c0> = %.17g\n", lineproplist[i].c0);
    fprintf(fp, "    <c1> = %.17g\n", lineproplist[i].c1);
    fprintf(fp, "  <EndBdry>\n");
  }

  fprintf(fp, "[BlockProps]  = %i\n", blockproplist.GetSize());
  for (i = 0; i < blockproplist.GetSize(); i++) {
    fprintf(fp, "  <BeginBlock>\n");
    fprintf(fp, "    <BlockName> = \"%s\"\n", (const char*)blockproplist[i].BlockName);
    fprintf(fp, "    <ex> = %.17g\n", blockproplist[i].ex);
    fprintf(fp, "    <ey> = %.17g\n", blockproplist[i].ey);
    fprintf(fp, "    <qv> = %.17g\n", blockproplist[i].qv);
    fprintf(fp, "  <EndBlock>\n");
  }

  fprintf(fp, "[ConductorProps]  = %i\n", circproplist.GetSize());
  for (i = 0; i < circproplist.GetSize(); i++) {
    fprintf(fp, "  <BeginConductor>\n");
    fprintf(fp, "    <ConductorName> = \"%s\"\n", (const char*)circproplist[i].CircName);
    fprintf(fp, "    <Vc> = %.17g\n", circproplist[i].V);
    fprintf(fp, "    <qc> = %.17g\n", circproplist[i].q);
    fprintf(fp, "    <ConductorType> = %i\n", circproplist[i].CircType);
    fprintf(fp, "  <EndConductor>\n");
  }

  // write out node list
  fprintf(fp, "[NumPoints] = %i\n", nodelist.GetSize());
  for (i = 0; i < nodelist.GetSize(); i++) {
    for (j = 0, t = 0; j < nodeproplist.GetSize(); j++)
      if (nodeproplist[j].PointName == nodelist[i].BoundaryMarker)
        t = j + 1;
    fprintf(fp, "%.17g	%.17g	%i	%i	", nodelist[i].x, nodelist[i].y, t,
        nodelist[i].InGroup);

    for (j = 0, t = 0; j < circproplist.GetSize(); j++)
      if (circproplist[j].CircName == nodelist[i].InConductor)
        t = j + 1;
    fprintf(fp, "%i\n", t);
  }

  // write out segment list
  fprintf(fp, "[NumSegments] = %i\n", linelist.GetSize());
  for (i = 0; i < linelist.GetSize(); i++) {
    for (j = 0, t = 0; j < lineproplist.GetSize(); j++)
      if (lineproplist[j].BdryName == linelist[i].BoundaryMarker)
        t = j + 1;
    fprintf(fp, "%i	%i	", linelist[i].n0, linelist[i].n1);
    if (linelist[i].MaxSideLength < 0)
      fprintf(fp, "-1	");
    else
      fprintf(fp, "%.17g	", linelist[i].MaxSideLength);
    fprintf(fp, "%i	%i	%i	", t, linelist[i].Hidden, linelist[i].InGroup);

    for (j = 0, t = 0; j < circproplist.GetSize(); j++)
      if (circproplist[j].CircName == linelist[i].InConductor)
        t = j + 1;
    fprintf(fp, "%i\n", t);
  }

  // write out arc segment list
  fprintf(fp, "[NumArcSegments] = %i\n", arclist.GetSize());
  for (i = 0; i < arclist.GetSize(); i++) {
    for (j = 0, t = 0; j < lineproplist.GetSize(); j++)
      if (lineproplist[j].BdryName == arclist[i].BoundaryMarker)
        t = j + 1;
    fprintf(fp, "%i	%i	%.17g	%.17g	%i	%i	%i	", arclist[i].n0, arclist[i].n1,
        arclist[i].ArcLength, arclist[i].MaxSideLength, t,
        arclist[i].Hidden, arclist[i].InGroup);

    for (j = 0, t = 0; j < circproplist.GetSize(); j++)
      if (circproplist[j].CircName == arclist[i].InConductor)
        t = j + 1;
    fprintf(fp, "%i	%.17g\n", t, arclist[i].mySideLength);
  }

  // write out list of holes;
  for (i = 0, j = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>")
      j++;
  fprintf(fp, "[NumHoles] = %i\n", j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType == "<No Mesh>") {
      fprintf(fp, "%.17g	%.17g	%i\n", blocklist[i].x, blocklist[i].y,
          blocklist[i].InGroup);
      k++;
    }

  // write out regional attributes
  fprintf(fp, "[NumBlockLabels] = %i\n", blocklist.GetSize() - j);
  for (i = 0, k = 0; i < blocklist.GetSize(); i++)
    if (blocklist[i].BlockType != "<No Mesh>") {
      fprintf(fp, "%.17g	%.17g	", blocklist[i].x, blocklist[i].y);
      for (j = 0, t = 0; j < blockproplist.GetSize(); j++)
        if (blockproplist[j].BlockName == blocklist[i].BlockType)
          t = j + 1;
      fprintf(fp, "%i	", t);
      if (blocklist[i].MaxArea > 0)
        fprintf(fp, "%.17g	", sqrt(4. * blocklist[i].MaxArea / PI));
      else
        fprintf(fp, "-1	");

      fprintf(fp, "%i	%i\n", blocklist[i].InGroup,
          blocklist[i].IsExternal + blocklist[i].IsDefault);
      k++;
    }
  fclose(fp);

  return TRUE;
}

BOOL CbeladrawDoc::LoadMesh()
{
  int i, j, k, q, nl;
  CString pathname, rootname, infile;
  FILE* fp;
  char s[1024];

  // clear out the old mesh...
  meshnode.RemoveAll();
  meshline.RemoveAll();
  greymeshline.RemoveAll();

  pathname = GetPathName();
  if (pathname.GetLength() == 0) {
    MsgBox("No mesh to display");
    return FALSE;
  }

  rootname = pathname.Left(pathname.ReverseFind('.'));

  // read meshnodes;
  infile = rootname + ".node";
  if ((fp = fopen(infile, "rt")) == NULL) {
    MsgBox("No mesh to display");
    return FALSE;
  }
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);
  meshnode.SetSize(k);
  CNode node;
  for (i = 0; i < k; i++) {
    fgets(s, 1024, fp);
    sscanf(s, "%i	%lf	%lf", &j, &node.x, &node.y);
    meshnode.SetAt(i, node);
  }
  fclose(fp);

  // read meshlines;
  infile = rootname + ".edge";
  if ((fp = fopen(infile, "rt")) == NULL) {
    MsgBox("No mesh to display");
    return FALSE;
  }
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);
  meshline.SetSize(k);
  fclose(fp);

  infile = rootname + ".ele";
  if ((fp = fopen(infile, "rt")) == NULL) {
    MsgBox("No mesh to display");
    return FALSE;
  }
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);

  CPoint segm;
  int n[3], p;
  for (i = 0, nl = 0; i < k; i++) {
    fgets(s, 1024, fp);
    sscanf(s, "%i	%i	%i	%i	%i", &q, &n[0], &n[1], &n[2], &j);
    for (q = 0; q < 3; q++) {
      p = q + 1;
      if (p == 3)
        p = 0;
      if (n[p] > n[q]) {
        segm.x = n[p];
        segm.y = n[q];
        if (j != 0)
          meshline.SetAt(nl++, segm);
        else
          greymeshline.Add(segm);
      }
    }
  }
  meshline.SetSize(nl);
  fclose(fp);

  // clear out temporary files
  infile = rootname + ".ele";
  DeleteFile(infile);
  infile = rootname + ".node";
  DeleteFile(infile);
  infile = rootname + ".edge";
  DeleteFile(infile);
  infile = rootname + ".pbc";
  DeleteFile(infile);
  infile = rootname + ".poly";
  DeleteFile(infile);

  return TRUE;
}

void CbeladrawDoc::UpdateUndo()
{

  int i;

  undonodelist.RemoveAll();
  undolinelist.RemoveAll();
  undoarclist.RemoveAll();
  undoblocklist.RemoveAll();

  for (i = 0; i < nodelist.GetSize(); i++)
    undonodelist.Add(nodelist[i]);
  for (i = 0; i < linelist.GetSize(); i++)
    undolinelist.Add(linelist[i]);
  for (i = 0; i < arclist.GetSize(); i++)
    undoarclist.Add(arclist[i]);
  for (i = 0; i < blocklist.GetSize(); i++)
    undoblocklist.Add(blocklist[i]);
}

void CbeladrawDoc::Undo()
{
  int i;

  CArray<CNode, CNode&> tempnodelist;
  CArray<CSegment, CSegment&> templinelist;
  CArray<CArcSegment, CArcSegment&> temparclist;
  CArray<CBlockLabel, CBlockLabel&> tempblocklist;

  tempnodelist.RemoveAll();
  templinelist.RemoveAll();
  temparclist.RemoveAll();
  tempblocklist.RemoveAll();

  for (i = 0; i < nodelist.GetSize(); i++)
    tempnodelist.Add(nodelist[i]);
  for (i = 0; i < linelist.GetSize(); i++)
    templinelist.Add(linelist[i]);
  for (i = 0; i < arclist.GetSize(); i++)
    temparclist.Add(arclist[i]);
  for (i = 0; i < blocklist.GetSize(); i++)
    tempblocklist.Add(blocklist[i]);

  nodelist.RemoveAll();
  linelist.RemoveAll();
  arclist.RemoveAll();
  blocklist.RemoveAll();

  for (i = 0; i < undonodelist.GetSize(); i++)
    nodelist.Add(undonodelist[i]);
  for (i = 0; i < undolinelist.GetSize(); i++)
    linelist.Add(undolinelist[i]);
  for (i = 0; i < undoarclist.GetSize(); i++)
    arclist.Add(undoarclist[i]);
  for (i = 0; i < undoblocklist.GetSize(); i++)
    blocklist.Add(undoblocklist[i]);

  undonodelist.RemoveAll();
  undolinelist.RemoveAll();
  undoarclist.RemoveAll();
  undoblocklist.RemoveAll();

  for (i = 0; i < tempnodelist.GetSize(); i++)
    undonodelist.Add(tempnodelist[i]);
  for (i = 0; i < templinelist.GetSize(); i++)
    undolinelist.Add(templinelist[i]);
  for (i = 0; i < temparclist.GetSize(); i++)
    undoarclist.Add(temparclist[i]);
  for (i = 0; i < tempblocklist.GetSize(); i++)
    undoblocklist.Add(tempblocklist[i]);
}

BOOL CbeladrawDoc::ScanPreferences()
{
  FILE* fp;
  CString fname;

  fname = BinDir + "beladraw.cfg";

  fp = fopen(fname, "rt");
  if (fp != NULL) {
    BOOL flag = FALSE;
    char s[1024];
    char q[1024];
    char* v;

    // parse the file
    while (fgets(s, 1024, fp) != NULL) {
      sscanf(s, "%s", q);

      if (_strnicmp(q, "<Precision>", 11) == 0) {
        v = StripKey(s);
        sscanf(v, "%lf", &d_prec);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<MinAngle>", 10) == 0) {
        v = StripKey(s);
        sscanf(v, "%lf", &d_minangle);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Depth>", 7) == 0) {
        v = StripKey(s);
        sscanf(v, "%lf", &d_depth);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<Coordinates>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_coord);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<LengthUnits>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_length);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ProblemType>", 13) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_type);
        q[0] = NULL;
      }
    }
    fclose(fp);
    return TRUE;
  }

  return FALSE;
}

BOOL CbeladrawDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;
  return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CbeladrawDoc::OnEditExterior()
{
  // TODO: Add your command handler code here
  CExteriorProps dlg;

  dlg.m_Ro = extRo;
  dlg.m_Ri = extRi;
  dlg.m_Zo = extZo;
  if (dlg.DoModal() == IDOK) {
    extRo = dlg.m_Ro;
    extRi = dlg.m_Ri;
    extZo = dlg.m_Zo;
  }
}

BOOL CbeladrawDoc::CanCreateRadius(int n)
{
  // check to see if a selected point, specified by n, can be
  // converted to a radius.  To be able to be converted to a radius,
  // the point must be an element of either 2 lines, 2 arcs, or
  // 1 line and 1 arc.

  int j, k;

  for (k = 0, j = 0; k < linelist.GetSize(); k++)
    if ((linelist[k].n0 == n) || (linelist[k].n1 == n))
      j++;
  for (k = 0; k < arclist.GetSize(); k++)
    if ((arclist[k].n0 == n) || (arclist[k].n1 == n))
      j++;

  if (j == 2)
    return TRUE;

  return FALSE;
}

BOOL CbeladrawDoc::CreateRadius(int n, double r)
{
  // replace the node indexed by n with a radius of r

  if (r <= 0)
    return FALSE;

  int nseg = 0;
  int narc = 0;
  int k, arc[2], seg[2];

  for (k = 0; k < linelist.GetSize(); k++)
    if ((linelist[k].n0 == n) || (linelist[k].n1 == n))
      seg[nseg++] = k;
  for (k = 0; k < arclist.GetSize(); k++)
    if ((arclist[k].n0 == n) || (arclist[k].n1 == n))
      arc[narc++] = k;
  if ((narc + nseg) != 2)
    return FALSE;

  // there are three valid cases:
  switch (nseg - narc) {
  case 0: // One arc and one line
  {
    CComplex c, u, p0, p1, q, p[4], v[8], i1[8], i2[8];
    double rc, b, R[4], phi;
    CArcSegment ar;
    int j, m;

    // inherit the boundary condition from the arc so that
    // we can apply it to the newly created arc later;
    ar.InGroup = arclist[arc[0]].InGroup;
    ar.BoundaryMarker = arclist[arc[0]].BoundaryMarker;

    // get the center and radius of the circle associated with the arc;
    GetCircle(arclist[arc[0]], c, rc);

    // get the locations of the endpoints of the segment;
    p0 = nodelist[n].CC();
    if (linelist[seg[0]].n0 == n)
      p1 = nodelist[linelist[seg[0]].n1].CC();
    else
      p1 = nodelist[linelist[seg[0]].n0].CC();

    u = (p1 - p0) / abs(p1 - p0); // unit vector along the line
    q = p0 + u * Re((c - p0) / u); // closest point on line to center of circle
    u = I * u; // unit normal to the line

    p[0] = q + r * u;
    R[0] = rc + r;
    p[1] = q - r * u;
    R[1] = rc + r;
    p[2] = q + r * u;
    R[2] = rc - r;
    p[3] = q - r * u;
    R[3] = rc - r;

    for (j = 0, k = 0; k < 4; k++) {
      b = R[k] * R[k] - abs(p[k] - c) * abs(p[k] - c);
      if (b >= 0) {
        b = sqrt(b);
        v[j++] = p[k] + I * b * (p[k] - c) / abs(p[k] - c);
        v[j++] = p[k] - I * b * (p[k] - c) / abs(p[k] - c);
      }
    }

    // locations of the centerpoints that could be for the radius that
    // we are looking for are stored in v.  We now need to paw through
    // them to find the one solution that we are after.
    u = (p1 - p0) / abs(p1 - p0); // unit vector along the line
    for (m = 0, k = 0; k < j; k++) {
      i1[m] = p0 + u * Re((v[k] - p0) / u); // intersection with the line
      i2[m] = c + rc * (v[k] - c) / abs(v[k] - c); // intersection with the arc;
      v[m] = v[k];

      // add this one to the list of possibly valid solutions if
      // both of the intersection points actually lie on the arc
      if (ShortestDistanceFromArc(i2[m], arclist[arc[0]]) < (r / 10000.) && ShortestDistance(Re(i1[m]), Im(i1[m]), seg[0]) < (r / 10000.)
          && abs(i1[m] - i2[m]) > (r / 10000.)) {
        m++;
        if (m == 2)
          break;
      }
    }

    if (m == 0)
      return FALSE;

    // But there are also special cases where there could be two answers.
    // We then pick the solution that has the center point closest to the point to be removed.
    if (m > 1) {
      if (abs(v[0] - p0) < abs(v[1] - p0))
        j = 0;
      else
        j = 1;
    } else
      j = 0; // The index of the winning case is in j....

    UpdateUndo();
    AddNode(Re(i1[j]), Im(i1[j]), r / 10000.);
    AddNode(Re(i2[j]), Im(i2[j]), r / 10000.);
    UnselectAll();

    // delete the node that is to be replace by a radius;
    n = ClosestNode(Re(p0), Im(p0));
    nodelist[n].IsSelected = TRUE;
    DeleteSelectedNodes();

    // compute the angle spanned by the new arc;
    phi = arg((i2[j] - v[j]) / (i1[j] - v[j]));
    if (phi < 0) {
      c = i2[j];
      i2[j] = i1[j];
      i1[j] = c;
      phi = fabs(phi);
    }

    // add in the new radius;
    ar.n0 = ClosestNode(Re(i1[j]), Im(i1[j]));
    ar.n1 = ClosestNode(Re(i2[j]), Im(i2[j]));
    ar.ArcLength = phi / DEG;
    AddArcSegment(ar);

    return TRUE;
  }
  case 2: // Two lines
  {
    CComplex p0, p1, p2;
    double phi, len;
    CArcSegment ar;

    if (linelist[seg[0]].n0 == n)
      p1 = nodelist[linelist[seg[0]].n1].CC();
    else
      p1 = nodelist[linelist[seg[0]].n0].CC();

    if (linelist[seg[1]].n0 == n)
      p2 = nodelist[linelist[seg[1]].n1].CC();
    else
      p2 = nodelist[linelist[seg[1]].n0].CC();

    p0 = nodelist[n].CC();

    // get the angle between the lines
    phi = arg((p2 - p0) / (p1 - p0));

    // check to see if this case is degenerate
    if (fabs(phi) > (179. * DEG))
      return FALSE;

    // check to see if the points are in the wrong order
    // and fix it if they are.
    if (phi < 0) {
      p0 = p1;
      p1 = p2;
      p2 = p0;
      p0 = nodelist[n].CC();
      k = seg[0];
      seg[0] = seg[1];
      seg[1] = k;
      phi = fabs(phi);
    }

    len = r / tan(phi / 2.); // distance from p0 to the tangency point;

    // catch the case where the desired radius is too big to fit;
    if ((abs(p1 - p0) < len) || (abs(p2 - p0) < len))
      return FALSE;

    // compute the locations of the tangency points;
    p1 = len * (p1 - p0) / abs(p1 - p0) + p0;
    p2 = len * (p2 - p0) / abs(p2 - p0) + p0;

    // inherit the boundary condition from one of the segments
    // so that we can apply it to the newly created arc later;
    ar.BoundaryMarker = linelist[seg[0]].BoundaryMarker;
    ar.InGroup = linelist[seg[0]].InGroup;

    // add new nodes at ends of radius
    UpdateUndo();
    AddNode(Re(p1), Im(p1), len / 10000.);
    AddNode(Re(p2), Im(p2), len / 10000.);
    UnselectAll();

    // delete the node that is to be replace by a radius;
    n = ClosestNode(Re(p0), Im(p0));
    nodelist[n].IsSelected = TRUE;
    DeleteSelectedNodes();

    // add in the new radius;
    ar.n0 = ClosestNode(Re(p2), Im(p2));
    ar.n1 = ClosestNode(Re(p1), Im(p1));
    ar.ArcLength = 180. - phi / DEG;
    AddArcSegment(ar);

    return TRUE;
  }
  case -2: // Two arcs
  {
    int j;
    CComplex c0, c1, c2, p[8], i1[8], i2[8];
    double a[8], b[8], c, d[8], x[8], r0, r1, r2, phi;
    CArcSegment ar;

    r0 = r;
    GetCircle(arclist[arc[0]], c1, r1);
    GetCircle(arclist[arc[1]], c2, r2);
    c = abs(c2 - c1);

    // solve for all of the different possible cases;
    a[0] = r1 + r0;
    b[0] = r2 + r0;
    a[1] = r1 + r0;
    b[1] = r2 + r0;
    a[2] = r1 - r0;
    b[2] = r2 - r0;
    a[3] = r1 - r0;
    b[3] = r2 - r0;
    a[4] = r1 - r0;
    b[4] = r2 + r0;
    a[5] = r1 - r0;
    b[5] = r2 + r0;
    a[6] = r1 + r0;
    b[6] = r2 - r0;
    a[7] = r1 + r0;
    b[7] = r2 - r0;
    for (k = 0; k < 8; k++) {
      x[k] = (b[k] * b[k] + c * c - a[k] * a[k]) / (2. * c * c);
      d[k] = sqrt(b[k] * b[k] - x[k] * x[k] * c * c);
    }
    for (k = 0; k < 8; k += 2) {
      // solve for the center point of the radius for each solution.
      p[k] = ((1 - x[k]) * c + I * d[k]) * (c2 - c1) / abs(c2 - c1) + c1;
      p[k + 1] = ((1 - x[k]) * c - I * d[k]) * (c2 - c1) / abs(c2 - c1) + c1;
    }

    c0 = nodelist[n].CC();

    for (j = 0, k = 0; k < 8; k++) {
      i1[j] = c1 + r1 * (p[k] - c1) / abs(p[k] - c1); // compute possible intersection points
      i2[j] = c2 + r2 * (p[k] - c2) / abs(p[k] - c2); // with the arcs;
      p[j] = p[k];

      // add this one to the list of possibly valid solutions if
      // both of the intersection points actually lie on the arc
      if (ShortestDistanceFromArc(i1[j], arclist[arc[0]]) < (r0 / 10000.) && ShortestDistanceFromArc(i2[j], arclist[arc[1]]) < (r0 / 10000.) && abs(i1[j] - i2[j]) > (r0 / 10000.)) {
        j++;
        if (j == 2)
          break;
      }
    }

    // There could be no valid solutions...
    if (j == 0)
      return FALSE;

    // But there are also special cases where there could be two answers.
    // We then pick the solution that has the center point closest to the point to be removed.
    if (j > 1) {
      if (abs(p[0] - c0) < abs(p[1] - c0))
        j = 0;
      else
        j = 1;
    } else
      j = 0; // The index of the winning case is in j....

    // inherit the boundary condition from one of the segments
    // so that we can apply it to the newly created arc later;
    ar.BoundaryMarker = arclist[arc[0]].BoundaryMarker;
    ar.InGroup = arclist[arc[0]].InGroup;

    // add new nodes at ends of radius
    UpdateUndo();
    AddNode(Re(i1[j]), Im(i1[j]), c / 10000.);
    AddNode(Re(i2[j]), Im(i2[j]), c / 10000.);
    UnselectAll();

    // delete the node that is to be replace by a radius;
    n = ClosestNode(Re(c0), Im(c0));
    nodelist[n].IsSelected = TRUE;
    DeleteSelectedNodes();

    // compute the angle spanned by the new arc;
    phi = arg((i2[j] - p[j]) / (i1[j] - p[j]));
    if (phi < 0) {
      c0 = i2[j];
      i2[j] = i1[j];
      i1[j] = c0;
      phi = fabs(phi);
    }

    // add in the new radius;
    ar.n0 = ClosestNode(Re(i1[j]), Im(i1[j]));
    ar.n1 = ClosestNode(Re(i2[j]), Im(i2[j]));
    ar.ArcLength = phi / DEG;
    AddArcSegment(ar);

    return TRUE;
  }
  }

  return FALSE;
}