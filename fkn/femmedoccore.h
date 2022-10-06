// femmeDoc.h : interface of the CFemmeDoc class
//
/////////////////////////////////////////////////////////////////////////////
#define muo 1.2566370614359173e-6
#define Golden 0.3819660112501051517954131656

class CFemmeDocCore {

  // Attributes
  public:
  CFemmeDocCore();
  ~CFemmeDocCore();

  // General problem attributes
  double Frequency;
  double Precision;
  double Relax;
  int LengthUnits;
  int ACSolver;
  BOOL ProblemType;
  BOOL Coords;
  CString PrevSoln;
  int PrevType;

  // Vector containing previous solution for incremental permeability analysis
  double* Aprev;

  // axisymmetric external region parameters
  double extRo, extRi, extZo;

  CFknDlg* TheView;

  // CArrays containing the mesh information
  int BandWidth;
  CNode* meshnode;
  CElement* meshele;

  int NumNodes;
  int NumEls;

  // lists of properties
  int NumBlockProps;
  int NumPBCs;
  int NumLineProps;
  int NumPointProps;
  int NumCircProps;
  int NumBlockLabels;
  int NumCircPropsOrig;
  int NumAGEs;

  CMaterialProp* blockproplist;
  CBoundaryProp* lineproplist;
  CPointProp* nodeproplist;
  CCircuit* circproplist;
  CBlockLabel* labellist;
  CCommonPoint* pbclist;
  CAirGapElement* agelist;
  // stuff usually kept track of by CDocument
  char* PathName;

  BOOL bDump; // set to dump stiffness matrix in a matlab-readable formatdir

  // Operations
  public:
  BOOL LoadPrev();
  BOOL LoadPrevA(CString myFile, double* V);
  BOOL LoadMesh();
  BOOL OnOpenDocument();
  BOOL Cuthill();
  BOOL SortElements();
  BOOL Static2D(CBigLinProb& L);
  BOOL WriteStatic2D(CBigLinProb& L);
  BOOL Harmonic2D(CBigComplexLinProb& L);
  BOOL WriteHarmonic2D(CBigComplexLinProb& L);
  BOOL StaticAxisymmetric(CBigLinProb& L);
  BOOL HarmonicAxisymmetric(CBigComplexLinProb& L);
  void GetPrevAxiB(int k, double& B1p, double& B2p);
  void GetPrev2DB(int k, double& B1p, double& B2p);
  void GetFillFactor(int lbl);
  double ElmArea(int i);
  void CleanUp();
  BOOL MatlabDumpStatic2D(CBigLinProb& L);
};

/////////////////////////////////////////////////////////////////////////////

double GetNewMu(double mu, int BHpoints, CComplex* BHdata, double muc, double B);
double Power(double x, int n);
