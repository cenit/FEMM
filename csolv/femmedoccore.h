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
  double Precision;
  double Frequency;
  double Depth;
  int LengthUnits;
  BOOL ProblemType;
  BOOL Coords;

  // Axisymmetric external region parameters
  double extRo, extRi, extZo;

  CcsolvDlg* TheView;

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
  BOOL OnOpenDocument();
  BOOL Cuthill();
  BOOL SortElements();
  BOOL AnalyzeProblem(CBigComplexLinProb& L);
  CComplex ChargeOnConductor(int OnConductor, CBigComplexLinProb& L);
  BOOL WriteResults(CBigComplexLinProb& L);
  void CleanUp();
};

/////////////////////////////////////////////////////////////////////////////

double Power(double x, int n);
