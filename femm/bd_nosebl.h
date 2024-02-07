namespace beladrawdata {

/////////////////////////////////////////////////////////////////////////////
// CNode -- structure that holds information about each control point.

class CNode {
  public:
  CNode();

  double x, y;
  int xs, ys;
  BOOL IsSelected;
  CString BoundaryMarker;
  CString InConductor;
  int InGroup;

  double GetDistance(double xo, double yo);
  CComplex CC();
  void ToggleSelect();

  private:
};

/////////////////////////////////////////////////////////////////////////////
// CSegment -- structure that holds information about lines joining control pts

class CSegment {
  public:
  CSegment();

  int n0, n1;
  BOOL IsSelected;
  BOOL Hidden;
  double MaxSideLength;
  CString BoundaryMarker;
  CString InConductor;
  int InGroup;

  void ToggleSelect();

  private:
};

class CArcSegment {
  public:
  CArcSegment();

  int n0, n1;
  BOOL NormalDirection;
  BOOL IsSelected;
  BOOL Hidden;
  double MaxSideLength, ArcLength, mySideLength;
  CString BoundaryMarker;
  CString InConductor;
  int InGroup;

  void ToggleSelect();

  private:
};
/////////////////////////////////////////////////////////////////////////////
// CBlockLabel -- structure that holds block label information

class CBlockLabel {
  public:
  CBlockLabel();

  double x, y;
  double MaxArea;
  BOOL IsSelected;
  CString BlockType;
  int InGroup;
  BOOL IsExternal;
  BOOL IsDefault;

  void ToggleSelect();
  double GetDistance(double xo, double yo);

  private:
};

class CMaterialProp {
  public:
  CMaterialProp();
  ~CMaterialProp();

  CString BlockName;
  double ex, ey; // permittivity, relative
  double qv; // and volume charge density

  private:
};

class CBoundaryProp {
  public:
  CBoundaryProp();

  CString BdryName;
  int BdryFormat; // type of boundary condition we are applying
                  // 0 = Fixed Voltage
                  // 1 = Mixed BC
                  // 2 = Surface Charge Density
                  // 3 = Periodic
                  // 4 = Antiperiodic

  double V; // Fixed value of V for BdryFormat=0;
  double c0, c1; // Coefficients for BdryFormat=1;
  double qs; // Surface charge density for Bdryformat=2;

  private:
};

class CPointProp {
  public:
  CPointProp();

  CString PointName;
  double V; // prescribed nodal voltage
  double qp; // point charge density

  private:
};

class CCircuit {
  public:
  CCircuit();

  CString CircName;
  double V;
  double q;
  int CircType;

  private:
};

class CPeriodicBoundary {
  public:
  CPeriodicBoundary();

  CString BdryName;
  int BdryFormat; // 0 = Periodic
                  // 1 = Antiperiodic
  int nseg; // number of segs with this bc
  int narc; // number of arcs with this bc
  int seg[2]; // (arc)segments to which is applied

  private:
};

class CCommonPoint {
  public:
  CCommonPoint();
  void Order();

  int x, y, t;

  private:
};

}

using namespace beladrawdata;