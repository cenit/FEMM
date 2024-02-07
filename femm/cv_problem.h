namespace cviewtype {

/////////////////////////////////////////////////////////////////////////////
// CNode -- structure that holds information about each control point.

class CNode {
  public:
  CNode();

  double x, y;
  int xs, ys;
  BOOL IsSelected;
  int BoundaryMarker;
  int InGroup, InConductor;

  double GetDistance(double xo, double yo);
  CComplex CC();
  void ToggleSelect();

  private:
};

class CMeshNode {
  public:
  CMeshNode();

  CComplex v;
  double x, y;
  double msk;
  int xs, ys;
  BOOL Q, IsSelected;

  double GetDistance(double xo, double yo);
  CComplex CC();

  private:
};
/////////////////////////////////////////////////////////////////////////////
// CSegment -- structure that holds information about lines joining control pts

class CSegment {
  public:
  CSegment();

  int n0, n1;
  double MaxSideLength;
  BOOL IsSelected;
  BOOL Hidden;
  int BoundaryMarker;
  int InGroup, InConductor;

  void ToggleSelect();

  private:
};

class CArcSegment {
  public:
  CArcSegment();

  int n0, n1;
  BOOL IsSelected;
  BOOL Hidden;
  double MaxSideLength, ArcLength;
  int BoundaryMarker;
  int InGroup, InConductor;

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
  int BlockType;
  int InGroup;
  BOOL IsExternal;
  BOOL IsDefault;

  void ToggleSelect();
  double GetDistance(double xo, double yo);

  private:
};

class CMaterialProp {
  public:
  CString BlockName;

  CComplex Kx, Ky; // effective conductivity
  double ox, oy; // dc conductivity
  CComplex ex, ey; // electrical permittivity
  CMaterialProp();

  private:
};

class CBoundaryProp {
  public:
  CBoundaryProp();

  CString BdryName;
  CString InConductor;

  int BdryFormat; // type of boundary condition we are applying
                  // 0 = Fixed Voltage
                  // 1 = Mixed BC
                  // 2 = Surface Current Density
                  // 3 = Periodic
                  // 4 = Antiperiodic

  CComplex V; // Fixed value of V for BdryFormat=0;
  CComplex c0, c1; // Coefficients for BdryFormat=1;
  CComplex qs; // Surface current density for Bdryformat=2;

  private:
};

class CPointProp {
  public:
  CPointProp();

  CString PointName;
  CString InConductor;
  CComplex V, qp;

  private:
};

class CCircuit {
  public:
  CCircuit();

  CString CircName;
  CComplex V, q;
  int CircType;

  private:
};

class CElement {
  public:
  int p[3];
  int blk, lbl;
  CComplex Jx, Jy; // elemental AC current density
  CComplex jx[3], jy[3]; // smoothed AC current density at corners
  CComplex ctr;
  double rsqr;
  int n[3]; // Add 3 ints to store elem's neigh.

  //		CComplex E();

  private:
};

class CPointVals {
  public:
  CComplex v; // voltage
  CComplex Ex, Ey; // electric field intensity
  CComplex Dx, Dy; // electric flux density
  CComplex Jcx, Jcy; // conduction current density
  CComplex Jdx, Jdy; // displacement current density
  CComplex Jx, Jy; // AC current density
  CComplex Kx, Ky; // effective conductivity
  double ox, oy; // dc conductivity
  CComplex ex, ey; // permittivity

  CPointVals();

  private:
};

}

using namespace cviewtype;