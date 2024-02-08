/////////////////////////////////////////////////////////////////////////////
// CNode -- structure that holds information about each mesh point.

class CNode {
  public:
  double x, y;
  int bc;

  private:
};

class CMeshline {
  public:
  int n0, n1;

  private:
};

class CElement {
  public:
  int p[3];
  int e[3];
  CComplex mu1, mu2, v12;
  int blk;
  int lbl;
  double Jprev;

  private:
};

class CBlockLabel {
  public:
  double x, y;
  double MaxArea;
  double MagDir;
  int BlockType;
  int InCircuit;
  int InMask;
  int InGroup;
  int Turns;
  BOOL IsExternal;
  BOOL IsDefault;
  char* MagDirFctn;

  // used for proximity effect regions only.
  CComplex ProximityMu;
  BOOL bIsWound;

  private:
};

class CCommonPoint {
  public:
  int x, y, t;

  private:
};
/////////////////////////////////////////////////////////////////////////////
// Classes that hold property data:  CMaterialProp, CBoundaryProp, CPointProp
class CMaterialProp {
  public:
  double mu_x, mu_y; // permeabilities, relative
  int BHpoints;
  double* Bdata;
  CComplex* Hdata;
  CComplex* slope;
  double H_c; // magnetization, A/m
  double Theta_m; // orientation of magnetization, degrees
  double Jr, Ji; // applied current density, MA/m^2
  double Cduct; // conductivity of the material, MS/m
  double Lam_d; // lamination thickness, mm
  double Theta_hn; // hysteresis angle, degrees
  double Theta_hx; // hysteresis angle, degrees
  double Theta_hy; // hysteresis angle, degrees
  int LamType; // lamination type/orientation
  double LamFill; // lamination fill factor;
  double WireD; // strand diameter, mm
  int NStrands; // number of strands per wire
  double MuMax; // maximum permeability for AC problems
  CMaterialProp();
  ~CMaterialProp();
  void GetSlopes();
  void GetSlopes(double omega);
  CComplex GetH(double B);
  CComplex GetdHdB(double B);
  CComplex Get_dvB2(double B);
  CComplex Get_v(double B);
  void GetBHProps(double B, CComplex& v, CComplex& dv);
  void GetBHProps(double B, double& v, double& dv);
  CComplex LaminatedBH(double omega, int i);
  void IncrementalPermeability(double B, double w, CComplex& mu1, CComplex& mu2);
  void IncrementalPermeability(double B, double& mu1, double& mu2);

  private:
};

class CBoundaryProp {
  public:
  int BdryFormat; // type of boundary condition we are applying
                  // 0 = constant value of A
                  // 1 = Small skin depth eddy current BC
                  // 2 = Mixed BC
                  // 3 = SDI boundary (deprecated)
                  // 4 = Periodic
                  // 5 = Antiperiodic
                  // 6 = Periodic AGE
                  // 7 = Antiperiodic AGE

  double A0, A1, A2, phi; // set value of A for BdryFormat=0;

  double Mu, Sig; // material properties necessary to apply
                  // eddy current BC

  CComplex c0, c1; // coefficients for mixed BC

  double InnerAngle; // parameters needed to build air gap element
  double OuterAngle;

  // TO DO:  ``flux pipe?'' and ''line currents''
  // Line currents might be redundant, since we already have magnetization.

  private:
};

class CPointProp {
  public:
  double Jr, Ji; // applied point current, A
  double Ar, Ai; // prescribed nodal value;

  private:
};

class CCircuit {
  public:
  double dVolts_re, dVolts_im;
  double Amps_re, Amps_im;
  int CircType;
  int OrigCirc;

  // some member variables used to store some results
  CComplex J;
  CComplex dV;
  int Case;

  private:
};

class CQuadPoint {
  public:
  int n0, n1, n2, n3;
  double w0, w1, w2, w3;
};

class CAirGapElement {
  public:
  char BdryName[80];
  int BdryFormat; // 0 = Periodic
                  // 1 = Antiperiodic

  int totalArcElements; // total elements in the initial meshing
  double totalArcLength; // sum of the angles of all constituent arcs
  double ri, ro; // inner and outer radii of the air gap element
  double InnerAngle; // Angle in degrees through which the inner ring (rotor) is turned
  double OuterAngle; // Angle in degrees through which the inner ring (stator) is turned
  double InnerShift; // fraction of an element that inner mesh is shifted relative to annular mesh
  double OuterShift; // fraction of an element that outer mesh is shifted relative to annular mesh
  CComplex agc; // center of the air gap element

  CQuadPoint* node; // mapping of mesh nodes onto an annular ring

  private:
};