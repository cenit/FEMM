namespace femmedata {

/////////////////////////////////////////////////////////////////////////////
// CNode -- structure that holds information about each control point.

class CNode {
  public:
  CNode();

  double x, y;
  int xs, ys;
  BOOL IsSelected;
  CString BoundaryMarker;
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
  double MagDir;
  int Turns;
  BOOL IsSelected;
  CString BlockType;
  CString InCircuit;
  CString MagDirFctn;
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
  void StripBHData(CString& b, CString& h);
  void BHDataToCString(CString& b, CString& h);

  CString BlockName;
  double mu_x, mu_y; // permeabilities, relative

  int BHpoints; // number of B-H datapoints;
  CComplex* BHdata; // array of B-H pairs;

  int LamType; // flag that tells how block is laminated;
               //	0 = not laminated or laminated in plane;
               //  1 = laminated in the x-direction;
               //  2 = laminated in the y-direction;
  double LamFill; // lamination fill factor, dimensionless;
  double Theta_m; // direction of the magnetization, degrees
  double H_c; // magnetization, A/m
  CComplex Jsrc; // applied current density, MA/m^2
  double Cduct; // conductivity of the material, MS/m
  double Lam_d; // lamination thickness, mm
  double Theta_hn; // max hysteresis angle, degrees, for nonlinear problems
  double Theta_hx; // hysteresis angle, degrees, x-direction
  double Theta_hy; // and y-direction, for anisotropic linear problems.
  int NStrands; // number of strands per wire
  double WireD; // strand diameter, mm

  private:
};

class CBoundaryProp {
  public:
  CBoundaryProp();

  CString BdryName;
  int BdryFormat; // type of boundary condition we are applying
                  // 0 = constant value of A
                  // 1 = Small skin depth eddy current BC
                  // 2 = Mixed BC
                  // 3 = SDI
                  // 4 = Periodic
                  // 5 = Antiperiodic
                  // 6 = Periodic air gap element
                  // 7 = Antiperiodic air gap element

  double A0, A1, A2, phi; // set value of A for BdryFormat=0;

  double Mu, Sig; // material properties necessary to apply
                  // eddy current BC

  CComplex c0, c1; // coefficients for mixed BC

  double InnerAngle; // rotation of inner bdry for air gap element
  double OuterAngle; // rotation of outer bdry for air gap element

  // TO DO:  ``flux pipe?'' and ''line currents''
  // Line currents might be redundant, since we already have magnetization.

  private:
};

class CPointProp {
  public:
  CPointProp();

  CString PointName;
  CComplex Jp; // applied point current, Amps
  CComplex Ap; // prescribed nodal value of vector potential;

  private:
};

class CCircuit {
  public:
  CCircuit();

  CString CircName;
  CComplex Amps;
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

class CAirGapElement {
  public:
  CAirGapElement();
  ~CAirGapElement();

  CString BdryName;
  int BdryFormat; // 0 = Periodic
                  // 1 = Antiperiodic

  int totalArcElements; // total elements in the initial meshing
  double totalArcLength; // sum of the angles of all constituent arcs
  double ri, ro; // inner and outer radii of the air gap element
  double InnerAngle;
  double OuterAngle;
  CComplex agc; // center of the air gap element
  int* node; // node numbers that are part of the air gap element

  private:
};

class CQuadPoint {
  public:
  CQuadPoint();

  int n0, n1;
  double w0, w1;
};

}

using namespace femmedata;