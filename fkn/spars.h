class CEntry {
  public:
  double x; // value stored in the entry
  int c; // column that the entry lives in
  CEntry* next; // pointer to next entry in row;
  CEntry();

  private:
};

#define LAMBDA 1.5

class CBigLinProb {
  public:
  // data members

  double* V; // solution
  double* P; // search direction;
  double* R; // residual;
  double* U; // A * P;
  double* Z;
  double* b; // RHS of linear equation
  CEntry** M; // pointer to list of matrix entries;
  int n; // dimensions of the matrix;
  int bdw; // Optional matrix bandwidth parameter;
  double Precision; // error tolerance for solution

  // member functions

  CBigLinProb(); // constructor
  ~CBigLinProb(); // destructor
  int Create(int d, int bw); // initialize the problem
  void Put(double v, int p, int q);
  // use to create/set entries in the matrix
  double Get(int p, int q);
  void AddTo(double v, int p, int q);
  int PCGSolve(int flag); // flag==true if guess for V present;
  void MultPC(double* X, double* Y);
  void MultA(double* X, double* Y);
  void SetValue(int i, double x);
  void Periodicity(int i, int j);
  void AntiPeriodicity(int i, int j);
  void Wipe();
  double Dot(double* X, double* Y);
  void ComputeBandwidth();

  CFknDlg* TheView;

  private:
};

/////////////////////////////////////////////////////////////////////
// for complex matrices......

class CComplexEntry {
  public:
  CComplex x; // value stored in the entry
  int c; // column that the entry lives in
  CComplexEntry* next; // pointer to next entry in the row;
  CComplexEntry();

  private:
};

class CBigComplexLinProb {
  public:
  // data members

  CComplex* P; // search direction
  CComplex* U;
  CComplex* R; // residual
  CComplex* V;
  CComplex* Z;
  CComplex* b; // RHS of linear equation
  CComplex* uu;
  CComplex* vv;

  CComplexEntry** M; // pointer to list of matrix entries;
  CComplexEntry** Mh; // Hermitian matrix arising from N-R algorithm;
  CComplexEntry** Ma; // Antihermitian matrix arising from N-R algorithm;
  CComplexEntry** Ms; // Additional complex-symmetric matrix arising from N-R algorithm;
  int n; // dimensions of the matrix;
  int bdw; // optional bandwidth parameter;
  int bNewton; // Flag which denotes whether or not there are entries in Mh or Ms;
  int NumNodes;
  double Precision;

  // member functions

  CBigComplexLinProb(); // constructor
  ~CBigComplexLinProb(); // destructor
  int Create(int d, int bw, int nodes); // initialize the problem
  void Put(CComplex v, int p, int q, int k = 0); // use to create/set entries in the matrix
  CComplex Get(int p, int q, int k = 0);
  void AddTo(CComplex v, int p, int q);
  void MultA(CComplex* X, CComplex* Y, int k = 0);
  void MultConjA(CComplex* X, CComplex* Y, int k = 0);
  CComplex Dot(CComplex* x, CComplex* y);
  CComplex ConjDot(CComplex* x, CComplex* y);
  void SetValue(int i, CComplex x);
  void Periodicity(int i, int j);
  void AntiPeriodicity(int i, int j);
  void Wipe();
  void MultPC(CComplex* X, CComplex* Y);
  void MultAPPA(CComplex* X, CComplex* Y);

  // flag==false initializes solution to zero
  // flag==true  starts from solution of previous call
  int PBCGSolveMod(int flag); // Precondition Biconjugate Gradient
  int PCGSQStart();
  int PBCGSolve(int flag);
  int BiCGSTAB(int flag);
  int KludgeSolve(int flag);

  CFknDlg* TheView;

  private:
};
