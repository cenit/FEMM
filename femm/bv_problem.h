namespace belaviewtype {

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

  double x, y, V;
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
  double MagDir;
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
  double ex, ey; // permittivity, relative
  double qv; // volume charge density

  CMaterialProp();

  private:
};

class CBoundaryProp {
  public:
  CBoundaryProp();

  CString BdryName;
  CString InConductor;
  int BdryFormat;
  double V, qs;
  double c0, c1;

  private:
};

class CPointProp {
  public:
  CPointProp();

  CString PointName;
  CString InConductor;
  double V, qp;

  private:
};

class CCircuit {
  public:
  CCircuit();

  CString CircName;
  double V, q;
  int CircType;

  private:
};

class CElement {
  public:
  int p[3];
  int blk, lbl;
  CComplex D; // elemental flux density
  CComplex d[3]; // smoothed flux density at corners
  CComplex ctr;
  double rsqr;
  int n[3]; // Add 3 ints to store elem's neigh.

  CComplex E();

  private:
};

class CPointVals {
  public:
  double V; // vector potential
  CComplex D; // flux density
  CComplex e; // permeability
  CComplex E; // field intensity
  double nrg; // energy stored in the field

  CPointVals();

  private:
};

}

using namespace belaviewtype;