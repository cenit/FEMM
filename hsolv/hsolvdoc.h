#define muo 1.2566370614359173e-6
#define Golden 0.3819660112501051517954131656

class Chsolvdoc {

  // Attributes
  public:
  Chsolvdoc();
  ~Chsolvdoc();

  // General problem attributes
  double Precision;
  double Depth;
  int LengthUnits;
  BOOL ProblemType;
  BOOL Coords;
  double dT;
  CString PrevSoln;

  // Axisymmetric external region parameters
  double extRo, extRi, extZo;

  ChsolvDlg* TheView;

  // CArrays containing the mesh information
  int BandWidth;
  CNode* meshnode;
  CElement* meshele;

  int NumNodes;
  int NumEls;

  // Vector containing previous solution for time-transient analysis
  double* Tprev;

  // lists of properties
  int NumBlockProps;
  int NumPBCs;
  int NumLineProps;
  int NumPointProps;
  int NumCircProps;
  int NumBlockLabels;

  CMaterialProp* blockproplist;
  CBoundaryProp* lineproplist;
  CPointProp* nodeproplist;
  CCircuit* circproplist;
  CBlockLabel* labellist;
  CCommonPoint* pbclist;

  // stuff usually kept track of by CDocument
  char* PathName;

  // Operations
  public:
  BOOL LoadMesh();
  BOOL LoadPrev();
  BOOL OnOpenDocument();
  BOOL Cuthill();
  BOOL SortElements();
  BOOL AnalyzeProblem(CBigLinProb& L);
  double ChargeOnConductor(int OnConductor, CBigLinProb& L);
  BOOL WriteResults(CBigLinProb& L);
  void CleanUp();
};

/////////////////////////////////////////////////////////////////////////////

double Power(double x, int n);
