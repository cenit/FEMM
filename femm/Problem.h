namespace femmviewdata {
/////////////////////////////////////////////////////////////////////////////
// CNode -- structure that holds information about each control point.

class CNode {
  public:
  CNode();

  double x, y;
  int xs, ys;
  BOOL IsSelected;
  int BoundaryMarker;

  double GetDistance(double xo, double yo);
  CComplex CC();
  void ToggleSelect();

  private:
};

class CMeshNode {
  public:
  CMeshNode();

  double x, y; // coordinates of the node
  CComplex A; // vector potential
  double msk; // mask value, used for creating weighted stress tensor integrals
  int xs, ys; // screen coordinates of the node
  double Aprev; // previous solution for incremental permeability problems

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
  int InGroup;

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
  CString MagDirFctn;
  int Turns;
  BOOL IsSelected;
  int InCircuit;
  int BlockType;
  int InGroup;
  BOOL IsExternal;
  BOOL IsDefault;

  int Case;
  CComplex J, dVolts;

  // attributes used to keep track of wound coil properties...
  double FillFactor;
  CComplex o, mu;
  double LocalEnergy;

  void ToggleSelect();
  double GetDistance(double xo, double yo);

  private:
};

class CMaterialProp {
  public:
  CMaterialProp();
  ~CMaterialProp();

  void GetSlopes(double omega);
  CComplex LaminatedBH(double w, int i);

  double GetH(double b);
  CComplex GetH(CComplex b); // ``raw'' results
  CComplex GetdHdB(double B);
  double GetB(double h);

  void GetMu(double b1, double b2, double& mu1, double& mu2);
  void GetMu(CComplex b1, CComplex b2, CComplex& mu1, CComplex& mu2);
  void IncrementalPermeability(double B, CComplex& mu1, CComplex& mu2);
  void IncrementalPermeability(double B, double& mu1, double& mu2);
  double GetEnergy(double b); // straight from the
  double GetCoEnergy(double b); // BH curve data

  // routines that are actually called to get
  // energy and coenergy.  These catch and take
  // care of all of the weird special cases
  // that arise with laminated materials.
  double DoEnergy(double bx, double by);
  double DoCoEnergy(double bx, double by);
  double DoEnergy(CComplex bx, CComplex by);
  double DoCoEnergy(CComplex b1, CComplex b2);

  CString BlockName;
  double mu_x, mu_y; // permeabilities, relative
  int BHpoints; // number of points in the BH curve...
  double* Bdata;
  CComplex* Hdata; // entries in B-H curve;
  CComplex* slope; // slopes used in interpolation
                   // of BHdata
  int LamType; // flag that tells how block is laminated;
               //	0 = not laminated or laminated in plane;
               //  1 = laminated in the x-direction;
               //  2 = laminated in the y-direction;
  double LamFill; // lamination fill factor, dimensionless;
  double H_c; // magnetization, A/m
  double Nrg;
  double Jr, Ji; // applied current density, MA/m^2
  double Cduct; // conductivity of the material, MS/m
  double Lam_d; // lamination thickness, mm
  double Theta_hn; // hysteresis angle, degrees
  double Theta_hx; // hysteresis angle, degrees
  double Theta_hy; // hysteresis angle, degrees
  int NStrands; // number of strands per wire
  double WireD; // strand diameter, mm

  CComplex mu_fdx, mu_fdy; // complex permeability for harmonic problems;
  double MuMax; // maximum effective permeability, needed for incremental permeability problems
  double Frequency; // problem frequency in Hz, needed for incremental permeability problems
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

  double A0, A1, A2, phi; // set value of A for BdryFormat=0;

  double Mu, Sig; // material properties necessary to apply
                  // eddy current BC

  CComplex c0, c1; // coefficients for mixed BC

  private:
};

class CPointProp {
  public:
  CPointProp();

  CString PointName;
  double Jr, Ji; // applied point current, A
  double Ar, Ai; // prescribed nodal value;

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

class CElement {
  public:
  int p[3];
  int blk, lbl;
  CComplex B1, B2;
  CComplex b1[3], b2[3];
  double magdir;
  CComplex ctr;
  double rsqr;
  int n[3]; // Add 3 ints to store elem's neigh.

  double B1p, B2p; // previous solution elemental flux density for incremental permeability problems
  double Jp; // previous solution elemental current density for incremental permeability problems

  private:
};

class CPointVals {
  public:
  CComplex A; // vector potential
  CComplex B1, B2; // flux density
  CComplex mu1, mu2; // permeability
  CComplex mu12; // permeability cross-term, needed for incremental permeability problems
  CComplex H1, H2; // field intensity
  CComplex Je, Js; // eddy current and source current densities
  CComplex Hc; // Magnetization for regions with a PM.
  double c; // conductivity
  double E; // energy stored in the magnetic field
  double Ph; // power dissipated by hysteresis
  double Pe; // power dissipated by eddy currents
  double ff; // winding fill factor

  CPointVals();

  private:
};

class CQuadPoint {
  public:
  int n0, n1, n2, n3;
  double w0, w1, w2, w3;
};

class CAirGapElement {
  public:
  CString BdryName;
  int BdryFormat; // 0 = Periodic
                  // 1 = Antiperiodic

  int totalArcElements; // total elements in the initial meshing
  double totalArcLength; // sum of the angles of all constituent arcs
  double ri, ro; // inner and outer radii of the air gap element
  double InnerAngle;
  double OuterAngle;
  double InnerShift;
  double OuterShift;
  CComplex agc; // center of the air gap element

  CQuadPoint* qp;

  int nn;
  CComplex aco;
  CComplex *brc, *brs; // harmonic components of air gap centerline flux density
  CComplex *btc, *bts;
  CComplex *br, *bt;
  double *brcPrev, *brsPrev; // harmonic components of air gap centerline flux density
  double *btcPrev, *btsPrev;
  double *brPrev, *btPrev;
  int* nh; // list of number associated with each harmonic

  private:
};

}

using namespace femmviewdata;