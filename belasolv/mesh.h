#define PI 3.141592653589793238462643383
#define eo 8.85418781762e-12
#define AXISYMMETRIC 1
#define PLANAR 0

class CNode {
  public:
  double x, y;
  int bc;
  int InConductor;

  private:
};

class CElement {
  public:
  int p[3]; // nodes at the corners of the element
  int e[3]; // boundary condition applied to each edge of the element
  int blk; // block property applied to the element
  int lbl; // block label associated with the element

  private:
};

class CBlockLabel {
  public:
  double x, y;
  double MaxArea;
  int BlockType, InGroup;
  BOOL IsExternal;
  BOOL IsDefault;

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
  double ex, ey; // permittivity, relative
  double qv; // charge density, C/m^3

  private:
};

class CBoundaryProp {
  public:
  int BdryFormat; // type of boundary condition we are applying
                  // 0 = fixed voltage
                  // 1 = mixed BC
                  // 2 = surface charge
                  // 3 = periodic
                  // 4 = antiperiodic

  double V; // set value of V for BdryFormat=0;
  double qs; // surface charge density
  double c0, c1; // coefficients for mixed BC

  private:
};

class CPointProp {
  public:
  double V; // fixed nodal voltage
  double qp; // point current density;

  private:
};

class CCircuit {
  public:
  double V, q;
  int CircType;

  private:
};
